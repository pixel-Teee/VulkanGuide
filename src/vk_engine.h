// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <vk_types.h>

#include <vector>
#include <queue>
#include <functional>

#include "vk_mesh.h"//mesh 

#include <glm/glm.hpp>

struct MeshPushConstants {
	glm::vec4 data;
	glm::mat4 render_matrix;
};

struct DeletionQueue
{
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) {
		deletors.push_back(function);
	}

	void flush() {
		// reverse iterate the deletion queue to execute all the functions
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
			(*it)(); //call the function
		}

		deletors.clear();
	}
};

class VulkanEngine {
public:

	bool _isInitialized{ false };
	int _frameNumber{ 0 };

	VkExtent2D _windowExtent{ 600 , 300 };

	struct SDL_Window* _window{ nullptr };

	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	//run main loop
	void run();

	int _selectedShader{ 0 };

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

	VkPipelineLayout _trianglePipelineLayout;

	VkPipeline _trianglePipeline;

	VkPipeline _redTrianglePipeline;

	DeletionQueue _mainDeletionQueue;//deletion queue

	VmaAllocator _allocator;//vma lib allocator

	VkPipeline _meshPipeline;//mesh pipeline
	Mesh _triangleMesh;

	VkPipelineLayout _meshPipelineLayout;

	Mesh _monkeyMesh;

	VkImageView _depthImageView;
	AllocatedImage _depthImage;//VkImage and vma stats

	VkFormat _depthFormat;
private:

	void init_vulkan();

	void init_swapchain();

	void init_commands();

	void init_default_renderpass();

	void init_framebuffers();

	void init_sync_structures();

	//loads a shader module from a spir-v file, returns false if it errors
	bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);

	void init_pipelines();

	void load_meshes();

	void upload_mesh(Mesh& mesh);
};

class PipelineBuilder {
public:

	std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;

	VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
	VkViewport _viewport;
	VkRect2D _scissor;
	VkPipelineRasterizationStateCreateInfo _rasterizer;
	VkPipelineColorBlendAttachmentState _colorBlendAttachment;
	VkPipelineMultisampleStateCreateInfo _multisampling;
	VkPipelineLayout _pipelineLayout;

	VkPipelineDepthStencilStateCreateInfo _depthStencil;

	VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
};
