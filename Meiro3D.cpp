#include "DxLib.h"
#include "DxSubfunc.h"
#include "game.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Dx�̋N��
	if (sub_DxInt() == -1)
	{
		return -1;//�G���[�̏ꍇ�A�I��
	}

	//Pint�̒�`
	PXY Plocal;
	nRGB lcolor;

	Plocal.x = 320;
	Plocal.y = 240;

	lcolor.R = 255;
	lcolor.G = 255;
	lcolor.B = 255;

	//Point�̕`��
	sub_drowPoint(Plocal, lcolor);

	//Key���͑҂�
	sub_waitKeyAll();

	game_main();

	//Dx�̏I��
	sub_DxEnd();

	return 0;				// �\�t�g�̏I�� 
}