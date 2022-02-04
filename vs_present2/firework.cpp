#include "firework.h"
#include <time.h>


FireWork::FireWork() {
	pMem = GetImageBuffer();
	fire_begin_time = timeGetTime();
	srand((unsigned)time(NULL));
	for (int i = 0; i < FIRENUM; i++) {
		init(i);
		jetflag[i] = 1;
		fireflag[i] = 0;
	}
}

void FireWork::showJet(int num) {
	if (jetflag[num] == false || timeGetTime() - jet[num].lastime < JETGAP)
		return;

	jet[num].lastime = timeGetTime();
	int speed = 80, limit = (jet[num].y - jet[num].hy)/2 + jet[num].hy;
	int pos = 0;
	
	jet[num].y -= speed;

	if(jet[num].y > limit)
		putimage(jet[num].x, jet[num].y, &jet[num].img[0]);
	else
		putimage(jet[num].x, jet[num].y, &jet[num].img[1]);
	if (jet[num].y - speed < jet[num].hy) {
		jetflag[num] = false;
		fireflag[num] = true;
		firework[num].lastime = jet[num].lastime;
	}

	return;
}

void FireWork::init(int i) {
	
	jet[i].lastime = timeGetTime();

	jet[i].y = HEIGHT - 300;
	firework[i].x = jet[i].x = jet[i].hx = min_radium + rand() % (WIDTH - max_radium);//防止出界
	firework[i].y = jet[i].hy = min_radium + rand() % (HEIGHT - max_radium * 2);
	firework[i].r = 0;
	jet[i].y = JETSTART;
	firework[i].max_r = min_radium;
	if (max_radium - min_radium > 0)
		firework[i].max_r += rand() % (max_radium - min_radium);//当前烟花的最大爆炸半径
	
	//firework图片初始化
	int pos = rand() % FIRENUM;
	IMAGE img;
	std::wstring path = L"../resource/flower";
	path.append(std::to_wstring(pos + 1));
	path.append(L".jpg");
	LPCWSTR str = path.c_str();
	loadimage(&img, str, firework[i].max_r, firework[i].max_r);

	SetWorkingImage(&img);
	for (int j = 0; j < firework[i].max_r; j++) {
		for (int k = 0; k < firework[i].max_r; k++) {
			firework[i].img[j][k] = getpixel(j, k);
		}
	}

	//jet图片初始化
	IMAGE img1;
	loadimage(&img1, L"../resource/shoot.jpg", 200, 50);
	SetWorkingImage(&img1);
	pos %= 5;
	getimage(&jet[i].img[0], pos * 20, 0, 20, 50);//得到暗色
	getimage(&jet[i].img[1], (pos + 5) * 20, 0, 20, 50);//得到亮色	
	cleardevice();
	SetWorkingImage(NULL);
	return;
}

void FireWork::showFire(int num) {
	if (fireflag[num] == false || timeGetTime() - firework[num].lastime < FIREGAP)
		return;
	firework[num].lastime = timeGetTime();
	firework[num].r++;

	int base = firework[num].max_r / 2;
	for (double a = 0; a <= 6.28; a += 0.01)
	{
		int x1 = (int)(firework[num].x + firework[num].r*sin(a)) - (firework[num].x - base);
		int y1 = (int)(firework[num].y + firework[num].r*cos(a)) - (firework[num].y - base);
		if (x1 >= 0 && x1 < firework[num].max_r && y1 >= 0 && y1 < firework[num].max_r)
		{
			int b = firework[num].img[x1][y1] & 0xff;
			int g = (firework[num].img[x1][y1] >> 8) & 0xff;
			int r = (firework[num].img[x1][y1] >> 16);
			int xx = (int)(firework[num].x + firework[num].r*sin(a));
			int yy = (int)(firework[num].y + firework[num].r*cos(a));
			//筛掉黑色背景
			if (r > 0x20 && g > 0x20 && b > 0x20 && xx >= 0 && xx < 1000 && yy < 600 && yy >= 0)
				pMem[yy * 1000 + xx] = BGR(firework[num].img[x1][y1]);
		}
	}
	if (firework[num].r >= firework[num].max_r) {
		fireflag[num] = false;
		jetflag[num] = true;
		init(num);
	}
	
}

void FireWork::test() {
	BeginBatchDraw();
	bool cc = 0;
	while( true ) {
		/* 随机染黑一些点 使得烟花逐渐消散 */
		for (int i = 0; i < 1200; i++)
		{
				int px1 = rand() % WIDTH;
				int py1 = rand() % HEIGHT;
				pMem[py1 * 1000 + px1] = pMem[py1 * 1000 + px1 + 1] = BLACK;
		}
		for (int i = 0; i < FIRENUM; i++) {
			
				showJet(i);
				showFire(i);

			cc++;
		}
		bool flag = out_extra_text(timeGetTime());
		EndBatchDraw();
		if (_kbhit()) {
			getchar();//把输入的字符吃掉
			if (flag)
				break;
		}
	}
	return;
}

bool FireWork::out_extra_text(DWORD cur_time) {
	DWORD dist = cur_time - fire_begin_time;
	dist /= 1000;
	dist /= 4;
	int x = WIDTH/2 + 20;
	int y = 550;
	settextstyle(25, 0, L"宋体");
	settextcolor(YELLOW);
	switch (dist)
	{
	case 0:
	case 1: {break; }
	case 2: {outtextxy(x, y, L"2022啦！ 新年啦！"); break; }
	case 3: {outtextxy(x, y, L"今年也要记得看烟花"); break; }
	case 4: {outtextxy(x, y, L"所谓的烟花呢"); break; }
	case 5: {outtextxy(x, y, L"我看来就是美好而短暂的"); break; }
	case 6: {outtextxy(x, y, L"小小世界"); break; }
	case 7: {outtextxy(x, y, L"要去珍惜这种微小的感动啊"); break; }
	case 8: {break; }
	default:{outtextxy(x - 70, y, L"按下键盘任意键进入下一阶段 ╰(*°▽°*)╯"); break; }
	}
	if (dist > 8)
		return true;
	return false;
}