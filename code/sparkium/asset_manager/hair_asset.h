#pragma once
#include "sparkium/asset_manager/asset_manager_utils.h"

namespace sparkium {

struct HairMetadata {
  uint32_t num_curves;
  uint32_t num_control_points;
};

struct HairAsset {
  std::unique_ptr<vulkan::StaticBuffer<glm::vec3>> control_points_buffer_;
  std::unique_ptr<vulkan::AccelerationStructure> blas_;
  std::string name_;
  HairMetadata metadata_;
};

}  // namespace sparkium