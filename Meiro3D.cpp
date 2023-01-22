#include "DxLib.h"
#include "DxSubfunc.h"
#include "game.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Dxの起動
	if (sub_DxInt() == -1)
	{
		return -1;//エラーの場合、終了
	}

	//Pintの定義
	PXY Plocal;
	nRGB lcolor;

	Plocal.x = 320;
	Plocal.y = 240;

	lcolor.R = 255;
	lcolor.G = 255;
	lcolor.B = 255;

	//Pointの描画
	sub_drowPoint(Plocal, lcolor);

	//Key入力待ち
	sub_waitKeyAll();

	game_main();

	//Dxの終了
	sub_DxEnd();

	return 0;				// ソフトの終了 
}