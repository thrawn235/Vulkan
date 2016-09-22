#include <iostream>
#include <string>
#include <vector>

//#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>	//includes <vulkan/vulkan.h> if GLFW_INCLUDE_VULKAN is defined
using namespace std;

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
		/*VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
		swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.pNext = NULL:
		swapChainCreateInfo.flags = 0;
		swapChainCreateInfo.surface = surface;
		swapChainCreateInfo.minImageCount = 3;
		swapChainCreateInfo.imageFormat = format.format;
		swapChainCreateInfo.imageColorSpace = format.colorSpace;
		swapChainCreateInfo.imageExtend = { 640, 480 };
		swapChainCreateInfo.imageArrayLayers = 1;
		swapChainCreateInfo.imageUsage = 
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount =
		swapChainCreateInfo.pQueueFamilyIndices =
		swapChainCreateInfo.preTransform =
		swapChainCreateInfo.compositeAlpha =
		swapChainCreateInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		swapChainCreateInfo.clipped =
		swapChainCreateInfo.oldSwapChain =*/
		
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
		cout<<"Vulkan Initialized!"<<endl<<endl;
	}
	void DestroyVulkan()
	{
		cout<<"Destroying Vulkan(reversed order)..."<<endl;
		
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
