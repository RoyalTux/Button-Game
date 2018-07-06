#include "class_.h"

_class* _class::ptr = NULL;

_class::_class()
{
	ptr = this;
}

void _class::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL _class::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	//SetWindowText(hwnd, TEXT("00 : 00    Нажимайте на кнопочки в последовательности - от меньшего значения к большему"));
	HWND hOK = GetDlgItem(hwnd, IDOK);
	DestroyWindow(hOK);
	HWND hCancel = GetDlgItem(hwnd, IDCANCEL);
	DestroyWindow(hCancel);
	RECT clRect;
	GetClientRect(hwnd, &clRect);
	stoneSize = clRect.bottom / fildSize - 5;
	fildSizePcl = stoneSize * fildSize;
	CreateStonse(hwnd);
	CreateStartButton(hwnd);
	CreateStatusProgress(hwnd);
	CreateCounter(hwnd);
	CreateList(hwnd);
	return TRUE;
}

void _class::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HWND h = GetFocus();
	if (h == hUpdateSet)
	{
		MessageBox(hwnd, TEXT("Правила игры:\n\nНажимайте на кнопочки в последовательности - от меньшего значения к большему"), TEXT("Инструкция"), MB_OK | MB_DEFBUTTON2 | MB_ICONASTERISK);
		_class::StartNewGame(hwnd);
		t = 0;
	}
	if (h != hUpdateSet)
	{
		TCHAR text[5];
		GetWindowText(h, text, 4);
		INT stone = _wtoi(text);
		if (stone >= 0 && stone <= numberMax)
		{
			_class::CheckOrder(hwnd, h, stone);
		}
	}
}

void _class::Cls_OnTimer(HWND hwnd, UINT id)
{
	t++;
	sec = t % 60;
	min = (t - sec) / 60;
	wsprintf(timeCH, TEXT("%02d : %02d    Нажимайте на кнопочки в последовательности - от меньшего значения к большему"), min, sec);
	wsprintf(timeCH1, TEXT(" Время: %02d:%02d"), min, sec);
	SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM)timeCH1);
	//SetWindowText(hwnd, timeCH);
	if (nTime == t)
	{
		KillTimer(hwnd, 1);
		for (INT j = 0; j < fildSize; ++j)
		{
			for (INT i = 0; i < fildSize; ++i)
			{
				EnableWindow(Blocks[j][i], 0);
			}
		}
	}
	FLOAT perCent = t * 100 / nTime;
	SendMessage(hProgressBar, PBM_SETPOS, (INT)perCent, 0);
}

void _class::CreateStonse(HWND hwnd)
{
	srand(time(NULL));
	for (INT j = 0; j < fildSize; ++j)
	{
		for (INT i = 0; i < fildSize; ++i)
		{
			stoneNumber = rand() % (numberMax + 1);
			numberList.push_back(stoneNumber);
			_itow_s(stoneNumber, stoneNum, 10);
			Blocks[j][i] = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"),
				stoneNum, WS_CHILD | WS_VISIBLE,
				i * stoneSize, j * stoneSize,
				stoneSize, stoneSize, hwnd, 0, hInst, 0);
			EnableWindow(Blocks[j][i], 0);
		}
	}
}

void _class::CreateStartButton(HWND hwnd)
{
	hUpdateSet = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"),
		TEXT("START GAME"), WS_CHILD | WS_VISIBLE | BS_MULTILINE,
		fildSizePcl + 30, 300, 64, 64, hwnd, 0, hInst, 0);
}

void _class::CreateCounter(HWND hwnd)
{
	hSpin = GetDlgItem(hwnd, IDC_SPIN1);
	hEdit2 = GetDlgItem(hwnd, IDC_EDIT1);
	SendMessage(hSpin, UDM_SETRANGE32, 10, 100);
	SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEdit2), 0);
	TCHAR duration[4];
	_itow_s(nTime, duration, 10);
	SetWindowText(hEdit2, duration);
}

void _class::CreateStatusProgress(HWND hwnd)
{
	hStatusBar = CreateWindowEx(0, STATUSCLASSNAME, 0,
		WS_CHILD | WS_VISIBLE | SBT_TOOLTIPS | CCS_BOTTOM | SBARS_SIZEGRIP,
		0, 0, 0, 0, hwnd, HMENU(WM_USER), 0, 0);
	int parts[2] = { 400, -1 };
	SendMessage(hStatusBar, SB_SETBKCOLOR, 0, (LPARAM)RGB(255, 200, 255));
	SendMessage(hStatusBar, SB_SETPARTS, 2, (LPARAM)parts);
	SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM)TEXT(" Время: 00:00"));
	

	RECT r;
	SendMessage(hStatusBar, SB_GETRECT, 0, (LPARAM)&r);
	hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
		r.left + 3, r.top + 3, r.right - r.left, r.bottom - r.top,
		hStatusBar, NULL, NULL, NULL);
	SendMessage(hProgressBar, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255)));
	SendMessage(hProgressBar, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 255, 0)));
}

void _class::CreateList(HWND hwnd)
{
	hList = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), 0,
		WS_CHILD | WS_VISIBLE | LBS_NOSEL | LBS_STANDARD,
		fildSizePcl + 30, 25, 64, 220, hwnd, NULL, NULL, NULL);
}

void _class::StartNewGame(HWND hwnd)
{
	SendMessage(hList, LB_RESETCONTENT, 0, 0);
	SetTimer(hwnd, 1, 1000, NULL);
	numberList.clear();
	srand(time(NULL));
	for (INT j = 0; j < fildSize; ++j)
	{
		for (INT i = 0; i < fildSize; ++i)
		{
			stoneNumber = rand() % (numberMax + 1);
			numberList.push_back(stoneNumber);
			_itow_s(stoneNumber, stoneNum, 10);
			SetWindowText(Blocks[j][i], stoneNum);
			EnableWindow(Blocks[j][i], 1);
		}
	}
	numberList.list::sort();
	nTime = SendMessage(hSpin, UDM_GETPOS32, 0, 0);
}

BOOL _class::CheckOrder(HWND hwnd, HWND h, INT stone)
{
	if (stone == *numberList.begin())
	{
		numberList.pop_front();
		TCHAR* strNumber = new TCHAR[4];
		wsprintf(strNumber, TEXT("%3d"), stone);
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(strNumber));
		delete[] strNumber;
		EnableWindow(h, 0);
		if (numberList.size() == 0)
		{
			KillTimer(hwnd, 1);
		}
		return TRUE;
	}
	else return FALSE;
}

BOOL CALLBACK _class::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, ptr->Cls_OnTimer);
	}
	return FALSE;
}