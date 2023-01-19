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

//note that we store the VkPipeline and layout by value, not pointer.
//They are 64 bit handles to internal driver structures anyway so storing pointers to them isn't very useful

struct Material {
	VkDescriptorSet textureSet{ VK_NULL_HANDLE };//texture defaulted to null
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
};

struct Texture {
	AllocatedImage image;
	VkImageView imageView;
};

struct RenderObject {
	Mesh* mesh;//don't have ownership

	Material* material;//don't have ownership

	glm::mat4 transformMatrix;
};

struct GPUCameraData {
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewproj;
};

struct GPUSceneData {
	glm::vec4 fogColor; //w is for exponent
	glm::vec4 fogDistances; //x for min, y for max, zw unused.
	glm::vec4 ambientColor;
	glm::vec4 sunlightDirection; //w for sun power
	glm::vec4 sunlightColor;
};

struct GPUObjectData {
	glm::mat4 modelMatrix;
};

struct FrameData {
	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;

	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;

	//buffer that holds a single GPUCameraData to use when rendering
	AllocatedBuffer cameraBuffer;

	VkDescriptorSet globalDescriptor;

	AllocatedBuffer objectBuffer;//ssbo shader storage buffer
	VkDescriptorSet objectDescriptor;
};

struct UploadContext {
	VkFence _uploadFence;
	VkCommandPool _commandPool;
	VkCommandBuffer _commandBuffer;
};

//number of frames to overlap when rendering
constexpr unsigned int FRAME_OVERLAP = 2;

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

	VkExtent2D _windowExtent{ 800, 600 };

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

	//default array of renderable objects
	std::vector<RenderObject> _renderables;

	std::unordered_map<std::string, Material> _materials;
	std::unordered_map<std::string, Mesh> _meshes;
	std::unordered_map<std::string, Texture> _loadedTextures;

	//frame storage
	FrameData _frames[FRAME_OVERLAP];

	VkDescriptorSetLayout _globalSetLayout;
	VkDescriptorSetLayout _objectSetLayout;
	VkDescriptorSetLayout _singleTextureSetLayout;
	VkDescriptorPool _descriptorPool;

	VkPhysicalDeviceProperties _gpuProperties;

	GPUSceneData _sceneParameters;
	AllocatedBuffer _sceneParameterBuffer;

	UploadContext _uploadContext;

	//getter for the frame we are rendering to right now
	FrameData& get_current_frame();

	//create material and add it to the map
	Material* create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name);

	//returns nullptr if it can't be found
	Material* get_material(const std::string& name);

	//returns nullptr if it can't be found
	Mesh* get_mesh(const std::string& name);

	//our draw function
	void draw_objects(VkCommandBuffer cmd, RenderObject* first, int count);

	void init_scene();

	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

	size_t pad_uniform_buffer_size(size_t originalSize);

	void init_descriptors();

	//initialize command pool and fence in the upload context
	//instant-submit
	//move sematic
	//could use this from a background thread, separated from the render loop
	void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);
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

	void load_images();

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
