## add souce to this project's executable
add_executable(vulkan_guide
    "main.cpp"
    "vk_engine.h"
    "vk_engine.cpp"
    "vk_initializers.h"
    "vk_initializers.cpp"
    "vk_types.h")

set_property(TARGET vulkan_guide PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:vulkan_guide>")

target_include_directories(vulkan_guide PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(vulkan_guide VkBootstrap vma glm imgui stb_image)

target_link_libraries(vulkan_guide Vulkan::Vulkan sdl2)

add_dependencies(vulkan_guide Shaders)
