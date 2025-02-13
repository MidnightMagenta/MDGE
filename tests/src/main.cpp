#include <iostream>
#include <windows/win32_window.hpp>

int main() { 
	mdge::win32_window window;
	mdge::win32_window::win32_window_createInfo createInfo{};
	createInfo.size = {800, 800};
	createInfo.name = "Test window";
	createInfo.style = WS_OVERLAPPEDWINDOW;
	createInfo.color = {255, 255, 255};
	window.Create(createInfo);
	while (window.IsOpen()) { 
		window.Update(); 
	}
}