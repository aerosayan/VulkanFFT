#include <vulkan/vulkan.h>
#include <stdbool.h>
#define SUPPORTED_RADIX_LEVELS 3

typedef struct {
    VkAllocationCallbacks* allocator;
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    VkDevice device;
    VkQueue queue;
    VkCommandPool commandPool;
    VkFence fence;
    VkShaderModule shaderModules[SUPPORTED_RADIX_LEVELS];
    VkDeviceSize uboAlignment;
} VulkanFFTContext;

void initVulkanFFTContext(VulkanFFTContext* context);
void freeVulkanFFTContext(VulkanFFTContext* context);

typedef struct {
    VulkanFFTContext* context;
    VkDeviceSize size;
    VkBuffer hostBuffer, deviceBuffer;
    VkDeviceMemory deviceMemory;
} VulkanFFTTransfer;

VkShaderModule loadShaderModule(VulkanFFTContext* context, const uint32_t* code, size_t codeSize);
void createBuffer(VulkanFFTContext* context, VkBuffer* buffer, VkDeviceMemory* deviceMemory, VkBufferUsageFlags usage, VkMemoryPropertyFlags propertyFlags, VkDeviceSize size);
VkCommandBuffer createCommandBuffer(VulkanFFTContext* context, VkCommandBufferUsageFlags usageFlags);

void bufferTransfer(VulkanFFTContext* context, VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize size);
void* createVulkanFFTUpload(VulkanFFTTransfer* vulkanFFTTransfer);
void* createVulkanFFTDownload(VulkanFFTTransfer* vulkanFFTTransfer);
void freeVulkanFFTTransfer(VulkanFFTTransfer* vulkanFFTTransfer);

typedef struct {
    VulkanFFTContext* context;
    bool inverse, resultInSwapBuffer;
    struct VulkanFFTAxis {
        uint32_t sampleCount;
        uint32_t stageCount;
        uint32_t* stageRadix;
        VkDeviceSize uboSize;
        VkBuffer ubo;
        VkDeviceMemory uboDeviceMemory;
        VkDescriptorPool descriptorPool;
        VkDescriptorSetLayout* descriptorSetLayouts;
        VkDescriptorSet* descriptorSets;
        VkPipelineLayout pipelineLayout;
        VkPipeline* pipelines;
    } axes[3];
    VkDeviceSize bufferSize;
    VkBuffer buffer[2];
    VkDeviceMemory deviceMemory[2];
} VulkanFFTPlan;

void createVulkanFFT(VulkanFFTPlan* vulkanFFTPlan);
void recordVulkanFFT(VulkanFFTPlan* vulkanFFTPlan, VkCommandBuffer commandBuffer);
void destroyVulkanFFT(VulkanFFTPlan* vulkanFFTPlan);
