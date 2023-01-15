对于Vulkan引擎，我们使用一个确切的文件夹结构：

![image-20230115220559806](Image/Project Layout/image-20230115220559806-1673791567246-1.png)



# Libraries

在引擎，我们使用一些库的集合，大多数存储在/third_party。



所有在third-party的库是vendorded-in的，并且CMake将自动地编译它们，对于SDL，它被要求从项目中分离，并且告诉CMake哪里去找到它。



