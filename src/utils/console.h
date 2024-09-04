// console.h
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <windows.h>

void evtp();

extern HANDLE console;
extern COORD initialPosition;

void initCursorPosition(void);
void resetCursorPosition(void);
void c256(int color, const char *format);
void c256f(int color, const char *format, ...);

#define BLACK(text) "\x1b[30m" text "\x1b[0m"
#define RED(text) "\x1b[31m" text "\x1b[0m"
#define GREEN(text) "\x1b[32m" text "\x1b[0m"
#define YELLOW(text) "\x1b[33m" text "\x1b[0m"
#define BLUE(text) "\x1b[34m" text "\x1b[0m"
#define MAGENTA(text) "\x1b[35m" text "\x1b[0m"
#define CYAN(text) "\x1b[36m" text "\x1b[0m"
#define WHITE(text) "\x1b[37m" text "\x1b[0m"

#ifdef __cplusplus
}
#endif
#endif // CONSOLE_H