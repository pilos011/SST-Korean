/* ============================================================
 * lang.c - Super Star Trek 한국어 언어 시스템 구현
 * INI 파일 파서 + 명령어/장치명/고유명사 로더
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lang.h"

/* 전역 언어 데이터 */
LangCommand lang_commands[LANG_MAX_COMMANDS];
int         lang_num_commands = 0;
char        lang_devices[LANG_MAX_DEVICES][LANG_MAX_VALUE];

/* 기본 장치 이름 (INI 로드 실패 시 폴백) */
static const char *default_devices[] = {
    "",
    "단거리 센서",
    "장거리 센서",
    "페이저",
    "광자 튜브",
    "생명유지장치",
    "워프 엔진",
    "임펄스 엔진",
    "실드",
    "서브스페이스 무선",
    "셔틀 크래프트",
    "컴퓨터",
    "전송기",
    "실드 제어장치",
    "죽음 광선",
    "심우주 탐침",
    "클로킹 장치"
};

/* INI 섹션/키/값 저장소 (단순 선형 배열) */
#define MAX_INI_ENTRIES 256

typedef struct {
    char section[64];
    char key[64];
    char value[LANG_MAX_VALUE];
} IniEntry;

static IniEntry ini_entries[MAX_INI_ENTRIES];
static int      ini_count = 0;

/* 문자열 앞뒤 공백 제거 (in-place) */
static char *trim(char *s) {
    char *end;
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == '\0') return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) *end-- = '\0';
    return s;
}

/* INI 파일 파싱 */
static void ini_parse(const char *path) {
    FILE *fp = fopen(path, "r");
    char  line[256];
    char  section[64] = "";

    if (!fp) return;

    while (fgets(line, sizeof(line), fp)) {
        char *p = trim(line);
        if (*p == '\0' || *p == ';' || *p == '#') continue;

        if (*p == '[') {
            /* 섹션 헤더 */
            char *end = strchr(p, ']');
            if (end) {
                *end = '\0';
                strncpy(section, p + 1, sizeof(section) - 1);
                section[sizeof(section)-1] = '\0';
            }
        } else {
            /* 키=값 */
            char *eq = strchr(p, '=');
            if (eq && ini_count < MAX_INI_ENTRIES) {
                *eq = '\0';
                strncpy(ini_entries[ini_count].section, section, 63);
                strncpy(ini_entries[ini_count].key,     trim(p),   63);
                strncpy(ini_entries[ini_count].value,   trim(eq+1), LANG_MAX_VALUE-1);
                ini_entries[ini_count].section[63] = '\0';
                ini_entries[ini_count].key[63]     = '\0';
                ini_entries[ini_count].value[LANG_MAX_VALUE-1] = '\0';
                ini_count++;
            }
        }
    }
    fclose(fp);
}

/* 섹션+키로 값 조회 */
const char *lang_get(const char *section, const char *key, const char *def) {
    int i;
    for (i = 0; i < ini_count; i++) {
        if (strcmp(ini_entries[i].section, section) == 0 &&
            strcmp(ini_entries[i].key,     key)     == 0) {
            return ini_entries[i].value;
        }
    }
    return def;
}

/* 장치 이름 반환 */
const char *lang_device(int n) {
    if (n < 0 || n >= LANG_MAX_DEVICES) return "???";
    return lang_devices[n];
}

/* 언어 시스템 초기화 - INI 파일 로드 */
void lang_load(const char *ini_path) {
    int i;

    /* 기본값으로 초기화 */
    for (i = 0; i < LANG_MAX_DEVICES; i++) {
        if (i < 17) {
            strncpy(lang_devices[i], default_devices[i], LANG_MAX_VALUE-1);
        }
    }

    /* INI 파일 파싱 */
    ini_parse(ini_path);

    /* 장치 이름 로드 */
    for (i = 1; i < LANG_MAX_DEVICES; i++) {
        char key[8];
        const char *val;
        snprintf(key, sizeof(key), "%d", i);
        val = lang_get("Devices", key, NULL);
        if (val) {
            strncpy(lang_devices[i], val, LANG_MAX_VALUE-1);
        }
    }

    /* 명령어 별칭 로드 */
    lang_num_commands = 0;
    for (i = 0; i < ini_count && lang_num_commands < LANG_MAX_COMMANDS; i++) {
        if (strcmp(ini_entries[i].section, "Commands") == 0) {
            strncpy(lang_commands[lang_num_commands].eng,
                    ini_entries[i].key,   LANG_MAX_VALUE-1);
            strncpy(lang_commands[lang_num_commands].kor,
                    ini_entries[i].value, LANG_MAX_VALUE-1);
            lang_num_commands++;
        }
    }
}

/* 위험 명령어: 반드시 전체 단어 입력 필요 (접두어 매칭 불가)
 * sst.ini의 한국어 별칭도 완전일치만 허용 */
static const char *dangerous_cmds[] = {
    "abandon", "destruct", "freeze", "deathray", "quit", NULL
};

static int is_dangerous(const char *eng) {
    int i;
    for (i = 0; dangerous_cmds[i]; i++)
        if (strcmp(dangerous_cmds[i], eng) == 0) return 1;
    return 0;
}

/* 한국어 입력을 영어 명령어로 변환
 * 우선순위: (1) 완전일치  (2) 접두어 매칭(단축키 지원)
 * 위험 명령어(abandon/destruct/freeze/deathray/quit)는
 * 완전일치만 허용 — 접두어 단축어 불가 */
const char *lang_resolve_command(const char *input) {
    int i;
    size_t input_len = strlen(input);
    const char *prefix_match = NULL;  /* 접두어 매칭 후보 */
    int prefix_count = 0;             /* 중복 방지 카운터 */

    if (input_len == 0) return NULL;

    for (i = 0; i < lang_num_commands; i++) {
        /* (1) 완전일치 - 최우선 */
        if (strcmp(lang_commands[i].kor, input) == 0) {
            return lang_commands[i].eng;
        }
        /* (2) 접두어 매칭: 위험 명령어는 완전일치만 허용 */
        if (is_dangerous(lang_commands[i].eng)) continue;
        if (strncmp(lang_commands[i].kor, input, input_len) == 0) {
            prefix_match = lang_commands[i].eng;
            prefix_count++;
        }
    }

    /* 접두어 후보가 정확히 1개일 때만 반환 (중의성 없음) */
    if (prefix_count == 1) return prefix_match;

    return NULL;
}
