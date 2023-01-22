#pragma once
/*********************************************************************************************************************/
/*                                                                                                                   */
/*  DX�p�N�b�V�����t�@�C��                                                                                           */
/*                                                                                                                   */
/*********************************************************************************************************************/
#ifndef DX_SUBFUNC_H
#define DX_SUBFUNC_H

// ���W
struct _XY
{
	float x;
	float y;
};

using PXY = _XY;

// ���E�l�p
struct _LINEA
{
	PXY p1;
	PXY p2;
};

using LXY = _LINEA;
using BXY = _LINEA;

// �O�p
struct _TRIANGLE
{
	PXY p1;
	PXY p2;
	PXY p3;
};

using TXY = _TRIANGLE;


// �~
struct _CIRCLE
{
	PXY p1;
	float r;
};

using CXY = _CIRCLE;
#define CIRCL_POINT_COUNT 32

// �ȉ~
struct _OVERL
{
	PXY p1;
	float rx;
	float ry;
};

using OXY = _OVERL;
#define OVERL_POINT_COUNT 64

// �摜
struct _IMAG
{
	int x;
	int y;
	char* path;
};

using IXY = _IMAG;

// ������
struct _STRING
{
	int x;
	int y;
	char* moji;
};

using SXY = _STRING;

// �z�[�}�b�g
struct _FORM
{
	int x;
	int y;
};

using FXY = _FORM;

// �F
struct _Color
{
	int R;
	int G;
	int B;
};

using nRGB = _Color;

// �L�[����
// Hit key�̎�ފO�����ʕϐ�
extern int kcount; //���ׂ�L�[�̌�
extern int k01;
extern int k02;
extern int k03;
extern int k04;
extern int k05;
extern int k06;
extern int k07;
extern int k08;
extern int k09;
extern int k10;

// �֐�
extern int sub_DxInt();
extern void sub_DxEnd();
extern void sub_waitKeyAll();
extern int sub_HitKey(int);
extern int sub_waitKey();
extern void sub_cls(nRGB);
extern void sub_drowPoint(PXY, nRGB);
extern void sub_drowLine(LXY, nRGB);
extern void sub_drowBox(BXY, nRGB, bool);
extern void sub_drowCircle(CXY, nRGB, bool);
extern void sub_drowOval(OXY, nRGB, bool);
extern void sub_drowTriangle(TXY, nRGB, bool);
extern int sub_drowImg(IXY, bool);
extern int sub_drowString(SXY, nRGB, int, char*);
extern int sub_cFont(int, char*);
extern int sub_drowFormat(FXY, nRGB, const TCHAR*, ...); // ��A�����񂪘A�g����Ȃ�

#endif
