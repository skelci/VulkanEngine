$ErrorActionPreference = "Stop"

$ucrt = "C:\msys64\ucrt64\bin"
$env:Path = "$ucrt;$env:Path"

glslc shaders/shader.vert -o shaders/shader.vert.spv
glslc shaders/shader.frag -o shaders/shader.frag.spv
