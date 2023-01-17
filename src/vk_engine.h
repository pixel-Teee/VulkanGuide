// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>

#include <vector>

class VulkanEngine {
public:

	bool _isInitialized{ false };
	int _frameNumber{ 0 };

	VkExtent2D _windowExtent{ 1700 , 900 };

	struct SDL_Window* _window{ nullptr };

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	//run main loop
	void run();

public:
	//------omitted------
	VkInstance _instance;//vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger;//vulkan debug output handle
	VkPhysicalDevice _chosenGPU;//gpu chosen as the default device
	VkDevice _device;//vulkan device for commands
	VkSurfaceKHR _surface;//vulkan window surface


	//------other code------
	VkSwapchainKHR _swapchain;//from other articles

	//image format expected by the windowing system
	VkFormat _swapchainImageFormat;

	//array of images from the swapchain
	//vk image is a handle to the actual image object to use as texture or to render into
	std::vector<VkImage> _swapchainImages;

	//array of image-views from the swapchain
	std::vector<VkImageView> _swapchainImageViews;

	//------other code------
	VkQueue _graphicsQueue;//queue we will submit to
	uint32_t _graphicsQueueFamily;//family of that queue

	VkCommandPool _commandPool;//the command pool for our commands
	VkCommandBuffer _mainCommandBuffer;//the buffer we will record into

	VkRenderPass _renderPass;

	std::vector<VkFramebuffer> _framebuffers;

	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;

private:

	void init_vulkan();

	void init_swapchain();

	void init_commands();

	void init_default_renderpass();

	void init_framebuffers();

	void init_sync_structures();
};