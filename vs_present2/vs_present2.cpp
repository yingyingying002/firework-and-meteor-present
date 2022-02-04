#include "v2_present2.h"
#include "firework.h"
#include "meteor.h"
#include <Windows.h>
#include <thread>

int main() {

	initgraph(WIDTH, HEIGHT);
	int rabbit_x, rabbit_y=450;
	welcome(rabbit_x, rabbit_y);
	curtain(rabbit_y);

	/* 放烟花 */
	start_bgm(L"../resource/Turns.mp3", 2);

	FireWork *firework = new FireWork();
	PlaySound(L"../resource/firework_using.wav", NULL, SND_LOOP | SND_FILENAME | SND_ASYNC);
	firework->test();
	PlaySound(NULL, NULL, SND_FILENAME);
	/* 流星雨 */
	starRain();

	/* 结束语 */
	endfunc_begin_time = timeGetTime();
	endfunc();
	start_bgm(L"../resource/Turns.mp3", 0);
	closegraph();
	return 0;
}

void start_bgm(std::wstring c, int n)
{
	std::wstring File;
	File.clear();
	if (n == 1 || n == 2)	File.append(L"play ");
	else if (n == 0)	File.append(L"close ");
	File.append(c);
	if (n == 2)		File.append(L" repeat");
	LPCWSTR str = File.c_str();
	mciSendString(str, NULL, 0, NULL);
}

//鹰 盘旋并落在source_flag上面
void hover(int rabbit_x, int rabbit_y, IMAGE source_flag) {
	IMAGE img2, img3;
	int a = 300, b = 100;//椭圆a 与 b
	int source_x = WIDTH >> 1, source_y = HEIGHT / 10 * 3;//椭圆中心坐标
	int curx, cury;
	loadimage(&img2, L"../resource/eagle2.jpg", 80, 80);
	loadimage(&img3, L"../resource/eagle3.jpg", 80, 80);
	int cc = 0;
	for (double angle = PI / 2 + 0.01; angle < PI / 2 + PI; angle += 0.01) {
		double t = atan(tan(angle) * a / b);
		curx =int(source_x + a * cos(t)), cury = int(source_y + b * sin(t));
		if(angle < PI)
			putimage(curx, cury, &img3);
		else 
			putimage(curx, cury, &img2);
		putimage(rabbit_x, rabbit_y, &source_flag);
		cc++;
		Sleep(10);
		if (cc % 3 == 0)
			cleardevice();
	}
	PlaySound(L"../resource/eagle_using.wav", NULL, SND_FILENAME | SND_ASYNC);
	for (double angle = PI / 2 + PI + 0.01 ; angle < PI / 2 + 2 * PI; angle += 0.01) {
		double t = atan(tan(angle) * a / b);
		curx = int(source_x - a * cos(t)), cury = int(source_y - b * sin(t));
		if (angle < 2 * PI)
			putimage(curx, cury, &img2);
		else
			putimage(curx, cury, &img3);
		putimage(rabbit_x, rabbit_y, &source_flag);
		cc++;
		Sleep(10);
		if (cc % 3 == 0)
			cleardevice();
	}
	PlaySound(L"../resource/eagle_using.wav", NULL, SND_FILENAME | SND_ASYNC);
	for (double angle = PI / 2 + 0.01; angle < PI / 8 + PI; angle += 0.01) {
		double t = atan(tan(angle) * a / b);
		curx = int(source_x + a * cos(t)), cury = int(source_y + b * sin(t));
		if (angle < PI)
			putimage(curx, cury, &img3);
		else
			putimage(curx, cury, &img2);
		putimage(rabbit_x, rabbit_y, &source_flag);
		cc++;
		Sleep(10);
		if (cc % 3 == 0)
			cleardevice();
	}
	/* 往下 */
	double angle = atan(1.0*(rabbit_y -25 - cury) / (curx +25 - rabbit_x));
	double dist = getdist(curx, cury, rabbit_x+25, rabbit_y+25)/100;
	for (int i = 0; i < 100; i++) {
		curx -= int(dist * cos(angle));
		cury += int(dist * sin(angle));
		putimage(curx, cury, &img2);
		putimage(rabbit_x, rabbit_y, &source_flag);
		Sleep(10);
		cc++;
		if (cc % 3)
			cleardevice();
	}
	return;
}

void welcome(int &xx, int &yy) {
	const int R = HEIGHT / 3;//运动半径
	xx = WIDTH >> 1;
	yy = (HEIGHT >> 1) + R;//(xx,yy)圆心  后续表示为兔子位置

	IMAGE img, eagle_img, rabbit1, rabbit2, rabbit3;
	int ss=80;
	loadimage(&img, L"../resource/treasure.jpg", 50, 50);
	loadimage(&eagle_img, L"../resource/eagle1.jpg", ss, ss);
	loadimage(&rabbit1, L"../resource/rabbit1.jpg", ss, ss);
	loadimage(&rabbit2, L"../resource/rabbit2.jpg", ss, ss);
	loadimage(&rabbit3, L"../resource/rabbit3.jpg", ss, ss);

	hover(xx, yy, img);
	int eagle_x = xx, eagle_y = yy;
	// 鹰 平移到一侧
	cleardevice();
	for (int i = 0; i < 20; i++) {
		eagle_x -= 5;
		putimage(eagle_x, eagle_y, &eagle_img);
		putimage(xx, yy, &img);
		if(i%3==0)
			cleardevice();
		Sleep(20);
	}
	cleardevice();
	putimage(eagle_x, eagle_y, &eagle_img);
	Sleep(1000);
	//弹出小兔子
	PlaySound(L"../resource/rabbit_using.wav", NULL, SND_FILENAME | SND_ASYNC);
	putimage(xx, yy, &rabbit1);
	Sleep(2500);
	putimage(xx, yy, &rabbit2);
	Sleep(1000);

	//兔子往上跳跃 落地
	BeginBatchDraw();
	for (int i = 1; i < 10; i++) {
		yy = yy - (5 - i) * 6;
		cleardevice();
		putimage(eagle_x, eagle_y, &eagle_img);
		putimage(xx, yy, &rabbit3);
		EndBatchDraw();
		Sleep(25);
	}
	return;
}

void curtain(int limit) {
	IMAGE curtain_img, firework_backround(WIDTH, limit);
	PlaySound(L"../resource/curtain_using.wav", NULL, SND_LOOP | SND_FILENAME | SND_ASYNC);
	loadimage(&curtain_img, L"../resource/curtain.jpg", WIDTH, limit);
	putimage(0, 0, &curtain_img);
	Sleep(1000);
	double base = (limit*1.0) / 160;
	for (int i = 0; i < 160; i++) {
		SetWorkingImage(&firework_backround);
		cleardevice();
		putimage(0, int(0 - i * base), &curtain_img);
		SetWorkingImage(NULL);
		putimage(0, 0, &firework_backround);
		Sleep(15);
	}
	return;
}

void starRain() {
	Meteor *meteor = new Meteor();
	//cleardevice();
	SetWorkingImage(NULL);
	BeginBatchDraw();
	while (true) {
		cleardevice();
		//putimage(0, 0, &bk);
		meteor->drawStar();
		meteor->moveStar();
		bool flag = meteor->out_extra_text(timeGetTime());
		meteor->drawMeteor();
		meteor->moveMeteor();
		if (_kbhit()) {
			getchar();
			if (flag)
				break;
		}
		FlushBatchDraw();
		Sleep(1);
	}
	EndBatchDraw();
	return;
}

bool outFinalText(DWORD curtime) {
	DWORD dist = curtime - endfunc_begin_time;
	dist /= 1000;
	dist /= 4;
	int x = WIDTH/2 - 80;
	int y = 470;
	settextstyle(25, 0, L"宋体");
	settextcolor(YELLOW);
	switch (dist)
	{
		case 0: {break; }
		case 1: {outtextxy(x, y, L"月黑见渔灯"); break; }
		case 2: {outtextxy(x, y, L"孤光一点萤"); break; }
		case 3: {outtextxy(x, y, L"微微风簇浪"); break; }
		case 4: {outtextxy(x, y, L"散作满河星"); break; }
		case 5: {outtextxy(x, y, L"清·查慎行 《舟夜书所见》"); break; }
		case 6: {outtextxy(x, y, L"希望能让看的你开心"); break; }
		case 7: {
			outtextxy(x, y, L"预祝新的一年");
			break;
		}
		case 8: {
			outtextxy(x, y, L"预祝新的一年");
			outtextxy(x, y + 30, L"有车有房 百毒不侵 阖家幸福 身体健康 龙马精神");
			break;
		}
		case 9: 
		case 10:
		case 11: {
			outtextxy(x, y, L"预祝新的一年");
			outtextxy(x, y + 30, L"有车有房 百毒不侵 阖家幸福 身体健康 龙马精神");
			outtextxy(x, y + 60, L"顺顺利利 学业有成 荣华富贵 金钱豹子 前途无量");
			break;
		}
		case 12:
		case 13: {outtextxy(x-80, y, L" ≡ω≡ 新年快乐 ≡ω≡"); break; }
		default: {
			outtextxy(x-80, y, L" ≡ω≡ 新年快乐 ≡ω≡");
			outtextxy(x - 80, y + 60, L"表演结束啦，按下键盘任意键可以有序离场哦"); break;
		}
	}
	if (dist > 13)
		return true;
	return false;
}

void endfunc() {
	SetWorkingImage(NULL);
	cleardevice();
	IMAGE img;
	loadimage(&img, L"../resource/end.jpg", 350, 390);
	settextstyle(25, 0, L"宋体");
	settextcolor(YELLOW);

	Meteor *meteor = new Meteor();
	/* 加载背景 输出文字 */
	while (true) {
		cleardevice();
		BeginBatchDraw();
		meteor->drawStar();
		meteor->moveStar();
		if(timeGetTime() - endfunc_begin_time > 2000)
			putimage(325, 60, &img);

		bool flag = outFinalText(timeGetTime());
		if (_kbhit()) {
			getchar();
			if (flag)
				break;
		}
		FlushBatchDraw();
		Sleep(10);
	}
	EndBatchDraw();
}

double getdist(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}