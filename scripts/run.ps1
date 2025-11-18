$ErrorActionPreference = "Stop"

$ucrt = "C:\msys64\ucrt64\bin"
$env:Path = "$ucrt;$env:Path"

./scripts/build-debug.ps1
./scripts/compile-shaders.ps1
./build/VulkanEngine.exe
