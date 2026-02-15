$ErrorActionPreference = "Stop"

$ucrt = "C:\msys64\ucrt64\bin"
$env:Path = "$ucrt;$env:Path"
$env:VK_LAYER_PATH = "C:/msys64/ucrt64/bin"
$env:VK_INSTANCE_LAYERS = "VK_LAYER_KHRONOS_validation"

./scripts/build-debug.ps1
./build/VulkanEngine.exe
