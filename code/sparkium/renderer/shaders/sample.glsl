#ifndef SAMPLE_GLSL
#define SAMPLE_GLSL
#include "random.glsl"

/*All Sample Point Struct*/
struct LightSamplePoint
{
  vec3 position; 
  uint mesh_id, primitive_id; 
  float pdf; 
};
struct SamplePoint
{
  vec3 position;
  float pdf; 
};

vec3 UniformSampleTriangle(vec3 v0, vec3 v1, vec3 v2) 
{
    float u1 = RandomFloat();
    float u2 = RandomFloat();
    float sqrt_u1 = sqrt(u1);
    float b0 = 1.0 - sqrt_u1;
    float b1 = sqrt_u1 * (1.0 - u2);
    float b2 = sqrt_u1 * u2;
    return b0 * v0 + b1 * v1 + b2 * v2;
}

float TriangleArea(vec3 v0, vec3 v1, vec3 v2) 
{
    vec3 e1 = v1 - v0; 
    vec3 e2 = v2 - v0; 
    return 0.5 * length(cross(e1, e2)); 
}
vec3 TriangleNormal(vec3 v0, vec3 v1, vec3 v2) {
    vec3 e1 = v1 - v0; 
    vec3 e2 = v2 - v0; 
    return normalize(cross(e1, e2)); 
}

/*Light Sample*/
LightSamplePoint SampleDirectLighting() 
{
  float u = RandomFloat();
  uint entity_id = 0; 
  for(; entity_id < scene_settings.num_entity; entity_id++)
    if(metadatas[entity_id].emission_cdf > u)break; 
  uint mesh_id = metadatas[entity_id].mesh_id; 
  u = RandomFloat();
  int primitive_id = 0; 
  for(; primitive_id < mesh_metadatas[mesh_id].num_index / 3; primitive_id++)
    if(mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id] > u)break; 
  uint v0_id = index_buffers[mesh_id].indices[primitive_id * 3 + 0]; 
  uint v1_id = index_buffers[mesh_id].indices[primitive_id * 3 + 1]; 
  uint v2_id = index_buffers[mesh_id].indices[primitive_id * 3 + 2]; 
  Vertex v0 = GetVertex(mesh_id, v0_id); 
  Vertex v1 = GetVertex(mesh_id, v1_id); 
  Vertex v2 = GetVertex(mesh_id, v2_id); 
  vec3 v = UniformSampleTriangle(v0.position, v1.position, v2.position); 
  LightSamplePoint ret; 
  ret.position = v; 
  ret.mesh_id = mesh_id; 
  ret.primitive_id = primitive_id; 
  int pre_entity_id = 0;
  for(; pre_entity_id < entity_id; pre_entity_id++)
    if(metadatas[pre_entity_id].emission_cdf > 0.0)break; 
  float entity_pdf = metadatas[entity_id].emission_cdf - ((pre_entity_id == entity_id) ? 0 : metadatas[pre_entity_id].emission_cdf); 
  float mesh_pdf = mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id] - ((primitive_id == 0) ? 0 : mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id - 1]); 
  ret.pdf = entity_pdf * mesh_pdf / TriangleArea(v0.position, v1.position, v2.position); 
  return ret; 
}

/*Texture Sample*/
vec4 SampleTextureLinear(uint texture_id, vec2 uv) {
  return texture(sampler2D(sampled_textures[texture_id], samplers[0]), uv);
}

vec4 SampleTextureNearest(uint texture_id, vec2 uv) {
  return texture(sampler2D(sampled_textures[texture_id], samplers[1]), uv);
}

vec3 EnvmapSample(vec3 direction) {
  return texture(
             sampler2D(sampled_textures[envmap_data.envmap_id], samplers[0]),
             SampleEnvmapUV(direction))
             .xyz *
         envmap_data.scale;
}

/*Shape SampleS*/
vec3 UniformSampleHemisphere(vec3 normal) 
{
    float u1 = float(RandomUint()) / float(0xFFFFFFFFu);
    float u2 = float(RandomUint()) / float(0xFFFFFFFFu);

    float theta = 0.5 * PI * u1; 
    float phi = 2.0 * PI * u2;     

    vec3 local_dir = vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));

    vec3 tangent = normalize(cross(abs(normal.x) > 0.1 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0), normal));
    vec3 bitangent = cross(normal, tangent);

    return local_dir.x * tangent + local_dir.y * bitangent + local_dir.z * normal;
}

/*Radiance SampleS*/
SamplePoint CosineSampleHemisphere(vec3 normal)
{
    float u1 = float(RandomUint()) / float(0xFFFFFFFFu);
    float u2 = float(RandomUint()) / float(0xFFFFFFFFu);

    float r = sqrt(u1);
    float theta = 2.0 * PI * u2;

    float x = r * cos(theta);
    float y = r * sin(theta);
    float z = sqrt(1.0 - u1);

    vec3 local_dir = vec3(x, y, z);

    vec3 tangent = normalize(cross(abs(normal.x) > 0.1 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0), normal));
    vec3 bitangent = cross(normal, tangent);

    SamplePoint ret;
    ret.position = local_dir.x * tangent + local_dir.y * bitangent + local_dir.z * normal;
    ret.position = normalize(ret.position);
    ret.pdf = z / PI; // Cosine-weighted hemisphere PDF
    return ret;
}

vec3 GGXsampleMicroNormal(float alpha, vec3 N)
{
	vec2 rand = vec2(RandomFloat(), RandomFloat());
	float xi_1 = rand.x;
	float xi_2 = rand.y;

	float theta = atan(alpha * sqrt(xi_1) / sqrt(1 - xi_1));
	float phi = 2 * PI * xi_2;
	vec3 NotN = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 T = normalize(cross(NotN, N));	
	vec3 B = cross(N, T);		

	return T * (sin(theta) * cos(phi)) +
		B * (sin(theta) * sin(phi)) +
		N * cos(theta);
}
#endif