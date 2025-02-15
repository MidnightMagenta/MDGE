#include <iostream>
#include <../include/vulkan/vk_context.hpp>
#include <windows/win32_window.hpp>
#include <limits>

int main() { 
	mdge::win32_window window;
	mdge::win32_window::win32_window_createInfo createInfo{};
	createInfo.size = {800, 800};
	createInfo.name = "Test window";
	createInfo.style = WS_OVERLAPPEDWINDOW;
	createInfo.color = {255, 255, 255};
	window.Create(createInfo);

	mdge::vk::Context::s_createInfo contextInfo{};
	contextInfo.pWindow = &window;
	mdge::vk::Context context;
	context.Create(&contextInfo);

	while (window.IsOpen()) { 
		window.Update(); 
	}
}