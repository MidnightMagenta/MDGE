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
	PickPhysicalDevice(pCreateInfo);
	CreateDevice(pCreateInfo);
	CreateAllocator(pCreateInfo);
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

void mdge::vk::Context::PickPhysicalDevice(const s_createInfo *pCreateInfo) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, physicalDevices.data());

	std::map<uint32_t, VkPhysicalDevice> rankedDevices;

	for (const auto &physicalDevice : physicalDevices) {
		rankedDevices.insert(std::make_pair(RateDevice(physicalDevice, pCreateInfo), physicalDevice));
	}
	if (rankedDevices.rbegin()->first <= 0) { throw std::runtime_error("Could not find a suitable physical device"); }
	m_physicalDevice = rankedDevices.rbegin()->second;
}

void mdge::vk::Context::CreateDevice(const s_createInfo *pCreateInfo) {
	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	QueueFamilyIndeces queueIndeces = GetQueueIndeces(m_physicalDevice, m_surface);

	float queuePriority = 1.f;
	std::set<uint32_t> queueIndecesSet = {queueIndeces.graphicsQueue.value(), queueIndeces.presentQueue.value()};
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	for (uint32_t queueIndex : queueIndecesSet) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.queueFamilyIndex = queueIndex;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.enabledExtensionCount = pCreateInfo->enabledDeviceExtentionNames.size();
	deviceCreateInfo.ppEnabledExtensionNames = pCreateInfo->enabledDeviceExtentionNames.data();
	
	VkResult result = vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);
	if (result != VK_SUCCESS) { throw std::runtime_error("Failed to create Vulkan device with: " + result); }

	vkGetDeviceQueue(m_device, queueIndeces.graphicsQueue.value(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, queueIndeces.presentQueue.value(), 0, &m_presentQueue);
}

void mdge::vk::Context::CreateAllocator(const s_createInfo *pCreateInfo) {
	VmaAllocatorCreateInfo allocatorCreateInfo{};
	allocatorCreateInfo.device = m_device;
	allocatorCreateInfo.instance = m_instance;
	allocatorCreateInfo.physicalDevice = m_physicalDevice;
	allocatorCreateInfo.vulkanApiVersion = pCreateInfo->vkApiVersion;
	VkResult result = vmaCreateAllocator(&allocatorCreateInfo, &m_allocator);
	if (result != VK_SUCCESS) { throw std::runtime_error("Failed to create Vulkan allocator with: " + result); }
}

bool mdge::vk::Context::CheckLayerSupport(const std::vector<const char *> &layerNames) {
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

uint32_t mdge::vk::Context::RateDevice(VkPhysicalDevice device, const s_createInfo *pCreateInfo) {
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceProperties(device, &properties);
	vkGetPhysicalDeviceFeatures(device, &features);

	if (!GetQueueIndeces(device, m_surface).IsComplete()) {
		return 0;
	} else {
		SwapchainSupportDetails details = GetSwapchainSupportDetails(device, m_surface);
		if (details.formats.empty() || details.presentModes.empty()) { return 0; }
	}
	if (!CheckExtentionSupport(device, pCreateInfo->enabledDeviceExtentionNames)) { return 0; }

	uint32_t score = 0;

	switch (properties.deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			score += 0;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			score += 10;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			score += 50;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			score += 100;
			break;
		default:
			score += 0;
			break;
	}

	score += properties.limits.maxImageDimension2D / 1000;
	score += properties.limits.maxFramebufferHeight / 1000 + properties.limits.maxFramebufferWidth / 1000;

	return score;
}

bool mdge::vk::Context::CheckExtentionSupport(VkPhysicalDevice device, const std::vector<const char *> &extentionNames) {
	uint32_t count = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
	std::vector<VkExtensionProperties> extentions(count);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extentions.data());

	std::set<std::string> requiredExtentions(extentionNames.begin(), extentionNames.end());

	for (const auto &extention : extentions) { requiredExtentions.erase(extention.extensionName); }
	return requiredExtentions.empty();
}

mdge::vk::Context::QueueFamilyIndeces mdge::vk::Context::GetQueueIndeces(VkPhysicalDevice device,
																		 VkSurfaceKHR surface) {
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

	QueueFamilyIndeces indeces;
	for (size_t i = 0; i < queueFamilyProperties.size(); i++) {
		if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { indeces.graphicsQueue = i; }
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport) { indeces.presentQueue = i; }
		if (indeces.IsComplete()) { break; }
	}
	return indeces;
}

mdge::vk::Context::SwapchainSupportDetails mdge::vk::Context::GetSwapchainSupportDetails(VkPhysicalDevice device,
																						 VkSurfaceKHR surface) {
	SwapchainSupportDetails supportDetails;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &supportDetails.capabilities);

	uint32_t count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);
	if (count > 0) {
		supportDetails.formats.resize(count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, supportDetails.formats.data());
	}

	count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);
	if (count > 0) {
		supportDetails.presentModes.resize(count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, supportDetails.presentModes.data());
	}
	return supportDetails;
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