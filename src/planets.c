#include "sst.h"

static char classes[4][2]={"","M","N","O"};
static int height;

static int consumeTime(void) {
/* I think most of this avoidance was caused by overlay scheme.
   Let's see what happens if all events can occur here */

//	double asave;
	ididit = 1;
#if 0
	/* Don't wory about this */
	if (future[FTBEAM] <= d.date+Time && d.remcom != 0 && condit != IHDOCKED) {
		/* We are about to be tractor beamed -- operation fails */
		return 1;
	}
#endif
//	asave = future[FSNOVA];
//	future[FSNOVA] = 1e30; /* defer supernovas */
	events();	/* Used to avoid if future[FSCMOVE] within time */
//	future[FSNOVA] = asave;
	/*fails if game over, quadrant super-novas or we've moved to new quadrant*/
	if (alldone || d.galaxy[quadx][quady] == 1000 || justin != 0) return 1;
	return 0;
}

void preport(void) {
	int iknow = 0, i;
	skip(1);
	chew();
	prout("스팍-  \"함장님, 행성보고서 입니다.\"");
	skip(1);
	for (i = 1; i <= inplan; i++) {
		if (d.plnets[i].known
#ifdef DEBUG
			|| ( idebug && d.plnets[i].x !=0)
#endif
			) {
			iknow = 1;
#ifdef DEBUG
			if (idebug && d.plnets[i].known==0) proutn("(불명) ");
#endif
			cramlc(1, d.plnets[i].x, d.plnets[i].y);
			proutn("   등급: ");
			proutn(classes[d.plnets[i].pclass]);
			proutn("   ");
			if (d.plnets[i].crystals == 0) proutn("트릴륨 결정이 없습니다.");
			prout("트릴륨 결정이 존재합니다.");
			if (d.plnets[i].known==2) 
				prout("    셔틀 크래프트 갈릴레오가 행성 표면에 있습니다.");
		}
	}
	if (iknow==0) prout("정보 없음.");
}

void orbit(void) {
	double asave;

	skip(1);
	chew();
	ididit=0;
	if (inorbit!=0) {
		prout("이미 표준 궤도에 진입해 있습니다.");
		return;
	}
	if (damage[DWARPEN] != 0 && damage[DIMPULS] != 0) {
		prout("워프 엔진과 임펄스 엔진 모두 손상됨.");
		return;
	}
	if (plnetx == 0 || abs(sectx-plnetx) > 1 || abs(secty-plnety) > 1) {
		crmshp();
		prout(" 행성에 인접해 있지 않습니다.\n");
		return;
	}
	Time = 0.02+0.03*Rand();
	prout("조종사 술루-  \"함장님, 표준 궤도에 진입하고 있습니다.\"");
	newcnd();
	if (consumeTime()) return;
	proutn("술루-  \"궤도 진입 고도 ");
	cramf(height = (1400.+7200.*Rand()), 0, 2);
	prout(" km 상공.\"");
	inorbit = 1;
	return;
}

void sensor(void) {
	skip(1);
	chew();
	if (damage[DSRSENS] != 0.0) {
		prout("단거리 센서 손상됨.");
		return;
	}
	if (plnetx == 0) {
		prout("이 구역에는 행성이 없습니다.");
		return;
	}
	proutn("스팍-  \"센서 스캔--");
	cramlc(1, quadx, quady);
	prout("-");
	skip(1);
	proutn("         행성 위치:");
	cramlc(2, plnetx, plnety);
	proutn(" 등급: ");
	proutn(classes[d.plnets[iplnet].pclass]);
	prout(".");
	if (d.plnets[iplnet].known==2) 
		prout("         센서에 갈릴레오가 아직 행성 표면에 있음.");
	proutn("         탐지값:");
	if (d.plnets[iplnet].crystals == 0) proutn(" 없음");
	prout(" 딜리튬 결정 있음.\"");
	if (d.plnets[iplnet].known == 0) d.plnets[iplnet].known = 1;
	return;
}

void beam(void) {
	chew();
	skip(1);
	if (damage[DTRANSP] != 0) {
		prout("전송기 손상됨.");
		if (damage[DSHUTTL]==0 && (d.plnets[iplnet].known==2 || iscraft == 1)) {
			skip(1);
			prout("스팍-  \"함장님, 셔틀 크래프트 사용을 제안드려도 되겠습니까?\" ");
			if (ja() != 0) shuttle();
		}
		return;
	}
	if (inorbit==0) {
		crmshp();
		prout(" 표준 궤도 중이 아닙니다.");
		return;
	}
	if (shldup!=0) {
		prout("실드 가동 중 전송 불가.");
		return;
	}
	if (d.plnets[iplnet].known==0) {
		prout("스팍-  \"함장님, 이 행성에 대한 정보가 없습니다.");
		prout("  스타플릿 규정에 따르면 이 상황에서는");
		prout("  내려갈 수 없습니다.\"");
		return;
	}
	if (landed==1) {
		/* Coming from planet */
		if (d.plnets[iplnet].known==2) {
			proutn("스팍-  \"갈릴레오를 가져가시는 것이 좋지 않겠습니까?\" ");
			if (ja() != 0) {
				chew();
				return;
			}
			prout("승무원들이 외계인에게 빼앗기지 않도록 갈릴레오를 숨겼습니다.");
		}
		prout("상륙대가 집결, 함선으로 전송 준비 완료.");
		skip(1);
		prout("커크 함장이 통신기를 꺼냅니다...");
		prouts("삐익  삐익  삐익");
		skip(2);
		prout("\"커크가 엔터프라이즈에게-  좌표를 고정하라… 전송 시작.\"");
	}
	else {
		/* Going to planet */
		if (d.plnets[iplnet].crystals==0) {
			prout("스팍-  \"함장님, 그 논리를 이해할 수 없습니다.");
			prout("  딜리튬 결정이 없는 행성을 탐사 중입니다.");
			proutn("  이게 과연 현명한 선택일까요?\" ");
			if (ja()==0) {
				chew();
				return;
			}
		}
		prout("스콧-  \"함장님, 전송실 준비가 완료됐습니다.\"");
		skip(1);
		prout("커크 함장과 상륙대가 행성 표면으로 전송을 준비합니다.");
		skip(1);
		prout("커크-  \"전송시작.\"");
	}
	skip(1);
	prouts("우우우웅웅웅...  .  .  .  .   .    .");
	skip(2);
	if (Rand() > 0.98) {
		prouts("부우우우웅 . . .");
		skip(2);
		prout("스콧-  \"이런!  그들을 놓쳤어!\"");
		finish(FLOST);
		return;
	}
	prouts(".    .   .  .  .  .  .에.에.에에에에이이이이이우우우");
	skip(2);
	prout("전송 완료.");
	landed = -landed;
	if (landed==1 && d.plnets[iplnet].known==2) {
		prout("셔틀 크래프트 갈릴레오가 여기 있습니다!");
	}
	if (landed!=1 && imine==1) {
		icrystl = 1;
		cryprob = 0.05;
	}
	imine = 0;
	return;
}

void mine(void) {

	ididit = 0;
	skip(1);
	chew();
	if (landed!= 1) {
		prout("채굴대가 행성에 없습니다.");
		return;
	}
	if (d.plnets[iplnet].crystals == 0) {
		prout("이 행성에는 딜리튬 결정이 없습니다.");
		return;
	}
	if (imine == 1) {
		prout("당신은 이번 항해에 충분한 결정을 이미 채굴했습니다.");
		return;
	}
	if (icrystl == 1 && cryprob == 0.05) {
		proutn("신선한 결정을 이미 탑재하여 ");
		crmshp();
		skip(1);
		prout("지금은 더 채굴할 이유가 없습니다.");
		return;
	}
	Time = (0.1+0.2*Rand())*d.plnets[iplnet].pclass;
	if (consumeTime()) return;
	prout("채굴 작전 완료.");
	imine = 1;
	return;
}

void usecrystals(void) {

	skip(1);
	chew();
	if (icrystl!=1) {
		prout("딜리튬 결정이 없습니다.");
		return;
	}
	if (energy >= 1000) {
		prout("스팍-  \"함장님, 스타플릿 규정상 그런 작전은 금지되어 있습니다.");
		prout("  황색경보 상태를 제외하고.");
		return;
	}
	prout("스팍- \"함장님, ");
	prout("  정제되지 않은 딜리튬 결정을 배의 동력으로 사용하면");
	prout("  시스템이 심각한 폭발 위험에 처할 수 있습니다.");
	proutn("  이게 과연 현명한 선택일까요?\" ");
	if (ja()==0) {
		chew();
		return;
	}
	skip(1);
	prout("엔지니어링 담당 스콧-  \"(꿀꺽) Aye 함장님.");
	prout("  스팍과 내가 시도할 것이다.\"");
	skip(1);
	prout("스팍-  \"결정이 준비되었습니다, 함장님.");
	prout("  회로 활성화 준비 완료.\"");
	skip(1);
	prouts("스콧-  \"행운을 빌어봅시다, 함장님!\"");
	skip(1);
	if (Rand() <= cryprob) {
		prouts("  \"지금 작동시켜! - - 안 통해! 소용없어!***");
		skip(2);
		prouts("***적.색.경.보!  적. ㅅ*ㅐ********************************");
		skip(1);
		stars();
		prouts("******************   콰아아-아앙!!!!   *******************");
		skip(1);
		kaboom();
		return;
	}
	energy += 5000.0*(1.0 + 0.9*Rand());
	prouts("  \"지금 작동시켜! - - ");
	prout("계측기가");
	prout("   날뜁니다, 하지만 이건!!");
	prout("   작동 성공!!  축하합니다, 함장님!\"");
	cryprob *= 2.0;
	return;
}

void shuttle(void) {

	chew();
	skip(1);
	ididit = 0;
	if(damage[DSHUTTL] != 0.0) {
		if (damage[DSHUTTL] == -1.0) {
			if (inorbit && d.plnets[iplnet].known == 2)
				prout("페어리 퀸에는 셔틀 격납고가 없습니다.");
			else
				prout("페어리 퀸에는 셔틀 크래프트가 없습니다.");
		}
		else if (damage[DSHUTTL] > 0)
			prout("갈릴레오가 손상되었습니다.");
		else prout("셔틀 크래프트는 현재 다른 임무 중입니다.");
		return;
	}
	if (inorbit==0) {
		crmshp();
		prout(" 표준 궤도에 있지 않습니다.");
		return;
	}
	if ((d.plnets[iplnet].known != 2) && iscraft != 1) {
		prout("셔틀 크래프트를 현재 사용할 수 없습니다.");
		return;
	}
	if (landed==-1 && d.plnets[iplnet].known==2) {
		prout("셔틀크래프트를 회수하려면 내려가야 합니다.");
		return;
	}
	if (shldup!=0 || condit == IHDOCKED) {
		prout("셔틀 크래프트는 실드를 통과할 수 없습니다.");
		return;
	}
	if (d.plnets[iplnet].known==0) {
		prout("스팍-  \"함장님, 이 행성에 대한 정보가 없습니다.");
		prout("  스타플릿 규정에 따르면 이 상황에서는");
		prout("  내려가시면 안됩니다.\"");
		return;
	}
	Time = 3.0e-5*height;
	if (Time >= 0.8*d.remtime) {
		prout("부장 스팍-  \"함장님, 계산하면 그런한");
		prout(" 기동은 잔여시간의 약 ");
		cramf(100*Time/d.remtime,0,4);
		prout("% 를");
		prout("소모할 것으로 보입니다.");
		prout("정말로 계속하시겠습니까?\" ");
		if (ja()==0) {
			Time = 0.0;
			return;
		}
	}
	if (landed == 1) {
		/* Kirk on planet */
		if (iscraft==1) {
			/* Galileo on ship! */
			if (damage[DTRANSP]==0) {
				proutn("스팍-  \"전송기를 사용하시겠습니까?\" ");
				if (ja() != 0) {
					beam();
					return;
				}
				proutn("셔틀 승무원");
			}
			else
				proutn("구조대");
			prout("이(가) 갈릴레오에 탑승하여 행성 표면으로 향합니다.");
			iscraft = 0;
			skip(1);
			if (consumeTime()) return;
			d.plnets[iplnet].known=2;
			prout("이동 완료.");
			return;
		}
		else {
			/* Ready to go back to ship */
			prout("당신과 채굴팀은 엔터프라이즈호로 돌아가기 위해");
			prout("셔틀 크래프트에 탑승하였습니다.");
			skip(1);
			prout("짧은 비행이 시작된다 . . .");
			d.plnets[iplnet].known=1;
			icraft = 1;
			skip(1);
			landed = -1;
			if (consumeTime()) return;
			iscraft = 1;
			icraft = 0;
			if (imine!=0) {
				icrystl = 1;
				cryprob = 0.05;
			}
			imine = 0;
			prout("이동 완료.");
			return;
		}
	}
	else {
		/* Kirk on ship */
		/* and so is Galileo */
		prout("채굴대가 격납 갑판에 집결,");
		prout("셔틀 크래프트 \"갈릴레오\" 탑승준비 완료.");
		skip(1);
		prouts("격납고 문이 열리고 여정이 시작된다.");
		skip(1);
		icraft = 1;
		iscraft = 0;
		if (consumeTime()) return;
		d.plnets[iplnet].known = 2;
		landed = 1;
		icraft = 0;
		prout("이동 완료.");
		return;
	}
}

void deathray(void) {
	double r = Rand();
	
	ididit = 0;
	skip(1);
	chew();
	if (ship != IHE) {
		prout("페어리 퀸에는 죽음의 광선이 탑재되어 있지 않습니다.");
		return;
	}
	if (nenhere==0) {
		prout("술루-  \"하지만 함장님, 이 구역에는 적이 존재하지 않습니다.\"");
		return;
	}
	if (damage[DDRAY] > 0.0) {
		prout("죽음의 광선이 손상되었습니다.");
		return;
	}
	prout("Spock-  \"함장님, 실험적 죽음의 광선");
	prout("  은 매우 예측이 불가능한 무기입니다.  다른 대안들도 감안하면,");
	prout("  이게 정말 현명한 판단이십니까?\" ");
	if (ja()==0) return;
	prout("스팍-  \"알겠습니다.\"");
	skip(1);
	ididit=1;
	prouts("삐.용 삐.용... 삐.용 삐.용... 삐.용 삐.용");
	skip(1);
	prout("승무원들이 비상 준비를 위해 분주히 움직인다.");
	prout("스팍과 스코티는 죽음의 광선을 준비하기 위하여,");
	prout("함선의 모든 동력 에네지를 장치로 집중시킬 준비를 한다.");
	skip(1);
	prout("스팍-  \"준비 완료되었습니다, 함장님.\"");
	prout("커크-  \"가동!\"");
	skip(1);
	prouts("위이이이이이이이이이이이이이이이이이이잉—");
	skip(1);
	if (r > .30) {
		prouts("술루- \"함장님!  동작합니다!\"");
		skip(2);
		while (nenhere > 0) {
			deadkl(kx[1],ky[1],quad[kx[1]][ky[1]],kx[1],ky[1]);
		}
		prout("소위 체코프-  \"축하합니다, 함장님!\"");
		if (d.remkl == 0) finish(FWON);
		prout("스팍-  \"함장님, '실험용 죽음의 광선'");
		if (Rand() <= 0.05) {
			prout("   은 아직 사용 할 수 있습니다.\"");
		}
		else {
			prout("   은 기능을 상실했습니다.\"");
			damage[DDRAY] = 39.95;
		}
		return;
	}
	r = Rand();	// Pick failure method 
	if (r <= .30) {
		prouts("술루- \"함장님!  됐습니다!\"");
		skip(1);
		prouts("구역을 안전하게 벗어났습니다.");
		skip(1);
		prout("***물질-반물질 반응 폭주, 내파 직전!**");
		skip(1);
		prouts("***적.색.경.보!  적. ㅅ*ㅐ********************************");
		skip(1);
		stars();
		prouts("******************   콰아아-아앙!!!!   *******************");
		skip(1);
		kaboom();
		return;
	}
	if (r <= .55) {
		prouts("술루- \"함장님!  야가반당강그라플, 브라크리그링란블라!\"");
		skip(1);
		prout("우후라 중위-  \"그라에엑! 그라에엑!\"");
		skip(1);
		prout("스팍-  \"흥미롭군요!  . . . 현재 함선의 모든 ");
		prout("  인간이 이상한 돌연변이로 변형된 것으로 보입니다.");
		prout("  벌칸인은 영향을 받지 않는 것 같습니다.");
		skip(1);
		prout("커크-  \"라아우치!  라아우치!\"");
		finish(FDRAY);
		return;
	}
	if (r <= 0.75) {
		int i,j;
		prouts("술루- \"함장님!  이건..  --뭐.뭣?!?!\"");
		skip(2);
		proutn("스팍-  \"그 표현은 ");
		prouts(" *경이롭다* 가 적절하겠군요,");
		prout(" 술루씨.");
		for (i=1; i<=10; i++)
			for (j=1; j<=10; j++)
				if (quad[i][j] == IHDOT) quad[i][j] = IHQUEST;
		prout("  함장님, 현재 우리 구역은 ");
		prouts(" - - - - - -  *그것들*로 가득찬 상태입니다.");
		skip(1);
		prout("  논리적으로 설명할 수 없습니다.\"");
		return;
	}
	prouts("술루- \"함장님!  죽음의 광선이 트리블을 만들어내고 있습니다!\"");
	skip(1);
	prout("스코티-  \"기관실에 트리블이 너무 많아서");
	prout("  기동할 수 없습니다, 함장님!\"");
	finish(FTRIBBLE);
	return;
}
