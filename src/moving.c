#include "sst.h"

static void getcd(int, int);

void lmove(void) {
	double angle, deltax, deltay, bigger, x, y,
    finald, finalx, finaly, stopegy;
    int oldquadx, oldquady;
	int trbeam = 0, n, l, ix, iy, kink, kinks, iquad;

	if (inorbit) {
		prout("조종사 술루- \"표준 궤도를 이탈합니다.\"");
		inorbit = 0;
	}

	angle = ((15.0 - direc) * 0.5235988);
	deltax = -sin(angle);
	deltay = cos(angle);
	if (fabs(deltax) > fabs(deltay))
		bigger = fabs(deltax);
	else
		bigger = fabs(deltay);
		
	deltay /= bigger;
	deltax /= bigger;

#ifdef CLOAKING
    if (iscloaked && d.date+Time >= future[FTBEAM])
    {  /* We can't be tracto beamed if cloaked, so move the event into the future */
        future[FTBEAM] = d.date + Time +
                         expran(1.5*intime/d.remcom);
    }
#endif
    
	/* If tractor beam is to occur, don't move full distance */
	if (d.date+Time >= future[FTBEAM]) {
		trbeam = 1;
		condit = IHRED;
		dist = dist*(future[FTBEAM]-d.date)/Time + 0.1;
		Time = future[FTBEAM] - d.date + 1e-5;
	}
	/* Move within the 구역 */
	quad[sectx][secty] = IHDOT;
	x = sectx;
	y = secty;
	n = 10.0*dist*bigger+0.5;

	if (n > 0) {
		for (l = 1; l <= n; l++) {
			ix = (x += deltax) + 0.5;
			iy = (y += deltay) + 0.5;
			if (ix < 1 || ix > 10 || iy < 1 || iy > 10) {
				/* Leaving 구역 -- allow final enemy attack */
				/* Don't do it if being pushed by Nova */
				if (nenhere != 0 && iattak != 2
#ifdef CLOAKING
				    && !iscloaked
#endif
				   ) {
					newcnd();
					for (l = 1; l <= nenhere; l++) {
						finald = sqrt((ix-kx[l])*(double)(ix-kx[l]) +
									  (iy-ky[l])*(double)(iy-ky[l]));
						kavgd[l] = 0.5 * (finald+kdist[l]);
					}
					if (d.galaxy[quadx][quady] != 1000) attack(0);
					if (alldone) return;
				}
				/* compute final position -- new 구역 and 섹터 */
				x = 10*(quadx-1)+sectx;
				y = 10*(quady-1)+secty;
				ix = x+10.0*dist*bigger*deltax+0.5;
				iy = y+10.0*dist*bigger*deltay+0.5;
				/* check for edge of galaxy */
				kinks = FALSE;
				do {
					kink = 0;
					if (ix <= 0) {
						ix = -ix + 1;
						kink = 1;
					}
					if (iy <= 0) {
						iy = -iy + 1;
						kink = 1;
					}
					if (ix > 80) {
						ix = 161 - ix;
						kink = 1;
					}
					if (iy > 80) {
						iy = 161 - iy;
						kink = 1;
					}
					if (kink) kinks = TRUE;
				} while (kink);

				if (kinks) {
					nkinks += 1;
					if (nkinks == 3) {
						/* Three strikes -- you're out! */
						finish(FNEG3);
						return;
					}
					prout("\n당신은 네거티브 에너지 장벽을 도약하는 시도를 하였고,\n"
						 "은하 경계에 도달했습니다. 세 번째로 시도하면\n"
                          "함선이 파괴됩니다.\n");
                }
				/* Compute final position in new 구역 */
                if (trbeam) return; /* Don't bother if we are to be beamed */
                oldquadx = quadx;
                oldquady = quady;
				quadx = (ix+9)/10;
                quady = (iy+9)/10;
                sectx = ix - 10*(quadx-1);
                secty = iy - 10*(quady-1);
                if (quadx != oldquadx || quady != oldquady) {
                    proutn("\n진입:");
                    cramlc(1, quadx, quady);
                } else {
                    prout("(네거티브 에너지 장벽이 구역을 방해합니다.)");
                }
                skip(1);
                quad[sectx][secty] = ship;
                newqad(0);
                /* [버그수정] 초신성 구역 진입 시 즉시 비상 탈출 */
                if (d.galaxy[quadx][quady] == 1000) {
                    atover(0);
                }
                return;
            }
			iquad = quad[ix][iy];
			if (iquad != IHDOT) {
				/* object encountered in flight path */
				stopegy = 50.0*dist/Time;
				dist=0.1*sqrt((sectx-ix)*(double)(sectx-ix) +
							  (secty-iy)*(double)(secty-iy));
				switch (iquad) {
					case IHT: /* Ram a Tholean */
					case IHK: /* Ram enemy ship */
					case IHC:
					case IHS:
					case IHR:
						sectx = ix;
						secty = iy;
						ram(0, iquad, sectx, secty);
						finalx = sectx;
						finaly = secty;
						break;
					case IHBLANK:
						skip(1);
						prouts("***적색경보!  적색경보!");
						skip(1);
						proutn("***");
						crmshp();
						proutn(" 이(가) 블랙홀로 빨려들어갔습니다:");
						cramlc(2, ix, iy);
						skip(1);
						finish(FHOLE);
						return;
					default:
						/* something else */
						skip(1);
						crmshp();
						if (iquad == IHWEB)
							proutn(" 이(가) 솔리안 웹에 가로막힘:");
						else
							proutn(" 이(가) 물체에 막혔습니다:");
						cramlc(2, ix,iy);
						prout(";");
						proutn("비상 정지 필요 ");
						cramf(stopegy, 0, 2);
						prout(" 유닛의 에너지.");
						energy -= stopegy;
						finalx = x-deltax+0.5;
						sectx = finalx;
						finaly = y-deltay+0.5;
						secty = finaly;
						if (energy <= 0) {
							finish(FNRG);
							return;
						}
						break;
                }
				goto label100;	/* sorry! */ /* ACTUALLY BREAK SHOULD WORK HERE */
            }
        }
		dist = 0.1*sqrt((sectx-ix)*(double)(sectx-ix) +
						(secty-iy)*(double)(secty-iy));
		sectx = ix;
		secty = iy;
	}
	finalx = sectx; /* THESE STATEMENTS DO NOTHING USEFUL */
	finaly = secty;
label100:
	/* No 구역 change -- compute new avg enemy distances */
	quad[sectx][secty] = ship;
	if (nenhere) {
		for (l = 1; l <= nenhere; l++) {
			finald = sqrt((ix-kx[l])*(double)(ix-kx[l]) +
						  (iy-ky[l])*(double)(iy-ky[l]));
			kavgd[l] = 0.5 * (finald+kdist[l]);
			kdist[l] = finald;
		}
		sortkl();
		if (d.galaxy[quadx][quady] != 1000 && iattak == 0)
			attack(0);
		for (l = 1 ; l <= nenhere; l++) kavgd[l] = kdist[l];
	}
	newcnd();
	iattak = 0;
	return;
}



static void getcd(int isprobe, int akey) {
	/* This program originally required input in terms of a (clock)
	   direction and distance. Somewhere in history, it was changed to
	   cartesian coordinates. So we need to convert. I think
	   "manual" input should still be done this way -- it's a real
	   pain if the computer isn't working! Manual mode is still confusing
	   because it involves giving x and y motions, yet the coordinates
	   are always displayed y - x, where +y is downward! */

	
	int irowq=quadx, icolq=quady, irows, icols, itemp=0, iprompt=0, key;
	double xi, xj, xk, xl;
	double deltax, deltay;
	int automatic = -1;

	/* Get course direction and distance. If user types bad values, return
	   with DIREC = -1.0. */

	direc = -1.0;
	
	if (landed == 1 && !isprobe) {
		prout("함장님! 다음 없이는 표준 궤도를 이탈할 수 없습니다:");
		proutn("이(가) 함선으로 귀환했습니다 ");
		crmshp();
		prout(".");
		chew();
		return;
	}
	while (automatic == -1) {
		if (damage[DCOMPTR]) {
			if (isprobe)
				prout("컴퓨터 손상됨; 수동 항법만 가능");
			else
				prout("컴퓨터 손상됨; 수동 이동만 가능");
			chew();
			automatic = 0;
			key = IHEOL;
			break;
		}
		if (isprobe && akey != -1) {
			/* For probe launch, use pre-scaned value first time */
			key = akey;
			akey = -1;
		}
		else 
			key = scan();

		if (key == IHEOL) {
			proutn("수동(수/manual) 또는 자동(자/automatic)? ");
			iprompt = 1;
			chew();
		}
		else if (key == IHALPHA) {
			if (isit("manual") || strcmp(citem,"수동")==0 || strcmp(citem,"수")==0) {
				automatic =0;
				key = scan();
				break;
			}
			else if (isit("automatic") || strcmp(citem,"자동")==0 || strcmp(citem,"자")==0) {
				automatic = 1;
				key = scan();
				break;
			}
			else {
				huh();
				chew();
				return;
			}
		}
		else { /* numeric */
			if (isprobe)
				prout("(수동 항법 적용됨.)");
			else
				prout("(수동 이동 적용됨.)");
			automatic = 0;
			break;
		}
	}

	if (automatic) {
		while (key == IHEOL) {
			if (isprobe)
				proutn("목표 구역 또는 구역&섹터: ");
			else
				proutn("목적지 섹터 또는 구역&섹터: ");
			chew();
			iprompt = 1;
			key = scan();
		}

		if (key != IHREAL) {
			huh();
			return;
		}
		xi = aaitem;
		key = scan();
		if (key != IHREAL){
			huh();
			return;
		}
		xj = aaitem;
		key = scan();
		if (key == IHREAL) {
			/* both 구역 and 섹터 specified */
			xk = aaitem;
			key = scan();
			if (key != IHREAL) {
				huh();
				return;
			}
			xl = aaitem;

			irowq = xi + 0.5;
			icolq = xj + 0.5;
			irows = xk + 0.5;
			icols = xl + 0.5;
		}
		else {
			if (isprobe) {
				/* only 구역 specified -- go to center of dest quad */
				irowq = xi + 0.5;
				icolq = xj + 0.5;
				irows = icols = 5;
			}
			else {
				irows = xi + 0.5;
				icols = xj + 0.5;
			}
			itemp = 1;
		}
		if (irowq<1 || irowq > 8 || icolq<1 || icolq > 8 ||
			irows<1 || irows > 10 || icols<1 || icols > 10) {
				huh();
				return;
			}
		skip(1);
		if (!isprobe) {
			if (itemp) {
				if (iprompt) {
					proutn("조종사 술루- \"코스 설정 완료: ");
					cramlc(2, irows, icols);
					prout(".\"");
				}
			}
			else prout("소위 체코프- \"코스 입력됨, 함장님.\"");
		}
		deltax = icolq - quady + 0.1*(icols-secty);
		deltay = quadx - irowq + 0.1*(sectx-irows);
	}
	else { /* manual */
		while (key == IHEOL) {
			proutn("X, Y 이동량: ");
			chew();
			iprompt = 1;
			key = scan();
		}
		itemp = 2;
		if (key != IHREAL) {
			huh();
			return;
		}
		deltax = aaitem;
        key = scan();
        if (key == IHEOL) {
            deltay = 0.0;
        } else 	if (key != IHREAL) {
			huh();
			return;
		} else {
            deltay = aaitem;
        }
        
        if (coordfixed) {
            double temp = deltax;
            deltax = deltay;
            deltay = -temp;
        }
	}
	/* Check for zero movement */
	if (deltax == 0 && deltay == 0) {
		chew();
		return;
	}
	if (itemp == 2 && !isprobe) {
		skip(1);
		prout("조종사 술루- \"Aye, 함장님.\"");
	}
	dist = sqrt(deltax*deltax + deltay*deltay);
	direc = atan2(deltax, deltay)*1.90985932;
	if (direc < 0.0) direc += 12.0;
	chew();
	return;

}
		


void impuls(void) {
	double power;

	ididit = 0;
	if (damage[DIMPULS]) {
		chew();
		skip(1);
		prout("엔지니어 스콧- \"임펄스 엔진이 손상되었습니다.\"");
		return;
	}

	if (energy > 30.0) {
		getcd(FALSE, 0);
		if (direc == -1.0) return;
		power = 20.0 + 100.0*dist;
	}
	else
		power = 30.0;

	if (power >= energy) {
		/* Insufficient power for trip */
		skip(1);
		prout("부함장 스팍- \"함장님, 임펄스 엔진");
		prout("가동에 20유닛 + 구역당 100유닛이 필요합니다. ");
		if (energy > 30) {
			proutn("따라서 최대 ");
			cramf(0.01 * (energy-20.0)-0.05, 0, 1);
			prout(" 구역까지 갈 수 있습니다.\"");
		}
		else {
			prout(" 따라서 가동 불가 합니다.\"");
		}
		chew();
		return;
	}
	/* Make sure enough time is left for the trip */
	Time = dist/0.095;
	if (Time >= d.remtime) {
		prout("부함장 스팍- \"함장님, 임펄스 엔진 속도는 0.95섹터/스타데이트입니다.");
		prout("정말로 이 시간을 쓰시겠습니까?");
		
		if (ja() == 0) { Time = 0.0; return;}
	}
	/* Activate impulse engines and pay the cost */
	lmove();
	ididit = 1;
	if (alldone) return;
	power = 20.0 + 100.0*dist;
	energy -= power;
//	Time = dist/0.095; Don't recalculate because lmove may have
//	adjusted it for tractor beaming
	if (energy <= 0) finish(FNRG);
	return;
}


void dock(void) {
	chew();
	if (condit == IHDOCKED) {
		prout("이미 도킹 중입니다.");
		return;
	}
	if (inorbit) {
		prout("먼저 표준 궤도를 이탈해야 합니다.");
		return;
	}
	if (basex==0 || abs(sectx-basex) > 1 || abs(secty-basey) > 1) {
		crmshp();
		prout("이(가) 스타베이스에 인접해 있지 않습니다.");
		return;
	}
#ifdef CLOAKING
	if (iscloaked) {
		prout("은폐 상태에서는 도킹할 수 없습니다.");
		return;
	}
#endif
	condit = IHDOCKED;
	prout("도킹 완료.");
	if (energy < inenrg) energy = inenrg;
	shield = inshld;
	torps = intorps;
    lsupres = inlsr;
#ifdef CAPTURE
    if (brigcapacity-brigfree > 0)
    {
        printf("%d명의 포로 클링온이 스타베이스로 이송되었습니다.\n", brigcapacity-brigfree);
        kcaptured += brigcapacity-brigfree;
        brigfree = brigcapacity;
    }
#endif
	if (stdamtim != 1e30 &&
		(future[FCDBAS] < 1e30 || isatb == 1) && iseenit == 0) {
		/* get attack report from base */
		prout("우후라 중위- \"함장님, 스타베이스에서 중요한 메시지가 왔습니다:\"");
		attakreport();
		iseenit = 1;
	}
}

void warp(int i) {
	int blooey=0, twarp=0, iwarp;
	double power;

	if (i!=2) { /* Not WARPX entry */
		ididit = 0;
#ifdef CLOAKING
		if (iscloaked) {
			chew();
			skip(1);
			prout("엔지니어 스콧- \"함장님, 클로킹 중에는 워프 엔진을 사용하지 않는 것이 좋습니다.\"");
			return;
		}
#endif
		if (damage[DWARPEN] > 10.0) {
			chew();
			skip(1);
			prout("엔지니어 스콧- \"워프 엔진이 손상되었습니다.\""); // Was "Impulse" 10/2013
			return;
		}
		if (damage[DWARPEN] > 0.0 && warpfac > 4.0) {
			chew();
			skip(1);
			prout("엔지니어 스콧- \"함장님, 손상 수리 전까지 워프 4만 가능합니다.\"");
			return;
		}
			
		/* Read in course and distance */
		getcd(FALSE, 0);
		if (direc == -1.0) return;

		/* Make sure starship has enough energy for the trip */
		power = (dist+0.05)*warpfac*warpfac*warpfac*(shldup+1);


		if (power >= energy) {
			/* Insufficient power for trip */
			ididit = 0;
			skip(1);
			prout("엔지니어링 → 브릿지--");
			if (shldup==0 || 0.5*power > energy) {
				iwarp = pow((energy/(dist+0.05)), 0.333333333);
				if (iwarp <= 0) {
					prout("할 수 없습니다, 함장님. 에너지가 부족합니다.");
				}
				else {
					proutn("에너지 부족이지만 워프 ");
					crami(iwarp, 1);
					if (shldup)
						prout(",\n실드를 내리면 가능합니다.");
					else
						prout(".");
				}
			}
			else
				prout("실드 가동 상태로 그 거리를 갈 에너지가 없습니다.");
			return;
		}
						
		/* Make sure enough time is left for the trip */
		Time = 10.0*dist/wfacsq;
		if (Time >= 0.8*d.remtime) {
			skip(1);
			prout("부함장 스팍- \"함장님, 계산결과");
			proutn("  이동에 약 ");
			cramf(100.0*Time/d.remtime, 0, 2);
			prout(" %의");
			prout("  남은 시간이 소모됩니다. 계속하시겠습니까?\"");
			if (ja() == 0) { Time = 0.0; return;}
		}
	}
	/* Entry WARPX */
	if (warpfac > 6.0) {
		/* Decide if engine damage will occur */
		double prob = dist*(6.0-warpfac)*(6.0-warpfac)/66.666666666;
		if (prob > Rand()) {
			blooey = 1;
			dist = Rand()*dist;
		}
		/* Decide if time warp will occur */
		if (0.5*dist*pow(7.0,warpfac-10.0) > Rand()) twarp=1;
#ifdef DEBUG
		if (idebug &&warpfac==10 && twarp==0) {
			blooey=0;
			proutn("시간 워프를 강제하시겠습니까? ");
			if (ja()==1) twarp=1;
		}
#endif
		if (blooey || twarp) {
			/* If time warp or engine damage, check path */
			/* If it is obstructed, don't do warp or damage */
			double angle = ((15.0-direc)*0.5235998);
			double deltax = -sin(angle);
			double deltay = cos(angle);
			double bigger, x, y;
			int n, l, ix, iy;
			if (fabs(deltax) > fabs(deltay))
				bigger = fabs(deltax);
			else
				bigger = fabs(deltay);
			
			deltax /= bigger;
			deltay /= bigger;
			n = 10.0 * dist * bigger +0.5;
			x = sectx;
			y = secty;
			for (l = 1; l <= n; l++) {
				x += deltax;
				ix = x + 0.5;
				if (ix < 1 || ix > 10) break;
				y += deltay;
				iy = y +0.5;
				if (iy < 1 || iy > 10) break;
				if (quad[ix][iy] != IHDOT) {
					blooey = 0;
					twarp = 0;
				}
			}
		}
	}
				

	/* Activate 워프 Engines and pay the cost */
	lmove();
	if (alldone) return;
	energy -= dist*warpfac*warpfac*warpfac*(shldup+1);
	if (energy <= 0) finish(FNRG);
	Time = 10.0*dist/wfacsq;
	if (twarp) timwrp();
	if (blooey) {
		damage[DWARPEN] = damfac*(3.0*Rand()+1.0);
		skip(1);
		prout("엔지니어링 → 브릿지--");
		prout("  스콧입니다. 워프 엔진이 손상되었습니다.");
		prout("  속도를 워프 4로 줄여야 합니다.");
	}
	ididit = 1;
	return;
}



void setwrp(void) {
	int key;
	double oldfac;
	
	while ((key=scan()) == IHEOL) {
		chew();
		proutn("워프 계수-");
	}
	chew();
	if (key != IHREAL) {
		huh();
		return;
	}
	if (damage[DWARPEN] > 10.0) {
		prout("워프 엔진이 작동하지 않습니다.");
		return;
	}
	if (damage[DWARPEN] > 0.0 && aaitem > 4.0) {
		prout("엔지니어 스콧- \"최선을 다하고 있습니다. 함장님,\n"
			  "  현재는 워프 4밖에 낼 수 없습니다.\"");
		return;
	}
	if (aaitem > 10.0) {
		prout("조종사 술루- \"우리의 최대 속도는 워프 10 입니다, 함장님.\"");
		return;
	}
	if (aaitem < 1.0) {
		prout("조종사 술루- \"워프 1 이하의 속도로는 운행할 수 없습니다, 함장님.\"");
		return;
	}
	oldfac = warpfac;
	warpfac = aaitem;
	wfacsq=warpfac*warpfac;
	if (warpfac <= oldfac || warpfac <= 6.0) {
		proutn("조종사 술루- \"워프 계수 ");
		cramf(warpfac, 0, 1);
		prout(", 함장님.\"");
		return;
	}
	if (warpfac < 8.00) {
		prout("엔지니어 스콧- \"Aye, 우리의 최대 안전 워프 속도는 6 입니다.\"");
		return;
	}
	if (warpfac == 10.0) {
		prout("엔지니어 스콧- \"Aye, 함장님, 해보겠습니다.\"");
		return;
	}
	prout("엔지니어 스콧- \"Aye, 함장님, 하지만 엔진이 동작하지 않을 수도 있습니다.\"");
	return;
}

void atover(int igrab) {
	double power, distreq;

	chew();
	/* is captain on planet? */
	if (landed==1) {
		if (damage[DTRANSP]) {
			finish(FPNOVA);
			return;
		}
		prout("스콧이 전송기 제어판으로 달려갑니다.");
		if (shldup) {
			prout("하지만 실드가 올라가 있으면 불가능합니다.");
			finish(FPNOVA);
		}
		prouts("필사적으로 당신을 구조하려 합니다 . . .");
		if (Rand() <= 0.5) {
			prout("실패했습니다.");
			finish(FPNOVA);
			return;
		}
		prout("성.공.했.습.니.다!");
		if (imine) {
			imine = 0;
			proutn("채굴된 결정이 ");
			if (Rand() <= 0.25) {
				prout("유실되었습니다.");
			}
			else {
				prout("저장되었습니다.");
				icrystl = 1;
			}
		}
	}
	if (igrab) return;

	/* Check to see if captain in shuttle craft */
	if (icraft) finish(FSTRACTOR);
	if (alldone) return;

	/* Inform captain of attempt to reach safety */
	skip(1);
	do {
		if (justin) {
			prouts("***적색경보!  적색경보!");
			skip(1);
			proutn("!! ");
			crmshp();
			prout(" 초신성이 포함된 구역에서");
			prouts(" 멈췄습니다.");
			skip(2);
		}
		proutn("***비상 자동 제어 장치가 함선을 끌어내려 합니다 ");
		crmshp();
		skip(1);
		prout("구역을 안전하게 벗어났습니다.");
		starch[quadx][quady] = damage[DRADIO] > 0.0 ? d.galaxy[quadx][quady]+1000:1;

		/* Try to use warp engines */
		if (damage[DWARPEN]) {
			skip(1);
			prout("워프 엔진 손상됨.");
			finish(FSNOVAED);
			return;
		}
		warpfac = 6.0+2.0*Rand();
		wfacsq = warpfac * warpfac;
		proutn("워프 계수 설정 ");
		cramf(warpfac, 1, 1);
		skip(1);
		power = 0.75*energy;
		dist = power/(warpfac*warpfac*warpfac*(shldup+1));
		distreq = 1.4142+Rand();
		if (distreq < dist) dist = distreq;
		Time = 10.0*dist/wfacsq;
		direc = 12.0*Rand();	/* How dumb! */
		justin = 0;
		inorbit = 0;
		warp(2);
		if (justin == 0) {
			/* This is bad news, we didn't leave 구역. */
			if (alldone) return;
			skip(1);
			prout("구역을 이탈할 에너지가 부족합니다.");
			finish(FSNOVAED);
			return;
		}
		/* Repeat if another snova */
	} while (d.galaxy[quadx][quady] == 1000);
	if (d.remkl==0) finish(FWON); /* Snova killed remaining enemy. */
}

void timwrp() {
	int l, ll, gotit;
	prout("***시.간.이.동에 돌입하였.");
	if (d.snap && Rand() < 0.5) {
		/* Go back in time */
		proutn("당신은 ");
		cramf(d.date-snapsht.date, 0, 2);
		prout(" 스타데이트 만큼 시간을 거슬러 이동하고 있습니다.");
		d = snapsht;
		d.snap = 0;
		if (d.remcom) {
			future[FTBEAM] = d.date + expran(intime/d.remcom);
			future[FBATTAK] = d.date + expran(0.3*intime);
		}
		future[FSNOVA] = d.date + expran(0.5*intime);
		future[FSNAP] = d.date +expran(0.25*d.remtime); /* next snapshot will
													   be sooner */
		if (d.nscrem) future[FSCMOVE] = 0.2777;
		isatb = 0;
		future[FCDBAS] = future[FSCDBAS] = 1e30;
		batx = baty = 0;

		/* Make sure Galileo is consistant -- Snapshot may have been taken
		   when on planet, which would give us two Galileos! */
		gotit = 0;
		for (l = 1; l <= inplan; l++) {
			if (d.plnets[l].known == 2) {
				gotit = 1;
				if (iscraft==1 && ship==IHE) {
					prout("체코프-  \"보안 보고서에 따르면 갈릴레오가 사라졌다고 합니다, 함장님!");
					iscraft = 0;
				}
			}
		}
		/* Likewise, if in the original time the Galileo was abandoned, but
		   was on ship earlier, it would have vanished -- lets restore it */
		if (iscraft==0 && gotit==0 && damage[DSHUTTL] >= 0.0) {
			prout("체코프-  \"보안보고에 따르면 갈릴레오가 도크에 다시 나타났다고 합니다!\"");
			iscraft = 1;
		}

		/* Revert star chart to earlier era, if it was known then*/
		if (damage[DRADIO]==0.0 || stdamtim > d.date) {
			for (l = 1; l <= 8; l++)
				for (ll = 1; ll <= 8; ll++)
					if (starch[l][ll] > 1)
						starch[l][ll]=damage[DRADIO]>0.0 ? d.galaxy[l][ll]+1000 :1;
			prout("스팍이 기억으로 성도를 재구성했습니다.");
			if (damage[DRADIO] > 0.0) stdamtim = d.date;
		}
	}
	else {
		/* Go forward in time */
		Time = -0.5*intime*log(Rand());
		proutn("당신은 시간을 ");
		cramf(Time, 1, 2);
		prout(" 스타데이트 만큼 앞으로 이동하고 있습니다.");
		/* cheat to make sure no tractor beams occur during time warp */
		future[FTBEAM] += Time;
		damage[DRADIO] += Time;
	}
	newqad(0);
}

void probe(void) {
	double angle, bigger;
	int key;
	/* New code to launch a deep space probe */
	if (nprobes == 0) {
		chew();
		skip(1);
		if (ship == IHE) 
			prout("엔지니어 스콧- \"더 이상 남은 심우주 탐침이 없습니다, 함장님.\"");
		else
			prout("페어리 퀸에는 심우주 탐침이 없습니다.");
		return;
	}
	if (damage[DDSP] != 0.0) {
		chew();
		skip(1);
		prout("엔지니어 스콧- \"탐침 발사기가 손상되었습니다.\"");
		return;
	}
	if (future[FDSPROB] != 1e30) {
		chew();
		skip(1);
		if (REPORTS) {
			prout("우후라- \"이전 탐침이 여전히 데이터를 보고하고 있습니다, 함장님.\"");
		} else {
			prout("스팍-  \"기록에 의하면 이전 탐침이 아직");
			prout("   목적지에 도달하지 못했습니다.\"");
		}
		return;
	}
	key = scan();

	if (key == IHEOL) {
		/* slow mode, so let Kirk know how many probes there are left */
		crami(nprobes,1);
		prout(nprobes==1 ? " 개의 탐침이 남아있습니다." : " 개의 탐침이 남아있습니다.");
		proutn("탐침을 발사하시겠습니까? ");
		if (ja()==0) return;
	}

	isarmed = FALSE;
	if (key == IHALPHA && strcmp(citem,"armed") == 0) {
		isarmed = TRUE;
		key = scan();
	}
	else if (key == IHEOL) {
		proutn("노바맥스 핵탄두를 무장하시겠습니까?");
		isarmed = ja();
	}
	getcd(TRUE, key);
	if (direc == -1.0) return;
	nprobes--;
		angle = ((15.0 - direc) * 0.5235988);
	probeinx = -sin(angle);
	probeiny = cos(angle);
	if (fabs(probeinx) > fabs(probeiny))
		bigger = fabs(probeinx);
	else
		bigger = fabs(probeiny);
		
	probeiny /= bigger;
	probeinx /= bigger;
	proben = 10.0*dist*bigger +0.5;
	probex = quadx*10 + sectx - 1;	// We will use better packing than original
	probey = quady*10 + secty - 1;
	probecx = quadx;
	probecy = quady;
	future[FDSPROB] = d.date + 0.01; // 섹터 이동 시간
	prout("소위 체코프-  \"심우주 탐침을 발사했습니다, 함장님.\"");
	return;
}

void help(void) {
	/* There's more than one way to move in this game! */
	double ddist, xdist, probf;
	int line, l, ix, iy;

	chew();
	/* Test for conditions which prevent calling for help */
	if (condit == IHDOCKED) {
		prout("우후라 중위-  \"함장님, 이미 도킹 중입니다.\"");
		return;
	}
	if (damage[DRADIO] != 0) {
		prout("서브스페이스 무선 손상됨.");
		return;
	}
	if (d.rembase==0) {
		prout("우후라 중위-  \"함장님, 스타베이스에서 응답이 없습니다.\"");
		return;
	}
	if (landed == 1) {
		proutn("탑승해야 합니다: ");
		crmshp();
		prout(".");
		return;
	}
	/* OK -- call for help from nearest starbase */
	nhelp++;
	if (basex!=0) {
		/* There's one in this 구역 */
		ddist = sqrt(square(basex-sectx)+square(basey-secty));
	}
	else {
		ddist = 1e30;
		for (l = 1; l <= d.rembase; l++) {
			xdist=10.0*sqrt(square(d.baseqx[l]-quadx)+square(d.baseqy[l]-quady));
			if (xdist < ddist) {
				ddist = xdist;
				line = l;
			}
		}
		/* Since starbase not in 구역, set up new 구역 */
		quadx = d.baseqx[line];
		quady = d.baseqy[line];
		newqad(1);
	}
	/* dematerialize starship */
	quad[sectx][secty]=IHDOT;
	proutn("위치:");
	cramlc(1, quadx, quady);
	proutn(" 에 있는 스타베이스가 비물질화에--");
	crmshp();
	prout(" 대응합니다.");
	/* Give starbase three chances to rematerialize starship */
	probf = pow((1.0 - pow(0.98,ddist)), 0.33333333);
	for (l = 1; l <= 3; l++) {
		switch (l) {
			case 1: proutn("1차"); break;
			case 2: proutn("2차"); break;
			case 3: proutn("3차"); break;
		}
		proutn(" 재물질화를 시도합니다. ");
		crmshp();
		prouts(" . . . . . ");
		if (Rand() > probf) break;
		prout("실패하였습니다.");
	}
	if (l > 3) {
		finish(FMATERIALIZE);
		return;
	}
	/* Rematerialization attempt should succeed if can get adj to base */
	for (l = 1; l <= 5; l++) {
		ix = basex+3.0*Rand()-1;
		iy = basey+3.0*Rand()-1;
		if (ix>=1 && ix<=10 && iy>=1 && iy<=10 && quad[ix][iy]==IHDOT) {
			/* found one -- finish up */
			prout("성공하였습니다.");
			sectx=ix;
			secty=iy;
			quad[ix][iy]=ship;
			dock();
			skip(1);
			prout("우후라 중위-  \"함장님, 해냈습니다!\"");
			return;
		}
	}
	finish(FMATERIALIZE);
	return;
}
