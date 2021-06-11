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
//#define NDEBUG
//vk 验证层一般是在调试模式下开启，在release模式下是关闭的，与扩展一样，验证层也需要通过指定其名称来启用
const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
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
    void createSurface();
    //render loop
    void mainLoop();
    void drawFrame();
    //resources clean
    void cleanup();

private:
    //物理设备
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;//支持绘图命令的队列族
        std::optional<uint32_t> presentFamily;//支持演示的队列族

        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    void pickPhysicalDevice();
    int rateDeviceSuitability(VkPhysicalDevice device);//给查询到的显卡设备评分。

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);//对显卡设备查询支持的命令队列
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    //logic device and queues
    //选择要使用的物理设备后，我们需要设置逻辑设备与它交互
    void createLogicalDevice();

    //________Swap chain
    void createSwapChain();
    void recreateSwapChain();
    void cleanupSwapChain();
    //交换链详细信息
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    //查询交换链支持的详细信息
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    //为交换链选择合适的表面格式
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    //交换范围是交换链图像的分辨率
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createImageViews();
    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<char>& code);
    void createRenderPass();
    void createDepthResources();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();
    bool hasStencilComponent(VkFormat format);

    void createFramebuffers();

    void createCommandPool();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createDescriptorSetLayout();//uniform buffer object

    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void updateUniformBuffer(uint32_t currentImage);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    //command buffer
    VkCommandBuffer beginSingleCommands();
    void endSingleCommands(VkCommandBuffer commandBuffer);
    void createCommandBuffers();
    void createSyncObjects();
private:
    GLFWwindow *m_vulkan_window;
    uint32_t m_width;
    uint32_t m_height;
    VkInstance m_instance;

    VkDebugUtilsMessengerEXT m_debugMessenger;
    //window surface
    VkSurfaceKHR m_surface;
    //physicalDevice
    VkPhysicalDevice m_physicalDevice {VK_NULL_HANDLE};
    //logic device
    VkDevice m_device;//逻辑设备句柄
    VkQueue m_graphicsQueue;//图形队列句柄
    VkQueue m_presentQueue;

    //swap chain
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    //VkImageView
    std::vector<VkImageView> m_swapChainImageViews;
    //framebuffers
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    //pipeline
    VkRenderPass m_renderPass;

    VkDescriptorSetLayout m_descriptorSetLayout;
    VkDescriptorPool m_descriptorPool;
    std::vector<VkDescriptorSet> m_descriptorSets;
    VkPipelineLayout m_pipelineLayout;

    VkPipeline m_graphicsPipeline;

    //Command buffers
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;

    //depth buffer
    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;

    //texture buffer
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureImageView;
    VkSampler m_textureSampler;

    //buffer paras
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;


    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;

    //semaphores
    std::vector<VkSemaphore> m_imageAvailableSemaphores;//已获取图像并准备好进行渲染
    std::vector<VkSemaphore> m_renderFinishedSemaphores;//已完成渲染并可以进行呈现
    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;
    size_t m_currentFrame = 0;
public:
    bool m_framebufferResized {false};//许多驱动程序和平台会VK_ERROR_OUT_OF_DATE_KHR在窗口大小调整后自动触发，但并不能保证一定会发生
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
