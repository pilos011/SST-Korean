#include "sst.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * reports.c - 상태 보고 및 스캔 (한국어 버전)
 * ============================================================ */

void attakreport(void) {
    if (future[FCDBAS] < 1e30) {
        proutn("구역");
        cramlc(1, batx, baty);
        prout("의 스타베이스가 현재 공격받고 있습니다.");
        proutn("스타데이트 ");
        cramf(future[FCDBAS], 0, 1);
        prout("까지 버틸 수 있습니다.");
    }
    if (isatb == 1) {
        proutn("구역");
        cramlc(1, d.isx, d.isy);
        prout("의 스타베이스가 슈퍼사령관의 공격을 받고 있습니다.");
        proutn("스타데이트 ");
        cramf(future[FSCDBAS], 0, 1);
        prout("까지 버틸 수 있습니다.");
    }
}

void report(int f) {
    char *s1, *s2, *s3;

    chew();
    s1 = (thawed ? "저장된" : "");
    switch (length) {
        case 1:  s2 = "단기"; break;
        case 2:  s2 = "중기"; break;
        case 4:  s2 = "장기"; break;
        default: s2 = "알 수 없는 길이";  break;
    }
    switch (skill) {
        case SNOVICE:   s3 = "초보 난이도"; break;
        case SFAIR:     s3 = "보통 난이도"; break;
        case SGOOD:     s3 = "고수 난이도"; break;
        case SEXPERT:   s3 = "전문가 난이도"; break;
        case SEMERITUS: s3 = "명예 난이도"; break;
        default:        s3 = "숙련 난이도"; break;
    }
    printf("\n당신은 %s %s %s 게임을 %s.\n",
           s1, s2, s3, alldone ? "하였습니다" : "하고 있습니다");
    if (skill > SGOOD && thawed && !alldone) prout("저장된 게임은 상패 대상이 아닙니다.");
    if (tourn) printf("이것은 토너먼트 게임 %d번입니다.\n", tourn);
    if (f)     printf("비밀 암호는 \"%s\" 입니다.\n", passwd);

    printf("클링온 %d척 중 %d척 격파됨",
           inkling, d.killk + d.killc + d.nsckill);
    if (d.killc)
        printf(", 사령관 %d명 포함.\n", d.killc);
    else if (d.killk + d.nsckill > 0)
        prout(", 사령관은 없음.");
    else
        prout(".");

    if (skill > SFAIR)
        printf("슈퍼사령관은 %s격파되었습니다.\n", d.nscrem ? "아직 " : "");

    if (d.rembase != inbase) {
        proutn("스타베이스 ");
        if (inbase - d.rembase == 1) proutn("1곳이");
        else {
            crami(inbase - d.rembase, 1);
            proutn("곳이");
        }
        proutn(" 파괴됨, ");
        crami(d.rembase, 1);
        prout("곳 남음.");
    } else {
        printf("스타베이스 총 %d곳.\n", inbase);
    }

    if (REPORTS || iseenit) {
        attakreport();
        iseenit = 1;
    }

    if (casual)
        printf("아군 사상자 %d명 발생.\n", casual);

#ifdef CAPTURE
    if (brigcapacity != brigfree)
        printf("영창에 클링온 %d명 수감 중.\n", brigcapacity - brigfree);
    if (kcaptured > 0)
        printf("클링온 %d명을 스타플릿에 인계했습니다.\n", kcaptured);
#endif

    if (nhelp)
        printf("구조 요청 %d회 발신.\n", nhelp);

    if (ship == IHE) {
        proutn("심우주 탐침 ");
        if (nprobes) crami(nprobes, 1);
        else proutn("없음");
        prout("개 보유.");
    }
    if (REPORTS && future[FDSPROB] != 1e30) {
        if (isarmed) proutn("무장된 심우주 탐침이");
        else         proutn("심우주 탐침이");
        cramlc(1, probecx, probecy);
        prout("에 있습니다.");
    }
    if (icrystl) {
        if (cryprob <= .05)
            prout("딜리튬 결정 탑재 중...아직 미사용.");
        else {
            int i = 0;
            double ai = 0.05;
            while (cryprob > ai) { ai *= 2.0; i++; }
            printf("딜리튬 결정을 %d회 사용했습니다.\n", i);
        }
    }
    skip(1);
}

void lrscan(void) {
    int x, y;
    chew();
    if (damage[DLRSENS] != 0.0) {
        if (condit != IHDOCKED) {
            prout("장거리 센서 손상됨.");
            return;
        }
        skip(1);
        proutn("스타베이스 장거리 스캔 결과 -");
    } else {
        skip(1);
        proutn("장거리 스캔 결과 -");
    }
    cramlc(1, quadx, quady);
    skip(1);

    if (coordfixed)
        for (y = quady + 1; y >= quady - 1; y--) {
            for (x = quadx - 1; x <= quadx + 1; x++) {
                if (x == 0 || x > 8 || y == 0 || y > 8)
                    printf("   -1");
                else {
                    printf("%5d", d.galaxy[x][y]);
                    starch[x][y] = damage[DRADIO] > 0 ? d.galaxy[x][y]+1000 : 1;
                }
            }
            putchar('\n');
        }
    else
        for (x = quadx - 1; x <= quadx + 1; x++) {
            for (y = quady - 1; y <= quady + 1; y++) {
                if (x == 0 || x > 8 || y == 0 || y > 8)
                    printf("   -1");
                else {
                    printf("%5d", d.galaxy[x][y]);
                    starch[x][y] = damage[DRADIO] > 0 ? d.galaxy[x][y]+1000 : 1;
                }
            }
            putchar('\n');
        }
}

void dreprt(void) {
    int jdam = FALSE, i;
    chew();

    for (i = 1; i <= ndevice; i++) {
        if (damage[i] > 0.0) {
            if (!jdam) {
                skip(1);
                prout("장치                     -수리 시간-");
                prout("                     비행중     도킹중");
                jdam = TRUE;
            }
            printf("  %-20s ", device[i]);
            cramf(damage[i] + 0.05, 8, 2);
            proutn("  ");
            cramf(docfac * damage[i] + 0.005, 8, 2);
            skip(1);
        }
    }
    if (!jdam) prout("모든 장치 정상 작동 중.");
}

void chart(int nn) {
    int i, j;

    chew();
    skip(1);
    if (stdamtim != 1e30 && stdamtim != d.date && condit == IHDOCKED) {
        prout("스팍- \"스타베이스 기록을 바탕으로 성도를 업데이트했습니다.\"");
        skip(1);
    }
    if (nn == 0) prout("알려진 은하의 성도");
    if (stdamtim != 1e30) {
        if (condit == IHDOCKED) {
            stdamtim = d.date;
            for (i = 1; i <= 8; i++)
                for (j = 1; j <= 8; j++)
                    if (starch[i][j] == 1) starch[i][j] = d.galaxy[i][j] + 1000;
        } else {
            proutn("(마지막 업데이트: ");
            cramf(d.date - stdamtim, 0, 1);
            prout(" 스타데이트 전)");
        }
    }
    if (nn == 0) skip(1);

    prout("      1    2    3    4    5    6    7    8");
    prout("    ----------------------------------------");
    if (nn == 0) prout("  -");

    if (coordfixed)
        for (j = 8; j >= 1; j--) {
            printf("%d -", j);
            for (i = 1; i <= 8; i++) {
                if      (starch[i][j] < 0)   printf("  .1.");
                else if (starch[i][j] == 0)  printf("  ...");
                else if (starch[i][j] > 999) printf("%5d", starch[i][j]-1000);
                else                          printf("%5d", d.galaxy[i][j]);
            }
            prout("  -");
        }
    else
        for (i = 1; i <= 8; i++) {
            printf("%d -", i);
            for (j = 1; j <= 8; j++) {
                if      (starch[i][j] < 0)   printf("  .1.");
                else if (starch[i][j] == 0)  printf("  ...");
                else if (starch[i][j] > 999) printf("%5d", starch[i][j]-1000);
                else                          printf("%5d", d.galaxy[i][j]);
            }
            prout("  -");
        }

    if (nn == 0) {
        skip(1);
        crmshp();
        proutn("은(는) 현재");
        cramlc(1, quadx, quady);
        prout("에 있습니다.");
    }
}

void srscan(int l) {
    /* 영어 약어 (da=날짜, co=상태, po=위치, ls=생명지원, wa=워프계수,
     * en=에너지, to=어뢰, sh=실드, kl=클링온, ti=시간) */
    static char requests[][3] = {"","da","co","po","ls","wa","en","to","sh","kl","ti"};
    /* 한국어 대응 (인덱스 동일) */
    static const char *kor_requests[] = {
        "", "날짜", "상태", "위치", "생명지원", "워프계수",
        "에너지", "어뢰", "실드", "클링온", "시간"
    };
    char *cp;
    int leftside=TRUE, rightside=TRUE, i, j, jj, k=0, nn=FALSE;
    int goodScan = TRUE;

    switch (l) {
        case 1: /* SRSCAN */
            if (damage[DSRSENS] != 0) {
                if (condit != IHDOCKED) {
                    prout("단거리 센서 손상됨");
                    goodScan = FALSE;
                } else {
                    prout("[스타베이스 센서 사용 중]");
                }
            }
            if (goodScan)
                starch[quadx][quady] = damage[DRADIO] > 0.0 ?
                                       d.galaxy[quadx][quady]+1000 : 1;
            scan();
            if (isit("chart")) nn = TRUE;
            if (isit("no"))    rightside = FALSE;
            chew();
            prout("\n    1 2 3 4 5 6 7 8 9 10");
            break;

        case 2: /* REQUEST */
            while (scan() == IHEOL)
                printf("원하는 정보: ");
            chew();
            for (k = 1; k <= 10; k++) {
                size_t clen = strlen(citem);
                size_t klen = strlen(kor_requests[k]);
                /* 영어 약어 매칭 */
                if (strncmp(citem, requests[k], min(2, clen)) == 0) break;
                /* 한국어 접두어 매칭 (1글자 이상, 한국어 전체의 접두어) */
                if (clen >= 3 && clen <= klen &&
                    strncmp(citem, kor_requests[k], clen) == 0) break;
            }
            if (k > 10) {
                prout("알 수 없는 요청입니다. 유효한 요청:\n"
                      "  날짜, 상태, 위치, 생명지원, 워프계수,\n"
                      "  에너지, 어뢰, 실드, 클링온, 시간.");
                return;
            }
            /* fall through */

        case 3: /* STATUS */
            chew();
            leftside = FALSE;
            skip(1);
    }

    for (i = 1; i <= 10; i++) {
        jj = (k != 0 ? k : i);
        if (leftside) {
            if (coordfixed) {
                printf("%2d  ", 11-i);
                for (j = 1; j <= 10; j++) {
                    if (goodScan || (abs((11-i)-secty) <= 1 && abs(j-sectx) <= 1))
                        printf("%c ", quad[j][11-i]);
                    else
                        printf("- ");
                }
            } else {
                printf("%2d  ", i);
                for (j = 1; j <= 10; j++) {
                    if (goodScan || (abs(i-sectx) <= 1 && abs(j-secty) <= 1))
                        printf("%c ", quad[i][j]);
                    else
                        printf("- ");
                }
            }
        }

        if (rightside) {
            switch (jj) {
                case 1:
                    /* 스타데이트(10칸)+패딩2 = 12칸 */
                    printf(" 스타데이트  %.1f", d.date);
                    break;
                case 2:
                    if (condit != IHDOCKED) newcnd();
                    switch (condit) {
                        case IHRED:    cp = "적색경보"; break;
                        case IHGREEN:  cp = "녹색";     break;
                        case IHYELLOW: cp = "황색경보"; break;
                        case IHDOCKED: cp = "도킹중";   break;
                        default:       cp = "???";      break;
                    }
                    /* 상태(4칸)+패딩8 = 12칸 */
                    printf(" 상태        %s", cp);
#ifdef CLOAKING
                    if (iscloaked) printf(", 은폐중");
#endif
                    break;
                case 3:
                    /* 위치(4칸)+패딩8 = 12칸 */
                    printf(" 위치       ");
                    cramlc(0, quadx, quady);
                    putchar(',');
                    cramlc(0, sectx, secty);
                    break;
                case 4:
                    /* 생명유지(8칸)+패딩4 = 12칸 */
                    printf(" 생명유지    ");
                    if (damage[DLIFSUP] != 0.0) {
                        if (condit == IHDOCKED)
                            printf("손상(스타베이스 지원중)");
                        else
                            printf("손상, 예비=%.2f", lsupres);
                    } else {
                        printf("정상");
                    }
                    break;
                case 5:
                    /* 워프 속도(9칸)+패딩3 = 12칸 */
                    printf(" 워프 속도   %.1f", warpfac);
                    break;
                case 6:
                    /* 에너지(6칸)+패딩6 = 12칸 */
                    printf(" 에너지      %.2f", energy);
                    break;
                case 7:
                    /* 어뢰(4칸)+패딩8 = 12칸 */
                    printf(" 어뢰        %d", torps);
                    break;
                case 8:
                    /* 실드(4칸)+패딩8 = 12칸 */
                    printf(" 실드        ");
                    if      (damage[DSHIELD] != 0) printf("손상,");
                    else if (shldup)               printf("가동,");
                    else                           printf("해제,");
                    printf(" %d%% / %.1f유닛",
                           (int)((100.0*shield)/inshld + 0.5), shield);
                    break;
                case 9:
                    /* 클링온 잔존(11칸)+패딩1 = 12칸 */
                    printf(" 클링온 잔존 %d", d.remkl);
                    break;
                case 10:
                    /* 남은 시간(9칸)+패딩3 = 12칸 */
                    printf(" 남은 시간   %.2f", d.remtime);
                    break;
            }
        }
        skip(1);
        if (k != 0) return;
    }
    if (nn) chart(1);
}

void eta(void) {
    int key, ix1, ix2, iy1, iy2, prompt = FALSE;
    int wfl;
    double ttime, twarp, tpower;

    if (damage[DCOMPTR] != 0.0) {
        prout("컴퓨터 손상됨. 포켓 계산기를 사용하십시오.");
        skip(1);
        return;
    }
    if (scan() != IHREAL) {
        prompt = TRUE;
        chew();
        proutn("목적지 구역 및/또는 섹터? ");
        if (scan() != IHREAL) { huh(); return; }
    }
    iy1 = aaitem + 0.5;
    if (scan() != IHREAL) { huh(); return; }
    ix1 = aaitem + 0.5;
    if (scan() == IHREAL) {
        iy2 = aaitem + 0.5;
        if (scan() != IHREAL) { huh(); return; }
        ix2 = aaitem + 0.5;
    } else {
        ix2 = ix1; iy2 = iy1;
        ix1 = quady; iy1 = quadx;
    }

    if (ix1 > 8 || ix1 < 1 || iy1 > 8 || iy1 < 1 ||
        ix2 > 10|| ix2 < 1 || iy2 > 10|| iy2 < 1) {
        huh(); return;
    }
    dist = sqrt(square(iy1-quadx+0.1*(iy2-sectx)) +
                square(ix1-quady+0.1*(ix2-secty)));
    wfl = FALSE;

    if (prompt) prout("모르는 값은 \"아니오\"를 입력하세요:");
    while (TRUE) {
        chew();
        proutn("도착 시간 또는 날짜? ");
        if (scan() == IHREAL) {
            ttime = aaitem;
            if (ttime > d.date) ttime -= d.date;
            if (ttime <= 1e-10 ||
                (twarp=(floor(sqrt((10.0*dist)/ttime)*10.0)+1.0)/10.0) > 10) {
                prout("도달할 수 없습니다, 함장님.");
                chew(); return;
            }
            if (twarp < 1.0) twarp = 1.0;
            break;
        }
        chew();
        proutn("워프 계수? ");
        if (scan() == IHREAL) {
            wfl   = TRUE;
            twarp = aaitem;
            if (twarp < 1.0 || twarp > 10.0) { huh(); return; }
            break;
        }
        prout("함장님, 두 값 중 하나는 알려주셔야 합니다.");
    }

    while (TRUE) {
        chew();
        ttime  = (10.0 * dist) / square(twarp);
        tpower = dist * twarp * twarp * twarp * (shldup + 1);
        if (tpower >= energy) {
            prout("에너지 부족, 함장님: 필요한 양은 ");
            cramf(tpower, 1, 1);
            proutn(" 유닛입니다.");
            if (shldup == 0 || tpower > energy * 2.0) {
                if (!wfl) return;
                proutn("다른 워프 계수? ");
                if (scan() == IHREAL) {
                    wfl = TRUE; twarp = aaitem;
                    if (twarp < 1.0 || twarp > 10.0) { huh(); return; }
                    continue;
                } else { chew(); skip(1); return; }
            }
            prout("하지만 실드를 내리면,");
            proutn("잔여");
            tpower /= 2;
        } else {
            proutn("잔여");
        }
        proutn(" 에너지는 ");
        cramf(energy - tpower, 1, 1);
        prout("유닛입니다.");
        if (wfl) {
            proutn("스타데이트 ");
            cramf(d.date + ttime, 1, 1);
            prout("에 도착합니다.");
        } else if (twarp == 1.0) {
            prout("어느 워프 속도든 충분합니다.");
        } else {
            proutn("필요한 최소 워프 속도: ");
            cramf(twarp, 1, 2);
            skip(1);
            proutn("도착 스타데이트: ");
            cramf(d.date + ttime, 1, 2);
            prout(".");
        }
        if (d.remtime < ttime)
            prout("그때까지 연방은 멸망해 있을 것입니다.");
        if (twarp > 6.0)
            prout("그 속도는 위험합니다, 함장님.");
        if ((isatb==1 && d.isy==ix1 && d.isx==iy1 &&
             future[FSCDBAS] < ttime+d.date) ||
            (future[FCDBAS] < ttime+d.date && baty==ix1 && batx==iy1))
            prout("그 스타베이스는 그때까지 파괴될 것입니다.");
        proutn("다른 워프 계수? ");
        if (scan() == IHREAL) {
            wfl = TRUE; twarp = aaitem;
            if (twarp < 1.0 || twarp > 10.0) { huh(); return; }
        } else {
            chew(); skip(1); return;
        }
    }
}
