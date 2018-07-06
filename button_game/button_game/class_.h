#pragma once
#include"header.h"

class _class
{
private:
	static CONST UINT fildSize = 4;// размер поля
	static CONST UINT numberMax = 100;// максимальное значение для чисел
	TCHAR stoneNum[5], timeCH[100], timeCH1[100];
	INT stoneSize, fildSizePcl, stoneNumber, k = 0, t = 0, sec, min, nTime = 60;
	HWND hUpdateSet, hTime, hStatusBar, hProgressBar, hSpin, hList, hEdit2;
	HINSTANCE hInst;
	list<INT>numberList;
	HWND Blocks[fildSize][fildSize];

	static _class *ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	void Cls_OnTimer(HWND hwnd, UINT id);

	void CreateStonse(HWND hwnd);
	void CreateStartButton(HWND hwnd);
	void CreateStatusProgress(HWND hwnd);
	void CreateCounter(HWND hwnd);
	void CreateList(HWND hwnd);

	VOID StartNewGame(HWND);
	BOOL CheckOrder(HWND hwnd, HWND h, INT stone);
public:
	_class();
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
};