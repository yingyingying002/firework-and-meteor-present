#pragma once

#include "BaseValue.h"
#define FIREGAP 20
#define JETGAP 200
#define min_radium 120
#define max_radium 240

struct JET {
	int x, y;//发射点坐标
	int hx, hy;//爆炸坐标
	IMAGE img[2];//明暗两种状态
	DWORD lastime;
};

struct FIREWORK {
	int r, max_r;//半径 最大爆炸半径
	int x, y;//爆炸位置
	int img[240][240];//存储像素信息
	DWORD lastime;
};

class FireWork {
public:
	FireWork();
	void init(int i);
	void test();
	bool out_extra_text(DWORD cur_time);
private:
	void showJet(int num);
	void showFire(int num);//参数为烟花id
	DWORD *pMem;
	JET jet[FIRENUM];
	FIREWORK firework[FIRENUM];
	bool jetflag[FIRENUM];
	bool fireflag[FIRENUM];
	DWORD fire_begin_time;
};

