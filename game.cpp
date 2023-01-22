#include "DxLib.h"
#include "DxSubfunc.h"
#include "game.h"

void game_main()
{
	nRGB rgb;
	rgb.R = 255;
	rgb.G = 0;
	rgb.B = 0;

	PXY xy;
	xy.x = 640 / 2;
	xy.y = 400 / 2;

	move_img(xy, rgb);

}

void move_img(PXY xy, nRGB rgb)
{
	kcount = 9;
	k01 = KEY_INPUT_S; //end
	k02 = KEY_INPUT_W; //-y 
	k03 = KEY_INPUT_X; //+y
	k04 = KEY_INPUT_A; //-x
	k05 = KEY_INPUT_D; //+x
	k06 = KEY_INPUT_Q; //-y,-x
	k07 = KEY_INPUT_Z; //+y,-x
	k08 = KEY_INPUT_E; //-y,+x
	k09 = KEY_INPUT_C; //+y,+x

	CXY xyr;
	xyr.p1.x = xy.x;
	xyr.p1.y = xy.y;
	xyr.r = 10;

	nRGB bgrand;
	bgrand.R = 0;
	bgrand.G = 0;
	bgrand.B = 0;

	FXY fxy;
	fxy.x = 0;
	fxy.y = 0;

	nRGB frgb;
	frgb.R = 255;
	frgb.G = 255;
	frgb.B = 255;

	int key;
	char ckey = ' ';
	int move = 2;

	while (1)
	{
		sub_cls(bgrand);
		sub_drowCircle(xyr, rgb, TRUE);
		DrawFormatString(0, 0, GetColor(255, 255, 255), "x = % f y = % f key = % c", xyr.p1.x, xyr.p1.y, ckey);
		//sub_drowFormat(fxy, frgb, "x = % d y = % d key = % c", (int)xyr.p1.x, (int)xyr.p1.y, ckey);

		key = sub_waitKey();

		if (key == KEY_INPUT_S) break;
		if (key == KEY_INPUT_W)
		{
			ckey = 'W';
			if (xyr.p1.y > xyr.r)
			{
				xyr.p1.y -= move;
			}
		}
		if (key == KEY_INPUT_X)
		{
			ckey = 'X';
			if (xyr.p1.y < 400 - xyr.r)
			{
				xyr.p1.y += move;
			}
		}
		if (key == KEY_INPUT_A)
		{
			ckey = 'A';
			if (xyr.p1.x > xyr.r)
			{
				xyr.p1.x -= move;
			}
		}
		if (key == KEY_INPUT_D)
		{
			ckey = 'D';
			if (xyr.p1.x < 640 - xyr.r)
			{
				xyr.p1.x += move;
			}
		}
		if (key == KEY_INPUT_Q)
		{
			ckey = 'Q';
			if (xyr.p1.y > xyr.r)
			{
				xyr.p1.y -= move;
			}
			if (xyr.p1.x > xyr.r)
			{
				xyr.p1.x -= move;
			}
		}
		if (key == KEY_INPUT_Z)
		{
			ckey = 'Z';
			if (xyr.p1.y < 400 - xyr.r)
			{
				xyr.p1.y += move;
			}
			if (xyr.p1.x > xyr.r)
			{
				xyr.p1.x -= move;
			}
		}
		if (key == KEY_INPUT_E)
		{
			ckey = 'E';
			if (xyr.p1.y > xyr.r)
			{
				xyr.p1.y -= move;
			}
			if (xyr.p1.x < 640 - xyr.r)
			{
				xyr.p1.x += move;
			}
		}
		if (key == KEY_INPUT_C)
		{
			ckey = 'C';
			if (xyr.p1.y < 400 - xyr.r)
			{
				xyr.p1.y += move;
			}
			if (xyr.p1.x < 640 - xyr.r)
			{
				xyr.p1.x += move;
			}
		}

		WaitTimer(100);

	}
}