#include <time.h>
#include "sst.h"

/* ============================================================
 * setup.c - 게임 초기화 및 설정 (한국어 버전)
 * ============================================================ */

void prelim(void) {
    skip(2);
    prout("==================================================");
    prout("         -슈퍼- 스타 트렉 (SUPER STAR TREK)");
    prout("                               For JunHo");
    prout("==================================================");
    skip(1);
    prout("  우주 - 최후의 개척지. 이것은 스타쉽 엔터프라이즈의");
    prout("  항해 기록이다. 임무: 5년간 미지의 세계를 탐험하고,");
    prout("  새로운 생명체와 문명을 발견하며, 아무도 가지 않은");
    prout("  곳을 대담하게 나아가는 것.");
    skip(1);
}

/* 게임 저장 */
void freeze(int boss) {
    FILE *fp;
    int key;
    if (boss) {
        strcpy(citem, "emsave.trk");
    } else {
        if ((key = scan()) == IHEOL) {
            proutn("파일명(최대 9자): ");
            key = scan();
        }
        if (key != IHALPHA) {
            huh();
            return;
        }
        chew();
        if (strchr(citem, '.') == NULL) {
            strcat(citem, ".trk");
        }
    }
    if ((fp = fopen(citem, "wb")) == NULL) {
        proutn("파일을 저장할 수 없습니다: ");
        proutn(citem);
        skip(1);
        return;
    }
    fwrite(&d,       sizeof(d),       1, fp);
    fwrite(&snapsht, sizeof(snapsht), 1, fp);
    fwrite(quad,     sizeof(quad),    1, fp);
    fwrite(kx,       sizeof(kx),      1, fp);
    fwrite(ky,       sizeof(ky),      1, fp);
    fwrite(starch,   sizeof(starch),  1, fp);
    fwrite(kpower,   sizeof(kpower),  1, fp);
    fwrite(kdist,    sizeof(kdist),   1, fp);
    fwrite(kavgd,    sizeof(kavgd),   1, fp);
    fwrite(damage,   sizeof(damage),  1, fp);
    fwrite(future,   sizeof(future),  1, fp);
    fwrite(&a,       sizeof(a),       1, fp);
    fwrite(passwd,   sizeof(passwd),  1, fp);
    fclose(fp);
    proutn("게임이 저장되었습니다: ");
    proutn(citem);
    skip(1);
}

/* 게임 불러오기 */
void thaw(void) {
    FILE *fp;
    int key;

    passwd[0] = '\0';
    if ((key = scan()) == IHEOL) {
        proutn("파일명: ");
        key = scan();
    }
    if (key != IHALPHA) {
        huh();
        return;
    }
    chew();
    if (strchr(citem, '.') == NULL) {
        strcat(citem, ".trk");
    }
    if ((fp = fopen(citem, "rb")) == NULL) {
        proutn("게임 파일을 찾을 수 없습니다: ");
        proutn(citem);
        skip(1);
        return;
    }
    fread(&d,       sizeof(d),       1, fp);
    fread(&snapsht, sizeof(snapsht), 1, fp);
    fread(quad,     sizeof(quad),    1, fp);
    fread(kx,       sizeof(kx),      1, fp);
    fread(ky,       sizeof(ky),      1, fp);
    fread(starch,   sizeof(starch),  1, fp);
    fread(kpower,   sizeof(kpower),  1, fp);
    fread(kdist,    sizeof(kdist),   1, fp);
    fread(kavgd,    sizeof(kavgd),   1, fp);
    fread(damage,   sizeof(damage),  1, fp);
    fread(future,   sizeof(future),  1, fp);
    fread(&a,       sizeof(a),       1, fp);
    fread(passwd,   sizeof(passwd),  1, fp);
    fclose(fp);
}

/* 함선 포기 */
void abandn(void) {
    int nb, l;

    chew();
    if (condit == IHDOCKED) {
        if (ship != IHE) {
            prout("페어리 퀸을 포기할 수 없습니다.");
            return;
        }
    } else {
        if (damage[DSHUTTL] == -1) {
            prout("페어리 퀸에는 셔틀 크래프트가 없습니다.");
            return;
        }
        if (damage[DSHUTTL] < 0) {
            prout("셔틀 크래프트는 현재 수리 중입니다.");
            return;
        }
        if (damage[DSHUTTL] > 0) {
            prout("셔틀 크래프트가 손상되었습니다.");
            return;
        }
        if (landed == 1) {
            prout("엔터프라이즈에 탑승해야 합니다.");
            return;
        }
        if (iscraft != 1) {
            prout("셔틀 크래프트를 현재 사용할 수 없습니다.");
            return;
        }
        skip(1);
        prouts("***함선 포기!  함선 포기!");
        skip(1);
        prouts("***전원 함선 포기!");
        skip(2);
        prout("함장과 승무원들이 셔틀 크래프트로 탈출합니다.");
        prout("나머지 승무원들은 가장 가까운 거주 가능한");
        prout("행성으로 전송됩니다.");
        if (d.rembase == 0) {
            finish(FABANDN);
            return;
        }
        skip(1);
        icrystl = 0;
        nprobes = 0;
        prout("클링온에게 포로로 잡혔다가 포로교환으로");
        prout("연방에 인계되었습니다.");
        nb = Rand() * d.rembase + 1;
        if (quadx != d.baseqx[nb] || quady != d.baseqy[nb]) {
            quadx = d.baseqx[nb];
            quady = d.baseqy[nb];
            sectx = secty = 5;
            newqad(1);
        }
        for (;;) {
            quad[sectx][secty] = IHDOT;
            for (l = 1; l <= 10; l++) {
                sectx = 3.0 * Rand() - 1.0 + basex;
                secty = 3.0 * Rand() - 1.0 + basey;
                if (sectx >= 1 && sectx <= 10 &&
                    secty >= 1 && secty <= 10 &&
                    quad[sectx][secty] == IHDOT) break;
            }
            if (l < 11) break;
            sectx = 5;
            secty = 5;
            newqad(1);
        }
    }
    quad[sectx][secty] = ship = IHF;
    prout("스타플릿이 당신에게 구형이지만 아직 사용 가능한");
    prout("스타쉽 페어리 퀸의 지휘권을 부여했습니다.");
    if (icrystl != 0) prout("딜리튬 결정은 이전 함선에 남아 있습니다.");
    imine = 0;
    iscraft = 0;
#ifdef CAPTURE
    brigcapacity = 300;
    brigfree = brigcapacity;
#endif
#ifdef CLOAKING
    iscloaked = iscloaking = FALSE;
#endif
    condit = IHDOCKED;
    for (l = 1; l <= ndevice; l++) damage[l] = 0.0;
    damage[DSHUTTL] = -1;
    energy  = inenrg = 3000.0;
    shield  = inshld = 1250.0;
    torps   = intorps = 6;
    lsupres = inlsr   = 3.0;
    shldup  = 0;
    warpfac = 5.0;
    wfacsq  = 25.0;
}

/* 게임 설정 */
void setup(void) {
    int i, j, krem, klumper;
    int ix, iy;
    alldone = gamewon = 0;
#ifdef DEBUG
    idebug = 0;
#endif
    if (choose()) return;

    ship   = IHE;
    energy = inenrg = 5000.0;
    shield = inshld  = 2500.0;
    shldchg = shldup = 0;
    inlsr   = 4.0;
    lsupres = 4.0;
    iran8(&quadx, &quady);
    iran10(&sectx, &secty);
    torps   = intorps = 10;
    nprobes = (int)(3.0 * Rand() + 2.0);
    warpfac = 5.0;
    wfacsq  = warpfac * warpfac;
    for (i = 0; i <= ndevice; i++) damage[i] = 0.0;

    batx = baty = 0;
    d.date = indate = 100.0 * (int)(31.0 * Rand() + 20.0);
    d.killk = d.killc = nkinks = nhelp = resting = casual = d.nromkl = 0;
    isatb = iscate = imine = icrystl = icraft = d.nsckill = d.nplankl = d.starkl = 0;
    iscraft = 1;
    landed  = -1;
    alive   = 1;
    docfac  = 0.25;
    for (i = 1; i <= 8; i++)
        for (j = 1; j <= 8; j++) d.newstuf[i][j] = starch[i][j] = 0;

    future[FSNOVA]  = d.date + expran(0.5 * intime);
    future[FTBEAM]  = d.date + expran(1.5 * (intime / d.remcom));
    future[FSNAP]   = d.date + 1.0 + Rand();
    future[FBATTAK] = d.date + expran(0.3 * intime);
    future[FCDBAS]  = 1e30;
    future[FSCMOVE] = d.nscrem ? d.date + 0.2777 : 1e30;
    future[FSCDBAS] = 1e30;
    future[FDSPROB] = 1e30;
    stdamtim = 1e30;

    instar = 0;
    for (i = 1; i <= 8; i++)
        for (j = 1; j <= 8; j++) {
            int k = Rand() * 9.0 + 1.0;
            instar += k;
            d.galaxy[i][j] = k;
        }

    /* 스타베이스 배치 */
    for (i = 1; i <= inbase; i++) {
        int contflag;
        do {
            do iran8(&ix, &iy);
            while (d.galaxy[ix][iy] >= 10);
            contflag = FALSE;
            for (j = i - 1; j > 0; j--) {
                double distq = square(ix - d.baseqx[j]) + square(iy - d.baseqy[j]);
                if (distq < 6.0 * (6 - inbase) && Rand() < 0.75) {
                    contflag = TRUE;
                    break;
                }
            }
        } while (contflag);
        d.baseqx[i]  = ix;
        d.baseqy[i]  = iy;
        starch[ix][iy] = -1;
        d.galaxy[ix][iy] += 10;
    }

    /* 일반 클링온 배치 */
    krem = inkling - incom - d.nscrem;
    klumper = 0.25 * skill * (9.0 - length) + 1.0;
    if (klumper > 9) klumper = 9;
    do {
        double r = Rand();
        int klump = (1.0 - r * r) * klumper;
        if (klump > krem) klump = krem;
        krem  -= klump;
        klump *= 100;
        do iran8(&ix, &iy);
        while (d.galaxy[ix][iy] + klump >= 1000);
        d.galaxy[ix][iy] += klump;
    } while (krem > 0);

    /* 클링온 사령관 배치 */
    for (i = 1; i <= incom; i++) {
        do {
            do { iran8(&ix, &iy); }
            while ((d.galaxy[ix][iy] < 99 && Rand() < 0.75) ||
                   d.galaxy[ix][iy] > 899);
            for (j = 1; j < i; j++)
                if (d.cx[j] == ix && d.cy[j] == iy) break;
        } while (j < i);
        d.galaxy[ix][iy] += 100;
        d.cx[i] = ix;
        d.cy[i] = iy;
    }

    /* 행성 배치 */
    for (i = 1; i <= inplan; i++) {
        do iran8(&ix, &iy);
        while (d.newstuf[ix][iy] > 0);
        d.newstuf[ix][iy] = 1;
        d.plnets[i].x        = ix;
        d.plnets[i].y        = iy;
        d.plnets[i].pclass   = Rand() * 3.0 + 1.0;
        d.plnets[i].crystals = 1.5 * Rand();
        d.plnets[i].known    = 0;
    }

    /* 로뮬란 배치 */
    for (i = 1; i <= d.nromrem; i++) {
        iran8(&ix, &iy);
        d.newstuf[ix][iy] += 10;
    }

    /* 슈퍼사령관 배치 */
    if (d.nscrem > 0) {
        do iran8(&ix, &iy);
        while (d.galaxy[ix][iy] >= 900);
        d.isx = ix;
        d.isy = iy;
        d.galaxy[ix][iy] += 100;
    }

    if (Rand() < 0.1 && thingx != -1) {
        iran8(&thingx, &thingy);
    } else {
        thingx = thingy = 0;
    }

    skip(3);
    d.snap = 0;

    if (skill == SNOVICE) {
        printf("현재 스타데이트: %d\n", (int)d.date);
        printf("연방이 클링온의 침략군에 의해 공격받고 있습니다.\n");
        printf("스타쉽 엔터프라이즈의 함장으로서, 당신의 임무는\n");
        printf("클링온 침략군 %d척의 전투함을 찾아 격파하는 것입니다.\n", inkling);
        printf("\n초기 %d 스타데이트가 임무 완수에 주어집니다.\n", (int)intime);
        printf("임무 진행 중 추가 시간이 주어질 수도 있습니다.\n\n");
        printf("지원 스타베이스 %d곳 위치: ", inbase);
    } else {
        printf("스타데이트 %d\n\n", (int)d.date);
        printf("클링온 %d척,\n미확인 수의 로뮬란\n", inkling);
        if (d.nscrem) printf("그리고 슈퍼사령관 1명(!)이 출현합니다.\n");
        printf("%d 스타데이트\n스타베이스 %d곳 위치: ", (int)intime, inbase);
    }

    for (i = 1; i <= inbase; i++) {
        cramlc(0, d.baseqx[i], d.baseqy[i]);
        if (i < inbase) proutn("  ");
    }
    skip(2);
    proutn("엔터프라이즈 현재 위치:");
    cramlc(1, quadx, quady);
    proutn(" ");
    cramlc(2, sectx, secty);
    skip(2);
    prout("행운을 빕니다!");
    if (d.nscrem) proutn("  꼭 필요하실 겁니다.");
    skip(1);
    newqad(0);
    if (nenhere) shldup = 1.0;
    if (neutz) attack(0);
}

/* 게임 유형 및 난이도 선택 */
int choose(void) {
    tourn  = 0;
    thawed = 0;
    skill  = 0;
    length = 0;
    /* [버그수정] 이전 게임의 잔여 입력 버퍼 완전 초기화 */
    chew();
    citem[0] = '\0';
    /* 이전 게임의 잔여 입력 버퍼 완전 초기화 */
    chew();
    citem[0] = '\0';

    while (TRUE) {
        if (fromcommandline)
            fromcommandline = 0;
        else
            proutn("게임 유형 - 일반(일/regular), 토너먼트(토/tournament), 저장된게임(저/frozen): ");

        scan();
        if (strlen(citem) == 0) continue;

        if (isit("tournament") ||
            strcmp(citem,"토너먼트")==0 || strcmp(citem,"토너")==0 ||
            strcmp(citem,"토")==0) {
            while (scan() == IHEOL) {
                proutn("토너먼트 번호 입력: ");
            }
            if (aaitem == 0) { chew(); continue; }
            tourn   = (int)aaitem;
            thingx  = -1;
            srand((unsigned int)(int)aaitem);
            break;
        }
        if (isit("frozen") ||
            strcmp(citem,"저장")==0 || strcmp(citem,"불러오기")==0 ||
            strcmp(citem,"저")==0) {
            thaw();
            chew();
            if (*passwd == 0) continue;
            randomize();
            Rand(); Rand(); Rand(); Rand();
            if (!alldone) thawed = 1;
            report(1);
            return TRUE;
        }
        if (isit("regular") || isit("r") ||
            strcmp(citem,"일반")==0 || strcmp(citem,"일")==0) {
            skip(2);
            randomize();
            Rand(); Rand(); Rand(); Rand();
            break;
        }
        proutn("\"");
        proutn(citem);
        prout("\" 은(는) 알 수 없는 입력입니다.");
        chew();
    }

    while (length == 0 || skill == 0) {
        if (scan() == IHALPHA) {
            if      (isit("short")    || strcmp(citem,"단기")==0 || strcmp(citem,"단")==0) length = 1;
            else if (isit("medium")   || strcmp(citem,"중기")==0 || strcmp(citem,"중")==0) length = 2;
            else if (isit("long")     || strcmp(citem,"장기")==0 || strcmp(citem,"장")==0) length = 4;
            else if (isit("novice")   || strcmp(citem,"초보")==0 || strcmp(citem,"초")==0) skill  = SNOVICE;
            else if (isit("fair")     || strcmp(citem,"보통")==0 || strcmp(citem,"보")==0) skill  = SFAIR;
            else if (isit("good")     || strcmp(citem,"고수")==0 || strcmp(citem,"고")==0) skill  = SGOOD;
            else if (isit("expert")   || strcmp(citem,"전문가")==0|| strcmp(citem,"전")==0) skill = SEXPERT;
            else if (isit("emeritus") || strcmp(citem,"명예")==0 || strcmp(citem,"명")==0) skill  = SEMERITUS;
            else {
                proutn("\"");
                proutn(citem);
                prout("\" 은(는) 알 수 없는 입력입니다.");
            }
        } else {
            chew();
            if      (length == 0) proutn("게임 길이 선택 - 단기(단/short), 중기(중/medium), 장기(장/long): ");
            else if (skill  == 0) proutn("난이도 - 초보(초/novice), 보통(보/fair), 고수(고/good), 전문가(전/expert), 명예(명/emeritus): ");
        }
    }

    /* 이전 입력 버퍼를 완전히 비우고 새 줄에서 암호를 읽음 */
    chew();
    citem[0] = '\0';
    while (TRUE) {
        proutn("비밀 암호를 입력하세요(최대 9자, 자폭 시 필요): ");
        if (scan() == IHALPHA && citem[0] != '\0') {
            strncpy(passwd, citem, sizeof(passwd)-1);
            passwd[sizeof(passwd)-1] = '\0';
            chew();
            break;
        }
        chew();
        prout("암호가 비어있습니다. 한 단어(영문 또는 한글)를 입력하세요.");
    }

#ifdef DEBUG
    if (strcmp(passwd, "debug") == 0) idebug = 1;
#endif

    damfac      = 0.5 * skill;
    d.rembase   = 3.0 * Rand() + 2.0;
    inbase      = d.rembase;
    inplan      = (PLNETMAX / 2) + (PLNETMAX / 2 + 1) * Rand();
    d.nromrem   = (2.0 + Rand()) * skill;
    d.nscrem    = (skill > SFAIR ? 1 : 0);
    d.remtime   = 7.0 * length;
    intime      = d.remtime;
    d.remkl     = 2.0 * intime * ((skill + 1 - 2 * Rand()) * skill * 0.1 + .15);
    inkling     = d.remkl;
    incom       = skill + 0.0625 * inkling * Rand();
    d.remcom    = min(10, incom);
    incom       = d.remcom;
    d.remres    = (inkling + 4 * incom) * intime;
    inresor     = d.remres;
    if (inkling > 50) {
        inbase = (d.rembase += 1);
    }
#ifdef CAPTURE
    brigcapacity = 400;
    brigfree     = brigcapacity;
    kcaptured    = 0;
#endif
#ifdef CLOAKING
    ncviol      = 0;
    iscloaked   = FALSE;
    iscloaking  = FALSE;
#endif
    return FALSE;
}

/* 빈 공간에 개체 배치 */
void dropin(int iquad, int *ix, int *iy) {
    do iran10(ix, iy);
    while (quad[*ix][*iy] != IHDOT);
    quad[*ix][*iy] = iquad;
}

/* 함선 상태 갱신 */
void newcnd(void) {
    condit = IHGREEN;
    if (energy < 1000.0) condit = IHYELLOW;
    if (d.galaxy[quadx][quady] > 99 || d.newstuf[quadx][quady] > 9)
        condit = IHRED;
}

/* 적 거리 정렬 (버블 정렬) */
void sortkl(void) {
    double t;
    int sw, j, k;

    if (nenhere < 2) return;
    do {
        sw = FALSE;
        for (j = 1; j < nenhere; j++) {
            if (kdist[j] > kdist[j + 1]) {
                sw = TRUE;
                t = kdist[j]; kdist[j] = kdist[j+1]; kdist[j+1] = t;
                t = kavgd[j]; kavgd[j] = kavgd[j+1]; kavgd[j+1] = t;
                k = kx[j];   kx[j]    = kx[j+1];    kx[j+1]    = k;
                k = ky[j];   ky[j]    = ky[j+1];    ky[j+1]    = k;
                t = kpower[j]; kpower[j] = kpower[j+1]; kpower[j+1] = t;
            }
        }
    } while (sw);
}

/* 새 구역 진입 설정 */
void newqad(int shutup) {
    int quadnum = d.galaxy[quadx][quady];
    int newnum  = d.newstuf[quadx][quady];
    int i, j, ix, iy, nplan;

    iattak  = 1;
    justin  = 1;
    basex   = basey  = 0;
    klhere  = 0;
    comhere = 0;
    plnetx  = plnety = 0;
    ishere  = 0;
    irhere  = 0;
    iplnet  = 0;
    nenhere = 0;
    neutz   = 0;
    inorbit = 0;
    landed  = -1;
    ientesc = 0;
    ithere  = 0;
    iseenit = 0;

#ifdef CLOAKING
    isviolreported = FALSE;
#endif

    if (iscate) {
        iscate  = 0;
        ientesc = 1;
    }

    for (i = 1; i <= 10; i++)
        for (j = 1; j <= 10; j++) quad[i][j] = IHDOT;

    /* [버그수정] 심벌을 먼저 배치 — 초신성 구역이어도 atover() 호출까지 표시됨 */
    quad[sectx][secty] = ship;

    if (quadnum > 999) return;  /* 초신성 구역: 심벌만 남기고 종료 */

    klhere  = quadnum / 100;
    irhere  = newnum / 10;
    nplan   = newnum % 10;
    nenhere = klhere + irhere;

    /* 솔리안 배치 결정 */
    if ((skill <  SGOOD && Rand() <= 0.02) ||
        (skill == SGOOD && Rand() <= 0.05) ||
        (skill >  SGOOD && Rand() <= 0.08)) {
        do {
            ithx = Rand() > 0.5 ? 10 : 1;
            ithy = Rand() > 0.5 ? 10 : 1;
        } while (quad[ithx][ithy] != IHDOT);
        quad[ithx][ithy] = IHT;
        ithere = 1;
        if (quad[1][1]   == IHDOT) quad[1][1]   = 'X';
        if (quad[1][10]  == IHDOT) quad[1][10]  = 'X';
        if (quad[10][1]  == IHDOT) quad[10][1]  = 'X';
        if (quad[10][10] == IHDOT) quad[10][10] = 'X';
    }

    if (quadnum >= 100) {
        quadnum -= 100 * klhere;
        for (i = 1; i <= klhere; i++) {
            dropin(IHK, &ix, &iy);
            kx[i]    = ix;
            ky[i]    = iy;
            kdist[i] = kavgd[i] = sqrt(square(sectx-ix) + square(secty-iy));
            kpower[i] = Rand() * 150.0 + 300.0 + 25.0 * skill;
        }
        for (i = 1; i <= d.remcom; i++)
            if (d.cx[i] == quadx && d.cy[i] == quady) break;
        if (i <= d.remcom) {
            quad[ix][iy] = IHC;
            kpower[klhere] = 950.0 + 400.0 * Rand() + 50.0 * skill;
            comhere = 1;
        }
        if (quadx == d.isx && quady == d.isy) {
            quad[kx[1]][ky[1]] = IHS;
            kpower[1] = 1175.0 + 400.0 * Rand() + 125.0 * skill;
            iscate = 1;
            ishere = 1;
        }
    }

    for (i = klhere + 1; i <= nenhere; i++) {
        dropin(IHR, &ix, &iy);
        kx[i]    = ix;
        ky[i]    = iy;
        kdist[i] = kavgd[i] = sqrt(square(sectx-ix) + square(secty-iy));
        kpower[i] = Rand() * 400.0 + 450.0 + 50.0 * skill;
    }
    sortkl();

    if (quadnum >= 10) {
        quadnum -= 10;
        dropin(IHB, &basex, &basey);
    }

    if (nplan) {
        for (i = 1; i <= inplan; i++)
            if (d.plnets[i].x == quadx && d.plnets[i].y == quady) break;
        if (i <= inplan) {
            iplnet = i;
            dropin(IHP, &plnetx, &plnety);
        }
    }

    newcnd();

    for (i = 1; i <= quadnum; i++) dropin(IHSTAR, &ix, &iy);

    /* 로뮬란 중립 구역 확인 */
    if (irhere > 0 && klhere == 0 && basex == 0) {
        neutz = 1;
        if (REPORTS) {
            skip(1);
            prout("우후라 중위- \"함장님, 긴급 메시지가 수신됩니다.");
            prout("  오디오로 연결하겠습니다.\"  딸깍");
            skip(1);
            prout("침입자! 당신은 로뮬란 중립 구역을 침범했습니다.");
            prout("즉시 떠나십시오, 그렇지 않으면 격파될 것입니다!");
        }
    }

    if (shutup == 0) {
        if (thingx == quadx && thingy == quady) {
            dropin(IHQUEST, &ix, &iy);
            thingx = thingy = 0;
            if (damage[DSRSENS] == 0.0) {
                skip(1);
                prout("스팍- \"함장님, 이것은 매우 이례적인 상황입니다.");
                prout("    단거리 스캔을 확인해 주시기 바랍니다.\"");
            }
        }
    }

    /* 블랙홀 배치 */
    for (i = 1; i <= 3; i++)
        if (Rand() > 0.5) dropin(IHBLANK, &ix, &iy);

    if (ithere) {
        if (quad[1][1]   == 'X') quad[1][1]   = IHDOT;
        if (quad[1][10]  == 'X') quad[1][10]  = IHDOT;
        if (quad[10][1]  == 'X') quad[10][1]  = IHDOT;
        if (quad[10][10] == 'X') quad[10][10] = IHDOT;
    }
}
