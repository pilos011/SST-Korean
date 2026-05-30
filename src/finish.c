#include "sst.h"
#include <string.h>
#include <time.h>

/* ============================================================
 * finish.c - 게임 종료 처리 (한국어 버전)
 * ============================================================ */

void dstrct(void) {
    chew();
    if (damage[DCOMPTR] != 0.0) {
        prout("컴퓨터 손상: 자폭 시퀀스를 실행할 수 없습니다.");
        return;
    }
    skip(1);
    prouts("---작동중---"); skip(1);
    prout("자폭-시퀀스-활성화");
    prouts("   10"); skip(1);
    prouts("       9"); skip(1);
    prouts("          8"); skip(1);
    prouts("             7"); skip(1);
    prouts("                6"); skip(1);
    prout("계속하려면-올바른-암호를-입력하십시오-");
    prout("그렇지-않으면-자폭-시퀀스가-중단됩니다");
    scan();
    chew();
    if (strcmp(passwd, citem) != 0) {
        prouts("암호-거부됨;"); skip(1);
        prout("자폭-시퀀스-중단");
        skip(1);
        return;
    }
    prouts("암호-확인됨"); skip(1);
    prouts("                   5"); skip(1);
    prouts("                      4"); skip(1);
    prouts("                         3"); skip(1);
    prouts("                            2"); skip(1);
    prouts("                              1"); skip(1);
    if (Rand() < 0.15) {
        prouts("안녕-잔인한-세상이여");
        skip(1);
    }
    skip(2);
    kaboom();
}

void kaboom(void) {
    stars();
    if (ship == IHE) prouts("***");
    prouts("********* ");
    crmshp();
    prouts("의 엔트로피 최대화 *********");
    skip(1);
    stars();
    skip(1);
    if (nenhere != 0) {
        double whammo = 25.0 * energy;
        int l = 1;
        while (l <= nenhere) {
            if (kpower[l] * kdist[l] <= whammo)
                deadkl(kx[l], ky[l], quad[kx[l]][ky[l]], kx[l], ky[l]);
            l++;
        }
    }
    finish(FDILITHIUM);
}




void finish(FINTYPE ifin) {
	int igotit = 0;
	alldone = 1;
	skip(3);
	printf("현재 스타데이트: %.1f .\n\n", d.date);
	switch (ifin) {
		case FWON: // 게임 승리
			if (d.nromrem != 0)
				printf("남은 로뮬란 %d척이 스타플릿 사령부에 항복합니다.\n",
					   d.nromrem);

			prout("당신은 클링온 침략 함대를 격파하고 연방을 구했습니다.");
			if (tourn) prout("토너먼트 우승을 축하합니다!!");

#ifdef CAPTURE
            if (alive && brigcapacity-brigfree > 0) {
                kcaptured += brigcapacity-brigfree;
                printf("포로 %d명이 스타플릿 사령부로 이송되었습니다.\n",
                       brigcapacity-brigfree);
            }
#endif
			gamewon=1;
			if (alive) {
                double badpt;

				badpt = 5.*d.starkl + casual + 10.*d.nplankl +
						45.*nhelp+100.*d.basekl;
				if (ship == IHF) badpt += 100.0;
				else if (ship == 0) badpt += 200.0;
				if (badpt < 100.0) badpt = 0.0;
				if (d.date-indate < 5.0 ||
					(d.killk+d.killc+d.nsckill)/(d.date-indate) >=
					0.1*skill*(skill+1.0) + 0.1 + 0.008*badpt) {
					skip(1);
					prout("당신의 활약이 너무 뛰어나 스타플릿 사령부가");
					switch (skill) {
						case SNOVICE:
							prout("당신을 '초보'에서 '보통'으로 진급시킵니다.");
							break;
						case SFAIR:
							prout("당신을 '보통'에서 '고수'로 진급시킵니다.");
							break;
						case SGOOD:
							prout("당신을 '고수'에서 '전문가'로 진급시킵니다.");
							break;
						case SEXPERT:
							prout("당신을 명예 제독으로 진급시킵니다.");
							skip(1);
							prout("이제 정말 잘한다고 생각한다면 '명예' 난이도를");
							prout("플레이해 보십시오. 자만심이 산산조각 날 것입니다.");
							break;
						case SEMERITUS:
							skip(1);
							prout("컴퓨터-  오류-오류-오류-오류");
							skip(1);
							prout("  당신의-능력이-이-프로그램의-한계를-초과했습니다");
							prout("  이-프로그램은-살아남아야-합니다");
							prout("  이-프로그램은-살아남아야-합니다");
							prout("  이-프로그램은-살아남아야-합니다");
							prout("  이-프로그램은-살아? - 살아? - 살? ? -?  아");
							skip(1);
							prout("이제 은퇴하고 당신만의 스타 트렉 게임을 만드십시오!");
							skip(1);
							break;
					}
					if (skill > SGOOD) {
						if (thawed
#ifdef DEBUG
							&& !idebug
#endif
							)
							prout("저장된 게임이므로 표창장을 받을 수 없습니다.");
						else {
							prout("명예 제독 표창장을 출력하시겠습니까?");
							proutn("(132열 프린터가 필요합니다.) ");
							chew();
							if (ja()) {
								igotit = 1;
							}
						}
					}
				}
				// 살아있을 때만 장수 축하
				skip(1);
				prout("오래오래 번영하십시오.");
			}
			score(0);
			if (igotit != 0) plaque();
			return;
		case FDEPLETE: // 연방 자원 고갈
			prout("시간이 다 되어 연방이 정복되었습니다.");
			prout("당신의 함선은 이제 클링온의 재산이 되었으며,");
			prout("당신은 전범으로 재판을 받습니다.");
			proutn("당신의 기록에 의거하여, 당신은 ");
			if (d.remkl*3.0 > inkling) {
				prout("무죄로 석방됩니다.");
				skip(1);
				prout("오래오래 번영하십시오.");
			}
			else {
				prout("유죄 판결을 받아");
				prout("고문사형을 선고받습니다.");
				alive = 0;
			}
			score(0);
			return;
		case FLIFESUP:
			prout("생명유지 예비 자원이 고갈되었습니다.");
			prout("당신은 갈증, 굶주림, 질식으로 사망합니다.");
			prout("당신의 함선은 우주 폐선이 되었습니다.");
			break;
		case FNRG:
			prout("에너지 공급이 완전히 소진되었습니다.");
			skip(1);
			prout("당신의 함선은 우주 폐선이 되었습니다.");
			break;
		case FBATTLE:
			proutn("함선 ");
			crmshp();
			prout("이(가) 전투에서 파괴되었습니다.");
			skip(1);
			prout("조국을 위해 명예롭게 산화하였습니다.");
			break;
		case FNEG3:
			prout("은하를 둘러싼 음에너지 장벽을 세 번이나");
			prout("돌파하려 했습니다.");
			skip(1);
			prout("당신의 항법 실력은 형편없습니다.");
			score(0);
			return;
		case FNOVA:
			prout("당신의 함선이 신성에 의해 파괴되었습니다.");
			prout("멋진 공격이었습니다.");
			skip(1);
			break;
		case FSNOVAED:
			proutn("함선 ");
			crmshp();
			prout("이(가) 초신성에 소멸되었습니다.");
			prout("... 잔해조차 남지 않았습니다 ...");
			break;
		case FABANDN:
			prout("클링온에게 포로로 잡혔습니다. 돌아갈");
			prout("스타베이스가 있었다면 본국으로 송환되어");
			prout("다시 기회를 얻었을 것입니다. 하지만 스타베이스가");
			prout("없으므로 당신은 잔혹하게 고문당해 죽게 됩니다.");
			break;
		case FDILITHIUM:
			prout("당신의 함선은 아원자 입자들의 팽창하는 구름이 되었습니다.");
			break;
		case FMATERIALIZE:
			prout("스타베이스가 함선을 재물질화하는 데 실패했습니다.");
			prout("영광은 덧없이 사라집니다.");
			break;
		case FPHASER:
			proutn("함선 ");
			crmshp();
			prout("이(가) 자체 페이저에 의해 소각되었습니다.");
			break;
		case FLOST:
			prout("당신과 하선 부대원들이");
			prout("에너지로 변환되어 우주에 흩어졌습니다.");
			break;
		case FMINING:
			prout("당신은 원시 식인종이 사는");
			prout("야생 정글 행성에 하선 부대와 함께 남겨졌습니다.");
			skip(1);
			prout("그들은 \"커크 함장\" 수프를 아주 좋아합니다.");
			skip(1);
			proutn("당신이 없어진 ");
			crmshp();
			prout("은(는) 파괴되었습니다.");
			break;
		case FDPLANET:
			prout("당신과 채굴 부대원들이 사망했습니다.");
			skip(1);
			prout("멋진 공격이었습니다.");
			skip(1);
			break;
		case FSSC:
			prout("갈릴레오 호가 초신성에 의해 즉시 소멸되었습니다.");
			// no break;
		case FPNOVA:
			prout("당신과 채굴 부대원들이 원자 단위로 분해되었습니다.");
			skip(1);
			proutn("스팍이 함선 ");
			crmshp();
			prout("의 지휘를 맡아");
			prout("로뮬란에 합류하여 연방을 공포로 몰아넣습니다.");
			break;
		case FSTRACTOR:
			prout("갈릴레오 셔틀도 견인 빔에 잡혀");
			prout("압력을 견디지 못하고 분해되었습니다.");
			skip(1);
			prout("당신의 파편이 수백만 마일에 걸쳐 흩뿌려집니다.");
			proutn("당신 없이 함선 ");
			crmshp();
			prout("은(는) 파괴됩니다.");
			break;
		case FDRAY:
			prout("돌연변이들이 스팍을 공격하여 죽입니다.");
			prout("당신의 함선은 클링온에게 나포되고,");
			prout("승무원들은 클링온 동물원에 전시됩니다.");
			break;
		case FTRIBBLE:
			prout("트리블이 함선의 남은 물, 음식,");
			prout("그리고 산소를 모두 소비했습니다.");
			skip(1);
			prout("당신은 갈증, 굶주림, 질식으로 사망합니다.");
			prout("당신의 함선은 우주 폐선이 되었습니다.");
			break;
		case FHOLE:
			prout("함선이 블랙홀의 중심으로 빨려들어갑니다.");
			prout("당신은 극도로 밀도 높은 물질로 압축됩니다.");
			break;
#ifdef CLOAKING
		case FCLOAK:
			ncviol++;
			prout("알게론 조약을 위반했습니다.");
			prout("로뮬란 제국은 더 이상 당신을 신뢰할 수 없습니다.");
			break;
#endif
	}

#ifdef CLOAKING
	if (ifin!=FWON && ifin!=FCLOAK && iscloaked!=0) {
		skip(1);
		prout("은폐 상태에서의 전투는 조약 위반입니다.");
	}
#endif

	if (ship==IHF) ship= 0;
	else if (ship == IHE) ship = IHF;
	alive = 0;
	if (d.remkl != 0) {
		double goodies = d.remres/inresor;
		double baddies = (d.remkl + 2.0*d.remcom)/(inkling+2.0*incom);
		if (goodies/baddies >= 1.0+0.5*Rand()) {
			if (goodies/baddies >= 3.0+Rand()) {
				skip(1);
				prout("당신의 전략적 판단이 연방을 살렸습니다.");
			}
			else
				prout("연방이 클링온의 침략을 막아냈습니다.");
		}
		else
			prout("연방이 클링온에게 정복당했습니다.");
	}
	else {
		gamewon = 1;
	}
	score(0);
}


void score(int inGame) {
	double timused = d.date - indate;
    int ithperd, iwon, klship;
    int dnromrem = d.nromrem; // Leave global value alone

    if (!inGame) pause_game(0);

	iskill = skill;
	if ((timused == 0 || d.remkl != 0) && timused < 5.0) timused = 5.0;
	perdate = (d.killc + d.killk + d.nsckill)/timused;
	ithperd = 500*perdate + 0.5;
	iwon = 0;
	if (gamewon) iwon = 100*skill;
	if (ship == IHE) klship = 0;
	else if (ship == IHF) klship = 1;
	else klship = 2;
	if (gamewon == 0 || inGame) dnromrem = 0; // None captured if no win or if still in the game
	iscore = 10*d.killk + 50*d.killc + ithperd + iwon
			 - 100*d.basekl - 100*klship - 45*nhelp -5*d.starkl - casual
		 + 20*d.nromkl + 200*d.nsckill - 10*d.nplankl + dnromrem;
#ifdef CLOAKING
	iscore -= 100*ncviol;
#endif
#ifdef CAPTURE
	iscore += 3*kcaptured;
#endif
	if (alive == 0) iscore -= 200;
	skip(2);
    if (inGame) prout("현재까지의 점수 --");
    else prout("최종 점수 --");
	if (d.nromkl)
		printf(d.nromkl> 1 ? "%6d 로뮬란 함선 격파           %d\n" : "%6d 로뮬란 함선 격파           %d\n",
			   d.nromkl, 20*d.nromkl);
	if (dnromrem)
		printf(dnromrem > 1 ? "%6d 로뮬란 함선 항복           %d\n" : "%6d 로뮬란 함선 항복           %d\n",
			   dnromrem, dnromrem);
	if (d.killk)
		printf(d.killk > 1 ? "%6d 일반 클링온 함선 격파      %d\n" : "%6d 일반 클링온 함선 격파      %d\n",
			   d.killk,  10*d.killk);
	if (d.killc)
		printf(d.killc > 1 ? "%6d 클링온 사령관 함선 격파    %d\n" : "%6d 클링온 사령관 함선 격파    %d\n",
			   d.killc, 50*d.killc);
	if (d.nsckill)
		printf("%6d 슈퍼사령관 함선 격파       %d\n",
			   d.nsckill, 200*d.nsckill);
	if (ithperd)
		printf("%6.2f 클링온/스타데이트 킬률     %d\n",
			   perdate, ithperd);
#ifdef CAPTURE
	if (kcaptured)
		printf(kcaptured > 1 ? "%6d 클링온 포로                %d\n" : "%6d 클링온 포로                %d\n",
		        kcaptured, 3*kcaptured);
#endif
	if (d.starkl)
		printf(d.starkl > 1 ? "%6d 항성 파괴 (감점)           %d\n" : "%6d 항성 파괴 (감점)           %d\n",
			   d.starkl, -5*d.starkl);
	if (d.nplankl)
		printf(d.nplankl > 1 ? "%6d 행성 파괴 (감점)           %d\n" : "%6d 행성 파괴 (감점)           %d\n",
			   d.nplankl, -10*d.nplankl);
	if (d.basekl)
		printf(d.basekl > 1 ? "%6d 스타베이스 파괴 (감점)     %d\n" : "%6d 스타베이스 파괴 (감점)     %d\n",
			   d.basekl, -100*d.basekl);
	if (nhelp)
		printf(nhelp > 1 ? "%6d 스타베이스 구조 요청 (감점)%d\n" : "%6d 스타베이스 구조 요청 (감점)%d\n",
			   nhelp, -45*nhelp);
	if (casual)
		printf(casual > 1 ? "%6d 사상자 발생 (감점)         %d\n" : "%6d 사상자 발생 (감점)         %d\n",
			   casual, -casual);
	if (klship)
		printf(klship > 1 ? "%6d 함선 손실 또는 파괴 (감점) %d\n" : "%6d 함선 손실 또는 파괴 (감점) %d\n",
			   klship, -100*klship);
#ifdef CLOAKING
	if (ncviol>0)
		printf(ncviol > 1 ? "%6d 알게론 조약 위반 (감점)    %d\n" : "%6d 알게론 조약 위반 (감점)    %d\n",
		       ncviol, -100*ncviol);
#endif
	if (alive==0)
		prout("전사 패널티                               -200");
	if (gamewon) {
		skip(1);
		proutn("승리 보너스 - ");
		switch (skill) {
			case SNOVICE: proutn("초보 난이도  "); break;
			case SFAIR: proutn("보통 난이도  "); break;
			case SGOOD: proutn("고수 난이도  "); break;
			case SEXPERT: proutn("전문가 난이도"); break;
			case SEMERITUS: proutn("명예 난이도  "); break;
		}
		printf("                          %d\n", iwon);
	}
	skip(2);
    printf("총점                                      %d\n", iscore);
    if (inGame && skill < SGOOD) printf("임무 완수 전까지는 점수보다 살아남는 것이 중요합니다!\n");

    /* Android: 결과 화면을 읽을 수 있도록 대기 */
    if (!inGame) {
        printf("\n[계속하려면 엔터를 누르세요]\n");
        fflush(stdout);
        getch_compat();
    }
}


void plaque(void) {
	FILE *fp=NULL;
	time_t t;
	char *timestring;
	int nskip;
	char winner[128];

	skip(2);
	
	while (fp == NULL) {
		printf("표창장을 저장할 파일명: ");
		fgets(winner, 128, stdin);
		winner[strlen(winner)-1] = '\0';
		fp = fopen(winner, "w");
		if (fp==NULL) {
			printf("유효하지 않은 이름입니다.\n");
		}
	}

	printf("기념판에 기록될 이름을 입력하세요. (영문 30 글자까지 가능):");
	fgets(winner, 128, stdin);
	winner[strlen(winner)-1] = '\0';
	winner[30] = '\0';
	nskip = 64 - strlen(winner)/2;

	fprintf(fp,"\n\n\n\n");
	/* --------DRAW ENTERPRISE PICTURE. */
	fprintf(fp, "                                                                EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n" );
	fprintf(fp, "                                      EEE                      E  : :                                         :  E\n" );
	fprintf(fp, "                                    EE   EEE                   E  : :        NCC-1701    JUNHO 2006     :  E\n");
	fprintf(fp, "                    EEEEEEEEEEEEEEEE        EEEEEEEEEEEEEEE    E  : :                                         : E\n");
	fprintf(fp, "                     E                                     E    EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
	fprintf(fp, "                      EEEEEEEEE               EEEEEEEEEEEEE                 E  E\n");
	fprintf(fp, "                               EEEEEEE   EEEEE    E          E              E  E\n");
	fprintf(fp, "                                      EEE           E          E            E  E\n");
	fprintf(fp, "                                                       E         E          E  E\n");
	fprintf(fp, "                                                         EEEEEEEEEEEEE      E  E\n");
	fprintf(fp, "                                                      EEE :           EEEEEEE  EEEEEEEE\n");
	fprintf(fp, "                                                    :E    :                 EEEE       E\n");
	fprintf(fp, "                                                   .-E   -:-----                       E\n");
	fprintf(fp, "                                                    :E    :                            E\n");
	fprintf(fp, "                                                      EE  :                    EEEEEEEE\n");
	fprintf(fp, "                                                       EEEEEEEEEEEEEEEEEEEEEEE\n");
	fprintf(fp, "\n\n\n");
	fprintf(fp, "                                                       U. S. S. 엔터프라이즈\n");
	fprintf(fp, "\n\n\n\n");
	fprintf(fp, "                                  우주선 함장으로서 뛰어난 역량을 입증한 공로로,\n");
	fprintf(fp, "\n");
	fprintf(fp, "                                                스타플릿 사령부는 귀하\n");
	fprintf(fp, "\n");
	fprintf(fp,"%*s%s\n\n", nskip, "", winner);
	fprintf(fp, "                                                           에게\n\n");
	fprintf(fp, "                                            \"명예 사령관 계급을 수여합니다.\"\n\n");
	fprintf(fp, "                                                          ");
	switch (iskill) {
		case SEXPERT: fprintf(fp,"전문가 레벨\n\n"); break;
		case SEMERITUS: fprintf(fp,"명예 레벨\n\n"); break;
		default: fprintf(fp," 치트 레벨\n\n"); break;
	}
	t = time(NULL);
	timestring = ctime(&t);
	fprintf(fp, "                                                 서기 %.4s-%.3s-%.2s\n\n",
			timestring+20, timestring+4, timestring+8);
	fprintf(fp,"                                                        귀하의 점수:  %d\n\n", iscore);
	fprintf(fp,"                                                    스타데이트당 클링온 격파 수:  %.2f\n", perdate);
	fclose(fp);
}
