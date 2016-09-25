#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string>

//#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>	//includes <vulkan/vulkan.h> if GLFW_INCLUDE_VULKAN is defined
using namespace std;

vector<char> LoadBinaryFile(string filename)
{
	cout<<"trying to open "<<filename<<endl;
	ifstream file(filename, ios::ate | ios::binary);
	if(file.is_open())
	{
		cout<<"file opend..."<<endl;
		cout<<"determin filesize..."<<endl;
		size_t fileSize = (size_t) file.tellg();
		cout<<"creating buffer..."<<endl;
		std::vector<char> buffer(fileSize);
		cout<<"back to start of file..."<<endl;
		file.seekg(0);
		cout<<"trying to read all the bytes at once"<<endl;
		file.read(buffer.data(), fileSize);
		cout<<"bytes read!"<<endl;
		cout<<"closing filestream..."<<endl;
		file.close();
		cout<<"file closed!"<<endl;
		return buffer;
	}
	else
	{
		cout<<"could not open file!"<<endl;
	}
	cout<<"done reading "<<filename<<endl;
}

class VulkanEngine
{
	protected:
	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	uint32_t queueFamilyIndex;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkSemaphore ImageAvailibleSemaphore;
	VkSemaphore RenderingFinishedSemaphore;
	VkSurfaceFormatKHR format;
	VkSwapchainKHR swapchain;
	vector<VkImage> swapChainImages;
	vector<VkImageView> swapChainImageViews;
	
	public:
	void PollEvents()
	{
		glfwPollEvents();
	}
	bool ShouldClose()
	{
		return glfwWindowShouldClose(window);
	}
	
	
	void InitWindow()
	{
		cout<<"Initializing Window..."<<endl;
		cout<<"Initializing glfw..."<<endl;
		glfwInit();
		cout<<"glfw initialized!..."<<endl;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		cout<<"creating glfwWinodw..."<<endl;
		window = glfwCreateWindow(640,480, "Vulkan", NULL, NULL);
		cout<<"window created..."<<endl<<endl;
	}
	void CreateInstance()
	{
		cout<<"Creating Instance..."<<endl;
		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;
		cout<<"getting glfw Extensions..."<<endl;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		for(int i = 0; i < glfwExtensionCount; i++)
		{
			cout<<"Extension "<<i<<": "<<glfwExtensions[i]<<endl;
		}
		cout<<"got glfw Extensions!"<<endl;
		
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.pApplicationInfo = NULL; //optional s.o.
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		
		cout<<"calling vKCreateInstance..."<<endl;
		vkCreateInstance(&createInfo, NULL, &instance);
		cout<<"Instance Created!"<<endl<<endl;
	}
	void PickPhysicalDevice()
	{
		cout<<"Picking physical Device..."<<endl;
		cout<<"Listing all Devices..."<<endl;
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
		cout<<"found "<<deviceCount<<" Devices!"<<endl;
		vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		for(int i = 0; i < deviceCount; i++)
		{
			uint32_t extensionCount = 0;
			vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extensionCount, NULL);
			cout<<"found "<<extensionCount<<" availible Device Extensions"<<endl;
			vector<VkExtensionProperties> deviceExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extensionCount, deviceExtensions.data());
			for(int f = 0; f < extensionCount; f++)
			{
				cout<<"Device "<<i<<": Extension: "<<f<<": "<<deviceExtensions[f].extensionName<<endl;
			}
			
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);
			cout<<"Device "<<i<<": API Version: "<<deviceProperties.apiVersion<<endl;
			cout<<"Device "<<i<<": Driver Version: "<<deviceProperties.driverVersion<<endl;
			cout<<"Device "<<i<<": Vendor ID: "<<deviceProperties.vendorID<<endl;
			cout<<"Device "<<i<<": Device ID: "<<deviceProperties.deviceID<<endl;
			cout<<"Device "<<i<<": Device Name: "<<deviceProperties.deviceName<<endl;
			
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(devices[i], &deviceFeatures);
			cout<<"Device "<<i<<": robustBufferAccess: "<<deviceFeatures.robustBufferAccess<<endl;
			cout<<"Device "<<i<<": geometryShader: "<<deviceFeatures.geometryShader<<endl;
			cout<<"Device "<<i<<": tessellationShader: "<<deviceFeatures.tessellationShader<<endl;
			
			cout<<"trying to find Queue Families for Device "<<i<<" ..."<<endl;
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, NULL);
			cout<<"Device "<<i<<": Found "<<queueFamilyCount<<" queue Families!"<<endl;
			
			vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, queueFamilies.data());
			for(int f = 0; f < queueFamilyCount; f++)
			{
				cout<<"Device "<<i<<": queue Family "<<f<<": contains "<<queueFamilies[f].queueCount<<" queue"<<endl;
				if(queueFamilies[f].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					cout<<"Device "<<i<<": queue Family "<<f<<": Graphics Bit true"<<endl;
				}
				else
				{
					cout<<"Device "<<i<<": queue Family "<<f<<": Graphics Bit false"<<endl;
				}
				if(queueFamilies[f].queueFlags & VK_QUEUE_COMPUTE_BIT)
				{
					cout<<"Device "<<i<<": queue Family "<<f<<": Compute Bit true"<<endl;
				}
				else
				{
					cout<<"Device "<<i<<": queue Family "<<f<<": Compute Bit false"<<endl;
				}
				if(queueFamilies[f].queueFlags & VK_QUEUE_TRANSFER_BIT)
				{
					cout<<"Device "<<i<<": queue Family "<<f<<": Transfer Bit true"<<endl;
				}
				else
				{
					cout<<"Device "<<i<<": queue Family "<<f<<": Transfer Bit false"<<endl;
				}
				if(queueFamilies[f].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
				{
					cout<<"Device "<<i<<": queue Family "<<f<<": sparse binding Bit true"<<endl;
				}
				else
				{
					cout<<"Device "<<i<<": queue Family "<<f<<": sparse binding Bit false"<<endl;
				}
				
				
			}
			cout<<"------------------------------------"<<endl;
		}
		cout<<"setting (main) physical device to device 0..."<<endl;
		physicalDevice = devices[0]; //quick and dirty lol
		cout<<"setting (main) queue family device 0..."<<endl;
		queueFamilyIndex = 0;
		cout<<"physical Device picked!"<<endl<<endl;
	}
	void CreateLogicalDevice()
	{
		cout<<"creating logical Device..."<<endl;
		//QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex; //picking the 0th queue family
		queueCreateInfo.queueCount = 1;
		float queuePriority = 1.0;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		
		VkPhysicalDeviceFeatures deviceFeatures = {}; //alles VK_FALSE by default
		
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = 0;
		const char* ExtensionName = "VK_KHR_swapchain";
		createInfo.ppEnabledExtensionNames = &ExtensionName;
		
		cout<<"calling vkCreateDevice..."<<endl;
		vkCreateDevice(physicalDevice, &createInfo, NULL, &device);
		cout<<"logical Device created!"<<endl;
		
		cout<<"trying to get graphics Queue..."<<endl;
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &graphicsQueue);
		cout<<"Queue retrieved!"<<endl;
		cout<<"trying to get presentation Queue..."<<endl;
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &presentQueue);
		cout<<"Queue retrieved!"<<endl;
		cout<<"logical Device creation finished!"<<endl<<endl;
	}
	void CreateSurface()
	{
		cout<<"Creating Surface..."<<endl;
		cout<<"calling glfwCreateWindowSurface..."<<endl;
		glfwCreateWindowSurface(instance, window, NULL, &surface);
		cout<<"Surface creation done!"<<endl;
		VkBool32 presentSupport = false;
		cout<<"listing Surface Features..."<<endl;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, &presentSupport);
		if(presentSupport)
		{
			cout<<"supports presentation"<<endl;
		}
		else
		{
			cout<<"does not support presentation"<<endl;
		}
		
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
		cout<<"Surface minImageCount = "<<surfaceCapabilities.minImageCount<<endl;
		cout<<"Surface maxImageCount = "<<surfaceCapabilities.maxImageCount<<endl;
		cout<<"Surface minImageWidth = "<<surfaceCapabilities.minImageExtent.width<<endl;
		cout<<"Surface maxImageWidth = "<<surfaceCapabilities.maxImageExtent.width<<endl;
		cout<<"Surface minImageHeight = "<<surfaceCapabilities.minImageExtent.height<<endl;
		cout<<"Surface maxImageHeight = "<<surfaceCapabilities.maxImageExtent.height<<endl;
		cout<<"Surface maxImageArrayLayers = "<<surfaceCapabilities.maxImageArrayLayers<<endl;
		
		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, NULL);
		vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());
		cout<<"Found "<<formatCount<<" Formats!"<<endl;
		for(int i = 0; i < formatCount; i++)
		{
			cout<<"Format "<<i<<": "<<surfaceFormats[i].format<<" ";
			if(surfaceFormats[i].format == VK_FORMAT_UNDEFINED)
			{
				cout<<"VK_FORMAT_UNDEFINED"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_R8G8B8A8_UNORM)
			{
				cout<<"VK_FORMAT_R8G8B8A8_UNORM"<<endl;
			}
			if(surfaceFormats[i].format == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
			{
				cout<<"VK_COLORSPACE_SRGB_NONLINEAR_KHR"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_R8G8B8A8_SRGB)
			{
				cout<<"VK_FORMAT_R8G8B8A8_SRGB"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_R8G8B8A8_SINT)
			{
				cout<<"VK_FORMAT_R8G8B8A8_SINT"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_R8G8B8A8_UINT)
			{
				cout<<"VK_FORMAT_R8G8B8A8_UINT"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM)
			{
				cout<<"VK_FORMAT_B8G8R8A8_UNORM"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SNORM)
			{
				cout<<"VK_FORMAT_B8G8R8A8_SNORM"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_USCALED)
			{
				cout<<"VK_FORMAT_B8G8R8A8_USCALED"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SSCALED)
			{
				cout<<"VK_FORMAT_B8G8R8A8_SSCALED"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UINT)
			{
				cout<<"VK_FORMAT_B8G8R8A8_UINT"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SINT)
			{
				cout<<"VK_FORMAT_B8G8R8A8_SINT"<<endl;
			}
			if(surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB)
			{
				cout<<"VK_FORMAT_B8G8R8A8_SRGB"<<endl;
			}
		}
		cout<<"selecting format (quick and dirty) 0"<<endl;
		format = surfaceFormats[0];
		
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, NULL);
		vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount,presentModes.data());
		cout<<"found "<<presentModeCount<<" present Modes!"<<endl;
		for(int i = 0; i < presentModeCount; i++)
		{
			cout<<"present Mode "<<i<<": ";
			if(presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				cout<<"Immediate"<<endl;
			}
			if(presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				cout<<"Mailbox"<<endl;
			}
			if(presentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
			{
				cout<<"FIFO"<<endl;
			}
			if(presentModes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
			{
				cout<<"FIFO relaxed"<<endl;
			}
		}
		
		cout<<"Surface Creation done!"<<endl<<endl;
	}
	void CreateSemaphore()
	{
		cout<<"creating Semaphores..."<<endl;
		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.pNext = NULL;
		semaphoreCreateInfo.flags = 0;
		cout<<"calling vkCreateSemaphore(ImageAvailibleSemaphore)..."<<endl;
		vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &ImageAvailibleSemaphore);
		cout<<"calling vkCreateSemaphore(RenderingFinishedSemaphore)..."<<endl;
		vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &RenderingFinishedSemaphore);
		cout<<"Semaphore created!"<<endl<<endl;
	}
	void CreateSwapChain()
	{
		cout<<"creating Swapchain..."<<endl;
		VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
		swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.pNext = NULL;
		swapChainCreateInfo.flags = 0;
		swapChainCreateInfo.surface = surface;
		swapChainCreateInfo.minImageCount = 3;
		swapChainCreateInfo.imageFormat = format.format;
		swapChainCreateInfo.imageColorSpace = format.colorSpace;
		swapChainCreateInfo.imageExtent = { 640, 480 };
		swapChainCreateInfo.imageArrayLayers = 1;
		swapChainCreateInfo.imageUsage = 
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = NULL;
		swapChainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainCreateInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		swapChainCreateInfo.clipped = true;
		swapChainCreateInfo.oldSwapchain = NULL;
		
		cout<<"calling VkCreateSwapchainKHR..."<<endl;
		vkCreateSwapchainKHR(device, &swapChainCreateInfo, NULL, &swapchain);
		
		cout<<"retrieving Images from Swapchain"<<endl;
		
		uint32_t imageCount = 0;
		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL);
		cout<<"retrieved "<<imageCount<<" images"<<endl;
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapChainImages.data());
		cout<<"Images retrieved!"<<endl;
		
		cout<<"swapchain created!"<<endl<<endl;
	}
	void CreateImageViews()
	{
		cout<<"creating Image Views..."<<endl;
		cout<<"found "<<swapChainImages.size()<<" swap chain images. Creating "<<swapChainImages.size()<<" Image Views"<<endl;
		cout<<"resizing ImageViews vector..."<<endl;
		swapChainImageViews.resize(swapChainImages.size());
		for(int i = 0; i < swapChainImages.size(); i++)
		{
			cout<<"Creating "<<i<<". Image View"<<endl;
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];
			createInfo.format = format.format;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			cout<<"calling vkCreateImage view"<<endl;
			vkCreateImageView(device, &createInfo, NULL, &swapChainImageViews[i]);
		}
		cout<<"image view creation done!"<<endl<<endl;
	}
	void CreateGraphicsPipeline()
	{
		cout<<"creating Graphics Pipeline..."<<endl;
		cout<<"loading spv files..."<<endl;
		vector<char> vertexShaderCode = LoadBinaryFile("./vert.spv");
		vector<char> fragmentShaderCode = LoadBinaryFile("./frag.spv");
		
		cout<<"creating Shader Modules..."<<endl;
		cout<<"creating Vertex Shader Module..."<<endl;
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = vertexShaderCode.size();
		createInfo.pCode = (uint32_t*)vertexShaderCode.data();
		cout<<"calling vkCreateShaderModule..."<<endl;
		VkShaderModule VertexShaderModule;
		vkCreateShaderModule(device, &createInfo, NULL, &VertexShaderModule);
		
		cout<<"creating Fragment Shader Module..."<<endl;
		//VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = fragmentShaderCode.size();
		createInfo.pCode = (uint32_t*)fragmentShaderCode.data();
		cout<<"calling vkCreateShaderModule..."<<endl;
		VkShaderModule FragmentShaderModule;
		vkCreateShaderModule(device, &createInfo, NULL, &FragmentShaderModule);
		
		cout<<"creating shader Stages"<<endl;
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = NULL;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = NULL;
		
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable = false;
		
		VkViewport viewport = {};
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = 640;
		viewport.height = 480;
		viewport.minDepth = 0;
		viewport.maxDepth = 1;
		
		VkRect2D scissor = {};
		scissor.offset = {0,0};
		scissor.extent = { 640, 480 };
		
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
		
		VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = {};
		vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderStageCreateInfo.module = VertexShaderModule;
		vertexShaderStageCreateInfo.pName = "main";
		vertexShaderStageCreateInfo.pSpecializationInfo = NULL;
		
		VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = {};
		rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerCreateInfo.depthClampEnable = false;
		rasterizerCreateInfo.rasterizerDiscardEnable = false;
		rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizerCreateInfo.lineWidth = 1;
		rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizerCreateInfo.depthBiasEnable = false;
		rasterizerCreateInfo.depthBiasConstantFactor = 0;
		rasterizerCreateInfo.depthBiasClamp = 0;
		rasterizerCreateInfo.depthBiasSlopeFactor = 0;
		
		VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo = {};
		fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderStageCreateInfo.module = FragmentShaderModule;
		fragmentShaderStageCreateInfo.pName = "main";
		fragmentShaderStageCreateInfo.pSpecializationInfo = NULL;
		
		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional
		
		
		cout<<"done creating Graphics Pipeline!"<<endl<<endl;
	}
	void InitVulkan()
	{
		cout<<"Initializing Vulkan..."<<endl<<endl;
		InitWindow();
		CreateInstance();
		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateSurface();
		CreateSemaphore();
		CreateSwapChain();
		CreateImageViews();
		CreateGraphicsPipeline();
		cout<<"Vulkan Initialized!"<<endl<<endl;
	}
	void DestroyVulkan()
	{
		cout<<"Destroying Vulkan(reversed order)..."<<endl;
		
		cout<<"Destroying SwapChain..."<<endl;
		vkDestroySwapchainKHR(device, swapchain, NULL);
		cout<<"SwapChain destroyed!"<<endl;
		
		cout<<"Destroying Surface..."<<endl;
		vkDestroySurfaceKHR(instance, surface, NULL);
		cout<<"Surfface destroyed!"<<endl;
		
		cout<<"Destroying Device..."<<endl;
		vkDestroyDevice(device, NULL);
		cout<<"Device destroyed!"<<endl;
		
		cout<<"Destroying Instance..."<<endl;
		vkDestroyInstance(instance, NULL);
		cout<<"Instance destroyed!"<<endl;
		
		cout<<"Vulkan destroyed!"<<endl<<endl;
	}
};

class cProgram
{
	protected:
	VulkanEngine Graphics;
	
	void MainLoop()
	{
		while(!Graphics.ShouldClose())
		{
			Graphics.PollEvents();
		}
	}
	
	public:
	void Run()
	{
		Graphics.InitVulkan();
		MainLoop();
		Graphics.DestroyVulkan();
	}
};

int main()
{
	cProgram MainApplication;
	
	MainApplication.Run();
	
	return 0;
}
