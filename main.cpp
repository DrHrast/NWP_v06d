#include "main.h"
#include <cmath>
#include <math.h>
#include "rc.h"
#include "resource.h"
#include <numbers>


static COLORREF custCols[16];

int number_dialog::idd() const {
	return IDD_NUMBER;
}
bool number_dialog::on_init_dialog() {
	set_int(IDC_EDIT1, number);
	return true;
}
bool number_dialog::on_ok() {
	try {
		number = get_int(IDC_EDIT1);
	}
	catch (...){
		return false;
	}
	return true;
}

void main_window::on_paint(HDC hdc) {
	RECT rc;
	::GetClientRect(*this, &rc);
	int r = rc.bottom / 4;
	HBRUSH brush = CreateSolidBrush(default_color);
	HGDIOBJ selected_brush = SelectObject(hdc, brush);
	HPEN pen = CreatePen(PS_SOLID, 1, default_color);
	HGDIOBJ selected_pen = SelectObject(hdc, pen);

	SetROP2(hdc, R2_NOTXORPEN);
	if (is_anisotropic) {
		SetMapMode(hdc, MM_ANISOTROPIC);
		SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
		SetWindowExtEx(hdc, rc.bottom, rc.bottom, NULL);
//		SetViewportOrgEx(hdc, rc.right / 2, rc.bottom / 2, 0);
		for (int i = 0; i < default_number; ++i) {
			double a = 2 * std::numbers::pi * i / default_number;
			double x = rc.bottom / 2 + r * cos(a);
			double y = rc.bottom / 2 + r * sin(a);
			::Ellipse(hdc, x + r, y + r, x - r, y - r);
		}

	}
	else {
		for (int i = 0; i < default_number; ++i) {
			double a = 2 * std::numbers::pi * i / default_number;
			double x = rc.right / 2 + r * cos(a);
			double y = rc.bottom / 2 + r * sin(a);
			::Ellipse(hdc, x + r, y + r, x - r, y - r);
		}
	}
	DeleteObject(selected_brush);
	DeleteObject(selected_pen);
}

void main_window::on_command(int id){
	switch(id){
		case ID_COLOR: {
			CHOOSECOLOR cc;
			ZeroMemory(&cc, sizeof cc);
			cc.lStructSize = sizeof cc;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			cc.lpCustColors = custCols;
			cc.rgbResult = default_color;
			cc.hwndOwner = *this;
			if (ChooseColor(&cc))
				default_color = cc.rgbResult;
			::InvalidateRect(*this, 0, true);
			break;
		}
		case ID_NUMBER: {
			number_dialog nd;
			nd.number = default_number;
			if (nd.do_modal(0, *this) == IDOK) {
				default_number = nd.number;
				::InvalidateRect(*this, 0, true);
			}
			break;
		}
		case ID_FILE_ANISOTROPIC: {
			is_anisotropic = !is_anisotropic;
			InvalidateRect(*this, nullptr, true);
			::CheckMenuItem(::GetMenu(*this), ID_FILE_ANISOTROPIC, is_anisotropic ? MF_CHECKED : MF_UNCHECKED);
			::InvalidateRect(*this, 0, true);
			break;
		}
		case ID_EXIT: 
			DestroyWindow(*this); 
			break;
	}
}

void main_window::on_destroy(){
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	vsite::nwp::application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (int)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
