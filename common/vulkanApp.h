#ifndef _vulkan_app_h
#define _vulkan_app_h
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>
//vk 验证层一般是在调试模式下开启，在release模式下是关闭的，与扩展一样，验证层也需要通过指定其名称来启用
const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class VulkanApp
{
public:
    VulkanApp();
    void run();

private:
    void initWindow();
    //vulkan functions
    void initVulkan();
    void createInstance(); //createInsatnce,Insatnce是应用程序和Vulkan库之间的连接
    bool checkValidationLayerSupport();//用于检查所有请求的图层是否可用--验证层
    std::vector<const char *> getRequiredExtensions();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    void setupDebugMessenger();
    //render loop
    void mainLoop();
    //resources clean
    void cleanup();

private:
    //物理设备
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete()
        {
            return graphicsFamily.has_value();
        }
    };
    void pickPhysicalDevice();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    //logic device and queues
    //选择要使用的物理设备后，我们需要设置逻辑设备与它交互
    void createLogicalDevice();

private:
    GLFWwindow *m_vulkan_window;
    uint32_t m_width;
    uint32_t m_height;
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    //physicalDevice
    VkPhysicalDevice m_physicalDevice;
    //logic device
    VkDevice m_logicDevice;
    VkQueue m_graphicsQueue;

private:
    // 验证层 callback,
    //1.第一个参数指定消息的严重性，是以下标志：
    //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT：诊断消息
    //VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT：信息性消息，例如资源的创建
    //VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT：有关行为的消息不一定是错误，但很可能是应用程序中的错误
    //VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT：有关无效行为并可能导致崩溃的消息
    //-----------------------------------------
    //2.该messageType参数可以具有以下值：
    //VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT：发生了一些与规格或性能无关的事件
    //VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT：发生了违反规范或表明可能的错误的事情
    //VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT：Vulkan的潜在非最佳使用
    //-----------------------------------------
    //3.该pCallbackData参数引用VkDebugUtilsMessengerCallbackDataEXT包含消息本身详细信息的结构，最重要的成员是：
    //pMessage：调试消息以以空字符结尾的字符串
    //pObjects：与消息相关的Vulkan对象句柄数组
    //objectCount：数组中的对象数
    //4.最后，该pUserData参数包含在设置回调过程中指定的指针，并允许您将自己的数据传递给它。
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void *pUserData)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        //回调返回一个布尔值，该布尔值指示是否应终止触发验证层消息的Vulkan调用。如果回调返回true，
        //则该调用将因VK_ERROR_VALIDATION_FAILED_EXT错误而中止。通常这仅用于测试验证层本身，因此您应该始终返回VK_FALSE。
        return VK_FALSE;
    }
};

#endif
