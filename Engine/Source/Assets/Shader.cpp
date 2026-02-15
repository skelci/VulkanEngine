#include "Shader.hpp"

#include "EngineStatics.hpp"
#include "Log.hpp"
#include "Renderer.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>


CShader::~CShader() {
    VkDevice device = GEngine->GetRenderer()->GetDevice();
    if (VertexShaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device, VertexShaderModule, nullptr);
    }
    if (FragmentShaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device, FragmentShaderModule, nullptr);
    }
}

std::string CShader::ReadFileContent(const std::string& path) {
    if (!std::filesystem::exists(path)) return "";
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void CShader::SaveToFile(const std::string& path, const std::string& content) {
    std::filesystem::path p(path);
    std::filesystem::create_directories(p.parent_path());

    std::ofstream out(path);
    out << content;
    out.close();
}

std::vector<char> CShader::ReadBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        Log("Shader", ELogLevel::Error, "Failed to open binary file: " + filename);
        return {};
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

void CShader::LoadFromFile(const std::string& FilePath) {
    std::string fileContent = ReadFileContent(FilePath);
    if (fileContent.empty()) {
        Log("Shader", ELogLevel::Error, "Failed to load shader source: " + FilePath);
        return;
    }

    // --- PARSING STAGE ---
    Properties.clear();
    UBOSize = 0;
    std::string userCode;
    std::stringstream ss(fileContent);
    std::string line;
    int textureBinding = 1; // Start after UBO (binding 0)
    int uboOffset = 0;

    while (std::getline(ss, line)) {
        size_t propPos = line.find("// Property");
        if (propPos != std::string::npos) {
            // Format: // Property <Type> <Name> [=] [DefaultValue]
            std::string content = line.substr(propPos + 11);
            std::stringstream propSS(content);
            std::string typeStr, name;
            propSS >> typeStr >> name;

            if (typeStr.empty() || name.empty()) continue;

            std::string defaultValueStr;
            std::getline(propSS, defaultValueStr);

            // Trim leading/trailing whitespace
            size_t first = defaultValueStr.find_first_not_of(" \t");
            if (std::string::npos != first) {
                size_t last = defaultValueStr.find_last_not_of(" \t");
                defaultValueStr = defaultValueStr.substr(first, last - first + 1);
            } else {
                defaultValueStr = "";
            }

            if (!defaultValueStr.empty()) {
                // If user used '=', remove it
                if (defaultValueStr[0] == '=') {
                    defaultValueStr = defaultValueStr.substr(1);
                    // Trim again after removing '='
                    first = defaultValueStr.find_first_not_of(" \t");
                    if (std::string::npos != first) {
                        size_t last = defaultValueStr.find_last_not_of(" \t");
                        defaultValueStr = defaultValueStr.substr(first, last - first + 1);
                    }
                }

                DefaultValues[name] = defaultValueStr;
            }

            SShaderProperty prop;
            prop.Name = name;

            if (typeStr == "float") {
                prop.Type = EShaderPropertyType::Float;
                prop.Size = 4; // std140 float alignment is 4 bytes
                prop.Offset = uboOffset;
                uboOffset += 4;
            } else if (typeStr == "vec2") {
                prop.Type = EShaderPropertyType::Vec2;
                // std140 vec2 alignment is 8 bytes
                if (uboOffset % 8 != 0) uboOffset += (8 - (uboOffset % 8));
                prop.Size = 8;
                prop.Offset = uboOffset;
                uboOffset += 8;
            } else if (typeStr == "vec3") {
                prop.Type = EShaderPropertyType::Vec3;
                // std140 vec3 alignment is 16 bytes
                if (uboOffset % 16 != 0) uboOffset += (16 - (uboOffset % 16));
                prop.Size = 12; // vec3 is 12 bytes but aligned to 16
                prop.Offset = uboOffset;
                uboOffset += 16;
            } else if (typeStr == "vec4") {
                prop.Type = EShaderPropertyType::Vec4;
                if (uboOffset % 16 != 0) uboOffset += (16 - (uboOffset % 16));
                prop.Size = 16;
                prop.Offset = uboOffset;
                uboOffset += 16;
            } else if (typeStr == "texture") {
                prop.Type = EShaderPropertyType::Texture;
                prop.Size = 0;
                prop.Offset = 0;
                prop.Binding = textureBinding++;
            }

            Properties.push_back(prop);
        } else {
            userCode += line + "\n";
        }
    }

    // Pad UBO struct for std140 (must be multiple of 16)
    if (uboOffset > 0 && uboOffset % 16 != 0) {
        uboOffset += (16 - (uboOffset % 16));
    }
    UBOSize = uboOffset;

    // --- VERTEX GENERATION ---
    std::stringstream vertSS;
    vertSS << "#version 450\n\n";
    vertSS << "layout(set = 0, binding = 0) uniform CameraBuffer {\n";
    vertSS << "    mat4 view;\n";
    vertSS << "    mat4 proj;\n";
    vertSS << "} camera;\n\n";

    // Inject Material UBO if needed
    if (UBOSize > 0) {
        vertSS << "layout(std140, set = 1, binding = 0) uniform MaterialBuffer {\n";
        for (const auto& prop : Properties) {
            if (prop.Type != EShaderPropertyType::Texture) {
                std::string typeName;
                if (prop.Type == EShaderPropertyType::Float) typeName = "float";
                if (prop.Type == EShaderPropertyType::Vec2) typeName = "vec2";
                if (prop.Type == EShaderPropertyType::Vec3) typeName = "vec3";
                if (prop.Type == EShaderPropertyType::Vec4) typeName = "vec4";
                vertSS << "    " << typeName << " " << prop.Name << ";\n";
            }
        }
        vertSS << "} mat;\n\n";
    }

    // Inject Textures (Binding)
    for (const auto& prop : Properties) {
        if (prop.Type == EShaderPropertyType::Texture) {
            vertSS << "layout(set = 1, binding = " << prop.Binding << ") uniform sampler2D " << prop.Name << ";\n";
        }
    }

    vertSS << "layout(push_constant) uniform PushConstants {\n";
    vertSS << "    mat4 model;\n";
    vertSS << "} push;\n\n";
    vertSS << "layout(location = 0) in vec3 inPosition;\n";
    vertSS << "layout(location = 1) in vec3 inColor;\n";
    vertSS << "layout(location = 2) in vec2 inTexCoord;\n";
    vertSS << "layout(location = 3) in vec3 inNormal;\n\n";
    vertSS << "layout(location = 0) out vec3 fragColor;\n";
    vertSS << "layout(location = 1) out vec2 fragTexCoord;\n";
    vertSS << "layout(location = 2) out vec3 fragNormal;\n\n";

    // Inject user code
    vertSS << "#define VERTEX\n";
    vertSS << "// User Code Begin\n";
    vertSS << userCode << "\n";
    vertSS << "// User Code End\n\n";

    // Inject default if missing
    if (userCode.find("GetWPO") == std::string::npos) {
        vertSS << "vec3 GetWPO(vec3 pos) { return vec3(0.0); }\n\n";
    }

    vertSS << "void main() {\n";
    vertSS << "    vec3 offset = GetWPO(inPosition);\n";
    vertSS << "    gl_Position = camera.proj * camera.view * push.model * vec4(inPosition + offset, 1.0);\n";
    vertSS << "    fragColor = inColor;\n";
    vertSS << "    fragTexCoord = inTexCoord;\n";
    vertSS << "    mat3 normalMatrix = transpose(inverse(mat3(push.model)));\n";
    vertSS << "    fragNormal = normalize(normalMatrix * inNormal);\n";
    vertSS << "}\n";

    // --- FRAGMENT GENERATION ---
    std::stringstream fragSS;
    fragSS << "#version 450\n\n";

    // Inject Material Scope (Same layouts)
    if (UBOSize > 0) {
        fragSS << "layout(std140, set = 1, binding = 0) uniform MaterialBuffer {\n";
        for (const auto& prop : Properties) {
            if (prop.Type != EShaderPropertyType::Texture) {
                std::string typeName;
                if (prop.Type == EShaderPropertyType::Float) typeName = "float";
                if (prop.Type == EShaderPropertyType::Vec2) typeName = "vec2";
                if (prop.Type == EShaderPropertyType::Vec3) typeName = "vec3";
                if (prop.Type == EShaderPropertyType::Vec4) typeName = "vec4";
                fragSS << "    " << typeName << " " << prop.Name << ";\n";
            }
        }
        fragSS << "} mat;\n\n";
    }

    for (const auto& prop : Properties) {
        if (prop.Type == EShaderPropertyType::Texture) {
            fragSS << "layout(set = 1, binding = " << prop.Binding << ") uniform sampler2D " << prop.Name << ";\n";
        }
    }

    fragSS << "layout(location = 0) in vec3 fragColor;\n";
    fragSS << "layout(location = 1) in vec2 fragTexCoord;\n";
    fragSS << "layout(location = 2) in vec3 fragNormal;\n\n";
    fragSS << "layout(location = 0) out vec4 outColor;\n\n";

    // Inject user code
    fragSS << "#define FRAGMENT\n";
    fragSS << "// User Code Begin\n";
    fragSS << userCode << "\n";
    fragSS << "// User Code End\n\n";

    if (userCode.find("GetColor") == std::string::npos) {
        fragSS << "vec4 GetColor() { return vec4(fragColor, 1.0); }\n\n";
    }

    fragSS << "void main() {\n";
    fragSS << "    outColor = GetColor();\n";
    fragSS << "}\n";

    // Write temp files
    const std::filesystem::path sourcePath(FilePath);
    const std::filesystem::path parentDir = sourcePath.parent_path();
    const std::string stem = sourcePath.stem().string();

    std::filesystem::path saveDir = std::filesystem::path("Saved/Shaders") / parentDir;
    std::filesystem::create_directories(saveDir);

    std::string vertFileName = stem + "_Temp_Vertex.vert";
    std::string fragFileName = stem + "_Temp_Fragment.frag";

    std::string vertPath = (saveDir / vertFileName).string();
    std::string fragPath = (saveDir / fragFileName).string();

    SaveToFile(vertPath, vertSS.str());
    SaveToFile(fragPath, fragSS.str());

    // Compile
    std::string vertSpvName = stem + "_Temp_Vertex.spv";
    std::string fragSpvName = stem + "_Temp_Fragment.spv";

    std::string vertSpv = (saveDir / vertSpvName).string();
    std::string fragSpv = (saveDir / fragSpvName).string();

    std::string cmdVert = "glslc " + vertPath + " -o " + vertSpv;
    std::string cmdFrag = "glslc " + fragPath + " -o " + fragSpv;

    int retVert = std::system(cmdVert.c_str());
    if (retVert != 0) {
        Log("Shader", ELogLevel::Error, "Failed to compile vertex shader: " + FilePath);
        return;
    }

    int retFrag = std::system(cmdFrag.c_str());
    if (retFrag != 0) {
        Log("Shader", ELogLevel::Error, "Failed to compile fragment shader: " + FilePath);
        return;
    }

    // Load binaries
    auto vertCode = ReadBinaryFile(vertSpv);
    auto fragCode = ReadBinaryFile(fragSpv);

    if (vertCode.empty() || fragCode.empty()) return;

    // Create Modules
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    VkDevice device = GEngine->GetRenderer()->GetDevice();

    createInfo.codeSize = vertCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(vertCode.data());
    if (vkCreateShaderModule(device, &createInfo, nullptr, &VertexShaderModule) != VK_SUCCESS) {
        Log("Shader", ELogLevel::Error, "Failed to create vertex shader module!");
    }

    createInfo.codeSize = fragCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(fragCode.data());
    if (vkCreateShaderModule(device, &createInfo, nullptr, &FragmentShaderModule) != VK_SUCCESS) {
        Log("Shader", ELogLevel::Error, "Failed to create fragment shader module!");
    }
}
