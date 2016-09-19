VULKAN_SDK_PATH = /home/sebastian/Programming/C++/VulkanSDK/1.0.26.0/x86_64
CFLAGS = -std=c++14 -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

VulkanTest: VulkanTest.cpp
	g++ $(CFLAGS) -o VulkanTest VulkanTest.cpp $(LDFLAGS)
	
test: VulkanTest
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/explicit_layer.d ./VulkanTest
	
clean:
	rm -f VulkanTest
