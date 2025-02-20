#include "../include/windows/win32_window.hpp"

LRESULT mdge::Process(HWND hWind, UINT message, WPARAM wParam, LPARAM lParam) {
	mdge::win32_window *pThis;
	if (message == WM_CREATE) {
		CREATESTRUCT *pCreate = (CREATESTRUCT *) lParam;
		pThis = (mdge::win32_window *) pCreate->lpCreateParams;
		SetWindowLongPtr(hWind, GWLP_USERDATA, (LONG_PTR) pThis);
	} else {
		pThis = (mdge::win32_window *) GetWindowLongPtr(hWind, GWLP_USERDATA);
	}

	if (pThis) {
		return pThis->Process(message, wParam, lParam);
	} else {
		return DefWindowProc(hWind, message, wParam, lParam);
	}
}

void mdge::win32_window::Create(const CreateInfo &createInfo) {
	WNDCLASSEX windowClass{};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = (createInfo.fpWindowProcess == nullptr) ? mdge::Process : createInfo.fpWindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = createInfo.icon;
	windowClass.hCursor = LoadCursorA(0, IDC_ARROW);
	windowClass.hbrBackground = CreateSolidBrush(RGB(createInfo.color.r(), createInfo.color.g(), createInfo.color.b()));
	windowClass.lpszMenuName = nullptr;
	std::string className = createInfo.name + "-class";
	windowClass.lpszClassName = className.c_str();
	windowClass.hIconSm = nullptr;
	RegisterClassEx(&windowClass);

	mdm::uvec2_s size = GetWindowAdjustedSize(createInfo.size, createInfo.style, createInfo.style & WS_SYSMENU);

	m_window = CreateWindowEx(0, className.c_str(), createInfo.name.c_str(), createInfo.style, createInfo.position.x(),
							  createInfo.position.y(), size.w(), size.h(), 0, 0, GetModuleHandleA(0), this);
	if (!m_window) {
		throw std::runtime_error("Failed to create window");
		//TODO: Error handling
	}
	ShowWindow(m_window, SW_SHOW);
}

void mdge::win32_window::Update() const {
	MSG msg = {};
	while (PeekMessage(&msg, m_window, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void mdge::win32_window::WaitUpdate() const {
	MSG msg = {};
	if (GetMessage(&msg, m_window, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void mdge::win32_window::Destroy() { PostQuitMessage(0); }

LRESULT mdge::win32_window::Process(UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(m_window, message, wParam, lParam);
}

mdm::uvec2_s mdge::win32_window::GetSize() const {
	RECT windowSize;
	if (GetWindowRect(m_window, &windowSize)) {
		return {unsigned int(windowSize.right - windowSize.left), unsigned int(windowSize.bottom - windowSize.top)};
	}
	return {0, 0};
}

mdm::uvec2_s mdge::win32_window::GetClientSize() const {
	RECT windowSize;
	if (GetClientRect(m_window, &windowSize)) {
		return {unsigned int(windowSize.right - windowSize.left), unsigned int(windowSize.bottom - windowSize.top)};
	}
	return {0, 0};
}

void mdge::win32_window::SetSize(const mdm::uvec2_s &size) const {
	SetWindowPos(m_window, HWND_TOP, 0, 0, size.w(), size.h(), SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOACTIVATE);
}

mdm::uvec2_s mdge::win32_window::GetPosition() const {
	RECT rect{};
	if (GetWindowRect(m_window, &rect)) { return {unsigned int(rect.left), unsigned int(rect.top)}; }
	return {0, 0};
}

void mdge::win32_window::SetPosition(const mdm::uvec2_s position) const {
	SetWindowPos(m_window, HWND_TOP, position.x(), position.y(), 0, 0, SWP_NOSIZE | SWP_NOREPOSITION | SWP_NOACTIVATE);
}

void mdge::win32_window::SetVisible(bool visible) const {
	if (visible) {
		ShowWindow(m_window, SW_SHOW);
	} else {
		ShowWindow(m_window, SW_HIDE);
	}
}

void mdge::win32_window::SetCursorVisible(bool visible) { ShowCursor(visible); }

void mdge::win32_window::SetWindowCursor(Cursor cursor) { SetCursor(mdge::GetDefaultCursor(cursor)); }

void mdge::win32_window::SetTitle(const std::string &title) const { SetWindowTextA(m_window, title.c_str()); }

bool mdge::win32_window::HasFocus() const {
	if (GetFocus() == m_window) {
		return true;
	} else {
		return false;
	}
}

bool mdge::win32_window::Flash() const {
	FLASHWINFO flashInfo{};
	flashInfo.cbSize = sizeof(FLASHWINFO);
	flashInfo.hwnd = m_window;
	flashInfo.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG;
	return FlashWindowEx(&flashInfo);
}

mdm::uvec2_s mdge::win32_window::GetWindowAdjustedSize(const mdm::uvec2_s &size, DWORD style, bool hasMenuBar) {
	RECT rect{};
	rect.left = 0;
	rect.right = size.w();
	rect.top = 0;
	rect.bottom = size.h();
	if (AdjustWindowRect(&rect, style, hasMenuBar)) {
		return {unsigned int(rect.right - rect.left), unsigned int(rect.bottom - rect.top)};
	}
	return size;
}
