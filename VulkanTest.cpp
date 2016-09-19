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
		}
		cout<<"setting (main) physical device to device 0..."<<endl;
		physicalDevice = devices[0]; //quick and dirty lol
		cout<<"physical Device picked!"<<endl<<endl;
	}
	void InitVulkan()
	{
		cout<<"Initializing Vulkan..."<<endl<<endl;
		InitWindow();
		CreateInstance();
		PickPhysicalDevice();
		cout<<"Vulkan Initialized!"<<endl<<endl;
	}
	void DestroyVulkan()
	{
		cout<<"Destroying Vulkan..."<<endl;
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
