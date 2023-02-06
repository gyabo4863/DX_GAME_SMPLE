#include "DxLib.h"
#include "DxSubfunc.h"
#include "game.h"

#include <math.h>
#include <iostream>     // cout
#include <Windows.h>

#define BLOCK_SIZE		1000.0f		// �u���b�N�̃T�C�Y

#define BLOCK_NUM_X		35		// �w�����̃u���b�N��
#define BLOCK_NUM_Z		35		// �y�����̃u���b�N��

#define CAMERA_Y		500.0f		// �J�����̍���

#define MOVE_FRAME		30		// �ړ������Ɋ|����t���[����

#define TO_DO           10

#define KYOYOU          5

int toMap[TO_DO][2];

// �}�b�v( 1:��  0:�� )
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
	int KabeModel;		// �ǃ��f��
	int x, z;		// �ʒu
	int movx, movz;	// �ړ���̍��W
	int Muki;		// ����( 0:x���v���X����  1:z���}�C�i�X����  2:x���}�C�i�X����  3:z���v���X���� )
	int NowInput;		// ���݂̃t���[���̓���
	int FrameNo;		// �t���[���ԍ�
	int State;		// ���( 0:���͑҂� 1:�O�i�� 2:��ޒ� 3:������ 4:�E���� )
	int Count;		// �ėp�J�E���^
	int i, j;		// �ėp�ϐ�
	float f;		// �ėp�ϐ�
	VECTOR CamPos;		// �J�����̍��W
	VECTOR CamDir;		// �J�����̌����Ă������
	VECTOR CamTarg;	// �J�����̒����_

	//���H�̍�蒼��
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

	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);

	// �c�w���C�u�����̏�����
	if (DxLib_Init() < 0) return -1;

	// �ǃ��f���̓ǂ݂���
	KabeModel = MV1LoadModel("Kabe.mqo");

	// �ʒu�ƌ����̏�����
	x = 1;
	z = 1;
	Muki = 0;

	// �J�����̍��W�ƌ����ƒ����_���Z�b�g����
	CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
	CamDir = VGet(1.0f, 0.0f, 0.0f);
	CamTarg = VAdd(CamPos, CamDir);
	SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg);

	// ��Ԃ̏�����
	State = 0;

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// ���C�����[�v
	// �G�X�P�[�v�L�[���������܂Ń��[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ��ʂ��N���A����
		ClearDrawScreen();

		// ���݂̓��͂��擾����
		NowInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// ��Ԃɂ���ď����𕪊�
		switch (State)
		{
		case 0:	// ���͑҂����

			// �オ�����ꂽ������Ă�������Ɉړ������ԂɈڍs����
			if ((NowInput & PAD_INPUT_UP) != 0)
			{
				// �����ɂ���Ĉړ��������ς��
				switch (Muki)
				{
				case 0: movx = 1; movz = 0; break;		// �w���v���X����
				case 1: movx = 0; movz = -1; break;		// �y���}�C�i�X����
				case 2: movx = -1; movz = 0; break;		// �w���}�C�i�X����
				case 3: movx = 0; movz = 1; break;		// �y���v���X����
				}

				// �ړ���̃}�X������������ړ�����
				if (Map[z + movz][x + movx] == 1 || Map[z + movz][x + movx] == 2)
				{
					// ��Ԃ�O�i���ɂ���
					State = 1;
					Count = 0;
				}
			}

			// ���������ꂽ������Ă�������Ƌt�����Ɉړ�����
			if ((NowInput & PAD_INPUT_DOWN) != 0)
			{
				// �����ɂ���Ĉړ��������ς��
				switch (Muki)
				{
				case 0: movx = -1; movz = 0; break;		// �w���v���X����
				case 1: movx = 0; movz = 1; break;		// �y���}�C�i�X����
				case 2: movx = 1; movz = 0; break;		// �w���}�C�i�X����
				case 3: movx = 0; movz = -1; break;		// �y���v���X����
				}

				// �ړ���̃}�X������������ړ�����
				if (Map[z + movz][x + movx] == 1)
				{
					// ��Ԃ���ޒ��ɂ���
					State = 2;
					Count = 0;
				}
			}

			// ����������Ă���������Ă�����������ɂX�O�x�ύX����
			if ((NowInput & PAD_INPUT_LEFT) != 0)
			{
				// ��Ԃ������񒆂ɂ���
				State = 3;
				Count = 0;
			}

			// �E��������Ă���������Ă���������E�ɂX�O�x�ύX����
			if ((NowInput & PAD_INPUT_RIGHT) != 0)
			{
				// ��Ԃ��E���񒆂ɂ���
				State = 4;
				Count = 0;
			}

			break;

		case 1:	// �O�i�����
			// �J�E���g��i�߂�
			Count++;

			// �J�����̍��W���ړ��r���̍��W�ɂ���
			CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
			CamPos = VAdd(CamPos, VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME));
			CamTarg = VAdd(CamPos, CamDir);

			// �J�E���g���ړ����ԂɒB����������W���ړ����ē��͑҂���Ԃɖ߂�
			if (Count == MOVE_FRAME)
			{
				x += movx;
				z += movz;

				State = 0;
				Count = 0;
			}
			break;

		case 2:	// ��ޒ����
			// �J�E���g��i�߂�
			Count++;

			// �J�����̍��W���ړ��r���̍��W�ɂ���
			CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
			CamPos = VSub(CamPos, VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME));
			CamTarg = VAdd(CamPos, CamDir);

			// �J�E���g���ړ����ԂɒB����������W���ړ����ē��͑҂���Ԃɖ߂�
			if (Count == MOVE_FRAME)
			{
				x += movx;
				z += movz;

				State = 0;
				Count = 0;
			}
			break;

		case 3:	// �����񒆏��
			// �J�E���g��i�߂�
			Count++;

			// �����Ă�����������r���̕����ɂ���
			switch (Muki)
			{
			case 0: f = 0.0f; break;		// �w�v���X����
			case 1: f = -DX_PI_F / 2.0f; break;		// �y�}�C�i�X����
			case 2: f = DX_PI_F; break;		// �w�}�C�i�X����
			case 3: f = DX_PI_F / 2.0f; break;		// �y�v���X����
			}
			f += DX_PI_F / 2.0f * Count / MOVE_FRAME;
			CamDir.x = cos(f);
			CamDir.z = sin(f);
			CamTarg = VAdd(CamPos, CamDir);

			// �J�E���g�����ڎ��ԂɒB�������������ύX���ē��͑҂���Ԃɖ߂�
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

		case 4:	// �E���񒆏��
			// �J�E���g��i�߂�
			Count++;

			// �����Ă�����������r���̕����ɂ���
			switch (Muki)
			{
			case 0: f = 0.0f; break;		// �w�v���X����
			case 1: f = -DX_PI_F / 2.0f; break;		// �y�}�C�i�X����
			case 2: f = DX_PI_F; break;		// �w�}�C�i�X����
			case 3: f = DX_PI_F / 2.0f; break;		// �y�v���X����
			}
			f -= DX_PI_F / 2.0f * Count / MOVE_FRAME;
			CamDir.x = cos(f);
			CamDir.z = sin(f);
			CamTarg = VAdd(CamPos, CamDir);

			// �J�E���g�����ڎ��ԂɒB�������������ύX���ē��͑҂���Ԃɖ߂�
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

		// �J�����̈ʒu�ƌ������Z�b�g����
		SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg);

		// �}�b�v��`�悷��
		for (i = 0; i < BLOCK_NUM_Z; i++)
		{
			for (j = 0; j < BLOCK_NUM_X; j++)
			{
				// ���ł͂Ȃ��Ƃ���͕`�悵�Ȃ�
				if (Map[i][j] == 0) continue;

				// �ǃ��f���̍��W��ύX����
				MV1SetPosition(KabeModel, VGet(j * BLOCK_SIZE, 0.0f, i * BLOCK_SIZE));

				// �S���̕ǂ̏�Ԃŕ`�悷��t���[���ԍ���ύX����
				FrameNo = 0;
				if (Map[i][j + 1] == 0) FrameNo += 1;
				if (Map[i][j - 1] == 0) FrameNo += 2;
				if (Map[i + 1][j] == 0) FrameNo += 4;
				if (Map[i - 1][j] == 0) FrameNo += 8;

				// ����o�����ԍ��̃t���[����`�悷��
				MV1DrawFrame(KabeModel, FrameNo);
			}
		}

		// ����ʂ̓��e��\��ʂɔ��f����
		ScreenFlip();
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}

// ���������p�̃O���[�o���ϐ�
int x, z, dd = 0;  // ���W�ƌ����̌�
int mm[BLOCK_NUM_Z * BLOCK_NUM_X];  // �����̔z��

// �����������C��
void make_Maiz()
{

	// �z��̏�����
	for (int i = 0; i < BLOCK_NUM_Z; i++)
	{
		for (int j = 0; j < BLOCK_NUM_X; j++)
		{
			Map[i][j] = 0;
		}
	}

	// �J�n�ʒu�ݒ� �}�b�v�Ӗ��@0:�� 1:�ʘH 2:�ʘH�쐬��(�ǂ��n�_�Ƃ��č쐬)
	x = 1; z = 1; Map[z][x] = 1;

	int mt; // �����ꎞ�ۑ��p

	// �ŏ��̖��H���o��
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

	//�@���Ղ̖��H�쐬
	for (int j = 0; j < (BLOCK_NUM_Z * BLOCK_NUM_X) / 2; j++)
	{
		x = sub_Random(1, BLOCK_NUM_X - 2);
		z = sub_Random(1, BLOCK_NUM_Z - 2);
		if (Map[z][x] == 1) d_makeMap(x, z);
		else if (Map[z][x] == 0) s_makeMap(x, z);
	}

	// �ŏI�̖��H�쐬
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

	// �쐬�������̒ʘH��ʘH�ɕς���
	for (int i = 1; i < BLOCK_NUM_Z - 1; i++)
	{
		for (int j = 1; j < BLOCK_NUM_X - 1; j++)
		{
			if (Map[i][j] == 2) Map[i][j] = 1;
		}
	}

}

// �n�_���ʘH�̏ꍇ�̍쐬
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

// �n�_���ǂ̏ꍇ�̍쐬
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

// ��Ɠ_�ł̒ʘH�쐬
int d_SetMap(int dx, int dz, int mx, int mz, int cord, int nc)
{
	int rc = -1;

	//char numStr[1028];
	//sprintf(numStr, "x%d,z%d,mx%d,mz%d \n",dx,dz,mx,mz);
	//OutputDebugString(numStr);

	// ���݂�x���͈͓�
	if (dx + mx + mx > 0 && dx + mx + mx < BLOCK_NUM_X - 1)
	{
		// ���݂�z���͈͓�
		if (dz + mz + mz> 0 && dz + mz + mz < BLOCK_NUM_Z - 1)
		{
			// ���݈ʒu���w��̏�Ԃ��H
			if (Map[dz + mz + mz][dx + mx + mx] == cord)
			{
				// �w��̒ʘH��ԂɕύX
				Map[dz + mz][dx + mx] = nc;
				Map[dz + mz + mz][dx + mx + mx] = nc;
				rc = 0;
			}
		}
	}

	return rc;
}

// �ʘH�����̃_�u�����
int daburi()
{
	int mt = -1, t = 0;

	// ���e�͈͂܂Œ��߂�
	if (dd < KYOYOU + 1) return mt;

	// �������������`�F�b�N
	if (mm[dd] == mm[dd - 1]) t++;
	if (mm[dd] == mm[dd - 2]) t++;
	if (mm[dd] == mm[dd - 3]) t++;
	if (mm[dd] == mm[dd - 4]) t++;
	if (mm[dd] == mm[dd - 5]) t++;


	// ���e�ȏ㓯����ԂȂ�
	if (t >= KYOYOU - 1)
	{
		switch (mm[dd]) // �����ł킯��
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
