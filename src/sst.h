/* ============================================================
 * sst.h - Super Star Trek 헤더 (Windows/한국어 버전)
 * ============================================================ */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Windows 헤더 */
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define WIN32_LEAN_AND_MEAN
/* Windows defines min/max as macros - undefine them */
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

#ifndef INCLUDED
#define EXTERN extern
#else
#define EXTERN
#endif

/* 언어 시스템 포함 */
#include "lang.h"
/* lang.c additional */
const char *lang_resolve_command(const char *input);

// #define DEBUG
// #define SCORE
// #define CLOAKING
// #define CAPTURE

#ifdef CLOAKING
#define ndevice (16)
#else
#define ndevice (15)
#endif

#define phasefac (2.0)
#define PLNETMAX (10)
#define NEVENTS (8)

typedef struct {
    int x;
    int y;
    int pclass;
    int crystals;
    int known;
} PLANETS;

EXTERN struct foo {
    int snap,
        remkl,
        remcom,
        rembase,
        starkl,
        basekl,
        killk,
        killc,
        galaxy[9][9],
        cx[11],cy[11],
        baseqx[6],
        baseqy[6],
        newstuf[9][9],
        isx, isy,
        nscrem,
        nromkl,
        nromrem,
        nsckill,
        nplankl;
    PLANETS plnets[PLNETMAX+1];
#ifdef CAPTURE
    int kcaptured, brigfree;
#endif
    double date,
        remres,
        remtime;
} d, snapsht;

EXTERN char quad[11][11];

EXTERN struct foo2 {
    int inkling,
        inbase,
        incom,
        instar,
        intorps,
        condit,
        torps,
        ship,
        quadx,
        quady,
        sectx,
        secty,
        length,
        skill,
        basex,
        basey,
        klhere,
        comhere,
        casual,
        nhelp,
        nkinks,
        ididit,
        gamewon,
        alive,
        justin,
        alldone,
        shldchg,
        thingx,
        thingy,
        plnetx,
        plnety,
        inorbit,
        landed,
        iplnet,
        imine,
        inplan,
        nenhere,
        ishere,
        neutz,
        irhere,
        icraft,
        ientesc,
        iscraft,
        isatb,
        iscate,
#ifdef DEBUG
        idebug,
#endif
#ifdef CLOAKING
        iscloaked,
        iscloaking,
        ncviol,
        isviolreported,
#endif
#ifdef CAPTURE
        brigcapacity,
#endif
        iattak,
        icrystl,
        tourn,
        thawed,
        batx,
        baty,
        ithere,
        ithx,
        ithy,
        iseenit,
        probecx,
        probecy,
        proben,
        isarmed,
        nprobes;

    double inresor,
        intime,
        inenrg,
        inshld,
        inlsr,
        indate,
        energy,
        shield,
        shldup,
        warpfac,
        wfacsq,
        lsupres,
        dist,
        direc,
        Time,
        docfac,
        resting,
        damfac,
        stdamtim,
        cryprob,
        probex,
        probey,
        probeinx,
        probeiny;
} a;

#define inkling a.inkling
#define inbase a.inbase
#define incom a.incom
#define instar a.instar
#define intorps a.intorps
#define condit a.condit
#define torps a.torps
#define ship a.ship
#define quadx a.quadx
#define quady a.quady
#define sectx a.sectx
#define secty a.secty
#define length a.length
#define skill a.skill
#define basex a.basex
#define basey a.basey
#define klhere a.klhere
#define comhere a.comhere
#define casual a.casual
#define nhelp a.nhelp
#define nkinks a.nkinks
#define ididit a.ididit
#define gamewon a.gamewon
#define alive a.alive
#define justin a.justin
#define alldone a.alldone
#define shldchg a.shldchg
#define thingx a.thingx
#define thingy a.thingy
#define plnetx a.plnetx
#define plnety a.plnety
#define inorbit a.inorbit
#define landed a.landed
#define iplnet a.iplnet
#define imine a.imine
#define inplan a.inplan
#define nenhere a.nenhere
#define ishere a.ishere
#define neutz a.neutz
#define irhere a.irhere
#define icraft a.icraft
#define ientesc a.ientesc
#define iscraft a.iscraft
#define isatb a.isatb
#define iscate a.iscate
#ifdef DEBUG
#define idebug a.idebug
#endif
#ifdef CLOAKING
#define iscloaked a.iscloaked
#define iscloaking a.iscloaking
#define ncviol a.ncviol
#define isviolreported a.isviolreported
#endif
#ifdef CAPTURE
#define kcaptured d.kcaptured
#define brigfree d.brigfree
#define brigcapacity a.brigcapacity
#endif
#define iattak a.iattak
#define icrystl a.icrystl
#define tourn a.tourn
#define thawed a.thawed
#define batx a.batx
#define baty a.baty
#define ithere a.ithere
#define ithx a.ithx
#define ithy a.ithy
#define iseenit a.iseenit
#define inresor a.inresor
#define intime a.intime
#define inenrg a.inenrg
#define inshld a.inshld
#define inlsr a.inlsr
#define indate a.indate
#define energy a.energy
#define shield a.shield
#define shldup a.shldup
#define warpfac a.warpfac
#define wfacsq a.wfacsq
#define lsupres a.lsupres
#define dist a.dist
#define direc a.direc
#define Time a.Time
#define docfac a.docfac
#define resting a.resting
#define damfac a.damfac
#define stdamtim a.stdamtim
#define cryprob a.cryprob
#define probex a.probex
#define probey a.probey
#define probecx a.probecx
#define probecy a.probecy
#define probeinx a.probeinx
#define probeiny a.probeiny
#define proben a.proben
#define isarmed a.isarmed
#define nprobes a.nprobes

EXTERN int  kx[21], ky[21], starch[9][9];
EXTERN int  fromcommandline;
EXTERN int  coordfixed;

EXTERN char passwd[10];

/* device[] 배열: lang.c에서 INI 로드 후 채움 */
EXTERN char *device[ndevice+1];

EXTERN PLANETS nulplanet;

EXTERN double
    kpower[21],
    kdist[21],
    kavgd[21],
    damage[ndevice+1],
    future[NEVENTS+1];

EXTERN int iscore, iskill;
EXTERN double perdate;

typedef enum {FWON, FDEPLETE, FLIFESUP, FNRG, FBATTLE,
              FNEG3, FNOVA, FSNOVAED, FABANDN, FDILITHIUM,
              FMATERIALIZE, FPHASER, FLOST, FMINING, FDPLANET,
              FPNOVA, FSSC, FSTRACTOR, FDRAY, FTRIBBLE,
              FHOLE
#ifdef CLOAKING
   , FCLOAK
#endif
} FINTYPE;

typedef enum {SNOVICE=1, SFAIR, SGOOD, SEXPERT, SEMERITUS} SKILLTYPE;

EXTERN double aaitem;
EXTERN char citem[24];

/* 장치 번호 정의 */
#define DSRSENS 1
#define DLRSENS 2
#define DPHASER 3
#define DPHOTON 4
#define DLIFSUP 5
#define DWARPEN 6
#define DIMPULS 7
#define DSHIELD 8
#define DRADIO  9
#define DSHUTTL 10
#define DCOMPTR 11
#define DTRANSP 12
#define DSHCTRL 13
#define DDRAY   14
#define DDSP    15
#define DCLOAK  16

/* 미래 이벤트 */
#define FSPY    0
#define FSNOVA  1
#define FTBEAM  2
#define FSNAP   3
#define FBATTAK 4
#define FCDBAS  5
#define FSCMOVE 6
#define FSCDBAS 7
#define FDSPROB 8

#ifdef INCLUDED
PLANETS nulplanet = {0};

/* device[] 배열 - lang.c에서 INI 로드 후 설정됨 */
char *device[ndevice+1];
#endif

#define ALGERON (2311)

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define IHR 'R'
#define IHK 'K'
#define IHC 'C'
#define IHS 'S'
#define IHSTAR '*'
#define IHP 'P'
#define IHB 'B'
#define IHBLANK ' '
#define IHDOT '.'
#define IHQUEST '?'
#define IHE 'E'
#define IHF 'F'
#define IHT 'T'
#define IHWEB '#'
#define IHGREEN 'G'
#define IHYELLOW 'Y'
#define IHRED 'R'
#define IHDOCKED 'D'

/* 함수 프로토타입 */
void prelim(void);
void attack(int);
int choose(void);
void setup(void);
void score(int);
void atover(int);
void srscan(int);
void lrscan(void);
void phasers(void);
void photon(void);
void warp(int);
void sheild(int);
void dock(void);
void dreprt(void);
void chart(int);
void impuls(void);
void waiting(void);
void setwrp(void);
void events(void);
void report(int);
void eta(void);
void help(void);
void abandn(void);
void finish(FINTYPE);
void dstrct(void);
void kaboom(void);
void freeze(int);
void thaw(void);
void plaque(void);
int scan(void);
#define IHEOL (0)
#define IHALPHA (1)
#define IHREAL (2)
void chew(void);
void chew2(void);
void skip(int);
void prout(char *s);
void proutn(char *s);
void stars(void);
void newqad(int);
int ja(void);
void cramen(int);
void crmshp(void);
void cramlc(int, int, int);
double expran(double);
double Rand(void);
void iran8(int *, int *);
void iran10(int *, int *);
double square(double);
void dropin(int, int*, int*);
void newcnd(void);
void sortkl(void);
void lmove(void);
void ram(int, int, int, int);
void crmena(int, int, int, int, int);
void deadkl(int, int, int, int, int);
void timwrp(void);
void movcom(void);
void torpedo(double, double, int, int, double *);
void cramf(double, int, int);
void crami(int, int);
void huh(void);
void pause_game(int);
void nova(int, int);
void snova(int, int);
void scom(int *);
void hittem(double *);
void prouts(char *);
int isit(char *);
void preport(void);
void orbit(void);
void sensor(void);
void beam(void);
void mine(void);
void usecrystals(void);
void shuttle(void);
void deathray(void);
void debugme(void);
void attakreport(void);
void movetho(void);
void probe(void);

int min(int, int);
int max(int, int);
void randomize(void);
int getch_compat(void);
void win_setup_console(void);

#ifdef CLOAKING
void cloak(void);
#endif
#ifdef CAPTURE
void capture(void);
#endif

#ifdef CLOAKING
#define REPORTS ((condit==IHDOCKED || damage[DRADIO]<=0.0) && !iscloaked)
#else
#define REPORTS (condit==IHDOCKED || damage[DRADIO]<=0.0)
#endif
