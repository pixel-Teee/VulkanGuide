# Vulkan main objects and their use

**VkInstance**：vulkan上下文，被用于去访问驱动。

**VkPhysicalDevice**：一个GPU，被用于去访问物理GPU细节，比如特性，能力，内存大小，等。

**VkDevice**：逻辑GPU上下文，你实际在上面执行东西。

**VkBuffer**：一块GPU可见的内存。

**VkImage**：一个纹理，你可以写入和读取的。

**VkPipeline**：持有需要去绘制的gpu的状态，例如：着色器，光栅化选项，深度设置。

**VkRenderPass**：持有你要渲染到的图像的信息，所有的绘制命令必须在renderpass内部。

**VkFrameBuffer**：持有对于一个renderpass的目标图像。

**VkCommandBuffer**：编码了GPU的命令，所有的执行都在GPU那侧必须要被编码进VkCommandBuffer。

**VkQueue**：执行命令的端口。GPU将有一个集合的对了，对于不同的属性。一些允许只有图形命令，一些只允许内存命令，等。命令buffers被执行，通过提交它们到一个队列，将拷贝渲染命令到GPU，对于执行。

**VkDescriptorSet**：持有绑定信息，链接了shader的输入到数据，比如**VkBuffer**资源还有**VkImage**纹理。想象它作为一个gpu侧的指针，你绑定一次的。

**VkSwapchainKHR**：持有屏幕的图像。它允许你去渲染东西到一个可见的窗口。KHR后缀允许它来自一个扩展，在这个情况下是VK_KHR_swapchain。

**VkSemaphore**：同步GPU到GPU的命令的执行。被用于同步多个命令buffer的提交，在另一个之后。

**VkFence**：同步GPU到CPU命令的执行，用于确定命令缓冲区是否已经在GPU上执行完毕。



# High Level Vulkan application flow

## Engine Initialization

第一步，所有的事物都是被初始化。为了初始化Vulkan，你开始，通过创建一个VkInstance。从VkInstance，你询问VkPhysicalDevice句柄可用的列表，在机器上。例如，如果计算机同时具有专用GPU和集成图形，这里将有一个VkPhysicalDevice对于每个。在询问可用的VkPhysicalDevice的句柄的限制和特性之后，你创建了一个VkDevice从它身上。对于一个VkDevice，你然后从它身上获取一个VkQueue，允许你去执行命令。那么你去初始化VkSwapchainKHR。伴随着VkQueue句柄，你创建了VkCommandPool对象，开启你去分配从来自它的命令。



## Asset Initialization

一旦核心结构体被初始化了，你初始化将呈现的任何内容所需的资源。材质被加载，并且你创建VkPipeline对象的几何，对于着色器合并还有必要的参数，对于渲染材质。对于网格，你上传它们的顶点数据到VkBuffer资源，并且你上传它们的纹理到VkImage资源，确认图像是可读的布局。你也创建了VkRenderPass对象，无论主渲染过程是什么。例如，这里可能有一个VkRenderPass对于主渲染，还有另一个对于shadow pass。在一个真正的引擎，大多数这些可以被并行化，并且被做在一个背后的线程，尤其自动流水线窗口是相当昂贵的。



## Render Loop

目前所有的事物准备用于渲染，你第一步询问VkSwapchainKHR对于一个image，去渲染。



然后你分配一个VkCommandBuffer从一个VkCommandBufferPool或者重复使用一个已经分配的命令buffer，它已经执行完毕了，并且开始命令buffer，允许你写入buffer的。接下来，你开始渲染，通过开始一个VkRenderPass，描述你从swapchain请求的image，然后创建一个循环，你绑定VkPipeline的地方，绑定一些VkDescriptorSet资源(对于着色器参数)，绑定vertex buffers，然后执行一个绘制调用。一旦你已经完成了绘制对于一个pass，你以VkRenderPass结束。如果这里没有更多的东西去绘制，你以VkCommandBuffer结束。最终地，你提交命令buffer到队列对于渲染。这个将开始在命令buffer的命令的执行在gpu。如果你想去展示渲染的结果，你呈现你已经渲染的image到屏幕。因为执行可能还没有完成，你使用一个信号量去使得呈现到图像的屏幕等待渲染完毕。



在Vulkan中的渲染循环的伪代码：

```c++
//请求swapchain，对于我们可以渲染到的swapchain image的索引
int image_index = request_image(mySwapchain);

//create a new command buffer
VkCommandBuffer cmd = allocate_command_buffer();

//initialize the command buffer
vkBeginCommandBuffer(cmd, ...);

//start a new renderpass with the image index from swapchain as target to render onto
//each framebuffer refers to a image in the swapchain
vkCmdBeginRenderPass(cmd, main_render_pass, framebuffers[image_index]);

//rendering all objects
for(object in PassObjects)
{
	//bind the shaders and configuration used to render the object
	vkCmdBindPipeline(cmd, object.pipeline);
	
	//bind the vertex and index buffers for rendering the object
	vkCmdBindVertexBuffers(cmd, object.VertexBuffer, ...);
	vkCmdBindIndexBuffers(cmd, object.IndexBuffer, ...);
	
	//bind the descriptor sets for the object(shader inputs)
	vkCmdBindDescriptorSets(cmd, object.textureDescriptorSet);
	vkCmdBindDescriptorSets(cmd, object.parametersDescriptorSet);
	
	//execute drawing
	vkCmdDraw(cmd, ...);
}
```



































