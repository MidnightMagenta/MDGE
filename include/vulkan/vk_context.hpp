#ifndef VK_CONTEXT_H
#define VK_CONTEXT_H

#if defined(_DEBUG) && defined(MDGE_SUBSYSTEM_CONSOLE)
#include <iostream>
#endif

#include "../include/windows/win32_console.hpp"
#include "../include/windows/win32_window.hpp"
#include <initializer_list>
#include <string>
#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

namespace mdge::vk {
class Context {
public:
	struct CreateInfo {
		std::string applicationName = "Application";
		uint32_t applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		uint32_t vkApiVersion = VK_API_VERSION_1_3;
		std::vector<const char *> enabledExtentionNames = {VK_KHR_SURFACE_EXTENSION_NAME,
														   VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
		const mdge::win32_window *pWindow = nullptr;
	};

	Context() {}
	Context(const CreateInfo *pCreateInfo) { Create(pCreateInfo); }
	~Context();

	void Create(const CreateInfo *pCreateInfo);

private:
	void CreateInstance(const CreateInfo *pCreateInfo);
	void CreateSurface(const CreateInfo *pCreateInfo);
	void PickPhysicalDevice(const CreateInfo *pCreateInfo);
	void CreateDevice(const CreateInfo *pCreateInfo);

	static bool CheckLayerSupport(const std::vector<const char *> layerNames);

	VkInstance m_instance = VK_NULL_HANDLE;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device = VK_NULL_HANDLE;
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	VmaAllocator m_allocator = VK_NULL_HANDLE;

	VkQueue m_graphicsQueue = VK_NULL_HANDLE;
	VkQueue m_presentQueue = VK_NULL_HANDLE;

	VkSampleCountFlagBits m_msaaSampleCount = VK_SAMPLE_COUNT_1_BIT;

#ifdef _DEBUG
	VkDebugUtilsMessengerEXT m_debugMessenger;
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
