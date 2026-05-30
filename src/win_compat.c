/* ============================================================
 * win_compat.c - Windows 플랫폼 호환 레이어
 * Linux osx.c 를 Windows용으로 대체합니다.
 * ============================================================ */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

/* Windows 콘솔 한국어(UTF-8) 설정 */
void win_setup_console(void) {
    /* UTF-8 출력 설정 */
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    /* 콘솔 창 제목 */
    SetConsoleTitleA("슈퍼 스타 트렉 (Super Star Trek)");

    /* 콘솔 버퍼 크기를 넉넉하게 */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        COORD bufSize;
        bufSize.X = csbi.dwSize.X;
        bufSize.Y = 3000;
        SetConsoleScreenBufferSize(hOut, bufSize);
    }
}

/* 난수 초기화 */
void randomize(void) {
    srand((unsigned int)time(NULL));
}

/* 정수 최댓값 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/* 정수 최솟값 */
int min(int a, int b) {
    return (a < b) ? a : b;
}

/* 단일 키 입력 (에코 없음) */
int getch_compat(void) {
    return _getch();
}
