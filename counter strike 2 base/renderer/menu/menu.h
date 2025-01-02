#pragma once
#include <d3d11.h>
#include <tchar.h>

class menu_t
{
public:
	void menu();

	bool window_opened = false;
	bool main_active = false;
private:
};

inline menu_t* menu = new menu_t;