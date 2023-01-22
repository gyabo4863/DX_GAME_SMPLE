#include "DxLib.h"
#include "DxSubfunc.h"

int sub_DxInt()
{
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	return 0;
}

void sub_DxEnd()
{
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
}

void sub_waitKeyAll()
{
	WaitKey();				// �L�[���͑҂�
}

int sub_HitKey(int key)
{
	int rc;
	rc = CheckHitKey(key);  // �L�[�������H
	return rc;
}

int kcount = 0;
int k01;
int k02;
int k03;
int k04;
int k05;
int k06;
int k07;
int k08;
int k09;
int k10;

int sub_waitKey()
{
	int rc;

	if (kcount == 0) return -1;

	while (1)
	{
		if (sub_HitKey(k01))
		{
			rc = k01;
			break;
		}
		if (kcount > 1 && sub_HitKey(k02))
		{
			rc = k02;
			break;
		}
		if (kcount > 2 && sub_HitKey(k03))
		{
			rc = k03;
			break;
		}
		if (kcount > 3 && sub_HitKey(k04))
		{
			rc = k04;
			break;
		}
		if (kcount > 4 && sub_HitKey(k05))
		{
			rc = k05;
			break;
		}
		if (kcount > 5 && sub_HitKey(k06))
		{
			rc = k06;
			break;
		}
		if (kcount > 6 && sub_HitKey(k07))
		{
			rc = k07;
			break;
		}
		if (kcount > 7 && sub_HitKey(k08))
		{
			rc = k08;
			break;
		}
		if (kcount > 8 && sub_HitKey(k09))
		{
			rc = k09;
			break;
		}
		if (kcount > 9 && sub_HitKey(k10))
		{
			rc = k10;
			break;
		}

		// ���b�Z�[�W����
		if (ProcessMessage() == -1)
		{
			rc = -1;
			break;    // �G���[�����������烋�[�v�𔲂���
		}

	}
	return rc;
}

void sub_drowPoint(PXY xy, nRGB rgb)
{
	DrawPixel((int)xy.x, (int)xy.y, GetColor(rgb.R, rgb.G, rgb.B));	// �_��ł�
}

void sub_drowLine(LXY xy, nRGB rgb)
{
	DrawLineAA(xy.p1.x, xy.p1.y, xy.p2.x, xy.p2.y, GetColor(rgb.R, rgb.G, rgb.B));    // ����`��
}

void sub_drowBox(BXY xy, nRGB rgb, bool fill)
{
	DrawBoxAA(xy.p1.x, xy.p1.y, xy.p2.x, xy.p2.y, GetColor(rgb.R, rgb.G, rgb.B), fill);    // �l�p�`��`��
}

void sub_drowCircle(CXY xyr, nRGB rgb, bool fill)
{
	int count = CIRCL_POINT_COUNT;
	DrawCircleAA(xyr.p1.x, xyr.p1.y, xyr.r, count, GetColor(rgb.R, rgb.G, rgb.B), fill);  // �~��`��
}

void sub_drowOval(OXY xyr, nRGB rgb, bool fill)
{
	int count = OVERL_POINT_COUNT;
	DrawOvalAA(xyr.p1.x, xyr.p1.y, xyr.rx, xyr.ry, count, GetColor(rgb.R, rgb.G, rgb.B), fill); // �ȉ~��`��
}

void sub_drowTriangle(TXY xy, nRGB rgb, bool fill)
{
	// �O�p�`��`��
	DrawTriangleAA(xy.p1.x, xy.p1.y, xy.p2.x, xy.p2.y, xy.p3.x, xy.p3.y, GetColor(rgb.R, rgb.G, rgb.B), fill);
}

int sub_drowImg(IXY xyp, bool trans)
{
	int rc;
	// �摜�̎捞�E�`��
	rc = LoadGraphScreen(xyp.x, xyp.y, xyp.path, trans);
	return rc;
}

int sub_drowString(SXY xyp, nRGB rgb, int fsize, char* font)
{
	int rc;
	if (font != "")
	{
		// �`�悷�镶����̕����Z�b�g��ύX���܂�
		rc = ChangeFont(font);
		if (rc == -1) return -1;
	}

	// �`�悷�镶����̃T�C�Y��ݒ�
	SetFontSize(fsize);

	// ������̕`��
	rc = DrawString(xyp.x, xyp.y, xyp.moji, GetColor(rgb.R, rgb.G, rgb.B));

	return rc;
}

int sub_cFont(int fsize, char* font)
{
	int rc;
	if (font != "")
	{
		// �`�悷�镶����̕����Z�b�g��ύX���܂�
		rc = ChangeFont(font);
		if (rc == -1) return -1;
	}

	// �`�悷�镶����̃T�C�Y��ݒ�
	SetFontSize(fsize);
	return 0;
}

int sub_drowFormat(FXY xy, nRGB rgb, const TCHAR *s, ...)
{
	int rc;
	// FORMAT�����`��
	rc = DrawFormatString(xy.x, xy.y, GetColor(rgb.R, rgb.G, rgb.B), s);
	return rc;
}

void sub_cls(nRGB rgb)
{
	BXY xy;
	xy.p1.x = 0;
	xy.p1.y = 0;
	xy.p2.x = 640;
	xy.p2.y = 400;

	sub_drowBox(xy, rgb, TRUE); // �o�b�N�O���E���h�œh��Ԃ�
}