$ErrorActionPreference = "Stop"

# Use MSYS2 UCRT64 toolchain
$ucrt = "C:\msys64\ucrt64\bin"
$env:Path = "$ucrt;$env:Path"

cmake -S . -B build -G "Ninja" `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_C_COMPILER="$ucrt\gcc.exe" `
  -DCMAKE_CXX_COMPILER="$ucrt\g++.exe" `
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON `
  -DCMAKE_PREFIX_PATH="C:/msys64/ucrt64"

cmake --build build --config Release