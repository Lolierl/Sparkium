#ifndef MATERIAL_GLSL
#define MATERIAL_GLSL

#define MATERIAL_TYPE_LAMBERTIAN 0
#define MATERIAL_TYPE_SPECULAR 1
#define MATERIAL_TYPE_RETRACTIVE 2
#define MATERIAL_TYPE_METAL_ANISOTROPIC 3
#define MATERIAL_TYPE_ISOTROPIC_RETRACTIVE 4
#define MATERIAL_TYPE_METAL 5
#define MATERIAL_TYPE_VOLUME 6
#define MATERIAL_TYPE_MULTILAYER 7
#define MATERIAL_TYPE_NONMETAL 8
#define MATERIAL_TYPE_COLORED_RETRACTIVE 9

#define SPECTRUM_TYPE_D65 100
#define SPECTRUM_TYPE_D75 101
#define SPECTRUM_TYPE_D50 102
#define SPECTRUM_TYPE_SODIUM 103

#define ILLUMINANT_TYPE_LAMBERTIAN 200

struct Material {
  vec3 base_color;
  float subsurface;

  vec3 subsurface_radius;
  float metallic;

  vec3 subsurface_color;
  float specular;

  float specular_tint;
  float roughness;
  float anisotropic;
  float anisotropic_rotation;

  float sheen;
  float sheen_tint;
  float clearcoat;
  float clearcoat_roughness;

  float ior;
  float transmission;
  float transmission_roughness;
  float emission_strength;

  vec3 emission;
  float alpha;

  float sigma_a;
  float sigma_s;
  float g;
  float volume_emission_strength;

  float a;
  float b;
  float c;
  uint spectrum_type;

  vec3 illuminant_dir;
  uint illuminant_type;
  
  vec3 normal;
  uint type;
};

#endif
