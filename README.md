# 슈퍼 스타 트렉 (Super Star Trek) - 한국어 버전

클래식 텍스트 기반 우주 전략 게임을 한국어로 완전 번역한 Windows 콘솔 버전입니다.

---

## 📋 요구 사항

| 항목 | 내용 |
|------|------|
| OS | Windows 10 / 11 |
| 컴파일러 | MinGW-w64 (GCC 13+) |
| 편집기 | Visual Studio Code |
| 인코딩 | UTF-8 |

---

## ⚙️ 설치 및 빌드

### 1단계: MSYS2 설치 (MinGW-w64 포함)
```
https://www.msys2.org 에서 설치 프로그램 다운로드 후 실행
```

### 2단계: MinGW-w64 GCC 설치
MSYS2 터미널(MINGW64)을 열고 실행:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

### 3단계: PATH 환경변수 설정
시스템 환경변수 PATH에 추가:
```
C:\msys64\mingw64\bin
```

### 4단계: Visual Studio Code 확장 설치
- **C/C++** (Microsoft) - IntelliSense 지원
- **C/C++ Extension Pack** - 전체 팩

### 5단계: VSCode에서 빌드
1. `SST-Korean` 폴더를 VSCode로 열기
2. **터미널 → 작업 실행** (또는 `Ctrl+Shift+B`)
3. `🚀 빌드 - 슈퍼 스타 트렉` 선택

또는 폴더에서 `build.bat`를 더블클릭.

### 빌드 결과
```
SST-Korean/
├── sst.exe        ← 실행 파일 (단일 EXE)
├── sst.ini        ← 한국어 설정 파일 (수정 가능)
└── sst_plaque.txt ← 게임 클리어 명예의 전당 (자동 생성)
```

---

## 🎮 게임 실행

```cmd
cd SST-Korean
sst.exe
```

또는 `sst.exe`를 더블클릭 (cmd 창이 열립니다)

> **폰트 설정**: 콘솔 창 제목 표시줄 우클릭 → 속성 → 글꼴  
> **맑은 고딕** 또는 **굴림체** 선택 (한국어 표시)

---

## 🕹️ 명령어 목록

영어와 한국어 명령어를 모두 입력할 수 있습니다.

| 한국어 명령어 | 영어 명령어 | 설명 |
|--------------|------------|------|
| 단거리스캔   | srscan     | 현재 구역 단거리 스캔 |
| 장거리스캔   | lrscan     | 인근 구역 장거리 스캔 |
| 페이저       | phasers    | 페이저 발사 |
| 광자어뢰     | photons    | 광자 어뢰 발사 |
| 이동         | move       | 워프 이동 |
| 실드         | shields    | 실드 제어 |
| 도킹         | dock       | 스타베이스 도킹 |
| 손상보고     | damages    | 장치 손상 보고 |
| 성도         | chart      | 은하 성도 표시 |
| 임펄스 엔진     | impulse    | 임펄스 엔진 이동 |
| 휴식         | rest       | 수리 대기 |
| 워프         | warp       | 워프 계수 설정 |
| 상태         | status     | 함선 상태 표시 |
| 센서         | sensors    | 행성 센서 스캔 |
| 궤도         | orbit      | 행성 궤도 진입 |
| 전송         | transport  | 행성 전송 |
| 채굴         | mine       | 딜리튬 채굴 |
| 결정         | crystals   | 딜리튬 결정 사용 |
| 셔틀         | shuttle    | 셔틀 크래프트 사용 |
| 행성         | planets    | 행성 정보 |
| 보고         | report     | 게임 현황 보고 |
| 컴퓨터       | computer   | ETA 계산 |
| 명령어목록   | commands   | 명령어 목록 표시 |
| 탐침         | probe      | 심우주 탐침 발사 |
| 저장         | freeze     | 게임 저장 |
| 종료         | quit       | 게임 종료 |
| 자폭         | destruct   | 함선 자폭 |
| 함선포기     | abandon    | 함선 포기 |

---

## ⚙️ 한국어 설정 파일 (sst.ini)

`sst.ini` 파일에서 다음 항목을 자유롭게 수정할 수 있습니다:

```ini
[Commands]      ; 명령어 한국어 별칭
[Devices]       ; 함선 장치 이름
[ProperNouns]   ; 고유명사 (행성, 종족 등)
[SkillLevels]   ; 난이도 이름
[GameLength]    ; 게임 길이 이름
```

---

## 🎯 게임 목표

연방이 클링온의 침략군에 의해 공격받고 있습니다.
USS 엔터프라이즈의 함장으로서 제한된 시간 내에
모든 클링온 함선을 격파해야 합니다.

### 은하 지도 기호
| 기호 | 의미 |
|------|------|
| `E`  | 엔터프라이즈 |
| `K`  | 클링온 |
| `C`  | 클링온 사령관 |
| `S`  | 슈퍼사령관 |
| `R`  | 로뮬란 |
| `B`  | 스타베이스 |
| `*`  | 별 |
| `P`  | 행성 |
| `T`  | 솔리안 |
| `.`  | 빈 공간 |
| `#`  | 솔리안 웹 |
| ` `  | 블랙홀 |

---

## 📁 파일 구조

```
SST-Korean/
├── .vscode/
│   ├── tasks.json          VSCode 빌드 작업
│   ├── settings.json       편집기 설정
│   └── c_cpp_properties.json  IntelliSense 설정
├── src/
│   ├── sst.h               헤더 파일 (Windows/한국어 호환)
│   ├── sst.c               메인 루프 및 유틸리티
│   ├── setup.c             게임 초기화
│   ├── battle.c            전투 시스템
│   ├── events.c            이벤트 처리
│   ├── finish.c            게임 종료
│   ├── moving.c            이동 시스템
│   ├── planets.c           행성 상호작용
│   ├── reports.c           보고서 및 스캔
│   ├── ai.c                적 AI
│   ├── lang.h              언어 시스템 헤더
│   ├── lang.c              INI 파서 및 한국어 로더
│   └── win_compat.c        Windows 호환 레이어
├── sst.ini                 한국어 설정 (수정 가능)
├── build.bat               빌드 스크립트
└── README.md               이 파일
```

---

## 🐛 문제 해결

**한국어가 깨져 보이는 경우:**
1. 콘솔 창 → 우클릭 → 속성 → 글꼴 → **맑은 고딕** 선택
2. 또는 실행 전 `chcp 65001` 입력

**gcc 명령을 찾을 수 없는 경우:**
```cmd
set PATH=C:\msys64\mingw64\bin;%PATH%
```

**빌드 오류가 발생하는 경우:**
VSCode 터미널에서:
```cmd
gcc -v
```
GCC 버전이 출력되면 정상입니다.

---

원본 게임: Eric S. Raymond 의 Super Star Trek (C 버전)  
한국어화: Claude AI 지원 (2025)
