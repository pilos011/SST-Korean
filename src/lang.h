#ifndef LANG_H
#define LANG_H

/* ============================================================
 * lang.h - Super Star Trek 한국어 언어 시스템
 * INI 파일에서 명령어, 장치명, 고유명사를 로드합니다.
 * ============================================================ */

#define LANG_MAX_COMMANDS   40
#define LANG_MAX_DEVICES    17
#define LANG_MAX_VALUE      64
#define LANG_INI_FILE       "sst.ini"

/* 한국어 명령 별칭 구조체 */
typedef struct {
    char eng[LANG_MAX_VALUE];   /* 영어 명령어 */
    char kor[LANG_MAX_VALUE];   /* 한국어 별칭 */
} LangCommand;

/* 전역 언어 데이터 */
extern LangCommand lang_commands[LANG_MAX_COMMANDS];
extern int         lang_num_commands;
extern char        lang_devices[LANG_MAX_DEVICES][LANG_MAX_VALUE];

/* 함수 선언 */
void lang_load(const char *ini_path);
const char *lang_device(int n);
const char *lang_get(const char *section, const char *key, const char *def);

/* INI에서 로드한 한국어 명칭 접근 매크로 */
#define DEVICE_NAME(n)  lang_device(n)

#endif /* LANG_H */
