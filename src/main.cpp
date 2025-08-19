#include <vulkan/vulkan.h>
#include <cstdio>

int main() {
    uint32_t ver = 0;
    vkEnumerateInstanceVersion(&ver);
    printf("Vulkan version: %u.%u.%u\n",
           VK_VERSION_MAJOR(ver), VK_VERSION_MINOR(ver), VK_VERSION_PATCH(ver));
    return 0;
}