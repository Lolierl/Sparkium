#ifndef ENTITY_METADATA_GLSL
#define ENTITY_METADATA_GLSL

struct EntityMetadata {
  mat4 model;
  uint entity_id;
  uint mesh_id;
  uint albedo_texture_id;
  uint roughness_texture_id;
  uint normal_texture_id;
  uint albedo_detail_texture_id;
  float emission_cdf;
  float padding0;
  vec4 detail_scale_offset;
  // align to 16 bytes
};

#endif
