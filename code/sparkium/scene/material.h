#pragma once
#include "sparkium/scene/scene_utils.h"

namespace sparkium {

// Definition of material type ids
constexpr uint32_t MATERIAL_TYPE_LAMBERTIAN = 0;
constexpr uint32_t MATERIAL_TYPE_SPECULAR = 1;
constexpr uint32_t MATERIAL_TYPE_RETRACTIVE = 2;
constexpr uint32_t MATERIAL_TYPE_ISOTROPIC_RETRACTIVE = 4;
constexpr uint32_t MATERIAL_TYPE_METAL = 5;
constexpr uint32_t MATERIAL_TYPE_VOLUME = 6;
constexpr uint32_t MATERIAL_TYPE_MULTILAYER = 7;
constexpr uint32_t MATERIAL_TYPE_NONMETAL = 8;
constexpr uint32_t MATERIAL_TYPE_COLORED_RETRACTIVE = 9;

constexpr uint32_t SPECTRUM_TYPE_D65 = 100;
constexpr uint32_t SPECTRUM_TYPE_D75 = 101;
constexpr uint32_t SPECTRUM_TYPE_D50 = 102;
constexpr uint32_t SPECTRUM_TYPE_SODIUM = 103;

constexpr uint32_t ILLUMINANT_TYPE_LAMBERTIAN = 200;
constexpr uint32_t ILLUMINANT_TYPE_PARALLEL = 201;

// clang-format off
struct Material {
  glm::vec3 base_color{1.0f};
  float subsurface{0.0f};

  glm::vec3 subsurface_radius{1.0f};
  float metallic{0.0f};

  glm::vec3 subsurface_color{1.0f};
  float specular{0.0f};

  float specular_tint{0.0f};
  float roughness{0.05f};
  float anisotropic{0.0f};
  float anisotropic_rotation{0.0f};

  float sheen{0.0f};
  float sheen_tint{0.0f};
  float clearcoat{0.0f};
  float clearcoat_roughness{0.0f};

  float ior{1.15f};
  float transmission{0.0f};
  float transmission_roughness{0.0f};
  float emission_strength{0.0f};

  glm::vec3 emission{1.0f};
  float alpha{0.0f};

  float sigma_a{0.0f};
  float sigma_s{0.0f};
  float g{0.0f};
  float volume_emission_strength{0.0f};

  float a{ior};
  float b{0.0f};
  float c{0.0f};
  
  uint32_t spectrum_type{0};

  glm::vec3 illuminant_dir{0.5f, 0.5f, 1.0f};
  uint32_t illuminant_type{0};

  glm::vec3 normal{0.5f, 0.5f, 1.0f};
  uint32_t type{0};

  // This structure needs to be padded to 16 bytes
};
// clang-format on

}  // namespace sparkium
