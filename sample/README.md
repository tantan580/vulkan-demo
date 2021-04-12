# vulkan

###### 验证层

vkCreateDebugUtilsMessengerEXT`调用要求已创建一个有效实例，并且`vkDestroyDebugUtilsMessengerEXT`必须在实例销毁之前调用该实例。当前，这使我们无法调试[`vkCreateInstance`](https://www.khronos.org/registry/vulkan/specs/1.0/man/html/vkCreateInstance.html)and[`vkDestroyInstance`](https://www.khronos.org/registry/vulkan/specs/1.0/man/html/vkDestroyInstance.html)调用中的任何问题。



但是，如果您仔细阅读[扩展文档](https://github.com/KhronosGroup/Vulkan-Docs/blob/master/appendices/VK_EXT_debug_utils.txt#L120)，将会发现有一种方法可以专门为这两个函数调用创建一个单独的调试实用程序Messenger。它只需要`VkDebugUtilsMessengerCreateInfoEXT`在的`pNext`扩展字段中传递一个指向结构的指针即可[`VkInstanceCreateInfo`](https://www.khronos.org/registry/vulkan/specs/1.0/man/html/VkInstanceCreateInfo.html)。首先提取信使填充，将信息创建到一个单独的函数中：

###### 物理设备和队列

使用VkInstance初始化Vulkan库后，我们需要在系统中寻找并选择支持我们所需功能的图形卡。实际上，我们可以选择任意数量的图形卡并同时使用它们，但是在本教程中，我们将坚持使用符合我们需求的第一张图形卡。

也可以根据需求对查询到的显卡进行评分。

###### 队列

涉及到Vulkan中几乎所有的操作，从绘图到上载纹理的任何操作，都需要将命令提交到队列中。有不同类型的队列，它们来自不同的 *队列系列，*并且每个*队列系列*仅允许命令的子集。例如，可能有一个仅允许处理计算命令的队列系列，或者仅允许一个与内存传输相关的命令的队列系列。