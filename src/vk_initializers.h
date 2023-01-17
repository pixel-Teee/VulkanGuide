#pragma once

#include "vk_types.h"

namespace vkInit {
	VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);

	//VK_COMMAND_BUFFER_LEVEL_SECONDARY use for multi thread and sub commands
	VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	
	VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage, VkShaderModule shaderModule);

	VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info();

	VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info(VkPrimitiveTopology topology);

	VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygonMode);

	VkPipelineMultisampleStateCreateInfo multisampling_state_create_info();

	VkPipelineColorBlendAttachmentState color_blend_attachment_state();

	//root signature
	VkPipelineLayoutCreateInfo pipeline_layout_create_info();
}