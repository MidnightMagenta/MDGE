add_library("vma" STATIC
	vk_mem_alloc.h
	vma.cpp
)

target_include_directories("vma" PUBLIC 
    "$ENV{VULKAN_SDK}/Include"
)

target_link_libraries("vma" PUBLIC 
    "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib"
)