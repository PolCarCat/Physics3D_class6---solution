#ifndef __p2Log_H__
#define __p2Log_H__

#include <windows.h>
#include <stdio.h>

void log(const char file[], int line, const char* format, ...);

#endif