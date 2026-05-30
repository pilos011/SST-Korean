#define INCLUDED
#include "sst.h"
#include <ctype.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

/* ============================================================
 * sst.c - Super Star Trek 메인 루프 (한국어 버전)
 * ============================================================ */

static char line[256], *linep = line;
static int  linecount;

static void clearscreen(void) {
    system("cls");
}

/* 영어 명령어 배열 (내부 처리용, 변경 금지) */
static char *commands[] = {
    "srscan",
    "lrscan",
    "phasers",
    "photons",
    "move",
    "shields",
    "dock",
    "damages",
    "chart",
    "impulse",
    "rest",
    "warp",
    "status",
    "sensors",
    "orbit",
    "transport",
    "mine",
    "crystals",
    "shuttle",
    "planets",
    "request",
    "report",
    "computer",
    "commands",
    "emexit",
    "probe",
    "cloak",
    "capture",
    "score",
    "abandon",
    "destruct",
    "freeze",
    "deathray",
    "debug",
    "call",
    "quit",
    "help"
};

#define NUMCOMMANDS (sizeof(commands)/sizeof(char *))

/* 한국어 명령어 표시 (사용자가 보는 것) */
static void listCommands(int x) {
    /* x=TRUE : 사용자가 "명령어목록" 명령 직접 입력
     *          → 화면 지우고 표 출력 후 키 입력 대기
     * x=FALSE: helpme()에서 잘못된 명령어 안내용
     *          → 현재 화면에 그냥 출력, 일시정지 없음 */
    if (x) {
        system("cls");   /* 화면 지우기 - 스크롤 방지 */
        skip(1);
    }
    prout("  ┌──────────────────────────┬──────────────────────────┬──────────────────────────┬──────────────────────────┐");
    prout("  │ 단거리스캔  (단/S)       │ 이동        (이/M)       │ 페이저      (페/P)       │ 구조요청    (구/CALL)    │");
    prout("  │ 상태        (상/ST)      │ 임펄스 엔진 (임/I)       │ 광자어뢰    (광/PHO)     │ 함선포기    (함/ABANDON) │");
    prout("  │ 장거리스캔  (장/L)       │ 워프        (워/W)       │ 실드        (실/SH)      │ 자폭        (자/DESTRUCT)│");
    prout("  │ 성도        (성/C)       │ 휴식        (휴/R)       │ 도킹        (도/D)       │ 종료        (종/QUIT)    │");
    prout("  │ 손상보고    (손/DA)      │ 보고        (보/REP)     │ 센서        (센/SE)      │ 궤도        (궤/O)       │");
    prout("  │ 전송        (전/T)       │ 채굴        (채/MI)      │ 결정        (결/CR)      │ 셔틀        (셔/SHU)     │");
    prout("  │ 행성        (행/PL)      │ 요청        (요/REQ)     │ 죽음광선    (죽/DEATHRAY)│ 저장        (저/FREEZE)  │");
    prout("  │ 컴퓨터      (컴/CO)      │ 비상탈출    (비/E)       │ 탐침        (탐/PR)      │ 명령어목록  (명/COMMANDS)│");
    prout("  └──────────────────────────┴──────────────────────────┴──────────────────────────┴──────────────────────────┘");
    prout("  * 한글 첫 음절 또는 괄호 안 영어로 입력.  예: 단 또는 S = 단거리스캔");
    prout("  * 전체 단어 필요: ABANDON(함선포기) DESTRUCT(자폭) QUIT(종료) FREEZE(저장) DEATHRAY(죽음광선)");
#ifdef SCORE
    prout("  * SCORE(점/SC) - 현재 점수 조회");
#endif
#ifdef CLOAKING
    prout("  * CLOAK(은/CL) - 클로킹 장치");
#endif
#ifdef CAPTURE
    prout("  * CAPTURE(포/CA) - 클링온 포획");
#endif
    if (x) {
        prout("  * HELP <명령어> - 특정 명령 도움말  (예: help 이동)");
        skip(1);
        proutn("  [아무 키나 누르면 게임으로 돌아갑니다] ");
        fflush(stdout);
        _getch();
        putchar('\n');
        system("cls");   /* 표 지우고 게임 화면으로 복귀 */
    }
}

static void helpme(void) {
    int i, j;
    char cmdbuf[32];
    char linebuf[132];
    FILE *fp;
    int key;
    key = scan();
    while (TRUE) {
        if (key == IHEOL) {
            proutn("어떤 명령어의 도움말이 필요하십니까? ");
            key = scan();
        }
        if (key == IHEOL) return;

        /* 한국어 명령어를 영어로 변환 */
        const char *resolved = lang_resolve_command(citem);
        if (resolved) {
            strncpy(citem, resolved, sizeof(citem)-1);
        }

        for (i = 0; i < (int)NUMCOMMANDS; i++) {
            if (strcmp(commands[i], citem)==0) break;
        }
        if (i != (int)NUMCOMMANDS) break;
        skip(1);
        prout("유효한 명령어 목록:");
        listCommands(FALSE);
        key = IHEOL;
        chew();
        skip(1);
    }
    if (i == 23) {
        /* "commands" → listCommands(TRUE) 직접 호출
         * sst.doc ABBREV 검색 시 종결자 문제로 전체 파일 출력됨 */
        listCommands(TRUE);
        return;
    } else {
        strcpy(cmdbuf, "  Mnemonic:  ");
        j = 0;
        while ((cmdbuf[j+13] = toupper(commands[i][j])) != 0) j++;
    }
    fp = fopen("sst.doc", "r");
    if (fp == NULL) {
        prout("스팍-  \"함장님, 해당 정보가 컴퓨터에 없습니다.");
        prout("   SST.DOC 파일을 sst.exe 와 같은 폴더에 놓아 주십시오.\"");
        return;
    }
    i = strlen(cmdbuf);
    do {
        if (fgets(linebuf, 132, fp) == NULL) {
            prout("스팍- \"함장님, 해당 명령어에 대한 정보가 없습니다.\"");
            fclose(fp);
            return;
        }
    } while (strncmp(linebuf, cmdbuf, i) != 0);

    skip(1);
    prout("스팍- \"함장님, 다음 정보를 찾았습니다:\"");
    skip(1);

    do {
        if (linebuf[0]!=12) {
            linebuf[strlen(linebuf)-1] = '\0';
            prout(linebuf);
        }
        fgets(linebuf,132,fp);
    } while (strstr(linebuf, "******")==NULL);
    fclose(fp);
}

static void makemoves(void) {
    int i, hitme;
    char ch;
    while (TRUE) {
        hitme = FALSE;
        justin = 0;
        Time = 0.0;
        i = -1;
        while (TRUE) {
            chew();
            skip(1);
            proutn("명령> ");
            if (scan() == IHEOL) continue;

            /* 한국어 명령어를 영어로 변환 시도 */
            const char *resolved = lang_resolve_command(citem);
            if (resolved) {
                strncpy(citem, resolved, sizeof(citem)-1);
            }
            /* "임펄스 엔진" 처럼 명령어+엔진 두 단어 형태 처리
             * scan()이 "임펄스"만 읽고 "엔진"이 버퍼에 남는 경우 소비 */
            {
                char *saved_lp = linep;
                char saved_cmd[32];
                strncpy(saved_cmd, citem, sizeof(saved_cmd)-1);
                saved_cmd[sizeof(saved_cmd)-1] = '\0';
                if (scan() == IHALPHA &&
                    (strcmp(citem, "엔진") == 0 || strcmp(citem, "engine") == 0)) {
                    /* "엔진" 토큰 소비됨 — 명령어는 saved_cmd로 복원 */
                } else {
                    linep = saved_lp;   /* "엔진"이 아니면 버퍼 되돌리기 */
                }
                strncpy(citem, saved_cmd, sizeof(citem)-1);
                citem[sizeof(citem)-1] = '\0';
            }

            for (i=0; i < 29; i++)
                if (isit(commands[i]))
                    break;
            if (i < 29) break;

            /* 나머지 명령어들 (약어 없음) */
            for (; i < (int)NUMCOMMANDS; i++)
                if (strcmp(commands[i], citem) == 0)
                    break;
            if (i < (int)NUMCOMMANDS) break;
            huh();
        }
        switch (i) {
            case 0:  srscan(1);   break;
            case 1:  lrscan();    break;
            case 2:  phasers();   hitme=TRUE; break;
            case 3:  photon();    hitme=TRUE; break;
            case 4:  warp(0);     hitme=TRUE; break;
            case 5:  sheild(1);   hitme=ididit; break;
            case 6:  dock();      break;
            case 7:  dreprt();    break;
            case 8:  chart(0);    break;
            case 9:  impuls();    hitme=TRUE; break;
            case 10: waiting();   hitme=ididit; break;
            case 11: setwrp();    break;
            case 12: srscan(3);   break;
            case 13: sensor();    break;
            case 14: orbit();     hitme=ididit; break;
            case 15: beam();      hitme=ididit; break;
            case 16: mine();      hitme=ididit; break;
            case 17: usecrystals(); hitme=ididit; break;
            case 18: shuttle();   hitme=ididit; break;
            case 19: preport();   break;
            case 20: srscan(2);   break;
            case 21: report(0);   break;
            case 22: eta();       break;
            case 23: listCommands(TRUE); break;
            case 24: /* emexit - 비상탈출: 저장 후 즉시 종료 */
                freeze(TRUE);
                prout("게임이 emsave.trk 에 저장되었습니다.");
                prout("비상 탈출합니다.");
                skip(1);
                system("cls");
                exit(0);
                break;
            case 25: probe();     hitme=ididit; break;
#ifdef CLOAKING
            case 26: cloak();     hitme=ididit; break;
#endif
#ifdef CAPTURE
            case 27: capture();   hitme=ididit; break;
#endif
#ifdef SCORE
            case 28: score(0);    break;
#endif
            case 29: abandn();    break;
            case 30: dstrct();    break;
            case 31: freeze(FALSE); break;
            case 32: deathray();  hitme=ididit; break;
#ifdef DEBUG
            case 33: debugme();   break;
#endif
            case 34: help();      hitme=ididit; break;
            case 35:
                alldone = 1;
#ifdef DEBUG
                if (idebug) prout("종료 명령 실행");
#endif
                break;
            case 36: helpme();    break;
        }
        /* -------------------------------------------------------
         * 원본 makemoves()와 동일한 이벤트 처리 루프
         * 각 명령 실행 후 시간이 흘렀으면 events()를 호출하여
         * 초신성/견인빔/사령관이동 등 시간 이벤트를 처리한다.
         * ------------------------------------------------------- */
        for (;;) {
            if (alldone) break;
            /* 이동 등으로 Time이 경과했으면 이벤트 처리 */
            if (Time != 0.0) {
                events();
                if (alldone) break;
            }
            /* 현재 구역이 초신성 구역이면 즉시 비상 탈출 */
            if (d.galaxy[quadx][quady] == 1000) {
                atover(0);
                continue;
            }
            /* 솔리안 웹 이동 (적이 없을 때) */
            if (nenhere == 0) movetho();
            /* 적의 반격 (이동 직후 첫 턴, justin==0) */
            if (hitme && justin == 0) {
                attack(2);          /* 2 = 어뢰+페이저 모두 허용 */
                if (alldone) break;
                if (d.galaxy[quadx][quady] == 1000) {
                    atover(0);
                    hitme = TRUE;
                    continue;
                }
            }
            break;
        }
        if (alldone) break;
    }
}

/* ---- 출력 유틸리티 ---- */

void skip(int i) {
    while (i-- > 0) putchar('\n');
}

void prout(char *s) {
    printf("%s\n", s);
}

void proutn(char *s) {
    printf("%s", s);
}

/* 느린 출력 (극적 효과용) */
void prouts(char *s) {
    clock_t endTime;
    while (*s) {
        endTime = clock() + (clock_t)(CLOCKS_PER_SEC * 0.04);
        while (clock() < endTime);
        putchar(*s++);
        fflush(stdout);
    }
}

void huh(void) {
    chew();
    skip(1);
    prout("함장님, 다시 말씀해 주시겠습니까?");
}

int isit(char *s) {
    return strncmp(s, citem, max(1, strlen(citem))) == 0;
}

/* ---- 스캔/입력 ---- */

int scan(void) {
    char *p;

    if (linep == line) {
        if (fgets(line, sizeof(line)-1, stdin) == NULL) {
            printf("입력 오류\n");
            exit(0);
        }
        /* CR/LF 제거 */
        for (p = line; *p; p++) {
            if (*p == '\n' || *p == '\r') { *p = '\0'; break; }
        }
    }
    while (*linep == ' ') linep++;
    if (*linep == '\0') {
        chew();
        citem[0] = '\0';   /* IHEOL 시 citem 반드시 초기화 */
        return IHEOL;
    }
    if (isalpha((unsigned char)*linep) || *linep == '_' ||
        (unsigned char)*linep >= 0x80) {
        /* -------------------------------------------------------
         * 알파 토큰: 영어(ASCII) 또는 한국어(UTF-8 멀티바이트)
         * 한국어 UTF-8 Hangul Syllables: 0xEA~0xED 범위 (3바이트)
         * isalpha()는 ASCII 전용 → 0x80 이상 체크로 한국어 처리
         * ------------------------------------------------------- */
        p = citem;
        if (isalpha((unsigned char)*linep) || *linep == '_') {
            /* 영어 단어: 소문자로 변환하며 읽기 */
            while (isalpha((unsigned char)*linep) || *linep == '_')
                *p++ = tolower((unsigned char)*linep++);
        } else {
            /* 한국어(멀티바이트): 공백/탭/숫자/부호 전까지 전부 읽기 */
            while (*linep && *linep != ' ' && *linep != '\t' &&
                   !isdigit((unsigned char)*linep) &&
                   *linep != '.' && *linep != '-' && *linep != '\0') {
                *p++ = *linep++;
            }
        }
        *p = '\0';
        return IHALPHA;
    }
    if (isdigit(*linep) || *linep == '.' || *linep == '-') {
        aaitem = strtod(linep, &linep);
        return IHREAL;
    }
    /* 기타 단일 문자 */
    citem[0] = *linep++;
    citem[1] = '\0';
    return IHALPHA;
}

void chew(void) {
    linep = line;
    *line = '\0';
}

void chew2(void) {
    /* 현재 줄의 나머지 버림 */
    while (*linep && *linep != '\n') linep++;
}

int ja(void) {
    while (TRUE) {
        proutn("[예(y)/아니오(n)] ");
        scan();
        chew();
        /* 예: y, yes, 예, 네, 응 */
        if (strcmp(citem, "y")      == 0 || strcmp(citem, "yes")  == 0 ||
            strcmp(citem, "예")     == 0 || strcmp(citem, "네")   == 0 ||
            strcmp(citem, "응")     == 0 || strcmp(citem, "ok")   == 0)
            return TRUE;
        /* 아니오: n, no, 아니오, 아니, 노 */
        if (strcmp(citem, "n")      == 0 || strcmp(citem, "no")    == 0 ||
            strcmp(citem, "아니오") == 0 || strcmp(citem, "아니") == 0 ||
            strcmp(citem, "노")     == 0)
            return FALSE;
        prout("'예(y)' 또는 '아니오(n)'로 대답해 주세요.");
    }
}

void cramf(double x, int w, int d) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%*.*f", w, d, x);
    proutn(buf);
}

void crami(int i, int w) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%*d", w, i);
    proutn(buf);
}

void cramlc(int l, int x, int y) {
    char buf[64];
    if (l == 0) {
        snprintf(buf, sizeof(buf), " %d-%d", x, y);
    } else {
        if (coordfixed)
            snprintf(buf, sizeof(buf), " 구역 %d-%d", y, x);
        else
            snprintf(buf, sizeof(buf), " 구역 %d-%d", x, y);
    }
    proutn(buf);
}

void crmshp(void) {
    char *s;
    switch (ship) {
        case IHE: s = "엔터프라이즈호"; break;
        case IHF: s = "페어리 퀸호";    break;
        default:  s = "???";            break;
    }
    proutn(s);
}

void cramen(int i) {
    char *s;
    switch (i) {
        case IHR: s = "로뮬란"; break;
        case IHK: s = "클링온"; break;
        case IHC: s = "클링온 사령관"; break;
        case IHS: s = "슈퍼사령관"; break;
        case IHT: s = "솔리안"; break;
        default:  s = "???";   break;
    }
    proutn(s);
}

void crmena(int i, int enemy, int l, int x, int y) {
    if (i) proutn("  ");
    cramen(enemy);
    proutn(" (");
    cramlc(l, x, y);
    proutn(")");
}

void stars(void) {
    proutn("*****");
}

double Rand(void) {
    return (double)rand() / (double)RAND_MAX;
}

double expran(double d) {
    return -d * log(Rand() + 1e-7);
}

double square(double d) {
    return d*d;
}

void iran8(int *i, int *j) {
    *i = (int)(8.0*Rand()) + 1;
    *j = (int)(8.0*Rand()) + 1;
}

void iran10(int *i, int *j) {
    *i = (int)(10.0*Rand()) + 1;
    *j = (int)(10.0*Rand()) + 1;
}


void pause_game(int i) {
    if (i == 1) {
        if (skill > SGOOD) return; /* 고급 게임에서는 pause 없음 */
        proutn("[계속하려면 엔터를 누르세요]");
        fflush(stdout);
        _getch();
        putchar('\n');
    } else {
        proutn("[계속하려면 엔터를 누르세요]");
        fflush(stdout);
        _getch();
        putchar('\n');
    }
}

/* pause_game() 대체 (pause는 Windows에서 POSIX 함수) */
void pause_func(int i) { pause_game(i); }

#ifdef DEBUG
void debugme(void) {
    proutn("레벨 초기화? ");
    if (ja() != 0) {
        if (energy < inenrg) energy = inenrg;
        shield = inshld;
        torps  = intorps;
        lsupres = inlsr;
    }
    proutn("손상 초기화? ");
    if (ja() != 0) {
        int i;
        for (i=0; i <= ndevice; i++) if (damage[i] > 0.0) damage[i] = 0.0;
        stdamtim = 1e30;
    }
    proutn("디버그 토글? ");
    if (ja() != 0) {
        idebug = !idebug;
        if (idebug) prout("디버그 출력 ON");
        else prout("디버그 출력 OFF");
    }
    proutn("선택적 손상 적용? ");
    if (ja() != 0) {
        int i, key;
        for (i=1; i <= ndevice; i++) {
            proutn("파괴: ");
            proutn(device[i]);
            proutn("? ");
            chew();
            key = scan();
            if (key == IHALPHA && isit("y")) {
                damage[i] = 10.0;
                if (i == DRADIO) stdamtim = d.date;
            }
        }
    }
    proutn("이벤트 확인/변경? ");
    if (ja() != 0) {
        int i;
        for (i = 1; i < NEVENTS; i++) {
            int key;
            if (future[i] == 1e30) continue;
            switch (i) {
                case FSNOVA:  proutn("초신성          "); break;
                case FTBEAM:  proutn("견인 빔         "); break;
                case FSNAP:   proutn("스냅샷          "); break;
                case FBATTAK: proutn("기지 공격       "); break;
                case FCDBAS:  proutn("기지 파괴       "); break;
                case FSCMOVE: proutn("슈퍼사령관 이동 "); break;
                case FSCDBAS: proutn("슈퍼사령관 기지파괴"); break;
            }
            cramf(future[i]-d.date, 8, 2);
            chew();
            proutn("  ?");
            key = scan();
            if (key == IHREAL) {
                future[i] = d.date + aaitem;
            }
        }
        chew();
    }
    proutn("은하 전체 공개? ");
    if (ja() != 0) {
        int i, j;
        for (i = 1; i < 9; i++)
            for (j = 1; j < 9; j++)
                starch[i][j] = 1;
    }
}
#endif

/* ---- 메인 ---- */

int main(int argc, char **argv) {
    int i;

    /* Windows 콘솔 설정 (UTF-8, 한국어) */
    win_setup_console();

    /* 언어 시스템 초기화 (INI 로드) */
    lang_load(LANG_INI_FILE);

    /* device[] 배열을 INI에서 로드한 한국어 이름으로 채우기 */
    /* 장치 이름을 INI에서 로드한 한국어 이름으로 설정 */
    {
        int _di;
        for (_di = 0; _di <= ndevice; _di++)
            device[_di] = (char*)lang_device(_di);
    }

    fromcommandline = 0;
    coordfixed = 0;

    /* 명령행 인수 처리 */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            coordfixed = TRUE;
        } else if (strcmp(argv[i], "-t") == 0) {
            /* test mode - ignored */
        } else if (strcmp(argv[i], "-r") == 0 && i+1 < argc) {
            srand(atoi(argv[++i]));
        } else {
            /* 토너먼트 번호 */
            tourn = atoi(argv[i]);
            if (tourn) {
                srand(tourn);
                fromcommandline = 1;
            }
        }
    }
    if (!fromcommandline) randomize();

    /* 게임 루프 */
    while (TRUE) {
        clearscreen();
        prelim();
        /* 이전 게임의 잔여 입력 버퍼를 비우고 새 게임 시작 */
        chew();
        citem[0] = '\0';
        setup();
        if (alldone) {
            score(0);
            alldone = 0;
            chew();
            citem[0] = '\0';
            continue;
        }
        makemoves();
        if (alldone) {
            alldone = 0;
            chew();
            citem[0] = '\0';
            continue;
        }
        break;
    }

    return 0;
}
