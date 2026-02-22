#pragma once

#include "AssetBase.hpp"
#include "EngineStatics.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Vector.hpp"

#include <vulkan/vulkan.h>

#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

struct SShaderProperty;


class CMaterial : public CAssetBase {
    CLASS_BODY(CMaterial, CAssetBase)

public:
    ~CMaterial();

    VkPipeline GetPipeline() const { return Pipeline; }
    VkPipelineLayout GetPipelineLayout() const { return PipelineLayout; }
    VkDescriptorSet GetDescriptorSet() const { return DescriptorSet; }

    template <typename T>
    void SetProperty(const std::string& name, const T& value) {
        const SShaderProperty* prop = GetShaderProperty(name);
        if (!prop) {
            Log("Material", ELogLevel::Warning, "Trying to set unknown property '" + name + "' on material.");
            return;
        }

        bool typeMatch = false;
        if constexpr (std::is_same_v<T, float>) {
            if (prop->Type == EShaderPropertyType::Float) typeMatch = true;
        } else if constexpr (std::is_same_v<T, SVector2>) {
            if (prop->Type == EShaderPropertyType::Vec2) typeMatch = true;
        } else if constexpr (std::is_same_v<T, SVector3>) {
            if (prop->Type == EShaderPropertyType::Vec3) typeMatch = true;
        } else if constexpr (std::is_same_v<T, SVector4>) {
            if (prop->Type == EShaderPropertyType::Vec4) typeMatch = true;
        } else if constexpr (std::is_same_v<T, std::shared_ptr<CTexture>>) {
            if (prop->Type == EShaderPropertyType::Texture) typeMatch = true;
        }

        if (typeMatch) {
            Properties[name] = value;
            bDirty = true;
        } else {
            const std::string enumToString[] = {"Float", "Vector2", "Vector3", "Color", "Texture"};
            Log("Material", ELogLevel::Error,
                "Type mismatch for property '" + name + "'. Expected type: " +
                    enumToString[static_cast<int>(prop->Type)] + ", provided type: " + typeid(T).name());
        }
    }

    bool IsValid() const { return Pipeline != VK_NULL_HANDLE; }

    // Call this before drawing
    void UpdateBuffer();

protected:
    void LoadFromFile(const std::string& FilePath) override;

private:
    std::shared_ptr<CShader> Shader;

    std::map<std::string, std::variant<float, SVector2, SVector3, SVector4, std::shared_ptr<CTexture>>> Properties;

    VkPipeline Pipeline = VK_NULL_HANDLE;
    VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;

    VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;
    VkDescriptorSet DescriptorSet = VK_NULL_HANDLE;

    VkBuffer UniformBuffer = VK_NULL_HANDLE;
    VkDeviceMemory UniformBufferMemory = VK_NULL_HANDLE;
    void* UniformBufferMapped = nullptr;

    bool bDirty = true;

    void CreatePipeline();
    void CreateDescriptorSet();
    void CreateUniformBuffer();

    const SShaderProperty* GetShaderProperty(const std::string& name) const;
};
