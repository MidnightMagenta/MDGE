#ifndef WIN32_CONSOLE_H
#define WIN32_CONSOLE_H

#include <Windows.h>

#define CONSOLE_RED 12
#define CONSOLE_GREEN 10
#define CONSOLE_BLUE 9
#define CONSOLE_YELLOW 14
#define CONSOLE_WHITE 7

inline void setConsoleColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}


#endif// CONSOLE_H