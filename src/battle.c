#include "sst.h"
#include <math.h>

/* ============================================================
 * battle.c - 전투 시스템 (한국어 버전)
 * ============================================================ */

#ifdef CLOAKING
void cloak(void) {
    int key;
    enum {NONE, CLON, CLOFF} action = NONE;

    if (ship == IHF) {
        prout("페어리 퀸에는 클로킹 장치가 없습니다.");
        return;
    }
    key = scan();
    if (key == IHREAL) return;
    if (key == IHALPHA) {
        if (isit("on") || isit("켜기")) {
            if (iscloaked) { prout("클로킹 장치가 이미 켜져 있습니다."); return; }
            action = CLON;
        } else if (isit("off") || isit("끄기")) {
            if (!iscloaked) { prout("클로킹 장치가 이미 꺼져 있습니다."); return; }
            action = CLOFF;
        } else { huh(); return; }
    } else {
        if (!iscloaked) {
            proutn("클로킹 장치를 켜겠습니까? ");
            if (ja() == 0) return;
            action = CLON;
        }
        if (iscloaked) {
            proutn("클로킹 장치를 끄겠습니까? ");
            if (ja() == 0) return;
            action = CLOFF;
        }
        if (action == NONE) return;
    }
    if (action == CLOFF) {
        if (irhere && d.date >= ALGERON && !isviolreported) {
            prout("스팍- \"함장님, 알게론 조약이 유효합니다. 정말 괜찮겠습니까?\"");
            if (ja() == 0) return;
        }
        prout("엔지니어 스콧- \"예, 알겠습니다.\"");
        iscloaked = FALSE;
        if (irhere && d.date >= ALGERON && !isviolreported) {
            prout("로뮬란 함선이 알게론 조약 위반을 탐지했습니다!");
            ncviol++;
            isviolreported = TRUE;
        }
        return;
    }
    if (damage[DCLOAK] != 0) {
        prout("엔지니어 스콧- \"클로킹 장치가 손상되었습니다.\"");
        return;
    }
    if (condit == IHDOCKED) {
        prout("도킹 중에는 클로킹을 사용할 수 없습니다.");
        return;
    }
    if (d.date >= ALGERON && !isviolreported) {
        prout("스팍- \"함장님, 클로킹 장치 사용은 알게론 조약 위반입니다.");
        proutn("  그래도 진행하시겠습니까? ");
        if (ja() == 0) return;
    }
    prout("엔지니어 스콧- \"클로킹 장치가 작동되었습니다.\"");
    iscloaking = TRUE;
    if (irhere && d.date >= ALGERON && !isviolreported) {
        prout("로뮬란 함선이 알게론 조약 위반을 탐지했습니다!");
        ncviol++;
        isviolreported = TRUE;
    }
}
#endif

/* 실드 제어 */
void sheild(int i) {
    int key;
    enum {NONE, SHUP, SHDN, NRG} action = NONE;

    ididit = 0;
    if (i == 2) action = SHUP;
    else {
        key = scan();
        if (key == IHALPHA) {
            if (isit("transfer") || strcmp(citem,"전환")==0 || strcmp(citem,"전")==0) action = NRG;
            else {
                chew();
                if (damage[DSHIELD]) { prout("실드 손상 및 해제됨."); return; }
                if      (isit("up") || strcmp(citem,"올리기")==0 || strcmp(citem,"올")==0) action = SHUP;
                else if (isit("down") || strcmp(citem,"내리기")==0 || strcmp(citem,"내")==0) action = SHDN;
            }
        }
        if (action == NONE) {
            proutn("실드 에너지를 변경하시겠습니까? ");
            if (ja()) {
                proutn("실드에 전환할 에너지량: ");
                action = NRG;
            } else if (damage[DSHIELD]) {
                prout("실드 손상 및 해제됨.");
                return;
            } else if (shldup) {
                proutn("실드가 올라가 있습니다. 내리겠습니까? ");
                if (ja()) action = SHDN;
                else { chew(); return; }
            } else {
                proutn("실드가 내려가 있습니다. 올리겠습니까? ");
                if (ja()) action = SHUP;
                else { chew(); return; }
            }
        }
    }

    switch (action) {
        case SHUP:
            if (shldup) { prout("실드가 이미 올라가 있습니다."); return; }
            shldup  = 1;
            shldchg = 1;
            if (condit != IHDOCKED) energy -= 50.0;
            prout("실드 올림.");
            if (energy <= 0) {
                skip(1);
                prout("실드 가동으로 마지막 에너지 소진.");
                finish(FNRG);
                return;
            }
            ididit = 1;
            return;
        case SHDN:
            if (!shldup) { prout("실드가 이미 내려가 있습니다."); return; }
            shldup  = 0;
            shldchg = 1;
            prout("실드 내림.");
            ididit  = 1;
            return;
        case NRG:
            while (scan() != IHREAL) {
                chew();
                proutn("실드에 전환할 에너지량: ");
            }
            chew();
            if (aaitem == 0) return;
            if (aaitem > energy) { prout("함선 에너지 부족."); return; }
            ididit = 1;
            if (shield + aaitem >= inshld) {
                prout("실드 에너지 최대.");
                if (shield + aaitem > inshld) prout("초과 에너지는 함선으로 반환됩니다.");
                energy -= inshld - shield;
                shield  = inshld;
                return;
            }
            if (aaitem < 0.0 && energy - aaitem > inenrg) {
                skip(1);
                prout("엔지니어링 → 브릿지--");
                prout("  스콧입니다. 전력 회로 문제로 실드를 드레인할 수 없습니다.");
                ididit = 0;
                return;
            }
            if (shield + aaitem < 0) {
                prout("모든 실드 에너지를 함선으로 전환.");
                energy += shield;
                shield  = 0.0;
                return;
            }
            proutn("스콧- \"");
            if (aaitem > 0) prout("실드로 에너지를 전환합니다.\"");
            else            prout("실드에서 에너지를 드레인합니다.\"");
            shield += aaitem;
            energy -= aaitem;
            return;
        case NONE:
            break;
    }
}

/* 충돌 처리 */
void ram(int ibumpd, int ienm, int ix, int iy) {
    double type = 1.0, extradm;
    int icas, l;

    prouts("***적색경보!  적색경보!");
    skip(1);
    prout("***충돌 임박.");
    skip(2);
    proutn("***");
    crmshp();
    switch (ienm) {
        case IHR: type = 1.5; break;
        case IHC: type = 2.0; break;
        case IHS: type = 2.5; break;
        case IHT: type = 0.5; break;
    }
    proutn(ibumpd ? "이(가) 충돌당함: " : "이(가) 충돌: ");
    crmena(0, ienm, 2, ix, iy);
    if (ibumpd) proutn(" (원래 위치)");
    skip(1);
    deadkl(ix, iy, ienm, sectx, secty);
    proutn("***");
    crmshp();
    prout("이(가) 심각하게 손상되었습니다.");
    icas = 10.0 + 20.0 * Rand();
    proutn("***의무실 보고: 사상자 ");
    crami(icas, 1);
    prout("명.");
    casual += icas;
    for (l = 1; l <= ndevice; l++) {
        if (l == DDRAY) continue;
        if (damage[l] < 0) continue;
        extradm = (10.0 * type * Rand() + 1.0) * damfac;
        damage[l] += Time + extradm;
    }
    shldup = 0;
    if (d.remkl) {
        pause_game(2);
        dreprt();
    } else finish(FWON);
}

/* 어뢰 발사 처리 */
void torpedo(double course, double r, int inx, int iny, double *hit) {
    int l, iquad, ix, iy, jx, jy, shoved = 0, ll;
    double ac = course + 0.25 * r;
    double angle    = (15.0 - ac)     * 0.5235988;
    double bullseye = (15.0 - course) * 0.5235988;
    double deltax = -sin(angle), deltay = cos(angle), x = inx, y = iny, bigger;
    double ang, temp, xx, yy, kp, h1;

    bigger = fabs(deltax);
    if (fabs(deltay) > bigger) bigger = fabs(deltay);
    deltax /= bigger;
    deltay /= bigger;

    for (l = 1; l <= 15; l++) {
        x += deltax; ix = x + 0.5;
        if (ix < 1 || ix > 10) break;
        y += deltay; iy = y + 0.5;
        if (iy < 1 || iy > 10) break;
        if (l == 4 || l == 9) skip(1);
        cramf(x, 0, 1); proutn(" - "); cramf(y, 0, 1); proutn("   ");
        iquad = quad[ix][iy];
        if (iquad == IHDOT) continue;
        skip(1);
        switch (iquad) {
            case IHE:
            case IHF:
                skip(1);
                proutn("어뢰 명중: ");
                crmshp(); prout(".");
                *hit = fabs(700.0 + 100.0*Rand() -
                            1000.0*sqrt(square(ix-inx)+square(iy-iny))*
                            fabs(sin(bullseye-angle)));
                newcnd();
                if (landed == 1) return;
                ang  = angle + 2.5*(Rand()-0.5);
                temp = fabs(sin(ang));
                if (fabs(cos(ang)) > temp) temp = fabs(cos(ang));
                xx = -sin(ang)/temp; yy = cos(ang)/temp;
                jx = ix+xx+0.5;  jy = iy+yy+0.5;
                if (jx<1||jx>10||jy<1||jy>10) return;
                if (quad[jx][jy] == IHBLANK) { finish(FHOLE); return; }
                if (quad[jx][jy] != IHDOT)   return;
                sectx = jx; secty = jy;
                crmshp();
                shoved = 1;
                break;
            case IHC:
            case IHS:
                if (Rand() <= 0.05) {
                    crmena(1, iquad, 2, ix, iy);
                    prout("이(가) 반양성자 장치를 사용합니다;");
                    prout("   어뢰가 무력화되었습니다.");
                    return;
                }
            case IHR:
            case IHK:
                for (ll = 1; ll <= nenhere; ll++)
                    if (ix == kx[ll] && iy == ky[ll]) break;
                kp = fabs(kpower[ll]);
                h1 = fabs(700.0 + 100.0*Rand() -
                          1000.0*sqrt(square(ix-inx)+square(iy-iny))*
                          fabs(sin(bullseye-angle)));
                if (kp < h1) h1 = kp;
                kpower[ll] -= (kpower[ll] < 0 ? -h1 : h1);
                if (kpower[ll] == 0) { deadkl(ix,iy,iquad,ix,iy); return; }
                crmena(1, iquad, 2, ix, iy);
                ang  = angle + 2.5*(Rand()-0.5);
                temp = fabs(sin(ang));
                if (fabs(cos(ang)) > temp) temp = fabs(cos(ang));
                xx = -sin(ang)/temp; yy = cos(ang)/temp;
                jx = ix+xx+0.5; jy = iy+yy+0.5;
                if (jx<1||jx>10||jy<1||jy>10) { prout("이(가) 손상되었으나 파괴되지 않음."); return; }
                if (quad[jx][jy] == IHBLANK) { prout("이(가) 블랙홀로 빨려들어갔습니다."); deadkl(ix,iy,iquad,jx,jy); return; }
                if (quad[jx][jy] != IHDOT)   { prout("이(가) 손상되었으나 파괴되지 않음."); return; }
                prout("이(가) 손상됨--");
                kx[ll] = jx; ky[ll] = jy; shoved = 1;
                break;
            case IHB:
                prout("***스타베이스 파괴됨..");
                if (starch[quadx][quady] < 0) starch[quadx][quady] = 0;
                for (ll = 1; ll <= d.rembase; ll++)
                    if (d.baseqx[ll]==quadx && d.baseqy[ll]==quady) {
                        d.baseqx[ll] = d.baseqx[d.rembase];
                        d.baseqy[ll] = d.baseqy[d.rembase];
                        break;
                    }
                quad[ix][iy] = IHDOT;
                d.rembase--;
                basex = basey = 0;
                d.galaxy[quadx][quady] -= 10;
                d.basekl++;
                newcnd();
                return;
            case IHP:
                crmena(1, iquad, 2, ix, iy);
                prout("이(가) 파괴되었습니다.");
                d.nplankl++;
                d.newstuf[quadx][quady] -= 1;
                d.plnets[iplnet] = nulplanet;
                iplnet = plnetx = plnety = 0;
                quad[ix][iy] = IHDOT;
                if (landed == 1) finish(FDPLANET);
                return;
            case IHSTAR:
                if (Rand() > 0.10) { nova(ix, iy); return; }
                crmena(1, IHSTAR, 2, ix, iy);
                prout("이(가) 광자 폭발에 영향을 받지 않습니다.");
                return;
            case IHQUEST:
                skip(1);
                prouts("아아아이이이이이이에에에에에아아아아우우우으으으흐흐흐흐!!!");
                skip(1);
                prouts("    이크!     이크!    이크!        *캑!*  ");
                skip(1);
                proutn("스팍-");
                prouts("  \"흥미롭군요!\"");
                skip(1);
                quad[ix][iy] = IHDOT;
                return;
            case IHBLANK:
                skip(1);
                crmena(1, IHBLANK, 2, ix, iy);
                prout("이(가) 어뢰를 흡수했습니다.");
                return;
            case IHWEB:
                skip(1);
                prout("***어뢰가 솔리안 웹에 흡수되었습니다.");
                return;
            case IHT:
                skip(1);
                crmena(1, IHT, 2, ix, iy);
                h1 = fabs(700.0+100.0*Rand()-
                          1000.0*sqrt(square(ix-inx)+square(iy-iny))*
                          fabs(sin(bullseye-angle)));
                if (h1 >= 600) { prout("이(가) 파괴되었습니다."); quad[ix][iy]=IHDOT; ithere=ithx=ithy=0; return; }
                if (Rand() > 0.05) { prout("이(가) 광자 폭발에서 살아남았습니다."); return; }
                prout("이(가) 사라졌습니다.");
                quad[ix][iy] = IHWEB; ithere=ithx=ithy=0;
                { int dum, my; dropin(IHBLANK, &dum, &my); }
                return;
            default:
                skip(1);
                proutn("충돌 처리 불가: ");
                crmena(1, iquad, 2, ix, iy);
                skip(1);
                return;
        }
        break;
    }
    if (shoved) {
        quad[jx][jy] = iquad;
        quad[ix][iy] = IHDOT;
        proutn("이(가) 폭발로");
        cramlc(2, jx, jy);
        prout("로 이동됨.");
        skip(1);
        for (ll = 1; ll <= nenhere; ll++)
            kdist[ll] = kavgd[ll] = sqrt(square(sectx-kx[ll])+square(secty-ky[ll]));
        sortkl();
        return;
    }
    skip(1);
    prout("어뢰 빗나감.");
}

static void fry(double hit) {
    double ncrit, extradm;
    int ktr=1, l, ll, j, cdam[6], crptr;

    if (hit < (275.0-25.0*skill)*(1.0+0.5*Rand())) return;
    ncrit = 1.0 + hit/(500.0+100.0*Rand());
    proutn("***치명적 명중--");
    for (l = 1; l <= (int)ncrit; l++) {
        do {
            j = ndevice*Rand()+1.0;
        } while (damage[j] < 0.0 || (j==DSHUTTL && iscraft!=1) ||
#ifdef CLOAKING
                 (j==DCLOAK && ship!=IHE) ||
#endif
                 j==DDRAY);
        cdam[l] = j;
        extradm = (hit*damfac)/(ncrit*(75.0+25.0*Rand()));
        damage[j] += extradm;
        if (l > 1) {
            for (ll=2; ll<=l && j!=cdam[ll-1]; ll++);
            if (ll <= l) continue;
            ktr += 1;
            if (ktr == 3) skip(1);
            proutn(" 및 ");
        }
        proutn(device[j]);
    }
    prout("이(가) 손상되었습니다.");
    if (damage[DSHIELD] && shldup) {
        prout("***실드 강제 해제됨.");
        shldup = 0;
    }
#ifdef CLOAKING
    if (damage[DCLOAK] && iscloaked) {
        prout("***클로킹 장치 작동 불능.");
        iscloaked = FALSE;
    }
#endif
}

/* 적의 공격 처리 */
void attack(int k) {
    int percent, ihurt=0, l, i=0, jx, jy, iquad, itflag;
    int atackd=0, attempt=0;
    double hit;
    double pfac, dustfac, hitmax=0.0, hittot=0.0, chgfac=1.0, r;

#ifdef CLOAKING
    if (iscloaked && !iscloaking) return;
#endif
    iattak = 1;
    if (alldone) return;
    if (ithere) movetho();
    if (neutz) { neutz = 0; return; }
    if (((comhere || ishere) && (justin==0)) || skill==SEMERITUS) movcom();
    if (nenhere == 0) return;

    pfac = 1.0/inshld;
    if (shldchg == 1) chgfac = 0.25 + 0.5*Rand();
    skip(1);
    if (skill <= SFAIR) i = 2;

    for (l = 1; l <= nenhere; l++) {
        if (kpower[l] < 0) continue;
        r = Rand();
        if (condit == IHDOCKED) r *= 0.25;
        if (kpower[l] < 500)   r *= 0.25;
        jx = kx[l]; jy = ky[l];
        iquad = quad[jx][jy];
        itflag = (iquad==IHK && r>0.0005) || k==0 ||
                 (iquad==IHC && r>0.015)  ||
                 (iquad==IHR && r>0.3)    ||
                 (iquad==IHS && r>0.07);

        if (itflag) {
            if (condit == IHDOCKED) continue;
            attempt = 1;
            dustfac = 0.8 + 0.05*Rand();
            hit = kpower[l]*pow(dustfac, kavgd[l]);
            kpower[l] *= 0.75;
        } else {
            double course = 1.90985*atan2((double)secty-jy, (double)jx-sectx);
            hit = 0;
            proutn("***어뢰 발사됨");
            if (damage[DSRSENS] <= 0.0) {
                proutn(" - 발사원: ");
                crmena(0, iquad, i, jx, jy);
            }
            attempt = 1;
            prout("--");
            r = (Rand()+Rand())*0.5 - 0.5;
            r += 0.002*kpower[l]*r;
            torpedo(course, r, jx, jy, &hit);
            if (d.remkl == 0) finish(FWON);
            if (d.galaxy[quadx][quady]==1000 || alldone) return;
            if (hit == 0) continue;
        }
        if (shldup != 0 || shldchg != 0) {
            double absorb, hitsh, propor = pfac*shield;
            if (propor < 0.1) propor = 0.1;
            hitsh = propor*chgfac*hit + 1.0;
            atackd = 1;
            absorb = 0.8*hitsh;
            if (absorb > shield) absorb = shield;
            shield -= absorb;
            hit -= hitsh;
            if (propor > 0.1 && hit < 0.005*energy) continue;
        }
        atackd = 1;
        ihurt  = 1;
        cramf(hit, 0, 2);
        proutn("유닛 피격");
        if ((damage[DSRSENS]>0 && itflag) || skill<=SFAIR) {
            proutn(" - ");
            crmshp();
        }
        if (damage[DSRSENS]<=0.0 && itflag) {
            proutn(" - 발사원: ");
            crmena(0, iquad, i, jx, jy);
        }
        skip(1);
        if (hit > hitmax) hitmax = hit;
        hittot += hit;
        fry(hit);
        energy -= hit;
    }
    if (energy <= 0) { finish(FBATTLE); return; }
    if (attempt==0 && condit==IHDOCKED)
        prout("***적이 공격을 포기했습니다.");
    if (!atackd) return;
    percent = 100.0*pfac*shield + 0.5;
    if (!ihurt) {
        proutn("적의 공격으로 실드 강도가 ");
    } else {
        skip(1);
        proutn("남은 에너지: ");
        cramf(energy, 0, 2);
        proutn("    실드: ");
        if      (shldup)           proutn("가동, ");
        else if (damage[DSHIELD]==0) proutn("해제, ");
        else                       proutn("손상, ");
    }
    crami(percent, 1);
    proutn("%   남은 어뢰: ");
    crami(torps, 1);
    skip(1);
    if (hitmax >= 200 || hittot >= 500) {
        int icas = hittot*Rand()*0.015;
        if (icas >= 2) {
            skip(1);
            proutn("맥코이 박사- \"의무실에서 브릿지로. 이번 공격으로 사상자 ");
            crami(icas, 1);
            prout("명 발생");
            prout("   에 대해 보고합니다.\"");
            casual += icas;
        }
    }
    for (l = 1; l <= nenhere; l++) kavgd[l] = kdist[l];
    sortkl();
}

/* 적 격파 처리 */
void deadkl(int ix, int iy, int type, int ixx, int iyy) {
    int i, j;

    crmena(1, type, 2, ixx, iyy);
    if (type == IHR) {
        d.newstuf[quadx][quady] -= 10;
        irhere--;
        d.nromkl++;
        d.nromrem--;
    } else if (type == IHT) {
        ithere = 0;
    } else {
        d.galaxy[quadx][quady] -= 100;
        klhere--;
        d.remkl--;
        switch (type) {
            case IHC:
                comhere = 0;
                for (i=1; i<=d.remcom; i++)
                    if (d.cx[i]==quadx && d.cy[i]==quady) break;
                d.cx[i] = d.cx[d.remcom];
                d.cy[i] = d.cy[d.remcom];
                d.cx[d.remcom] = d.cy[d.remcom] = 0;
                d.remcom--;
                future[FTBEAM] = 1e30;
                if (d.remcom != 0)
                    future[FTBEAM] = d.date + expran(1.0*incom/d.remcom);
                d.killc++;
                break;
            case IHK: d.killk++; break;
            case IHS:
                d.nscrem = ishere = d.isx = d.isy = isatb = iscate = 0;
                d.nsckill = 1;
                future[FSCMOVE] = future[FSCDBAS] = 1e30;
                break;
        }
    }
    prout("이(가) 격파되었습니다.");
    quad[ix][iy] = IHDOT;
    if (d.remkl == 0) return;
    d.remtime = d.remres / (d.remkl + 4*d.remcom);
    if (type == IHT) return;
    for (i=1; i<=nenhere; i++)
        if (kx[i]==ix && ky[i]==iy) break;
    nenhere--;
    if (i <= nenhere) {
        for (j=i; j<=nenhere; j++) {
            kx[j]=kx[j+1]; ky[j]=ky[j+1];
            kpower[j]=kpower[j+1];
            kavgd[j]=kdist[j]=kdist[j+1];
        }
    }
    kx[nenhere+1]=ky[nenhere+1]=0;
    kdist[nenhere+1]=kavgd[nenhere+1]=kpower[nenhere+1]=0;
}

static int targetcheck(double x, double y, double *course) {
    double deltx, delty;
    if (x<1.0||x>10.0||y<1.0||y>10.0) { huh(); return 1; }
    deltx = 0.1*(y-secty);
    delty = 0.1*(sectx-x);
    if (deltx==0 && delty==0) {
        skip(1);
        prout("스팍- \"브릿지에서 의무실로. 맥코이 박사,");
        prout("  함장의 심리 상태를 즉시 점검할 것을 권합니다.");
        chew();
        return 1;
    }
    *course = 1.90985932*atan2(deltx, delty);
    return 0;
}

/* 광자 어뢰 발사 */
void photon(void) {
    double targ[4][3], course[4];
    double r, dummy;
    int key, n, i, osuabor;

    ididit = 0;
    if (damage[DPHOTON]) { prout("광자 튜브 손상됨."); chew(); return; }
    if (torps == 0)      { prout("어뢰가 없습니다."); chew(); return; }

    key = scan();
    for (;;) {
        if (key == IHALPHA) { huh(); return; }
        else if (key == IHEOL) {
            crami(torps, 1);
            prout("개의 어뢰가 남아 있습니다.");
            proutn("발사할 어뢰 수: ");
            key = scan();
        } else {
            n = aaitem + 0.5;
            if (n <= 0) { chew(); return; }
            if (n > 3)  prout("한 번에 최대 3발까지 발사 가능합니다.");
            else if (n <= torps) break;
            chew(); key = IHEOL;
        }
    }
    for (i = 1; i <= n; i++) {
        key = scan();
        if (i==1 && key==IHEOL) break;
        if (i==2 && key==IHEOL) {
            while (i <= n) { targ[i][1]=targ[1][1]; targ[i][2]=targ[1][2]; course[i]=course[1]; i++; }
            break;
        }
        if (key != IHREAL) { huh(); return; }
        targ[i][1] = aaitem;
        key = scan();
        if (key != IHREAL) { huh(); return; }
        targ[i][2] = aaitem;
        if (targetcheck(targ[i][1], targ[i][2], &course[i])) return;
    }
    chew();
    if (i==1 && key==IHEOL) {
        for (i=1; i<=n; i++) {
            proutn("어뢰 ");
            crami(i, 2);
            proutn("번 목표 섹터: ");
            key = scan();
            if (key != IHREAL) { huh(); return; }
            targ[i][1] = aaitem;
            key = scan();
            if (key != IHREAL) { huh(); return; }
            targ[i][2] = aaitem;
            chew();
            if (targetcheck(targ[i][1], targ[i][2], &course[i])) return;
        }
    }
    ididit = 1;
    osuabor = 0;
    for (i=1; i<=n && !osuabor; i++) {
        if (condit != IHDOCKED) torps--;
        r = (Rand()+Rand())*0.5 - 0.5;
        if (fabs(r) >= 0.47) {
            r = (Rand()+1.2)*r;
            if (n > 1) { prouts("***어뢰"); crami(i,2); prouts("번 오발."); }
            else prouts("***어뢰 오발.");
            skip(1);
            if (i < n) prout("  나머지 연사 중단.");
            osuabor = 1;
            if (Rand() <= 0.2) {
                prout("***오발로 광자 튜브 손상됨.");
                damage[DPHOTON] = damfac*(1.0+2.0*Rand());
                break;
            }
        }
#ifdef CLOAKING
        if (iscloaked) r *= 1.2;
        else
#endif
        if (shldup!=0 || condit==IHDOCKED) r *= 1.0 + 0.0001*shield;
        if (n != 1) { skip(1); proutn("어뢰"); crami(i,2); proutn("번 궤적: "); }
        else        { skip(1); proutn("어뢰 궤적: "); }
        torpedo(course[i], r, sectx, secty, &dummy);
        if (alldone || d.galaxy[quadx][quady]==1000) return;
    }
    if (d.remkl == 0) finish(FWON);
}

static void overheat(double rpow) {
    if (rpow > 1500) {
        double chekbrn = (rpow-1500.)*0.00038;
        if (Rand() <= chekbrn) {
            prout("무기 담당 술루- \"페이저 과열됨, 함장님.\"");
            damage[DPHASER] = damfac*(1.0+Rand())*(1.0+chekbrn);
        }
    }
}

static int checkshctrl(double rpow) {
    double hit;
    int icas;
    skip(1);
    if (Rand() < .998) { prout("실드 해제됨."); return 0; }
    prouts("***적색경보!  적색경보!");
    skip(2);
    hit = rpow*shield/inshld;
    energy -= rpow + hit*0.8;
    shield -= hit*0.2;
    if (energy <= 0.0) {
        prouts("술루- \"함장! 실드 오작***********************\"");
        skip(1); stars(); finish(FPHASER); return 1;
    }
    prouts("술루- \"함장! 실드 오작동! 페이저 사격 봉쇄됨!\"");
    skip(2);
    prout("우후라 중위- \"함장님, 모든 데크에서 피해 보고입니다.\"");
    icas = hit*Rand()*0.012;
    skip(1); fry(0.8*hit);
    if (icas) {
        skip(1);
        prout("맥코이 → 브릿지- \"심각한 방사선 화상, 함장님.");
        proutn("  ");
        crami(icas, 1);
        prout("명 사상.\"");
        casual += icas;
    }
    skip(1);
    prout("페이저 에너지가 실드에 분산되었습니다.");
    prout("적에게는 영향 없음.");
    overheat(rpow);
    return 1;
}

/* 페이저 발사 */


/* 도킹 */

/* 죽음의 광선 */

void phasers(void) {
	double hits[21], rpow, extra, powrem, over, temp;
	int kz = 0, k=1, i; /* Cheating inhibitor */
	int ifast=0, no=0, ipoop=1, msgflag = 1;
	enum {NOTSET, MANUAL, FORCEMAN, AUTOMATIC} automode = NOTSET;
	int key;

	skip(1);
	/* SR sensors and Computer */
	if (damage[DSRSENS]+damage[DCOMPTR] > 0) ipoop = 0;
	if (condit == IHDOCKED) {
		prout("스타베이스 실드를 통해 페이저를 발사할 수 없습니다.");
		chew();
		return;
	}
	if (damage[DPHASER] != 0) {
		prout("페이저 제어 손상됨.");
		chew();
		return;
	}
	if (shldup) {
		if (damage[DSHCTRL]) {
			prout("고속 실드 제어 손상됨.");
			chew();
			return;
		}
		if (energy <= 200.0) {
			prout("고속 실드 제어를 활성화할 에너지가 부족합니다.");
			chew();
			return;
		}
		prout("무기 담당 술루-  \"고속 실드 제어 활성화, 함장님.\"");
		ifast = 1;
		
	}
	ididit = 1;
	/* Original code so convoluted, I re-did it all */
	while (automode==NOTSET) {
		key=scan();
		if (key == IHALPHA) {
			if ((isit("manual") || strcmp(citem,"수동")==0 || strcmp(citem,"수")==0)) {
				if (nenhere==0) {
					prout("적이 없어 자동 발사할 수 없습니다.");
					chew();
					key = IHEOL;
					automode=AUTOMATIC;
				}
				else {
					automode = MANUAL;
					key = scan();
				}
			}
			else if ((isit("automatic") || strcmp(citem,"자동")==0 || strcmp(citem,"자")==0)) {
				if ((!ipoop) && nenhere != 0) {
					automode = FORCEMAN;
				}
				else {
					if (nenhere==0)
						prout("에너지가 우주 공간으로 흩어집니다.");
					automode = AUTOMATIC;
					key = scan();
				}
			}
			else if (isit("no")) {
				no = 1;
			}
			else {
				huh();
				ididit = 0;
				return;
			}
		}
		else if (key == IHREAL) {
			if (nenhere==0) {
				prout("에너지가 우주 공간으로 흩어집니다.");
				automode = AUTOMATIC;
			}
			else if (!ipoop)
				automode = FORCEMAN;
			else
				automode = AUTOMATIC;
		}
		else {
			/* IHEOL */
			if (nenhere==0) {
				prout("에너지가 우주 공간으로 흩어집니다.");
				automode = AUTOMATIC;
			}
			else if (!ipoop)
				automode = FORCEMAN;
			else 
			proutn("수동(수/manual) 또는 자동(자/automatic)? ");
		}
	}
				
	switch (automode) {
		case AUTOMATIC:
			if (key == IHALPHA && isit("no")) {
				no = 1;
				key = scan();
			}
			if (key != IHREAL && nenhere != 0) {
				proutn("페이저가 목표를 조준 완료했습니다. 가용 에너지=");
				cramf(ifast?energy-200.0:energy,1,2);
				skip(1);
			}
			do {
				while (key != IHREAL) {
					chew();
					proutn("발사할 유닛=");
					key = scan();
				}
				rpow = aaitem;
				if (rpow >= (ifast?energy-200:energy)) {
					proutn("가용 에너지= ");
					cramf(ifast?energy-200:energy, 1,2);
					skip(1);
					key = IHEOL;
				}
			} while (rpow >= (ifast?energy-200:energy));
			if (rpow<=0) {
				/* chicken out */
				ididit = 0;
				chew();
				return;
			}
			if ((key=scan()) == IHALPHA && isit("no")) {
				no = 1;
			}
			if (ifast) {
				energy -= 200; /* Go and do it! */
				if (checkshctrl(rpow)) return;
			}
			chew();
			energy -= rpow;
			extra = rpow;
			if (nenhere) {
				extra = 0.0;
				powrem = rpow;
				for (i = 1; i <= nenhere; i++) {
					hits[i] = 0.0;
					if (powrem <= 0) continue;
					hits[i] = fabs(kpower[i])/(phasefac*pow(0.90,kdist[i]));
					over = (0.01 + 0.05*Rand())*hits[i];
					temp = powrem;
					powrem -= hits[i] + over;
					if (powrem <= 0 && temp < hits[i]) hits[i] = temp;
					if (powrem <= 0) over = 0.0;
					extra += over;
				}
				if (powrem > 0.0) extra += powrem;
				hittem(hits);
			}
			if (extra > 0 && alldone == 0) {
				if (ithere) {
					proutn("*** 톨리안 웹이 공격 무력화 기능으로");
					if (nenhere>0) proutn("과잉 ");
					prout("페이저 에너지를 흡수하였습니다.");
				}
				else {
					cramf(extra, 0, 2);
					prout(" 에너지가 허공에 소모되었습니다.");
				}
			}
			break;

		case FORCEMAN:
			chew();
			key = IHEOL;
			if (damage[DCOMPTR]!=0)
				prout("전투 컴퓨터가 손상되었습니다. 수동 조준만 가능합니다.");
			else {
				skip(1);
				prouts("---작동 중---");
				skip(1);
				prout("근거리-센서들이-손상되었습니다.");
				prout("페이저-자동-발사를 하기에는-센서-데이터가-부족합니다.");
				prout("수동-발사를-하여야-합니다.");
				skip(1);
			}
		case MANUAL:
			rpow = 0.0;
			for (k = 1; k <= nenhere;) {
				int ii = kx[k], jj = ky[k];
				int ienm = quad[ii][jj];
				if (msgflag) {
					proutn("가용 에너지= ");
					cramf(energy-.006-(ifast?200:0), 0, 2);
					skip(1);
					msgflag = 0;
					rpow = 0.0;
				}
				if (damage[DSRSENS] && !(abs(sectx-ii) < 2 && abs(secty-jj) < 2) &&
					(ienm == IHC || ienm == IHS)) {
					cramen(ienm);
					prout(" 단거리 스캔 없이 목표 위치를 특정할 수 없습니다.");
					chew();
					key = IHEOL;
					hits[k] = 0; /* prevent overflow -- thanks to Alexei Voitenko */
					k++;
					continue;
				}
				if (key == IHEOL) {
					chew();
					if (ipoop && k > kz) {
						int irec=(fabs(kpower[k])/(phasefac*pow(0.9,kdist[k])))*
								 (1.01+0.05*Rand()) + 1.0;
						kz = k;
						proutn("(");
						crami(irec, 1);
						proutn(")  ");
					}
					proutn("발사할 에너지 유닛 ");
					crmena(0, ienm, 2, ii, jj);
					proutn("-  ");
					key = scan();
				}
				if (key == IHALPHA && isit("no")) {
					no = 1;
					key = scan();
					continue;
					}
				if (key == IHALPHA) {
					huh();
					ididit = 0;
					return;
				}
				if (key == IHEOL) {
					if (k==1) { /* Let me say I'm baffled by this */
						msgflag = 1;
					}
					continue;
				}
				if (aaitem < 0) {
					/* abort out */
					ididit = 0;
					chew();
					return;
				}
				hits[k] = aaitem;
				rpow += aaitem;
				/* If total requested is too much, inform and start over */
				
				if (rpow >= (ifast?energy-200:energy)) {
					prout("가용한 에너지를 초과했습니다. -- 다시 시도합니다.");
					chew();
					key = IHEOL;
					k = 1;
					msgflag = 1;
					continue;
				}
				key = scan(); /* scan for next value */
				k++;
			}
			if (rpow == 0.0) {
				/* zero energy -- abort */
				ididit = 0;
				chew();
				return;
			}
			if (key == IHALPHA && isit("no")) {
				no = 1;
			}
			energy -= rpow;
			chew();
			if (ifast) {
				energy -= 200.0;
				if (checkshctrl(rpow)) return;
			}
			hittem(hits);
			ididit=1;
			break;
			case NOTSET: break; // cannot occur
	}
	/* Say shield raised or malfunction, if necessary */
	if (alldone) return;
	if (ifast) {
		skip(1);
		if (no == 0) {
			if (Rand() >= 0.99) {
				prout("술루-  \"함장님, 고속-실드 제어 시스템에 이상이 발생했습니다 . . .");
				prouts("         딸깍 . . . 딸깍 . . . 펑 . . .");
				prout(" 반응이 없습니다, 함장님!");
				shldup = 0;
			}
			else
				prout("실드 올림.");
		}
		else
			shldup = 0;
	}
	overheat(rpow);
}

void hittem(double *hits) {
	double kp, kpow, wham, hit, dustfac, kpini;
	int nenhr2=nenhere, k=1, kk=1, ii, jj, ienm;

	skip(1);

	for (; k <= nenhr2; k++, kk++) {
		if ((wham = hits[k])==0) continue;
		dustfac = 0.9 + 0.01*Rand();
		hit = wham*pow(dustfac,kdist[kk]);
		kpini = kpower[kk];
		kp = fabs(kpini);
		if (phasefac*hit < kp) kp = phasefac*hit;
		kpower[kk] -= (kpower[kk] < 0 ? -kp: kp);
		kpow = kpower[kk];
		ii = kx[kk];
		jj = ky[kk];
		if (hit > 0.005) {
			cramf(hit, 0, 2);
			proutn(" 에너지 유닛의 타격을 입혔습니다. ");
		}
		else
			proutn(" 아주 경미한 타격입니다. ");
		ienm = quad[ii][jj];
		crmena(0,ienm,2,ii,jj);
		skip(1);
		if (kpow == 0) {
			deadkl(ii, jj, ienm, ii, jj);
			if (d.remkl==0) finish(FWON);
			if (alldone) return;
			kk--; /* don't do the increment */
		}
		else /* decide whether or not to emasculate klingon */
			if (kpow > 0 && Rand() >= 0.9 &&
				kpow <= ((0.4 + 0.4*Rand())*kpini)) {
				proutn("***스팍-  \"함장님, ");
				cramlc(2,ii,jj);
				skip(1);
				prout("   에 위치한 함선은 방금 화력을 상실했습니다.\"");
				kpower[kk] = -kpow;
			}
	}
	return;
}
