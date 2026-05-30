# Changelog

## v1.0.0 — 2026-05-30 (최초 안정 릴리즈)

### 신규 기능
- 클래식 Super Star Trek (Eric S. Raymond C 버전) 완전 한국어화
- Windows 64비트 단일 EXE 빌드 (`sst.exe`)
- `sst.ini` 파일을 통한 명령어/장치명/고유명사 커스터마이징
- 한국어 + 영어 명령어 동시 지원 (접두어 단축키 포함)
- 인게임 한국어 도움말 (`sst.doc`)

### 핵심 구현
- `scan()` UTF-8 한국어 바이트 처리 추가
- `lang_resolve_command()` 접두어 매칭으로 첫 음절 단축키 지원
- `reports.c` 단거리 스캔 한글 너비 보정 패딩
- Windows 콘솔 UTF-8 출력 (`SetConsoleOutputCP(CP_UTF8)`)
- 암호 입력 버그, 비상탈출 버그 등 10개 버그 수정

### 지원 플랫폼
- Windows 10 / 11 (64비트)
- 빌드: MSYS2 MinGW-w64 GCC 13+
