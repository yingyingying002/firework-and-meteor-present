#include "meteor.h"
#include <string>
#include <algorithm>


Meteor::Meteor() {
	meteor_begin_time = timeGetTime();
	srand((unsigned)time(NULL));
	init();
}

void Meteor::init() {
	for (int i = 0; i < STARNUM; i++) {//设置star基本属性
		star[i].x = rand() % WIDTH;
		star[i].y = rand() % HEIGHT;
		star[i].r = rand() % 3 + 1;
		star[i].speed = rand() % 5; //0 1 2 3 4
		star[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
	for (int i = 0; i < METEORNUM; i++) {//设置meteor基本属性
		meteor[i].x = rand() % (2 * getwidth()) - getwidth();   //[-1200,1200)
		meteor[i].y = rand()%200 - 400;
		meteor[i].speed = rand() % 3 + 1;
		meteor[i].id = rand() % METEORKIND;
	}
	/*  批量读入face1.jpg ~ facexx.jpg 
		因为loadimage参数设置path是LPCSTR 所以比较麻烦
		以下是unicode字符集下的转化过程
	*/
	std::wstring base = L"../resource/face";
	for (int i = 0; i < METEORKIND; i++) {
		std::wstring temp = base;
		temp.append(std::to_wstring(i + 1));
		temp.append(L".jpg");
		LPCWSTR str = temp.c_str();
		loadimage(&MeteorImg[i], str, 100, 50);
	}
	check();//如果发现流星重叠 修改一下
	return;
}

void Meteor::moveStar() {
	for (int i = 0; i < STARNUM; i++)
	{
		star[i].y += star[i].speed;
		if (star[i].y > HEIGHT)
		{
			star[i].y = 0;
		}
	}
	return;
}

void Meteor::moveMeteor() {
	for (int i = 0; i < METEORNUM; i++)
	{
		meteor[i].x += meteor[i].speed;
		meteor[i].y += meteor[i].speed;
		if (meteor[i].x >= WIDTH || meteor[i].y >= HEIGHT)
		{
			meteor[i].x = rand() % (2 * WIDTH) - WIDTH;
			meteor[i].y = 20 - 200;
		}
	}
	return;
}

void Meteor::drawStar() {
	for (int i = 0; i < STARNUM; i++)
	{
		//putpixel(star[i].x, star[i].y, star[i].color);
		setfillcolor(star[i].color);
		solidcircle(star[i].x, star[i].y, star[i].r);
	}
	return;
}

void Meteor::drawMeteor() {
	DWORD * pMem = GetImageBuffer();
	for (int i = 0; i < METEORNUM; i++)
		putimage(meteor[i].x, meteor[i].y, &MeteorImg[meteor[i].id]);
	
	return;
}

//流星数量很少 直接二重循环check
void Meteor::check() {
	std::sort(meteor, meteor + METEORNUM, [](METEOR A, METEOR B) {return A.x != B.x ? A.x < B.x : A.y < B.y; });
	for (int i = 0; i < METEORNUM; i++) {
		for (int j = i + 1; j < METEORNUM; j++) {
			if (meteor[i].speed == meteor[j].speed) {
				if (meteor[j].x - meteor[i].x < 100 && meteor[j].y - meteor[i].y < 50) {
					int temp = meteor[j].x % 3;
					if (temp == 0) {
						meteor[j].x += 5 + (meteor[j].x - meteor[i].x);
					}
					else if (temp == 1) {
						meteor[j].y += 5 + abs(meteor[j].y - meteor[i].y);
					}
					else {
						meteor[j].speed++;
					}
				}
			}
		}
	}
	return;
}

bool Meteor::out_extra_text(DWORD cur_time) {
	DWORD dist = cur_time - meteor_begin_time;
	dist /= 1000;
	dist /= 4;
	int x = WIDTH / 2 + 20;
	int y = 550;
	settextstyle(25, 0, L"宋体");
	settextcolor(YELLOW);
	switch (dist)
	{
		case 0: 
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:{break; }
		default: {
			outtextxy(x, y, L"按下键盘任意键进入下一阶段o(￣皿￣///)"); 
			break;
		}
	}
	if (dist >= 7)
		return true;
	return false;
}