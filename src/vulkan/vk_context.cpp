#include "../include/vulkan/vk_context.hpp"

mdge::vk::Context::~Context() {
	if (m_allocator != VK_NULL_HANDLE) {
		vmaDestroyAllocator(m_allocator);
		m_allocator = VK_NULL_HANDLE;
	}
	if (m_device != VK_NULL_HANDLE) {
		vkDestroyDevice(m_device, nullptr);
		m_device = VK_NULL_HANDLE;
	}
	if (m_surface != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		m_surface = VK_NULL_HANDLE;
	}
#ifdef _DEBUG
	if (m_debugMessenger != VK_NULL_HANDLE) {
		vkDestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
		m_debugMessenger = VK_NULL_HANDLE;
	}
#endif// _DEBUG
	if (m_instance != VK_NULL_HANDLE) {
		vkDestroyInstance(m_instance, nullptr);
		m_instance = VK_NULL_HANDLE;
	}
}

void mdge::vk::Context::Create(const s_createInfo *pCreateInfo) {
	CreateInstance(pCreateInfo);
	CreateSurface(pCreateInfo);
	CreateDevice(pCreateInfo);
}

void mdge::vk::Context::CreateInstance(const s_createInfo *pCreateInfo) {
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion = pCreateInfo->vkApiVersion;
	appInfo.applicationVersion = pCreateInfo->applicationVersion;
	appInfo.pApplicationName = pCreateInfo->applicationName.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	appInfo.pEngineName = "MDGE";

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	std::vector<const char *> enabledExtentionNames = pCreateInfo->enabledExtentionNames;
#ifdef _DEBUG
	enabledExtentionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	std::vector<const char *> layerNames = {"VK_LAYER_KHRONOS_validation"};

	VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
	if (CheckLayerSupport(layerNames)) {
		debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerCreateInfo.pfnUserCallback = DebugCallback;
		debugMessengerCreateInfo.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
											   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
											   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pNext = &debugMessengerCreateInfo;
		createInfo.enabledLayerCount = layerNames.size();
		createInfo.ppEnabledLayerNames = layerNames.data();
	} else {
		createInfo.pNext = nullptr;
		createInfo.enabledLayerCount = 0;
	}
#else
	createInfo.pNext = nullptr;
	createInfo.enabledLayerCount = 0;
#endif// _DEBUG

	createInfo.enabledExtensionCount = enabledExtentionNames.size();
	createInfo.ppEnabledExtensionNames = enabledExtentionNames.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
	if (result != VK_SUCCESS) { throw std::runtime_error("Failed to create a Vulkan instance with: " + result); }
#ifdef _DEBUG
	result = vkCreateDebugUtilsMessengerEXT(m_instance, &debugMessengerCreateInfo, nullptr, &m_debugMessenger);
	if (result != VK_SUCCESS) { throw std::runtime_error("Failed to create debug messenger with: " + result); }
#endif// _DEBUG
}

void mdge::vk::Context::CreateSurface(const s_createInfo *pCreateInfo) {
	if (!pCreateInfo->pWindow) { throw std::runtime_error("Failed to create window surface. Window is nullptr"); }
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hinstance = GetModuleHandleA(0);
	createInfo.hwnd = pCreateInfo->pWindow->GetNativeWindowHandle();
	VkResult result = vkCreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, &m_surface);
	if (result != VK_SUCCESS) { throw std::runtime_error("Failed to create window surface with: " + result); }
}

void mdge::vk::Context::PickPhysicalDevice(const s_createInfo *pCreateInfo) {}

void mdge::vk::Context::CreateDevice(const s_createInfo *pCreateInfo) {}

bool mdge::vk::Context::CheckLayerSupport(const std::vector<const char *> layerNames) {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> layerProperties(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());

	for (const char *layer : layerNames) {
		bool layerFound = false;
		for (const auto &layerProperties : layerProperties) {
			if (strcmp(layer, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		if (!layerFound) { return false; }
	}
	return true;
}

#ifdef _DEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL mdge::vk::Context::DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		setConsoleColor(CONSOLE_BLUE);
		std::cerr << "validation layer [VERBOSE]: " << pCallbackData->pMessage << std::endl;
	}
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		setConsoleColor(CONSOLE_GREEN);
		std::cerr << "validation layer [INFO]: " << pCallbackData->pMessage << std::endl;
	}
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		setConsoleColor(CONSOLE_YELLOW);
		std::cerr << "validation layer [WARNING]: " << pCallbackData->pMessage << std::endl;
	}
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		setConsoleColor(CONSOLE_RED);
		std::cerr << "validation layer [ERROR]: " << pCallbackData->pMessage << std::endl;
	}

	setConsoleColor(CONSOLE_WHITE);
	return VK_FALSE;
}
#endif