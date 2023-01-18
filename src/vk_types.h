// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vulkan/vulkan.h>

#include "include/vk_mem_alloc.h"//vma

struct AllocatedBuffer {
	VkBuffer _buffer;
	VmaAllocation _allocation;//vma allocation holds the state the VMA library uses
	//like the memory that buffer was allocated from, and it's size
};

//we will add our main reusable types here

struct AllocatedImage {
	VkImage _image;
	VmaAllocation _allocation;
};