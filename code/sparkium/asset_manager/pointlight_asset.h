#pragma once
#include "sparkium/asset_manager/asset_manager_utils.h"
#include <glm/glm.hpp>

namespace sparkium {

struct PointLight {
  glm::vec3 position;
  glm::vec3 color;
  float intensity;
};

struct PointLightAsset {
  PointLight light;
  std::unique_ptr<vulkan::StaticBuffer<PointLight>> light_buffer_;
  std::string name_;
};

}  // namespace sparkium