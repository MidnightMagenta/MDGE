#ifndef VK_CONTEXT_H
#define VK_CONTEXT_H

#if defined(_DEBUG) && defined(MDGE_SUBSYSTEM_CONSOLE)
#include <iostream>
#endif

#define VK_USE_PLATFORM_WIN32_KHR

#include "../include/windows/win32_console.hpp"
#include "../include/windows/win32_window.hpp"
#include <initializer_list>
#include <map>
#include <optional>
#include <string>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>
#include <set>

namespace mdge::vk {
class Context {
public:
	struct s_createInfo {
		std::string applicationName = "Application";
		uint32_t applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		uint32_t vkApiVersion = VK_API_VERSION_1_3;
		std::vector<const char *> enabledExtentionNames = {VK_KHR_SURFACE_EXTENSION_NAME,
														   VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
		std::vector<const char *> enabledDeviceExtentionNames = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
		const mdge::win32_window *pWindow = nullptr;
	};

	struct QueueFamilyIndeces {
		std::optional<uint32_t> graphicsQueue;
		std::optional<uint32_t> presentQueue;
		bool IsComplete() const { return graphicsQueue.has_value() && presentQueue.has_value(); }
	};

	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	Context() {}
	Context(const s_createInfo *pCreateInfo) { Create(pCreateInfo); }
	~Context();

	void Create(const s_createInfo *pCreateInfo);

	static QueueFamilyIndeces GetQueueIndeces(VkPhysicalDevice device, VkSurfaceKHR surface);
	static SwapchainSupportDetails GetSwapchainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	void CreateInstance(const s_createInfo *pCreateInfo);
	void CreateSurface(const s_createInfo *pCreateInfo);
	void PickPhysicalDevice(const s_createInfo *pCreateInfo);
	void CreateDevice(const s_createInfo *pCreateInfo);
	void CreateAllocator(const s_createInfo *pCreateInfo);

	static bool CheckLayerSupport(const std::vector<const char *> &layerNames);
	uint32_t RateDevice(VkPhysicalDevice device, const s_createInfo *pCreateInfo);
	static bool CheckExtentionSupport(VkPhysicalDevice device, const std::vector<const char *> &extentionNames);

	VkInstance m_instance = VK_NULL_HANDLE;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device = VK_NULL_HANDLE;
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
	std::vector<VkImage> m_swapchainImages;
	VmaAllocator m_allocator = VK_NULL_HANDLE;

	VkQueue m_graphicsQueue = VK_NULL_HANDLE;
	VkQueue m_presentQueue = VK_NULL_HANDLE;

	VkSampleCountFlagBits m_msaaSampleCount = VK_SAMPLE_COUNT_1_BIT;

#ifdef _DEBUG
	VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
														VkDebugUtilsMessageTypeFlagsEXT messageType,
														const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
														void *pUserData);

	VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
											const VkAllocationCallbacks *pAllocator,
											VkDebugUtilsMessengerEXT *pDebugMessenger) {
		auto func =
				(PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func == nullptr) { return VK_ERROR_EXTENSION_NOT_PRESENT; }
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}

	void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
										 const VkAllocationCallbacks *pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
																				"vkDestroyDebugUtilsMessengerEXT");
		if (func == nullptr) { return; }
		return func(instance, debugMessenger, pAllocator);
	}
#endif// _DEBUG
};
}// namespace mdge::vk

#endif// !VK_CONTEXT_H
