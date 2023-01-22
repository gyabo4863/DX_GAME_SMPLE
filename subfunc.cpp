#include "DxLib.h"
#include "DxSubfunc.h"

int sub_DxInt()
{
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	return 0;
}

void sub_DxEnd()
{
	DxLib_End();				// ＤＸライブラリ使用の終了処理
}

void sub_waitKeyAll()
{
	WaitKey();				// キー入力待ち
}

int sub_HitKey(int key)
{
	int rc;
	rc = CheckHitKey(key);  // キー押される？
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

		// メッセージ処理
		if (ProcessMessage() == -1)
		{
			rc = -1;
			break;    // エラーが発生したらループを抜ける
		}

	}
	return rc;
}

void sub_drowPoint(PXY xy, nRGB rgb)
{
	DrawPixel((int)xy.x, (int)xy.y, GetColor(rgb.R, rgb.G, rgb.B));	// 点を打つ
}

void sub_drowLine(LXY xy, nRGB rgb)
{
	DrawLineAA(xy.p1.x, xy.p1.y, xy.p2.x, xy.p2.y, GetColor(rgb.R, rgb.G, rgb.B));    // 線を描画
}

void sub_drowBox(BXY xy, nRGB rgb, bool fill)
{
	DrawBoxAA(xy.p1.x, xy.p1.y, xy.p2.x, xy.p2.y, GetColor(rgb.R, rgb.G, rgb.B), fill);    // 四角形を描画
}

void sub_drowCircle(CXY xyr, nRGB rgb, bool fill)
{
	int count = CIRCL_POINT_COUNT;
	DrawCircleAA(xyr.p1.x, xyr.p1.y, xyr.r, count, GetColor(rgb.R, rgb.G, rgb.B), fill);  // 円を描画
}

void sub_drowOval(OXY xyr, nRGB rgb, bool fill)
{
	int count = OVERL_POINT_COUNT;
	DrawOvalAA(xyr.p1.x, xyr.p1.y, xyr.rx, xyr.ry, count, GetColor(rgb.R, rgb.G, rgb.B), fill); // 楕円を描画
}

void sub_drowTriangle(TXY xy, nRGB rgb, bool fill)
{
	// 三角形を描画
	DrawTriangleAA(xy.p1.x, xy.p1.y, xy.p2.x, xy.p2.y, xy.p3.x, xy.p3.y, GetColor(rgb.R, rgb.G, rgb.B), fill);
}

int sub_drowImg(IXY xyp, bool trans)
{
	int rc;
	// 画像の取込・描画
	rc = LoadGraphScreen(xyp.x, xyp.y, xyp.path, trans);
	return rc;
}

int sub_drowString(SXY xyp, nRGB rgb, int fsize, char* font)
{
	int rc;
	if (font != "")
	{
		// 描画する文字列の文字セットを変更します
		rc = ChangeFont(font);
		if (rc == -1) return -1;
	}

	// 描画する文字列のサイズを設定
	SetFontSize(fsize);

	// 文字列の描画
	rc = DrawString(xyp.x, xyp.y, xyp.moji, GetColor(rgb.R, rgb.G, rgb.B));

	return rc;
}

int sub_cFont(int fsize, char* font)
{
	int rc;
	if (font != "")
	{
		// 描画する文字列の文字セットを変更します
		rc = ChangeFont(font);
		if (rc == -1) return -1;
	}

	// 描画する文字列のサイズを設定
	SetFontSize(fsize);
	return 0;
}

int sub_drowFormat(FXY xy, nRGB rgb, const TCHAR *s, ...)
{
	int rc;
	// FORMAT文字描画
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

	sub_drowBox(xy, rgb, TRUE); // バックグラウンドで塗りつぶし
}