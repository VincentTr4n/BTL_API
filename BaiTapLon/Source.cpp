
#include <windows.h>
#include "resource.h"
#include <Commdlg.h>
#include <string>
#include <cstring>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PanelProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

//void RegisterPanel(void);
//void CreateDialogBox(HWND);
//void RegisterDialogClass(HWND);

COLORREF ShowColorDialog(HWND);
COLORREF gColor = RGB(255, 255, 255);

HINSTANCE ghInstance;
//////////////
//////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//hihihihihihihihii
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndclass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CT));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass)){
		MessageBox(NULL, TEXT("this program requires window nt!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName, TEXT("Ứng Dụng Paint"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.lParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HBITMAP bm;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static int hinh, r, g, b, i, n = 0,kt=1;
	static float xStart, yStart, xEnd,yEnd;
	static HBRUSH hBrush;
	static POINT ptPoint[100000],apt_tg[3],apt_ht[4],apt_mt[7],apt_lg[6],apt_ng[5],apt_ns[12];
	static HPEN hPen,hPen1;




	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_CREATE:
		

		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, TEXT("Bạn có muốn thoát không?"), TEXT("Hỏi"), MB_YESNO | MB_ICONQUESTION) == IDYES)
			PostQuitMessage(0);
		return 0;

	
    case WM_MOUSEMOVE:
		
		if (hinh == ID_CHUOT){
			if (wParam & MK_LBUTTON)
			{
				ptPoint[n].x = LOWORD(lParam);
				ptPoint[n].y = HIWORD(lParam);
				hdc = GetDC(hwnd);
				SelectObject(hdc, hPen);
				if (n >= 1)
				{
					
					
					MoveToEx(hdc, ptPoint[n - 1].x, ptPoint[n - 1].y, NULL);
					LineTo(hdc, ptPoint[n].x, ptPoint[n].y);
				}
				n = n + 1;
				ReleaseDC(hwnd, hdc);
				break;
			}

		}
		if (hinh == ID_ERASER){
			if (wParam & MK_LBUTTON)
			{
			ptPoint[n].x = LOWORD(lParam);
			ptPoint[n].y = HIWORD(lParam);
			hdc = GetDC(hwnd);
			SelectObject(hdc, hPen);
			if (n >= 1)
			{


				MoveToEx(hdc, ptPoint[n - 1].x, ptPoint[n - 1].y, NULL);
				LineTo(hdc, ptPoint[n].x, ptPoint[n].y);
			}
			n = n + 1;
			ReleaseDC(hwnd, hdc);
			break;
		}
		}
		return 0;
	case WM_LBUTTONDOWN:
		xStart = LOWORD(lParam);
		yStart = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		n = 0;
		xEnd = LOWORD(lParam);
		yEnd = HIWORD(lParam);
		hdc = GetDC(hwnd);
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);
		if (hinh == ID_HINHCHUNHAT1){
			Rectangle(hdc, xStart, yStart, xEnd, yEnd);
		}

		if (hinh == ID_HINHCHUNHAT2){
			RoundRect(hdc, xStart, yStart, xEnd, yEnd,(xEnd-xStart)/20,(yEnd-yStart)/20);

		}
		if (hinh == ID_TAMGIAC1)
		{
			apt_tg[0].x = xStart;
			apt_tg[0].y = yStart;
			apt_tg[1].x = xEnd;
			apt_tg[1].y = yEnd;
			apt_tg[2].x = xStart;
			apt_tg[2].y = yEnd;
			Polygon(hdc, apt_tg,3);

		}
		if (hinh == ID_TAMGIAC2)
		{
			apt_tg[0].x = (xStart+xEnd)/2;
			apt_tg[0].y = yStart;
			apt_tg[1].x = xEnd;
			apt_tg[1].y = yEnd;
			apt_tg[2].x = xStart;
			apt_tg[2].y = yEnd;
			Polygon(hdc, apt_tg, 3);

		}
		if (hinh == ID_MUITENNGANG){
			apt_mt[0].x =xStart ;
			apt_mt[0].y =(3*yStart+yEnd)/4;
			apt_mt[1].x =(xStart+xEnd)/2 ;
			apt_mt[1].y = (3 * yStart + yEnd) / 4;
			apt_mt[2].x = (xStart + xEnd) / 2;
			apt_mt[2].y =yStart ;
			apt_mt[3].x = xEnd;
			apt_mt[3].y =(yStart+yEnd)/2 ;
			apt_mt[4].x = (xStart+xEnd)/2;
			apt_mt[4].y = yEnd;
			apt_mt[5].x = (xStart + xEnd) / 2;
			apt_mt[5].y = (yStart + 3*yEnd) / 4;
			apt_mt[6].x = xStart;
			apt_mt[6].y = (yStart + 3 * yEnd) / 4;
			Polygon(hdc, apt_mt, 7);
		}
		
		if (hinh == ID_MUITENDOC){
			apt_mt[0].x = (xStart+xEnd)/2;
			apt_mt[0].y = yStart;
			apt_mt[1].x = xEnd;
			apt_mt[1].y = (yStart+yEnd)/2;
			
			apt_mt[2].x = (3*xEnd+xStart)/4;
			apt_mt[2].y = (yStart+yEnd)/2;
			apt_mt[3].x = (3*xEnd + xStart) / 4;
			apt_mt[3].y = yEnd;
			apt_mt[4].x = (3*xStart+xEnd)/4;
			apt_mt[4].y = yEnd;
			apt_mt[5].x = (3 * xStart + xEnd) / 4;
			apt_mt[5].y = (yStart+yEnd)/2;
			apt_mt[6].x = xStart;
			apt_mt[6].y =( yStart+yEnd)/2;
			Polygon(hdc, apt_mt, 7);

		}
		if (hinh == ID_LUCGIAC){
			apt_lg[0].x =(xStart+xEnd)/2 ;
			apt_lg[0].y = yStart;

			apt_lg[1].x =xEnd;
			apt_lg[1].y =(3*yStart+yEnd)/4;

			apt_lg[2].x = xEnd;
			apt_lg[2].y = (yStart + 3*yEnd) / 4;

			apt_lg[3].x = (xEnd+xStart)/2;
			apt_lg[3].y = yEnd;

			apt_lg[4].x =xStart ;
			apt_lg[4].y = (yStart + 3 * yEnd) / 4;;

			apt_lg[5].x =xStart ;
			apt_lg[5].y = (3 * yStart + yEnd) / 4;
			
			Polygon(hdc, apt_lg, 6);
		}
		
		if (hinh == ID_HINHTHOI)
		{
			apt_ht[0].x = (xStart + xEnd) / 2;
			apt_ht[0].y = yStart;
			apt_ht[1].x = xEnd;
			apt_ht[1].y = (yStart + yEnd) / 2;
			apt_ht[2].x = (xStart + xEnd) / 2;
			apt_ht[2].y = yEnd;
			apt_ht[3].x = xStart;
			apt_ht[3].y = (yStart + yEnd) / 2;
			Polygon(hdc, apt_ht, 4);

		}
		if (hinh == ID_ECLIPSE)
		{
			Ellipse(hdc, xStart, yStart, xEnd, yEnd);
		}
		if (hinh == ID_DUONGTHANG){
			MoveToEx(hdc, xStart, yStart, NULL);
			LineTo(hdc, xEnd, yEnd);

		}
		if (hinh == ID_HINHNGUGIAC){
			apt_ng[0].x = (xStart + xEnd) / 2;
			apt_ng[0].y = yStart;
			apt_ng[1].x = xEnd;
			apt_ng[1].y = (yStart + yEnd) / 2;
			apt_ng[2].x = (xStart + 3*xEnd) / 4;
			apt_ng[2].y = yEnd;
			apt_ng[3].x = (3*xStart + xEnd) / 4;
			apt_ng[3].y = yEnd;
			apt_ng[4].x = xStart;
			apt_ng[4].y = (yStart + yEnd) / 2;
			Polygon(hdc, apt_ng, 5);
		}
		if (hinh == ID_SAO4CANH){
			apt_ns[0].x = (xEnd - xStart) / 2 + xStart;
			apt_ns[0].y = yStart;

			apt_ns[1].x = (xEnd - xStart) * 2 / 3 + xStart;
			apt_ns[1].y = (yEnd - yStart) / 3 + yStart;

			apt_ns[2].x = xEnd;
			apt_ns[2].y = (yEnd - yStart) / 2 + yStart;

			apt_ns[3].x = (xEnd - xStart) * 2 / 3 + xStart;
			apt_ns[3].y = (yEnd - yStart) * 2 / 3 + yStart;

			apt_ns[4].x = (xEnd - xStart) / 2 + xStart;
			apt_ns[4].y = yEnd;

			apt_ns[5].x = (xEnd - xStart) / 3 + xStart;
			apt_ns[5].y = (yEnd - yStart) * 2 / 3 + yStart;

			apt_ns[6].x = xStart;
			apt_ns[6].y = (yEnd - yStart) / 2 + yStart;

			apt_ns[7].x = (xEnd - xStart)  / 3 + xStart;
			apt_ns[7].y = (yEnd - yStart)  / 3 + yStart;
			Polygon(hdc, apt_ns, 8);
		}
		if (hinh==ID_SAO5CANH){
			apt_ns[0].x = (xEnd - xStart) / 2 + xStart;
			apt_ns[0].y = yStart;

			apt_ns[1].x = (xEnd - xStart) * 5 / 8 + xStart;
			apt_ns[1].y = (yEnd - yStart) * 2 / 5 + yStart;

			apt_ns[2].x = xEnd;
			apt_ns[2].y = (yEnd - yStart) * 2 / 5 + yStart;

			apt_ns[3].x = (xEnd - xStart) * 5 / 7 + xStart;
			apt_ns[3].y = (yEnd - yStart) * 3 / 5 + yStart;

			apt_ns[4].x = (xEnd - xStart) * 5 / 6 + xStart;
			apt_ns[4].y = yEnd;

			apt_ns[5].x = (xEnd - xStart) / 2 + xStart;
			apt_ns[5].y = (yEnd - yStart) * 7 / 9 + yStart;

			apt_ns[6].x = (xEnd - xStart) / 6 + xStart;
			apt_ns[6].y = yEnd;
			
			apt_ns[7].x = (xEnd - xStart) * 2 / 7 + xStart;
			apt_ns[7].y = (yEnd - yStart) * 3 / 5 + yStart;

			apt_ns[8].x = xStart;
			apt_ns[8].y = (yEnd - yStart) * 2 / 5 + yStart;

			apt_ns[9].x = (xEnd - xStart) * 3 / 8 + xStart;
			apt_ns[9].y = (yEnd - yStart) * 2 / 5 + yStart;
			Polygon(hdc, apt_ns, 10);
		}
		if (hinh == ID_SAO6CANH){
			apt_ns[0].x = (xEnd - xStart) / 2 + xStart;
			apt_ns[0].y = yStart;

			apt_ns[1].x = (xEnd - xStart) * 2 / 3 + xStart;
			apt_ns[1].y = (yEnd - yStart) / 4 + yStart;

			apt_ns[2].x = xEnd;
			apt_ns[2].y = (yEnd - yStart) / 4 + yStart;

			apt_ns[3].x = (xEnd - xStart) * 5 / 6 + xStart;
			apt_ns[3].y = (yEnd - yStart) / 2 + yStart;

			apt_ns[4].x = xEnd;
			apt_ns[4].y = (yEnd - yStart) * 3 / 4 + yStart;

			apt_ns[5].x = (xEnd - xStart) * 2 / 3 + xStart;
			apt_ns[5].y = (yEnd - yStart) * 3 / 4 + yStart;

			apt_ns[6].x = (xEnd - xStart) / 2 + xStart;
			apt_ns[6].y = yEnd;

			apt_ns[7].x = (xEnd - xStart) / 3 + xStart;
			apt_ns[7].y = (yEnd - yStart) * 3 / 4 + yStart;

			apt_ns[8].x = xStart;
			apt_ns[8].y = (yEnd - yStart) * 3 / 4 + yStart;

			apt_ns[9].x = (xEnd - xStart) / 6 + xStart;
			apt_ns[9].y = (yEnd - yStart) / 2 + yStart;

			apt_ns[10].x = xStart;
			apt_ns[10].y = (yEnd - yStart) / 4 + yStart;

			apt_ns[11].x = (xEnd - xStart) / 3 + xStart;
			apt_ns[11].y = (yEnd - yStart) / 4 + yStart;
			Polygon(hdc, apt_ns, 12);
		}
		
		
		ReleaseDC(hwnd, hdc);
		break;
	   
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_DUONGTHANG:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_ECLIPSE:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_HINHCHUNHAT1:
            hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_HINHCHUNHAT2:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_TAMGIAC1:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_TAMGIAC2:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_HINHTHOI:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_HINHNGUGIAC:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_LUCGIAC:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_MUITENNGANG:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;

		case ID_MUITENDOC:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_SAO4CANH:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_SAO5CANH:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case ID_SAO6CANH:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		
		case ID_CHUOT:
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		
		
		
		
		
		
			
	////////////////////////////////////////
		case ID_CQ_SOLID:
			gColor = ShowColorDialog(hwnd);
			hBrush = CreateSolidBrush(gColor);
			break;
		case ID_CQ_HORIZONTAL:
			gColor = ShowColorDialog(hwnd);
			hBrush = CreateHatchBrush(HS_HORIZONTAL,gColor);
			break;
			
		case ID_CQ_VERTICAL:
			gColor = ShowColorDialog(hwnd);
			hBrush = CreateHatchBrush(HS_VERTICAL, gColor);
			break;
		case ID_CQ_FDIALGONAL:
		    gColor = ShowColorDialog(hwnd);
			hBrush = CreateHatchBrush(HS_FDIAGONAL, gColor);
			break;
		case ID_CQ_BDIAGONAL:
			gColor = ShowColorDialog(hwnd);
			hBrush = CreateHatchBrush(HS_BDIAGONAL, gColor);
			break;
		case ID_CQ_CROSS:
			gColor = ShowColorDialog(hwnd);
			hBrush = CreateHatchBrush(HS_CROSS, gColor);
			break;
		case ID_CQ_DIAGCROSS:
			gColor = ShowColorDialog(hwnd);
			hBrush = CreateHatchBrush(HS_DIAGCROSS, gColor);
			break;
			//////////////////////////////////////////////////////////
		case ID_1:
			kt = 1;
			break;
		case ID_2:
			kt = 2;
			break;
		case ID_3:
			kt = 3;
			break;
		case ID_4:
			kt = 4;
			break;
		case ID_5:
			kt = 5;
			break;
		case ID_6:
			kt = 6;
			break;
		case ID_7:
			kt = 7;
			break;
		case ID_8:
			kt = 8;
			break;
		case ID_9:
			kt = 9;
			break;
		case ID_10:
			kt = 10;
			break;
			
		//////////////////////////////////////////////////////////////////
		case ID_PS_DOT:
			gColor = ShowColorDialog(hwnd);
			hPen = CreatePen(PS_DOT, kt, gColor);
			break;
		case ID_PS_DASH1:
			//CreateDialogBox(hwnd);
			gColor = ShowColorDialog(hwnd);
			hPen = CreatePen(PS_DASHDOT,kt, gColor);
			break;
		case ID_PS_SOLID:
			gColor = ShowColorDialog(hwnd);
		    hPen = CreatePen(PS_SOLID,kt, gColor);
			break;
		case ID_ERASER:
			hPen = CreatePen(PS_SOLID, 20, RGB(255, 255, 255));
			hinh = LOWORD(wParam);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
			
		case ID_NEW:
			SetClassLong(hwnd, GCLP_HBRBACKGROUND, (LONG)GetStockObject(WHITE_BRUSH));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		
		case ID_OPEN:
			OPENFILENAME ofn;
			char szFile[260];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = (LPWSTR)szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"All\0*.*\0Text\0*.txt\0PNG\0*.png\0";
			ofn.nFilterIndex = -1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ShellExecute(NULL, 0, L"c:\\outfile.txt", 0, 0, SW_SHOW);
			GetOpenFileName(&ofn);
			MessageBox(NULL, ofn.lpstrFile, L"File Name", MB_OK);
			
			break;
		case ID_SAVE:
			char szFileName[MAX_PATH] = "";

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0PNG (*.png)\0*.png\0";
			ofn.lpstrFile = (LPWSTR)szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = (LPCWSTR)L"txt";

			GetSaveFileName(&ofn);
			printf("the path is : %s\n", ofn.lpstrFile);
			getchar();
			break;
         ///////////////////////
		
		}
		break;
	


	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		BITMAP bitmap;
		HGDIOBJ oldBitmap;
		bm = (HBITMAP)LoadImage(NULL, L"E:/RazerChroma_1920x1080.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		HDC hdcMem;
		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, bm);
		GetObject(bm, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
		GetClientRect(hwnd, &rect);

		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	
	return DefWindowProc(hwnd, message, wParam, lParam);
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
//////////////////////////
