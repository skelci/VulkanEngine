# Vulkan Engine

## Overview
This project is a Vulkan-based game engine designed to provide a framework for developing graphics-intensive applications and games. It utilizes modern C++ features and leverages Vulkan for high-performance rendering.

## Getting Started
1. **Prerequisites**:
   refer to the [Installing C++ libraries on Windows](#installing-c-libraries-on-windows) section for setup instructions.

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/skelci/VulkanEngine.git
   cd VulkanEngine
   ```

3. **Build the Project**:
   - Open a terminal in the project directory.
   - Run the following command to configure and build the project:
   ```bash
   ./scripts/build.ps1
   ```

4. **Run the Application**:
   - After building, navigate to the `build` directory and run the executable.
   - Or:
   ```bash
   ./build/VulkanEngine.exe
   ```

## Contributing
Contributions are welcome! Please open an issue or submit a pull request for any enhancements or bug fixes.

Format code as follows:
`{ PointerAlignment: Left, MaxEmptyLinesToKeep: 3, BasedOnStyle: LLVM, UseTab: Never, IndentWidth: 4, TabWidth: 4, BreakBeforeBraces: Attach, AllowShortIfStatementsOnASingleLine: true, IndentCaseLabels: false, ColumnLimit: 0, AccessModifierOffset: -4, NamespaceIndentation: All, FixNamespaceComments: false }`

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## Installing C++ libraries on Windows
1. **Install MSYS2**:
    ```bash
    winget install -e --id MSYS2.MSYS2
    ```
2. **Open “MSYS2 UCRT64”** from Start menu and update:
    ```bash
    pacman -Syu
    # close, reopen "MSYS2 UCRT64", run again:
    pacman -Syu
    ```
3. **Install GCC toolchain + Vulkan packages (UCRT64)**:
    ```bash
    pacman -S --needed \
      mingw-w64-ucrt-x86_64-gcc \
      mingw-w64-ucrt-x86_64-cmake \
      mingw-w64-ucrt-x86_64-ninja \
      mingw-w64-ucrt-x86_64-gdb \
      mingw-w64-ucrt-x86_64-glm \
      mingw-w64-ucrt-x86_64-glfw \
      mingw-w64-ucrt-x86_64-glslang \
      mingw-w64-ucrt-x86_64-shaderc \
      mingw-w64-ucrt-x86_64-vulkan-headers \
      mingw-w64-ucrt-x86_64-vulkan-loader \
      mingw-w64-ucrt-x86_64-vulkan-validation-layers
    ```