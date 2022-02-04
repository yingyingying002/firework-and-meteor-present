#pragma once
#include "BaseValue.h"


struct STAR     //小星星
{
	int x, y, r;
	int speed;          //速度
	COLORREF color;     //颜色
};

struct METEOR	//流星
{
	int x, y, speed;//坐标 速度
	int id; //对应的样式
};

struct METEOR_IMG { //把样式转换成像素存储
	int xy[100][50];
};

class Meteor {
public:
	Meteor();
	void init();
	void moveStar();
	void moveMeteor();
	void drawStar();
	void drawMeteor();
	bool out_extra_text(DWORD cur_time);
private:
	DWORD meteor_begin_time;
	void check();//检查多个流星是否重叠
	STAR star[STARNUM];
	METEOR meteor[METEORNUM];
	IMAGE MeteorImg[METEORKIND]; //流星的20种样式
};