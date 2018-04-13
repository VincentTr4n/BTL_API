#include <windows.h>
#include "resource.h"
#include <Commdlg.h>
#include <string>
#include <cstring>
#include <iostream>
#include <ole2.h>
#include <olectl.h>
#include <chrono>
#include <thread>
#include <atlstr.h>
using namespace std;


wstring OpenFileDialog() {
	OPENFILENAME ofn;
	char szFile[260];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.txt\0BITMAP\0*.bmp\0";
	ofn.nFilterIndex = -1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
	wstring res(ofn.lpstrFile);
	return res;
}


COLORREF ShowColorDialog(HWND hwnd) {
	CHOOSECOLOR cc;
	static COLORREF crCustClr[16];
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)crCustClr;
	cc.rgbResult = RGB(0, 255, 0);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&cc);
	return cc.rgbResult;
}

