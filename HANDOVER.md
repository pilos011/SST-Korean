# HANDOVER.md — Super Star Trek 한국어판 인수인계 문서

> 이 문서는 다른 Claude 세션이 작업을 이어받을 수 있도록  
> 지금까지의 진행 상황, 기술 결정, 미해결 사항을 정리한 것입니다.

---

## 1. 프로젝트 개요

| 항목 | 내용 |
|------|------|
| **원본** | Super Star Trek (C 버전, Eric S. Raymond 계열) |
| **원본 파일** | `super-startrek-main.zip` (Linux용 소스 7000줄) |
| **목표** | 완전 한글화 + Windows 64비트 단일 EXE 빌드 |
| **결과물** | `SST-Korean.zip` (EXE + 소스 + 도움말 + 설정 파일) |

---

## 2. 파일 구조

```
SST-Korean/
├── sst.exe          ← Windows 64비트 실행 파일 (479KB)
├── sst.doc          ← 한국어 인게임 도움말 (38KB, 순수 텍스트)
├── sst.ini          ← 명령어/장치명/고유명사 설정 파일 (수정 가능)
├── build.bat        ← Windows 빌드 스크립트 (순수 ASCII 필수!)
├── README.md        ← 한국어 사용 설명서
├── HANDOVER.md      ← 이 파일
├── src/
│   ├── sst.h        ← 헤더 (Windows 호환 + 한국어 지원)
│   ├── sst.c        ← 메인 루프, scan(), ja(), listCommands()
│   ├── setup.c      ← 게임 초기화, choose() (게임유형/난이도 선택)
│   ├── battle.c     ← 전투: 페이저, 어뢰, 실드, 공격처리
│   ├── events.c     ← 이벤트: 초신성, 견인빔, 기지공격
│   ├── finish.c     ← 게임 종료: 점수계산, 상패
│   ├── moving.c     ← 이동: 워프, 임펄스, 탐침, 구조요청
│   ├── planets.c    ← 행성: 궤도, 전송, 채굴, 셔틀
│   ├── reports.c    ← 보고: srscan, lrscan, 성도, 상태
│   ├── ai.c         ← 적 AI: 사령관/슈퍼사령관 이동
│   ├── lang.h       ← 언어 시스템 헤더
│   ├── lang.c       ← INI 파서 + 한국어 명령어 해석
│   └── win_compat.c ← Windows 플랫폼 레이어 (osx.c 대체)
└── .vscode/
    ├── tasks.json          ← VSCode 빌드 태스크 (ASCII만 사용)
    ├── settings.json
    └── c_cpp_properties.json
```

---

## 3. 빌드 방법

### 크로스 컴파일 (Linux → Windows EXE)
```bash
x86_64-w64-mingw32-gcc -O2 \
  -Wno-unused-result -Wno-unused-variable -Wno-unused-function \
  -Wno-multichar -Wno-array-bounds \
  -finput-charset=UTF-8 -fexec-charset=UTF-8 \
  -Isrc \
  src/sst.c src/setup.c src/battle.c src/events.c \
  src/finish.c src/moving.c src/planets.c src/reports.c \
  src/ai.c src/lang.c src/win_compat.c \
  -o sst.exe -lm
```

### Windows에서 빌드
- MSYS2 설치 후 `pacman -S mingw-w64-x86_64-gcc`
- PATH에 `C:\msys64\mingw64\bin` 추가
- `build.bat` 더블클릭 또는 VSCode `Ctrl+Shift+B`

### ⚠️ build.bat 인코딩 주의
`build.bat`은 반드시 **순수 ASCII** 인코딩이어야 합니다.  
한국어 Windows의 CMD는 CP949로 배치파일을 읽어 UTF-8 한국어 주석이 깨집니다.  
→ 주석은 영어로만 작성하세요.

---

## 4. 핵심 기술 결정 및 구현

### 4-1. 한국어 입력 처리 (`sst.c` → `scan()`)

**문제**: 원본 `scan()`은 `isalpha()`(ASCII 전용)만 체크해 한국어 UTF-8 바이트 무시  
**해결**:
```c
if (isalpha((unsigned char)*linep) || *linep == '_' ||
    (unsigned char)*linep >= 0x80) {
    // 한국어: 공백/탭/숫자 전까지 전부 읽기
    while (*linep && *linep != ' ' && *linep != '\t' &&
           !isdigit((unsigned char)*linep)) {
        *p++ = *linep++;
    }
}
// IHEOL 반환 시 citem 반드시 초기화!
citem[0] = '\0';
return IHEOL;
```

**중요**: IHEOL 반환 시 `citem[0] = '\0'`이 필수입니다.  
없으면 이전 입력값이 citem에 남아 다음 scan()에서 오작동합니다.  
(예: 난이도 '초' 입력 후 암호 prompt에서 비어있는데 pass되는 버그)

---

### 4-2. 한국어 명령어 해석 (`lang.c` → `lang_resolve_command()`)

**접두어 매칭** (단축키 지원):
```c
const char *lang_resolve_command(const char *input) {
    // 1. 완전일치 우선 (exact match)
    // 2. 접두어 매칭 - 후보가 1개일 때만 반환 (중의성 방지)
    // 예: "단" → "단거리스캔" → "srscan"
}
```

**모든 명령어 첫 음절이 고유함** (중의성 없음, 검증 완료).

---

### 4-3. 언어 설정 파일 (`sst.ini`)

INI 파서(`lang.c`)가 게임 시작 시 `sst.ini`를 읽어:
- `[Commands]` 섹션: 한국어 별칭 ↔ 영어 명령어 매핑
- `[Devices]` 섹션: 함선 장치 이름 (화면에 표시됨)
- `[ProperNouns]` 섹션: 고유명사

`device[]` 배열은 `main()`에서 `lang_load()` 후 INI 값으로 채워짐.

---

### 4-4. 인게임 도움말 (`sst.doc`)

**형식**: 순수 텍스트 (CRLF), UTF-8  
**검색 방법**: `helpme()`가 `"  Mnemonic:  XXXX"` 문자열로 섹션을 찾고,  
`"******"` (별표 6개 이상)이 있는 줄에서 출력 종료.

**주의**: 섹션 끝에 `\f + ****...` 종결자가 없으면 파일 전체가 출력됨 (스크롤 버그).  
→ `sst.doc` 마지막에 `\f + ********수정 이력********` 추가로 해결.

**`COMMANDS` 섹션 특별 처리**:  
`i == 23` (commands 명령)일 때 sst.doc 검색 대신 `listCommands(TRUE)` 직접 호출.  
이유: sst.doc ABBREV 섹션 종결자 문제 우회.

---

### 4-5. 단거리 스캔 정렬 (`reports.c`)

**문제**: 한글 1글자 = 화면 2칸이지만 `printf("%Ns")`는 바이트 수 기준으로 너비 계산  
**해결**: 각 레이블의 화면 너비를 수동 계산하여 공백 패딩으로 맞춤

```
레이블 + 패딩 = 화면 기준 13칸 (모두 동일)
 스타데이트  (10칸 + 2칸 패딩 + 선행공백 1칸 = 13칸)
 상태        ( 4칸 + 8칸 패딩 + 선행공백 1칸 = 13칸)
 클링온 잔존 (11칸 + 1칸 패딩 + 선행공백 1칸 = 13칸)
```

---

### 4-6. 명령어 목록 표시 (`sst.c` → `listCommands()`)

```c
static void listCommands(int x) {
    // x=TRUE:  "명령" 명령어 직접 실행
    //          → system("cls") + 표 출력 + 키 대기 + system("cls")
    // x=FALSE: helpme()에서 오타 안내용
    //          → 현재 화면에 표만 출력, 일시정지 없음
}
```

---

### 4-7. 암호(password) 입력 버그 수정 (`setup.c`)

**문제**: `proutn()` (프롬프트)가 루프 끝에 있어 첫 실행 시 프롬프트 없이 `scan()` 호출  
→ 이전 입력의 잔여 개행문자를 읽어 암호가 이전 값(예: "초")으로 설정됨

**해결**:
```c
chew();           // 버퍼 완전 초기화
citem[0] = '\0';
while (TRUE) {
    proutn("비밀 암호를 입력하세요(최대 9자, 자폭 시 필요): ");
    if (scan() == IHALPHA && citem[0] != '\0') {
        strncpy(passwd, citem, sizeof(passwd)-1);
        break;
    }
    chew();
}
```

---

### 4-8. 비상탈출 (`sst.c` → case 24)

원래 설계: "화면 지우고 즉시 종료(exit)"  
```c
case 24: /* emexit */
    freeze(TRUE);         // emsave.trk 저장
    prout("게임이 저장되었습니다.");
    system("cls");
    exit(0);              // 프로그램 완전 종료
```
`alldone=TRUE`로 게임 루프로 돌아가던 이전 구현은 프롬프트 중복 버그 유발.

---

### 4-9. 게임 재시작 시 입력 버퍼 초기화 (`sst.c` 메인 루프, `setup.c`)

`quit` / 게임 종료 후 새 게임 시작 시 잔여 입력 버퍼 문제 방지:

```c
// main() 게임 루프
while (TRUE) {
    clearscreen();
    prelim();
    chew(); citem[0] = '\0';   // ← 핵심
    setup();
    ...
}

// choose() 시작부에도 동일하게
chew(); citem[0] = '\0';
```

---

## 5. 단축키 체계

### 명령어 입력 방식 (3가지 모두 작동)
| 방식 | 예시 |
|------|------|
| 한국어 전체 | `단거리스캔` |
| 한국어 첫 음절 | `단` |
| 영어 약어 | `s` 또는 `srscan` |

### 수동/자동 선택
```
수동(수/manual) 또는 자동(자/automatic)?
```
`수`, `manual`, `자`, `automatic` 모두 인식.

### 예/아니오
`예`, `네`, `응`, `y`, `yes` → TRUE  
`아니오`, `아니`, `n`, `no` → FALSE

### 게임 시작 선택지 형식
```
게임 유형 - 일반(일/regular), 토너먼트(토/tournament), 저장된게임(저/frozen):
게임 길이 선택 - 단기(단/short), 중기(중/medium), 장기(장/long):
난이도 - 초보(초/novice), 보통(보/fair), 고수(고/good), 전문가(전/expert), 명예(명/emeritus):
```
→ 영어는 소문자 전체 단어로 통일.

---

## 6. 해결된 버그 목록

| # | 버그 | 해결 |
|---|------|------|
| 1 | 한국어 명령어 전혀 작동 안 함 | `scan()` UTF-8 바이트 처리 추가 |
| 2 | 단거리 스캔 오른쪽 정렬 들쭉날쭉 | 각 레이블 화면 너비 수동 계산하여 패딩 |
| 3 | 게임 시작 시 암호 묻지 않음 | `proutn` 루프 앞으로 이동 + 버퍼 초기화 |
| 4 | `명령어목록` 실행 시 화면 전체 스크롤 | `listCommands(TRUE)` → `cls` + 표 + 키대기 |
| 5 | `help` → `commands` 선택 시 화면 스크롤 | `i==23` 시 `listCommands(TRUE)` 직접 호출 |
| 6 | `sst.doc` 도움말 전체 출력 (스크롤 버그) | ABBREV 섹션 끝에 `\f+****` 종결자 추가 |
| 7 | `비상탈출` 후 게임유형 프롬프트 2번 출력 | `emexit` → `exit(0)` (프로그램 즉시 종료) |
| 8 | `build.bat` 실행 시 오류 | 배치 파일을 순수 ASCII로 재작성 |
| 9 | `scan()` IHEOL 시 citem에 이전 값 잔존 | IHEOL 반환 전 `citem[0]='\0'` 추가 |
| 10 | 한국어 단축키 (첫 음절) 작동 안 함 | `lang_resolve_command()` 접두어 매칭 구현 |

---

## 7. 알려진 잠재 문제

### 콘솔 폰트
한국어 표시를 위해 콘솔 글꼴이 **맑은 고딕** 또는 **굴림체**여야 합니다.  
→ 콘솔 창 우클릭 → 속성 → 글꼴 → 맑은 고딕

### 단거리 스캔 정렬
현재 패딩 계산이 고정값입니다. 만약 장래에 레이블을 변경한다면  
`reports.c`의 case 1~10 각 레이블 패딩 공백 수를 수동으로 재계산해야 합니다.  
계산 공식: `패딩 = 13 - (화면너비)`, 화면너비: 한글 1자=2칸, ASCII 1자=1칸

### `sst.ini` 파일 위치
`sst.exe`와 동일 디렉토리에 있어야 합니다.  
없으면 기본값(lang.c의 `default_devices[]`)으로 동작합니다.

### `sst.doc` 파일 위치
`sst.exe`와 동일 디렉토리에 있어야 합니다.  
없으면 `help` 명령어가 "정보 없음" 메시지를 출력합니다.

---

## 8. 향후 개선 가능 사항

1. **페이지 처리** — 긴 출력(도움말 등)을 화면 단위로 페이징
2. **컬러 출력** — Windows ANSI 이스케이프로 적색/황색/녹색 경보 색상화
3. **사운드** — Windows `Beep()` API로 경보음 추가
4. **sst.ini 확장** — 게임 밸런스 파라미터도 INI로 분리 가능

---

## 9. 작업 환경

| 항목 | 값 |
|------|-----|
| 개발 OS | Linux (Ubuntu 24) |
| 크로스 컴파일러 | `gcc-mingw-w64-x86_64` (Ubuntu 패키지) |
| 타겟 | Windows 64비트 (PE32+) |
| C 표준 | C99 계열 (GCC 기본) |
| 소스 인코딩 | UTF-8 (`-finput-charset=UTF-8`) |
| 실행파일 인코딩 | UTF-8 (`-fexec-charset=UTF-8`) |
| Windows 콘솔 CP | `SetConsoleOutputCP(CP_UTF8)` (win_compat.c) |

---

## 10. 세션 간 주고받은 파일

모든 결과물은 `SST-Korean.zip`에 포함됩니다.  
새 세션에서 계속하려면:

```
1. SST-Korean.zip 다운로드
2. 새 Claude 세션에 SST-Korean.zip 업로드
3. 이 HANDOVER.md를 참고하여 작업 이어받기
```

---
