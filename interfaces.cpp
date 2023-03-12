// interfaces.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "interfaces.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int myCharSize = 32;
int myDistanceBetweenStrings = 8;
int myStringLength = 800;
COLORREF myTextColor = RGB(0, 0, 0);
WCHAR textBuffer[1000];
int carretPosition = 0;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_INTERFACES, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INTERFACES));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INTERFACES));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_INTERFACES);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, About);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_RBUTTONDOWN:
	{
		carretPosition = 0;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		int x = 0, y = 0, text_height;

		HFONT hfont = CreateFont(myCharSize, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
		// set text color and font
		COLORREF oldTextColor = SetTextColor(hdc, myTextColor);
		HFONT oldHFont = (HFONT)SelectObject(hdc, hfont);
		SetTextColor(hdc, myTextColor);
		SetBkMode(hdc, TRANSPARENT);

		SIZE psize{};
		if (GetTextExtentPoint32(hdc, _T("Sample"), static_cast<int>(_tclen(_T("Sample"))), &psize) == FALSE) //text size (w + h)
			throw NULL;
		text_height = psize.cy;

		int mX = LOWORD(lParam);
		int mY = HIWORD(lParam);

		for (int i = 0; i < wcslen(textBuffer); i++)
		{
			if (textBuffer[i] == '\r') //if enter
			{
				x = 0;
				y += text_height + myDistanceBetweenStrings;
			}
			else {
				GetTextExtentPoint32(hdc, textBuffer + i, 1, &psize);
				if ((x + psize.cx) > myStringLength) {
					x = 0;
					y += text_height + myDistanceBetweenStrings;
				}
				x += psize.cx;
			}
			if (y + text_height + myDistanceBetweenStrings > mY)
			{
				if (x > mX)
				{
					carretPosition = i + 1;
					break;
				}
			}
		}
		EndPaint(hWnd, &ps);

		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_BACK:
		{
			if ((_tcslen(textBuffer) > 0) && (carretPosition > 0)) // shift all the data after carret by 1
			{
				--carretPosition;
				wmemcpy(textBuffer + carretPosition, textBuffer + carretPosition + 1, _tcslen(textBuffer) - carretPosition + 1);
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		case VK_LEFT:
		{
			if (carretPosition > 0)
				carretPosition--;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		case VK_RIGHT:
		{
			if (carretPosition < wcslen(textBuffer))
				carretPosition++;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		}
	}
	break;
	case WM_CHAR:
	{
		switch (wParam)
		{
		case 0x08: // \b
		case 0x09: // \t
		case 0x0A: // \n
		case 0x1B: // ESC
			break;

		default:
			if (carretPosition < _tcslen(textBuffer))
				wmemmove(textBuffer + carretPosition + 1, textBuffer + carretPosition, _tcslen(textBuffer) - carretPosition); //move carret by 1 rightside

			textBuffer[carretPosition] = (TCHAR)wParam;

			carretPosition += 1;

			InvalidateRect(hWnd, NULL, TRUE); //redraw window
			break;
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		int x = 0, y = 0, text_height;

		HFONT hfont = CreateFont(myCharSize, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
		// set text color and font
		HFONT oldHFont = (HFONT)SelectObject(hdc, hfont);
		SetTextColor(hdc, myTextColor);
		SetBkMode(hdc, TRANSPARENT);

		SIZE psize{};
		if (GetTextExtentPoint32(hdc, _T("Sample"), static_cast<int>(_tclen(_T("Sample"))), &psize) == FALSE) //text size (w + h)
			throw NULL;
		text_height = psize.cy;

		Rectangle(hdc, 0, 0, myStringLength, 1000);
		for (int i = 0; i < wcslen(textBuffer); i++)
		{
			if (textBuffer[i] == '\r') //if enter
			{
				x = 0;
				y += text_height + myDistanceBetweenStrings;
			}
			else {
				GetTextExtentPoint32(hdc, textBuffer + i, 1, &psize);
				if ((x + psize.cx) > myStringLength) {
					x = 0;
					y += text_height + myDistanceBetweenStrings;
				}
				TextOut(hdc, x, y, textBuffer + i, 1);
				x += psize.cx;
			}
			if (i + 1 == carretPosition)
			{
				MoveToEx(hdc, x, y, NULL);
				LineTo(hdc, x, y + text_height);
			}
		}
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK)
			{
				myCharSize = GetDlgItemInt(hDlg, my_char_size, NULL, false);
				myDistanceBetweenStrings = GetDlgItemInt(hDlg, my_distance_between_strings, NULL, false);
				myStringLength = GetDlgItemInt(hDlg, my_string_length, NULL, false);
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == my_change_text_color) {
			CHOOSECOLOR cc;
			COLORREF acrCustClr[16]{};

			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hDlg;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor(&cc) == TRUE)
			{
				myTextColor = cc.rgbResult;
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}
