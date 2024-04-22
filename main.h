#include "nwpwin.h"
#include "nwpdlg.h"

class number_dialog : public vsite::nwp::dialog {
protected:
	int idd() const override;
	bool on_init_dialog() override;
	bool on_ok() override;

public:
	int number;
};

class main_window : public vsite::nwp::window {
public:
	int default_number = 6;
	COLORREF default_color = RGB(154, 0, 255);
	bool is_anisotropic = false;
protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
};
