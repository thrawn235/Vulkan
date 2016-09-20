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
		queueFamilyIndex = 0;
		cout<<"physical Device picked!"<<endl<<endl;
	}
	void CreateLogicalDevice()
	{
		cout<<"creating logical Device..."<<endl<<endl;
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
		
		cout<<"calling vkCreateDevice..."<<endl;
		vkCreateDevice(physicalDevice, &createInfo, NULL, &device);
		cout<<"logical Device created!"<<endl;
		
		cout<<"trying to get graphics Queue..."<<endl;
		vkGetDeviceQueue(device, queueFamilyIndex, 0, &graphicsQueue);
		cout<<"Queue retrieved!"<<endl;
		cout<<"logical Device creation finished!"<<endl<<endl;
	}
	void InitVulkan()
	{
		cout<<"Initializing Vulkan..."<<endl<<endl;
		InitWindow();
		CreateInstance();
		PickPhysicalDevice();
		CreateLogicalDevice();
		cout<<"Vulkan Initialized!"<<endl<<endl;
	}
	void DestroyVulkan()
	{
		cout<<"Destroying Vulkan(reversed order)..."<<endl;
		
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
