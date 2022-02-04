#pragma once

#include "BaseValue.h"
#include <time.h>
#include <process.h>
#include <string>

//#define DEBUG

DWORD endfunc_begin_time;
void welcome(int &xx, int &yy);
void curtain(int limit);
void starRain();
void endfunc();
void start_bgm(std::wstring c, int n);
void hover(int rabbit_x, int rabbit_y, IMAGE source_flag);
bool outFinalText(DWORD curtime);
double getdist(double x1, double y1, double x2, double y2);

