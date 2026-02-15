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
    std::string userCode = ReadFileContent(FilePath);
    if (userCode.empty()) {
        Log("Shader", ELogLevel::Error, "Failed to load shader source: " + FilePath);
        return;
    }

    // Vertex Generation
    std::stringstream vertSS;
    vertSS << "#version 450\n\n";
    vertSS << "layout(set = 0, binding = 0) uniform UniformBufferObject {\n";
    vertSS << "    mat4 view;\n";
    vertSS << "    mat4 proj;\n";
    vertSS << "} ubo;\n\n";
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
    vertSS << "    gl_Position = ubo.proj * ubo.view * push.model * vec4(inPosition + offset, 1.0);\n";
    vertSS << "    fragColor = inColor;\n";
    vertSS << "    fragTexCoord = inTexCoord;\n";
    vertSS << "    mat3 normalMatrix = transpose(inverse(mat3(push.model)));\n";
    vertSS << "    fragNormal = normalize(normalMatrix * inNormal);\n";
    vertSS << "}\n";

    // Fragment Generation
    std::stringstream fragSS;
    fragSS << "#version 450\n\n";
    fragSS << "layout(set = 1, binding = 0) uniform sampler2D DiffuseTexture;\n\n";
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
        fragSS << "vec4 GetColor() { return texture(DiffuseTexture, fragTexCoord); }\n\n";
    }

    fragSS << "void main() {\n";
    fragSS << "    outColor = GetColor();\n";
    fragSS << "}\n";

    // Write temp files
    // Ensure directory exists
    std::filesystem::create_directories("Saved/Shaders");

    std::string vertPath = "Saved/Shaders/Temp_Vertex.vert";
    std::string fragPath = "Saved/Shaders/Temp_Fragment.frag";

    SaveToFile(vertPath, vertSS.str());
    SaveToFile(fragPath, fragSS.str());

    // Compile
    std::string vertSpv = "Saved/Shaders/Temp_Vertex.spv";
    std::string fragSpv = "Saved/Shaders/Temp_Fragment.spv";

    // Using glslc. assuming it's in path
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
