#ifndef MDGE_WIN32_WINDOW_H
#define MDGE_WIN32_WINDOW_H

#define NOMINMAN
#include <MidnightMath/include/md_math.hpp>
#include <Windows.h>
#include <stdexcept>
#include <string>

#include "../include/windows/win32_cursor.hpp"

namespace mdge {
enum WindowStyle {
	DEFAULT = WS_OVERLAPPEDWINDOW,
	DEFAULT_NON_RESIZABLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	THIN_BORDER = WS_BORDER,
	TITLE_BAR = WS_CAPTION,
	CHILD = WS_CHILD,
	CLIP_CHILDREN = WS_CLIPCHILDREN,
	CLIP_SIBLINGS = WS_CLIPSIBLINGS,
	DISABLED = WS_DISABLED,
	DIALOG_BORDER = WS_DLGFRAME,
	GROUP = WS_GROUP,
	HORIZONTAL_SCROLL_BAR = WS_HSCROLL,
	VERTICAL_SCROLL_BAR = WS_VSCROLL,
	MINIMIZED = WS_MINIMIZE,
	MINIMIZE_BOX = WS_MINIMIZEBOX,
	MAXIMIZED = WS_MAXIMIZE,
	MAXIMIZE_BOX = WS_MAXIMIZEBOX,
	OVERLAPPED_WINDOW = WS_OVERLAPPED,
	POPUP = WS_POPUP,
	RESIZABLE = WS_SIZEBOX,
	MENU_BAR = WS_SYSMENU,
	VISIBLE = WS_VISIBLE,
};

LRESULT CALLBACK Process(HWND hWind, UINT message, WPARAM wParam, LPARAM lParam);

class win32_window {
public:
	struct win32_window_createInfo {
		mdm::uvec2_s size = {800, 800};
		std::string name = "Window";
		DWORD style = WS_OVERLAPPEDWINDOW;
		mdm::rgb_24 color = {255, 255, 255};
	};

	win32_window() {}
	win32_window(const win32_window_createInfo &createInfo) { Create(createInfo); }
	~win32_window() {}

	void Create(const win32_window_createInfo &createInfo);
	void Update();
	void WaitUpdate();
	void Destroy();
	LRESULT Process(UINT message, WPARAM wParam, LPARAM lParam);

	bool IsOpen() const { return IsWindow(m_window); }
	HWND GetNativeWindowHandle() const { return m_window; }

	mdm::uvec2_s GetSize();
	void SetSize(const mdm::uvec2_s &position);

	mdm::uvec2_s GetPosition();
	void SetPosition(const mdm::uvec2_s position);

	void SetVisible(bool visible);

	void SetCursorVisible(bool visible);
	void SetWindowCursor(Cursor cursor);

private:
	mdm::uvec2_s GetWindowAdjustedSize(const mdm::uvec2_s &size, DWORD style);

	HWND m_window = nullptr;
};
}// namespace mdge

#endif// !MDGE_WIN32_WINDOW_H
