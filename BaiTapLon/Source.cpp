
#include <windows.h>
#include "resource.h"
#include "helpers.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PanelProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK KichThuoc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK VeChu(HWND, UINT, WPARAM, LPARAM);
//void RegisterPanel(void);
//void CreateDialogBox(HWND);
//void RegisterDialogClass(HWND);

void MyPen(HWND);


COLORREF gColor = RGB(0, 0, 0);
COLORREF gColor1 = RGB(0, 0, 0);

static HMENU hMenu;
static HCURSOR cursor;
static HINSTANCE hInst;
static bool flag = true;
static HBITMAP hBitmap = NULL;

static wstring fileName;
static int val = 1;

HWND myhWnd;
HINSTANCE ghInstance;
//////////////
//////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//hihihihihihihihiihihihihi
	//hihu
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wndclass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_PEN1));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = szAppName;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	if (!RegisterClass(&wndclass)){
		MessageBox(NULL, TEXT("this program requires window nt!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName, TEXT("Ứng Dụng Paint"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	hInst = hInstance;
	//ShowWindow(hwnd, iCmdShow);
	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	myhWnd = hwnd;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.lParam;
}


//
// Stack lưu các hình đã vẽ

stack<HBITMAP> S;

//
// Các biến để vẽ hình
HDC hdc;
PAINTSTRUCT ps;
RECT rect;
static int hinh = ID_DUONGTHANG, style = PS_SOLID, n = 0, kt = val;
static int xStart, yStart, xEnd, yEnd;
static HBRUSH hBrush;
static POINT ptPoint[100000],apt_hs[NUM], apt_tg[3], apt_ht[4], apt_mt[7], apt_lg[6], apt_ng[5], apt_ns[12];
static HPEN hPen, hPen1;
static bool isNormal = true;

//
// Các biến để vẽ chữ 
HWND text = NULL;
HWND hStatic = NULL;
int tx = 0, ty = 0;
HFONT hFont;
int fontSize = 30;
TCHAR  ListItem[30];
TCHAR buffer[1 << 15];


//
// Hàm push ảnh vào stack;
void add(HWND hwnd) {
	hBitmap = GetCUrrentBM(hwnd);
	S.push(hBitmap);
	InvalidateRect(hwnd, NULL, TRUE);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT tmp;
	HBITMAP bm;
	TCHAR buffer[1 << 15];

	switch (message)
	{
	case WM_SIZE:

		break;
	case WM_CREATE:
		S.push(NULL);
		return 0;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		return (INT_PTR)(HBRUSH)GetStockObject(NULL_BRUSH);
	}
	case WM_CLOSE:
	
		if (MessageBox(hWnd, TEXT("Bạn có muốn thoát không?"), TEXT("Hỏi"), MB_YESNO | MB_ICONQUESTION) == IDYES)
			PostQuitMessage(0);
		return 0;
	//
	// Xử lý xự kiện click chuột phải để hiện menu pop up

	case WM_RBUTTONDOWN:
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENUPOPUP));
		POINT point;
		hMenu = GetSubMenu(hMenu, 0);
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		ClientToScreen(hWnd, &point);
		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
		break;

    case WM_MOUSEMOVE:
		
		//
		// Xự kiện di chuyển chuột -> vẽ bằng chuột
		// Nếu còn nhấn chuột thì sẽ lấy các tọa độ tương ứng và vẽ các đoạn thẳng rất nhỏ từ điểm trc đó đến điểm hiện tại
		// (n-1 -> n)

		if (hinh == ID_CHUOT){
			if (wParam & MK_LBUTTON)
			{
				ptPoint[n].x = LOWORD(lParam);
				ptPoint[n].y = HIWORD(lParam);
				hdc = GetDC(hWnd);
				SelectObject(hdc, hPen);
				if (n >= 1)
				{
					MoveToEx(hdc, ptPoint[n - 1].x, ptPoint[n - 1].y, NULL);
					LineTo(hdc, ptPoint[n].x, ptPoint[n].y);
				}
				n = n + 1;
				ReleaseDC(hWnd, hdc);
				break;
			}

		}
		
		//
		// Tương tự như vẽ bằng chuột nhưng sẽ vẽ ra màu trắng -> tẩy hình

		if (hinh == ID_ERASER){
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_ERASER));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			if (wParam & MK_LBUTTON)
			{
			ptPoint[n].x = LOWORD(lParam);
			ptPoint[n].y = HIWORD(lParam);
			hdc = GetDC(hWnd);
			SelectObject(hdc, hPen1);
			if (n >= 1)
			{


				MoveToEx(hdc, ptPoint[n - 1].x, ptPoint[n - 1].y, NULL);
				LineTo(hdc, ptPoint[n].x, ptPoint[n].y);
			}
			n = n + 1;
			ReleaseDC(hWnd, hdc);
			break;
		}
		}
		return 0;

	//
	// Xự kiện nhấn chuột -> lấy vị trí bắt đầu vẽ

	case WM_LBUTTONDOWN:
		
		xStart = LOWORD(lParam);
		yStart = HIWORD(lParam);

		break;
	
	//
	// Xự kiện thả chuột -> lấy vị trí kết thúc để xử lý các hình tương ứng

	case WM_LBUTTONUP:
		n = 0;
		xEnd = LOWORD(lParam);
		yEnd = HIWORD(lParam);
		hdc = GetDC(hWnd);
		SelectObject(hdc, hPen);
		
		// Nếu là vẽ bình thường thì không cho phép các hình đè nên nhau 
		// Ngược lại sẽ thì đc

		if(isNormal) SelectObject(hdc,GetStockObject(HOLLOW_BRUSH));
		else SelectObject(hdc, hBrush);

		// 
		// Phần xử lý tọa độ để vẽ các hình, xử dụng các hàm mà win32 cung cấp sẵn hoặc có thể tự tạo hình theo ý thích

		if (hinh == ID_HINHCHUNHAT1){
			Rectangle(hdc, xStart, yStart, xEnd, yEnd);
			add(hWnd);
		}

		if (hinh == ID_HINHCHUNHAT2){
			RoundRect(hdc, xStart, yStart, xEnd, yEnd,(xEnd-xStart)/20,(yEnd-yStart)/20);
			add(hWnd);

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
			add(hWnd);
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
			add(hWnd);
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
			add(hWnd);
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
			add(hWnd);
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
			add(hWnd);
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
			add(hWnd);

		}
		if (hinh == ID_ECLIPSE)
		{
			Ellipse(hdc, xStart, yStart, xEnd, yEnd);
			add(hWnd);
		}
		if (hinh == ID_DUONGTHANG){
			MoveToEx(hdc, xStart, yStart, NULL);
			LineTo(hdc, xEnd, yEnd);
			add(hWnd);
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
			add(hWnd);
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
			add(hWnd);
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
			add(hWnd);
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
			add(hWnd);
		}
		if (hinh == ID_HINHSIN)
		{
			for (int i = 0; i < NUM; i++)
			{
				apt_hs[i].x = xStart + i * (xEnd - xStart) / NUM;
				apt_hs[i].y = yStart + (int)((yEnd - yStart) / 2 * (1 - sin(TWOPI * i / NUM)));
			}

			Polyline(hdc, apt_hs, NUM);
			add(hWnd);
		}
		//InvalidateRect(hWnd, NULL, FALSE);
		if (hinh == ID_CHUOT || hinh == ID_ERASER) {
			add(hWnd);
		}
		
		ReleaseDC(hWnd, hdc);
		break;
	   
	
	case WM_COMMAND:
	
	// 
	// Phần xử lý chọn các hình vẽ
	// Lưu lại các ID của hình vào biến hinh

		switch (LOWORD(wParam))
		{
		case ID_DUONGTHANG:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_ECLIPSE:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_HINHCHUNHAT1:
            hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_HINHCHUNHAT2:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_TAMGIAC1:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_TAMGIAC2:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_HINHTHOI:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_HINHNGUGIAC:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_LUCGIAC:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_MUITENNGANG:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_MUITENDOC:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_SAO4CANH:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_SAO5CANH:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_SAO6CANH:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_CHUOT:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_HINHSIN:
			hinh = LOWORD(wParam);
			MyPen(hWnd);
			break;
		case ID_VECHU:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGVECHU), hWnd, VeChu);
			MyPen(hWnd);
			break;


	// Phần xử lý chổi quét
	////////////////////////////////////////
		case ID_CQ_SOLID:
			gColor1 = ShowColorDialog(hWnd);
			hBrush = CreateSolidBrush(gColor1);
			InvalidateRect(hWnd, NULL, FALSE);
			isNormal = false;
			break;
		case ID_CQ_HORIZONTAL:
			gColor1 = ShowColorDialog(hWnd);
			hBrush = CreateHatchBrush(HS_HORIZONTAL,gColor1);
			isNormal = false;
			break;
			
		case ID_CQ_VERTICAL:
			gColor1 = ShowColorDialog(hWnd);
			hBrush = CreateHatchBrush(HS_VERTICAL, gColor1);
			isNormal = false;
			break;
		case ID_CQ_FDIALGONAL:
		    gColor1 = ShowColorDialog(hWnd);
			hBrush = CreateHatchBrush(HS_FDIAGONAL, gColor1);
			isNormal = false;
			break;
		case ID_CQ_BDIAGONAL:
			gColor1 = ShowColorDialog(hWnd);
			hBrush = CreateHatchBrush(HS_BDIAGONAL, gColor1);
			isNormal = false;
			break;
		case ID_CQ_CROSS:
			gColor1 = ShowColorDialog(hWnd);
			hBrush = CreateHatchBrush(HS_CROSS, gColor1);
			isNormal = false;
			break;
		case ID_CQ_DIAGCROSS:
			gColor1 = ShowColorDialog(hWnd);
			hBrush = CreateHatchBrush(HS_DIAGCROSS, gColor);
			isNormal = false;
			break;
		case ID_NORMAL:
			isNormal = true;
			break;

	// Phần xử ký kích thước
	//////////////////////////////////////////////////////////
		case ID_1:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_PEN1));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 1, gColor);
			break;
		case ID_2:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_PEN1));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 2, gColor);
			break;
		case ID_3:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_PEN1));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 3, gColor);
			break;
		case ID_4:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_PEN1));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 4, gColor);
			break;
		case ID_5:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_PEN1));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 5, gColor);
			break;
		case ID_6:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CT));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 6, gColor);
			break;
		case ID_7:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CT));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 7, gColor);
			break;
		case ID_8:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CT));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 8, gColor);
			break;
		case ID_9:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CT));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 9, gColor);
			break;
		case ID_10:
			cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CT));
			SetCursor(cursor);
			SetClassLong(hWnd, -12, (DWORD)cursor);
			hPen = CreatePen(style, kt = 10, gColor);
			break;
	// Phần xử lý bút vẽ		
	//////////////////////////////////////////////////////////////////
		case ID_PS_DOT:
			gColor = ShowColorDialog(hWnd);
			hPen = CreatePen(style = PS_DOT, kt, gColor);
			hinh = hinh == ID_ERASER ? ID_DUONGTHANG : hinh;
			MyPen(hWnd);
			break;
		case ID_PS_DASH1:
			gColor = ShowColorDialog(hWnd);
			hPen = CreatePen(style = PS_DASHDOT,kt, gColor);
			hinh = hinh == ID_ERASER ? ID_DUONGTHANG : hinh;
			MyPen(hWnd);
			break;
		case ID_PS_SOLID:
			gColor = ShowColorDialog(hWnd);
			hPen = CreatePen(style = PS_SOLID, kt, gColor);
			hinh = hinh == ID_ERASER ? ID_DUONGTHANG : hinh;
			MyPen(hWnd);
			break;
		case ID_ERASER:
			//gColor = RGB(255, 255, 255);
			hPen1 = CreatePen(PS_SOLID,kt = 20, RGB(255, 255, 255));
			hinh = LOWORD(wParam);
			//InvalidateRect(hWnd, NULL, FALSE);
			break;
	// 
	// Menu item tùy chọn để thực hiện lấy ra 1 kích thước từ dialog

		case ID_TuyChon:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGKT), hWnd, KichThuoc);
			break;
	//
	// Xử lý xự kiện click chọn màu

		case ID_COL_RED:
			gColor = RGB(255, 0, 0);
			hPen = CreatePen(style, kt, gColor);
			break;
		case ID_COL_BLUE:
			gColor = RGB(0, 0, 255);
			hPen = CreatePen(style, kt, gColor);
			break;
		case ID_COL_GREEN:
			gColor = RGB(0,255,0);
			hPen = CreatePen(style, kt, gColor);
			break;
		case ID_COL_CUS:
			gColor = ShowColorDialog(hWnd);
			hPen = CreatePen(style, kt, gColor);
			break;
	
	//
	// Xử lý xự kiện menu item cho menu pop up

		case ID_FILE_COPY:
			GetWindowRect(hWnd, &tmp);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			bm = GetCUrrentBM(hWnd);
			OpenClipboard(0);
			EmptyClipboard();
			SetClipboardData(CF_BITMAP, bm);
			CloseClipboard();
			break;

		case ID_FILE_PASTE:
			OpenClipboard(0);
			hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
			CloseClipboard();
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case ID_FILE_SAVEPOPUP:
			fileName = SaveFileDialog();
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			RECT tmp;
			GetWindowRect(hWnd, &tmp);
			screenCapturePart(tmp.left + 8, tmp.top + 51.5, tmp.right - tmp.left - 20, tmp.bottom - tmp.top - 70, fileName.c_str());
			break;
		
		case ID_FILE_UNDO:
			if(S.size()>1){
				S.pop();
				hBitmap = S.top();
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		
	//
	// Tạo mới màn hình bằng các đổ nền màu trắng

		case ID_NEW:
			hBitmap = NULL;
			SetClassLong(hWnd, GCLP_HBRBACKGROUND, (LONG)GetStockObject(WHITE_BRUSH));
			InvalidateRect(hWnd, NULL, TRUE);
			break;
	
	//
	// Mở 1 file bitmap và set nó làm hình nền cho ứng 
	// Hàm xử lý mở file trong file helpers.h

		case ID_OPEN:
			fileName = OpenFileDialog();
			hBitmap = (HBITMAP)LoadImage(hInst, fileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
	// 
	// Mở 1 file để tạo 1 file cần save sau đó đổ ảnh bitmap từ màn hình vào file đó
	// Sử dụng cách chụp màn hình để lấy ảnh bitmap
	// Hàm xử lý nằm trong file helpers.h

		case ID_SAVE:
			fileName = SaveFileDialog();
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			GetWindowRect(hWnd, &tmp);
			screenCapturePart(tmp.left + 8, tmp.top + 51.5, tmp.right - tmp.left - 20, tmp.bottom - tmp.top - 70, fileName.c_str());
			break;
		
         ///////////////////////
		
		}
		break;
	


	case WM_PAINT:

	//
	// Xự kiện vẽ của chương trình nó sẽ được gọi lại khi gặp hàm InvalidateRect(hWnd, NULL, TRUE);
	// Mục đích để vẽ lại khi mở một ảnh mới -> tạo hình nền

		PAINTSTRUCT     ps;
		HDC             hdc;
		BITMAP          bitmap;
		HDC             hdcMem;
		HGDIOBJ         oldBitmap;

		hdc = BeginPaint(hWnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmap);

		GetObject(hBitmap, sizeof(bitmap), &bitmap);
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	}
	
	//if (hinh != ID_ERASER) MyPen(hWnd);

	return DefWindowProc(hWnd, message, wParam, lParam);
}


//
// Xử lý phần dialog kích thước để lấy ra kích thược tùy ý
// Các điều khiển : Slider và một button để xác nhận

INT_PTR CALLBACK KichThuoc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND cb;
	HWND sl;
	int tmp;
	int dwPos = 0;
	switch (message)
	{
	case WM_INITDIALOG:
		
		// 
		// Set dữ liệu cho slider tương ứng với range từ 0 -> 100
		// Ban đầu sẽ có giá trị tương ứng với kích thước của bút vẽ hiện tại
		
		sl = GetDlgItem(hDlg, IDC_SLIDERKT);
		SendMessage(sl, TBM_SETRANGE, (WPARAM)1, (LPARAM)MAKELONG(0, 100));
		SendMessage(sl, TBM_SETPOS, (WPARAM)1, kt);
		SetDlgItemText(hDlg, IDC_NUM, to_wstring(kt).c_str());

		return (INT_PTR)TRUE;
	case WM_HSCROLL:

		// Update giá trị cho static text ứng với value của slider khi ta kéo chuột

		dwPos = SendMessage(GetDlgItem(hDlg, IDC_SLIDERKT), TBM_GETPOS, 0, 0);
		SetDlgItemText(hDlg, IDC_NUM, to_wstring(dwPos).c_str());
		break;
	case WM_COMMAND:
		int id = LOWORD(wParam);
		if (id == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));				// Kết thức dialog khi bấn ok
			return (INT_PTR)TRUE;
		}
		if (id == BTN_OK) {

			kt = SendMessage(GetDlgItem(hDlg, IDC_SLIDERKT), TBM_GETPOS, 0, 0);			// Set kích thước ứng với giá trị của slider đã chọn
			hPen = CreatePen(PS_SOLID, kt, gColor);
			//MessageBox(NULL, to_wstring(val).c_str(), L"TEST", MB_OK);
			EndDialog(hDlg, LOWORD(wParam));				// Kết thức dialog khi bấn ok
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


//
//

INT_PTR CALLBACK VeChu(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND txEdit;
	HWND cbFont;
	TCHAR data[2][20] = { TEXT("Arial"), TEXT("Times New Roman") };
	switch (message)
	{
	case WM_INITDIALOG:

		// 
		// Set dữ liệu cho combo box tương ứng với tên font chữ trong mảng
		cbFont = GetDlgItem(hDlg, IDC_COMBO1);
		TCHAR ITEM[16];
		memset(&ITEM, 0, sizeof(ITEM));
		for (int i = 0; i < 2; i++) {
			wcscpy_s(ITEM, sizeof(ITEM) / sizeof(TCHAR), (TCHAR*)data[i]);
			SendMessage(cbFont, CB_ADDSTRING, (WPARAM)0, (LPARAM)ITEM);
		}
		SendMessage(cbFont, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

		return (INT_PTR)TRUE;
	case WM_COMMAND:
		int id = LOWORD(wParam);
		if (id == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (id == IDOK) {
			txEdit = GetDlgItem(hDlg, IDC_EDIT1);
			GetWindowText(txEdit, buffer, 1 << 15);

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,(WPARAM)0, (LPARAM)0);
			(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,(WPARAM)ItemIndex, (LPARAM)ListItem);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


// 
// Hàm xử lý hình cho con trỏ ứng với kích thước to nhỏ khác nhau
void MyPen(HWND hWnd) {
	int id = kt < 6 ? IDC_PEN1 : IDC_CT;
	cursor = LoadCursor(hInst, MAKEINTRESOURCE(id));
	SetCursor(cursor);
	SetClassLong(hWnd, -12, (DWORD)cursor);
}



