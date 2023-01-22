#pragma once
/*********************************************************************************************************************/
/*                                                                                                                   */
/*  DX用クッションファイル                                                                                           */
/*                                                                                                                   */
/*********************************************************************************************************************/
#ifndef DX_SUBFUNC_H
#define DX_SUBFUNC_H

// 座標
struct _XY
{
	float x;
	float y;
};

using PXY = _XY;

// 線・四角
struct _LINEA
{
	PXY p1;
	PXY p2;
};

using LXY = _LINEA;
using BXY = _LINEA;

// 三角
struct _TRIANGLE
{
	PXY p1;
	PXY p2;
	PXY p3;
};

using TXY = _TRIANGLE;


// 円
struct _CIRCLE
{
	PXY p1;
	float r;
};

using CXY = _CIRCLE;
#define CIRCL_POINT_COUNT 32

// 楕円
struct _OVERL
{
	PXY p1;
	float rx;
	float ry;
};

using OXY = _OVERL;
#define OVERL_POINT_COUNT 64

// 画像
struct _IMAG
{
	int x;
	int y;
	char* path;
};

using IXY = _IMAG;

// 文字列
struct _STRING
{
	int x;
	int y;
	char* moji;
};

using SXY = _STRING;

// ホーマット
struct _FORM
{
	int x;
	int y;
};

using FXY = _FORM;

// 色
struct _Color
{
	int R;
	int G;
	int B;
};

using nRGB = _Color;

// キー押下
// Hit keyの種類外部共通変数
extern int kcount; //調べるキーの個数
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

// 関数
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
extern int sub_drowFormat(FXY, nRGB, const TCHAR*, ...); // 回帰文字列が連携されない

#endif
