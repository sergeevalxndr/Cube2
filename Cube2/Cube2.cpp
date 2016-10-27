#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include <string>
#include <sstream>
#include "CubeClass.h"
#include "resource1.h"

#define cubsd 300
#define sizex 600

std::string fts(int a) {
	std::stringstream ss;
	ss << a;
	return ss.str();
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");

int APIENTRY _tWinMain(HINSTANCE This,	HINSTANCE Prev,	LPTSTR cmd,	int mode)                         // Режим отображения окна
{
	HWND hWnd;              // Дескриптор главного окна программы
	MSG msg;                // Структура для хранения сообщения
	WNDCLASSEX wc;  // Класс окна
				  // Определение класса окна
	wc.hInstance = This;
	wc.lpszClassName = WinName;                // Имя класса окна
	wc.lpfnWndProc = WndProc;                  // Функция окна
	wc.style = CS_HREDRAW | CS_VREDRAW;       // Стиль окна
	wc.hIcon = LoadIcon(wc.hInstance, _T("IDI_ICON1"));
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0);
	wc.hCursor = LoadCursor(wc.hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wc.lpszMenuName = NULL;       // Нет меню
	wc.cbClsExtra = 0;            // Нет дополнительных данных класса
	wc.cbWndExtra = 0;            // Нет дополнительных данных окна
								  // Заполнение окна белым цветом
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.cbSize = sizeof(wc);
	if (!RegisterClassEx(&wc)) return 0;   // Регистрация класса окна
										 // Создание окна
	hWnd = CreateWindow(WinName, // Имя класса окна
		_T("Вращающийся куб"),  // Заголовок окна
		WS_OVERLAPPED | WS_CAPTION |
		WS_SYSMENU | WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX,         // Стиль окна
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y   Размеры окна
		sizex, // width
		sizex, // Height
		HWND_DESKTOP, // Дескриптор родительского окна
		NULL,
		This,         // Дескриптор приложения
		NULL);
	ShowWindow(hWnd, mode); // Показать окно
							// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg);  // Посылает сообщение функции WndProc()
	}
	return 0;
}


HRGN rgn = CreateRectRgn(140, 140, 500, 500);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int sx = sizex, sy = sizex, rspeed = 100, mcrdx, mcrdy;
	static bool status1 = true, status2 = true, color = false, lbpressed = false, akson = false;
	HDC hdc;
	PAINTSTRUCT ps;
	static Cube cube;
	static HWND buttonx, buttony, bsplus, bsminus, reset, turncolor, curspeed, switchpv, about;
	static HBRUSH hbrush[6];
	HINSTANCE hInst;
	switch (message)
	{
	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		buttonx = CreateWindow(_T("button"), _T("Rotate OX"), WS_CHILD | WS_VISIBLE | WS_BORDER, 370, 40, 80, 20, hWnd, 0, hInst, NULL);
		ShowWindow(buttonx, SW_SHOWNORMAL);
		buttony = CreateWindow(_T("button"), _T("Rotate OY"), WS_CHILD | WS_VISIBLE | WS_BORDER, 470, 40, 80, 20, hWnd, 0, hInst, NULL);
		ShowWindow(buttony, SW_SHOWNORMAL);
		bsplus = CreateWindow(_T("button"), _T("Speed +"), WS_CHILD | WS_VISIBLE | WS_BORDER, 370, 80, 80, 20, hWnd, 0, hInst, NULL);
		ShowWindow(bsplus, SW_SHOWNORMAL);
		bsminus = CreateWindow(_T("button"), _T("Speed -"), WS_CHILD | WS_VISIBLE | WS_BORDER, 470, 80, 80, 20, hWnd, 0, hInst, NULL);
		ShowWindow(bsminus, SW_SHOWNORMAL);
		reset = CreateWindow(_T("button"), _T("Reset default status"), WS_CHILD | WS_VISIBLE | WS_BORDER, 380, 120, 150, 20, hWnd, 0, hInst, NULL);
		ShowWindow(reset, SW_SHOWNORMAL);
		turncolor = CreateWindow(_T("button"), _T("Color Cube"), WS_CHILD | WS_VISIBLE | WS_BORDER, 260, 40, 90, 20, hWnd, 0, hInst, NULL);
		ShowWindow(turncolor, SW_SHOWNORMAL);
		curspeed = CreateWindow(_T("static"), _T("Rotate Delay: 100ms"), WS_CHILD | WS_VISIBLE, 220, 80, 140, 20, hWnd, 0, hInst, NULL);
		ShowWindow(curspeed, SW_SHOWNORMAL);
		switchpv = CreateWindow(_T("button"), _T("Switch POV"), WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 40, 80, 20, hWnd, 0, hInst, NULL);
		ShowWindow(switchpv, SW_SHOWNORMAL);
		about = CreateWindow(_T("button"), _T("About"), WS_CHILD | WS_VISIBLE | WS_BORDER, 500, 520, 60, 20, hWnd, 0, hInst, NULL);
		ShowWindow(about, SW_SHOWNORMAL);
		hbrush[0] = CreateSolidBrush(RGB(255, 255, 0));
		hbrush[1] = CreateSolidBrush(RGB(255, 125, 0));
		hbrush[2] = CreateSolidBrush(RGB(255, 0, 125));
		hbrush[3] = CreateSolidBrush(RGB(0, 255, 125));
		hbrush[4] = CreateSolidBrush(RGB(125, 0, 255));
		hbrush[5] = CreateSolidBrush(RGB(0, 125, 255));
		break;
	case WM_COMMAND:
		if (lParam == (LPARAM)buttonx) {
			if (status1) {
				SetTimer(hWnd, 1, rspeed, NULL);
				status1 = false;
				SetWindowText((HWND)lParam, "Stop");
			}
			else {
				KillTimer(hWnd, 1);
				status1 = true;
				SetWindowText((HWND)lParam, "Rotate OX");
			}
		}
		if (lParam == (LPARAM)buttony) {
			if (status2) {
				SetTimer(hWnd, 2, rspeed, NULL);
				status2 = false; 
				SetWindowText((HWND)lParam, "Stop");
			}
			else {
				KillTimer(hWnd, 2);
				status2 = true;
				SetWindowText((HWND)lParam, "Rotate OY");
			}
		}
		if (lParam == (LPARAM)bsplus) {
			if (rspeed > 20) rspeed -= 20;
			std::string field;
			field = "Rotate Delay:" + fts(rspeed) + "ms";
			SetWindowText((HWND)((LPARAM)curspeed), field.c_str());
			if (!status1) {
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, rspeed, NULL);
			}
			if (!status2) {
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 2, rspeed, NULL);
			}
		}
		if (lParam == (LPARAM)bsminus) {
			rspeed += 20;
			std::string field;
			field = "Rotate Delay:" + fts(rspeed) + "ms";
			SetWindowText((HWND)((LPARAM)curspeed), field.c_str());
			if (!status1) {
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, rspeed, NULL);
			}
			if (!status2) {
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 2, rspeed, NULL);
			}
		}
		if (lParam == (LPARAM)reset) {
			if (!status1) {
				KillTimer(hWnd, 1);
				SetWindowText((HWND)((LPARAM)buttonx), "Rotate OX");
				status1 = true;
			}
			if (!status2) {
				KillTimer(hWnd, 2); 
				SetWindowText((HWND)((LPARAM)buttony), "Rotate OY");
				status2 = true;
			}
			cube.reset();
			InvalidateRgn(hWnd, rgn, true);
		}
		if (lParam == (LPARAM)turncolor) {
			if (color == false) {
				if (akson == false) {
					color = true;
					SetWindowText((HWND)lParam, "Frame Cube");
				}
				else MessageBox(hWnd, "Представление аксонометрической проекции в цвете временно не поддерживается в данной версии программы. \r\n", "Предупреждение!", NULL);
			} 
			else {
				color = false;
				SetWindowText((HWND)lParam, "Color Cube");
			}
			InvalidateRgn(hWnd, rgn, true);
		}
		if (lParam == (LPARAM)switchpv) {
			if (color == false) {
				cube.switchpv();
				akson == false ? akson = true : akson = false;
				InvalidateRgn(hWnd, rgn, true);
			}
			else MessageBox(hWnd, "Представление аксонометрической проекции в цвете временно не поддерживается в данной версии программы. \r\n", "Предупреждение!", NULL);
			}
		if (lParam == (LPARAM)about) {
			MessageBox(hWnd, "Разработчик: Сергеев Александр \r\nГруппа: И942\r\nКурс: 2\r\nОписание:    "
				"Данная программа представляет собой модель куба, нарисованную без использования графических и 3D библиотек. Куб представлен в двух моделях: пространственная "
				"и аксонометрическая. Обе модели можно вращать по двум координатным осям при помощи клавиш-стрелок, мыши или автоматически, при помощи специальных кнопок в окне программы."
				" Скорость вращения также можно регулировать. "
				"Куб имеет две формы представления: каркас и в цвете. Цветное представление пока что доступно только для пространственной модели."
				"", "Об этой программе", NULL);
		}
		SetFocus(hWnd);
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			cube.rotatex(-1);
			InvalidateRgn(hWnd, rgn, true);
			break;
		case 2:
			cube.rotatey(1);
			InvalidateRgn(hWnd, rgn, true);
			break;
		}
		break;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {
			float k = (mcrdx - LOWORD(lParam));
			cube.rotatex(k/10);
			InvalidateRgn(hWnd, rgn, true);
			k = (mcrdy - HIWORD(lParam));
			cube.rotatey(k/10);
			InvalidateRgn(hWnd, rgn, true);
		}
		break;
	case WM_LBUTTONDOWN:
		mcrdx = LOWORD(lParam);
		mcrdy = HIWORD(lParam);
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			cube.rotatex(1);
			InvalidateRgn(hWnd, rgn, true);
			break;
		case VK_RIGHT:
			cube.rotatex(-1);
			InvalidateRgn(hWnd, rgn, true);
			break;
		case VK_UP:
			cube.rotatey(-1);
			InvalidateRgn(hWnd, rgn, true);
			break;
		case VK_DOWN:
			cube.rotatey(1);
			InvalidateRgn(hWnd, rgn, true);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, sizex, sizex, NULL);
		SetViewportExtEx(hdc, sx, -sy, NULL);
		SetViewportOrgEx(hdc, 0, sy, NULL);
		cube.recalc();
		if (color == false) {
			for (int i = 0; i < 6; i++) {
				int id = cube.getidside(i, 0);
				MoveToEx(hdc, cube.crdxy[id].x + cubsd, cube.crdxy[id].y + cubsd, NULL);
				for (int j = 0; j < 4; j++) {
					int id2 = cube.getidside(i, j);
					int cx = static_cast<int>(cube.crdxy[id2].x);
					int cy = static_cast<int>(cube.crdxy[id2].y);
					LineTo(hdc, cx + cubsd, cy + cubsd);
				}
				int cx = static_cast<int>(cube.crdxy[id].x);
				int cy = static_cast<int>(cube.crdxy[id].y);
				LineTo(hdc, cx + cubsd, cy + cubsd);
			}
		}
		else {
			for (int i = 0; i < 6; i++) {
				int x[4], y[4];
				bool draw = cube.findnrm(i);
				for (int j = 0; j < 4; j++) {
					int id2 = cube.getidside(i, j);
					x[j] = static_cast<int>(cube.crdxy[id2].x) + cubsd;
					y[j] = static_cast<int>(cube.crdxy[id2].y) + cubsd;
				}
				if (draw) {
					POINT pt[4] = { { x[0], y[0] },{ x[1], y[1] },{ x[2], y[2] },{ x[3], y[3] } };
					BeginPath(hdc);
					Polyline(hdc, pt, 4);
					CloseFigure(hdc);
					EndPath(hdc);
					SelectObject(hdc, hbrush[i]);
					SetPolyFillMode(hdc, WINDING);
					FillPath(hdc);
				}
			}
		}
		MoveToEx(hdc, 100, 500, NULL);
		LineTo(hdc, 100, 550);
		MoveToEx(hdc, 100, 500, NULL);
		LineTo(hdc, 150, 500);
		MoveToEx(hdc, 100, 500, NULL);
		LineTo(hdc, 100 - 50*cos(M_PI_4), 500 - 50*sin(M_PI_4));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}