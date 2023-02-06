#include "DxLib.h"
#include "DxSubfunc.h"
#include "game.h"

#include <math.h>
#include <iostream>     // cout
#include <Windows.h>

#define BLOCK_SIZE		1000.0f		// ブロックのサイズ

#define BLOCK_NUM_X		35		// Ｘ方向のブロック数
#define BLOCK_NUM_Z		35		// Ｚ方向のブロック数

#define CAMERA_Y		500.0f		// カメラの高さ

#define MOVE_FRAME		30		// 移動や旋回に掛けるフレーム数

#define TO_DO           10

#define KYOYOU          5

int toMap[TO_DO][2];

// マップ( 1:道  0:壁 )
int Map[BLOCK_NUM_Z][BLOCK_NUM_X] /* =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,0,1,1,1,0,1,1,0,1,1,0,
	0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,
	0,1,1,1,1,1,1,0,1,0,0,1,1,1,1,0,
	0,1,0,1,0,0,0,0,1,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,
	0,0,0,1,1,1,0,1,0,0,0,1,0,0,1,0,
	0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,
	0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,
	0,0,0,1,1,1,1,1,0,0,0,1,0,0,1,0,
	0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,
	0,1,1,1,0,1,0,0,0,0,1,0,1,0,1,0,
	0,1,0,1,1,1,0,0,0,1,1,0,1,0,0,0,
	0,1,0,1,0,0,0,1,1,1,0,0,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
}*/;

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int KabeModel;		// 壁モデル
	int x, z;		// 位置
	int movx, movz;	// 移動先の座標
	int Muki;		// 向き( 0:x軸プラス方向  1:z軸マイナス方向  2:x軸マイナス方向  3:z軸プラス方向 )
	int NowInput;		// 現在のフレームの入力
	int FrameNo;		// フレーム番号
	int State;		// 状態( 0:入力待ち 1:前進中 2:後退中 3:左旋回中 4:右旋回中 )
	int Count;		// 汎用カウンタ
	int i, j;		// 汎用変数
	float f;		// 汎用変数
	VECTOR CamPos;		// カメラの座標
	VECTOR CamDir;		// カメラの向いている方向
	VECTOR CamTarg;	// カメラの注視点

	//迷路の作り直し
	make_Maiz();
	char numStr[1028];
	for (int i = 0; i < BLOCK_NUM_Z; i++)
	{
		for (int j = 0; j < BLOCK_NUM_X; j++)
		{
			sprintf(numStr, "%d,", Map[i][j]);
			OutputDebugString(numStr);
		}

		sprintf(numStr, "\n");
		OutputDebugString(numStr);
	}

	// ウインドウモードで起動
	ChangeWindowMode(TRUE);

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0) return -1;

	// 壁モデルの読みこみ
	KabeModel = MV1LoadModel("Kabe.mqo");

	// 位置と向きの初期化
	x = 1;
	z = 1;
	Muki = 0;

	// カメラの座標と向きと注視点をセットする
	CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
	CamDir = VGet(1.0f, 0.0f, 0.0f);
	CamTarg = VAdd(CamPos, CamDir);
	SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg);

	// 状態の初期化
	State = 0;

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// メインループ
	// エスケープキーが押されるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面をクリアする
		ClearDrawScreen();

		// 現在の入力を取得する
		NowInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// 状態によって処理を分岐
		switch (State)
		{
		case 0:	// 入力待ち状態

			// 上が押されたら向いている方向に移動する状態に移行する
			if ((NowInput & PAD_INPUT_UP) != 0)
			{
				// 向きによって移動方向が変わる
				switch (Muki)
				{
				case 0: movx = 1; movz = 0; break;		// Ｘ軸プラス方向
				case 1: movx = 0; movz = -1; break;		// Ｚ軸マイナス方向
				case 2: movx = -1; movz = 0; break;		// Ｘ軸マイナス方向
				case 3: movx = 0; movz = 1; break;		// Ｚ軸プラス方向
				}

				// 移動先のマスが道だったら移動する
				if (Map[z + movz][x + movx] == 1 || Map[z + movz][x + movx] == 2)
				{
					// 状態を前進中にする
					State = 1;
					Count = 0;
				}
			}

			// 下が押されたら向いている方向と逆方向に移動する
			if ((NowInput & PAD_INPUT_DOWN) != 0)
			{
				// 向きによって移動方向が変わる
				switch (Muki)
				{
				case 0: movx = -1; movz = 0; break;		// Ｘ軸プラス方向
				case 1: movx = 0; movz = 1; break;		// Ｚ軸マイナス方向
				case 2: movx = 1; movz = 0; break;		// Ｘ軸マイナス方向
				case 3: movx = 0; movz = -1; break;		// Ｚ軸プラス方向
				}

				// 移動先のマスが道だったら移動する
				if (Map[z + movz][x + movx] == 1)
				{
					// 状態を後退中にする
					State = 2;
					Count = 0;
				}
			}

			// 左が押されていたら向いている方向を左に９０度変更する
			if ((NowInput & PAD_INPUT_LEFT) != 0)
			{
				// 状態を左旋回中にする
				State = 3;
				Count = 0;
			}

			// 右が押されていたら向いている方向を右に９０度変更する
			if ((NowInput & PAD_INPUT_RIGHT) != 0)
			{
				// 状態を右旋回中にする
				State = 4;
				Count = 0;
			}

			break;

		case 1:	// 前進中状態
			// カウントを進める
			Count++;

			// カメラの座標を移動途中の座標にする
			CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
			CamPos = VAdd(CamPos, VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME));
			CamTarg = VAdd(CamPos, CamDir);

			// カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
			if (Count == MOVE_FRAME)
			{
				x += movx;
				z += movz;

				State = 0;
				Count = 0;
			}
			break;

		case 2:	// 後退中状態
			// カウントを進める
			Count++;

			// カメラの座標を移動途中の座標にする
			CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
			CamPos = VSub(CamPos, VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME));
			CamTarg = VAdd(CamPos, CamDir);

			// カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
			if (Count == MOVE_FRAME)
			{
				x += movx;
				z += movz;

				State = 0;
				Count = 0;
			}
			break;

		case 3:	// 左旋回中状態
			// カウントを進める
			Count++;

			// 向いている方向を旋回途中の方向にする
			switch (Muki)
			{
			case 0: f = 0.0f; break;		// Ｘプラス方向
			case 1: f = -DX_PI_F / 2.0f; break;		// Ｚマイナス方向
			case 2: f = DX_PI_F; break;		// Ｘマイナス方向
			case 3: f = DX_PI_F / 2.0f; break;		// Ｚプラス方向
			}
			f += DX_PI_F / 2.0f * Count / MOVE_FRAME;
			CamDir.x = cos(f);
			CamDir.z = sin(f);
			CamTarg = VAdd(CamPos, CamDir);

			// カウントが推移時間に達したら実方向を変更して入力待ち状態に戻る
			if (Count == MOVE_FRAME)
			{
				if (Muki == 0)
				{
					Muki = 3;
				}
				else
				{
					Muki--;
				}

				State = 0;
				Count = 0;
			}
			break;

		case 4:	// 右旋回中状態
			// カウントを進める
			Count++;

			// 向いている方向を旋回途中の方向にする
			switch (Muki)
			{
			case 0: f = 0.0f; break;		// Ｘプラス方向
			case 1: f = -DX_PI_F / 2.0f; break;		// Ｚマイナス方向
			case 2: f = DX_PI_F; break;		// Ｘマイナス方向
			case 3: f = DX_PI_F / 2.0f; break;		// Ｚプラス方向
			}
			f -= DX_PI_F / 2.0f * Count / MOVE_FRAME;
			CamDir.x = cos(f);
			CamDir.z = sin(f);
			CamTarg = VAdd(CamPos, CamDir);

			// カウントが推移時間に達したら実方向を変更して入力待ち状態に戻る
			if (Count == MOVE_FRAME)
			{
				if (Muki == 3)
				{
					Muki = 0;
				}
				else
				{
					Muki++;
				}

				State = 0;
				Count = 0;
			}
			break;
		}

		// カメラの位置と向きをセットする
		SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg);

		// マップを描画する
		for (i = 0; i < BLOCK_NUM_Z; i++)
		{
			for (j = 0; j < BLOCK_NUM_X; j++)
			{
				// 道ではないところは描画しない
				if (Map[i][j] == 0) continue;

				// 壁モデルの座標を変更する
				MV1SetPosition(KabeModel, VGet(j * BLOCK_SIZE, 0.0f, i * BLOCK_SIZE));

				// ４方の壁の状態で描画するフレーム番号を変更する
				FrameNo = 0;
				if (Map[i][j + 1] == 0) FrameNo += 1;
				if (Map[i][j - 1] == 0) FrameNo += 2;
				if (Map[i + 1][j] == 0) FrameNo += 4;
				if (Map[i - 1][j] == 0) FrameNo += 8;

				// 割り出した番号のフレームを描画する
				MV1DrawFrame(KabeModel, FrameNo);
			}
		}

		// 裏画面の内容を表画面に反映する
		ScreenFlip();
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}

// 自動生成用のグローバル変数
int x, z, dd = 0;  // 座標と向きの個数
int mm[BLOCK_NUM_Z * BLOCK_NUM_X];  // 向きの配列

// 自動生成メイン
void make_Maiz()
{

	// 配列の初期化
	for (int i = 0; i < BLOCK_NUM_Z; i++)
	{
		for (int j = 0; j < BLOCK_NUM_X; j++)
		{
			Map[i][j] = 0;
		}
	}

	// 開始位置設定 マップ意味　0:壁 1:通路 2:通路作成中(壁を始点として作成)
	x = 1; z = 1; Map[z][x] = 1;

	int mt; // 乱数一時保存用

	// 最初の迷路作り出し
	for (int j = 0; j < 30; j++)
	{
		for (int i = 0; i < TO_DO; i++)
		{
			mm[dd] = d_makeMap(x, z);
			toMap[i][0] = z; toMap[i][1] = x;
			dd++;
		}
		srand(time(NULL));
		mt = sub_Random(0, TO_DO);
		x = toMap[mt][1]; z = toMap[mt][0];
	}

#if 1

	//　中盤の迷路作成
	for (int j = 0; j < (BLOCK_NUM_Z * BLOCK_NUM_X) / 2; j++)
	{
		x = sub_Random(1, BLOCK_NUM_X - 2);
		z = sub_Random(1, BLOCK_NUM_Z - 2);
		if (Map[z][x] == 1) d_makeMap(x, z);
		else if (Map[z][x] == 0) s_makeMap(x, z);
	}

	// 最終の迷路作成
	for (int i = 1; i < BLOCK_NUM_Z - 1; i+=2)
	{
		for (int j = 1; j < BLOCK_NUM_X - 1; j+=2)
		{
			if (Map[i][j] == 0 ||  Map[i][j] == 2)
			{
				Map[i][j] = 1;
				// dz = 1
				if (d_SetMap(j, i, 0, 1, 1, 1) == 0) continue;
				// dx = 1
				if (d_SetMap(j, i, 1, 0, 1, 1) == 0) continue;
				// dz = -1
				if (d_SetMap(j, i, 0, -1, 1, 1) == 0) continue;
				// dx = - 1
				if (d_SetMap(j, i, -1, 0, 1, 1) == 0) continue;
				Map[i][j] = 0;
			}
		}
	}
#endif

	// 作成処理中の通路を通路に変える
	for (int i = 1; i < BLOCK_NUM_Z - 1; i++)
	{
		for (int j = 1; j < BLOCK_NUM_X - 1; j++)
		{
			if (Map[i][j] == 2) Map[i][j] = 1;
		}
	}

}

// 始点が通路の場合の作成
int d_makeMap(int xx, int zz)
{
	int mt, mxf = 0, mzf = 0, pxf = 0, pzf = 0, okf = 0;
	srand(time(NULL));

	int p = 12;
	int r[] = { 0,1,2,3,0,1,2,3,0,1,2,3 };
	int k,l,o;

	for (int i = 0; i <p; i ++) {
		k = r[i]; l = sub_Random(0, p); r[i] = r[l]; r[l] = k;
	}

	k = 0; o = 0;
	bool bkf;
	while (okf == 0 && mxf*mzf*pxf*pzf == 0 )
	{
		bkf = true;
		mt = r[k]; if (++k > p) k = 0;
		while (bkf) {
			switch (mt)
			{
			case 0: // dx = 1
				if (dd > KYOYOU && o < 1) {
					o++;
					if (mt = daburi() != -1) continue;
					mt = 0;
				}
				if (d_SetMap(xx, zz, 1, 0, 0, 1) == -1) {
					pxf = 1; bkf = false; break;
				}
				xx += 2; okf = 1; bkf = false;
				break;
			case 1: // dx = -1
				if (dd > KYOYOU &&  o < 1) {
					o++;
					if (mt = daburi() != -1) continue;
					mt = 1;
				}
				if (d_SetMap(xx, zz, -1, 0, 0, 1) == -1) {
					mxf = 1; bkf = false; break;
				}
				xx -= 2; okf = 1; bkf = false;
				break;
			case 2: // dz = 1
				if (dd > KYOYOU && o < 1) {
					o++;
					if (mt = daburi() != -1) continue;
					mt = 2;
				}
				if (d_SetMap(xx, zz, 0, 1, 0, 1) == -1) {
					pzf = 1; bkf = false; break;
				}
				zz += 2; okf = 1; bkf = false;
				break;
			case 3: // dz = -1
				if (dd > KYOYOU && o < 1) {
					o++;
					if (mt = daburi() != -1) continue;
					mt = 3;
				}
				if (d_SetMap(xx, zz, 0, -1, 0, 1) == -1) {
					mzf = 1; bkf = false; break;
				}
				zz -= 2; okf = 1; bkf = false;
				break;
			default:
				bkf = false;
				break;
			}
		}
	}

	x = xx;
	z = zz;

	return mt;

}

// 始点が壁の場合の作成
int s_makeMap(int xx, int zz)
{
	int mm, mxf = 0, mzf = 0, pxf = 0, pzf = 0, okf = 0;
	srand(time(NULL));

	int p = 12;
	int r[] = { 0,1,2,3,0,1,2,3,0,1,2,3 };
	int k, l;

	for (int i = 0; i < p; i++) {
		k = r[i]; l = sub_Random(0, p); r[i] = r[l]; r[l] = k;
	}

	k = 0;
	while (okf == 0 && mxf * mzf * pxf * pzf == 0)
	{
		mm = r[k]; if (++k > p) k = 0;
		switch (mm)
		{
		case 0: // dx = 1
			if (d_SetMap(xx, zz, 1, 0, 0, 2) == -1) {
				if (d_SetMap(xx, zz, 1, 0, 1, 2) == -1) {
					pxf = 1; continue;
				}
			}
			xx += 2; okf = 1;
			break;
		case 1: // dx = -1
			if (d_SetMap(xx, zz, -1, 0, 0, 2) == -1) {
				if (d_SetMap(xx, zz, -1, 0, 1, 2) == -1) {
					mxf = 1; continue;
				}
			}
			xx -= 2; okf = 1;
			break;
		case 2: // dz = 1
			if (d_SetMap(xx, zz, 0, 1, 0, 2) == -1) {
				if (d_SetMap(xx, zz, 0, 1, 1, 2) == -1) {
					pzf = 1; continue;
				}
			}
			zz += 2; okf = 1;
			break;
		case 3: // dz = -1
			if (d_SetMap(xx, zz, 0, -1, 0, 2) == -1) {
				if (d_SetMap(xx, zz, 0, -1, 1, 2) == -1) {
					mzf = 1; continue;
				}
			}
			zz -= 2; okf = 1;
			break;
		default:
			break;
		}
	}

	x = xx;
	z = zz;

	return mm;

}

// 作業点での通路作成
int d_SetMap(int dx, int dz, int mx, int mz, int cord, int nc)
{
	int rc = -1;

	//char numStr[1028];
	//sprintf(numStr, "x%d,z%d,mx%d,mz%d \n",dx,dz,mx,mz);
	//OutputDebugString(numStr);

	// 現在のxが範囲内
	if (dx + mx + mx > 0 && dx + mx + mx < BLOCK_NUM_X - 1)
	{
		// 現在のzが範囲内
		if (dz + mz + mz> 0 && dz + mz + mz < BLOCK_NUM_Z - 1)
		{
			// 現在位置が指定の状態か？
			if (Map[dz + mz + mz][dx + mx + mx] == cord)
			{
				// 指定の通路状態に変更
				Map[dz + mz][dx + mx] = nc;
				Map[dz + mz + mz][dx + mx + mx] = nc;
				rc = 0;
			}
		}
	}

	return rc;
}

// 通路向きのダブり解消
int daburi()
{
	int mt = -1, t = 0;

	// 許容範囲まで貯める
	if (dd < KYOYOU + 1) return mt;

	// 向きが同じかチェック
	if (mm[dd] == mm[dd - 1]) t++;
	if (mm[dd] == mm[dd - 2]) t++;
	if (mm[dd] == mm[dd - 3]) t++;
	if (mm[dd] == mm[dd - 4]) t++;
	if (mm[dd] == mm[dd - 5]) t++;


	// 許容以上同じ状態なら
	if (t >= KYOYOU - 1)
	{
		switch (mm[dd]) // 向きでわける
		{
		case 0: // dx +1
			if (z > BLOCK_NUM_Z / 2) mt = 3;
			else mt = 2;
			break;
		case 1: // dx -1
			if (z > BLOCK_NUM_Z / 2) mt = 3;
			else mt = 2;
			break;
		case 2: // dz + 1
			if (x > BLOCK_NUM_X / 2) mt = 1;
			else mt = 0;
			break;
		case 3: // dz -1
			if (x > BLOCK_NUM_X / 2) mt = 1;
			else mt = 0;
			break;

		}
	}

	return mt;
}
