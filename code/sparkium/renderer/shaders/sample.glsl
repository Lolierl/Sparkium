#ifndef SAMPLE_GLSL
#define SAMPLE_GLSL
#include "random.glsl"
#include "material.glsl"
#include "bxdf.glsl"

/*All Sample Point Struct*/
struct LightSamplePoint
{
  vec3 position; 
  uint mesh_id, primitive_id; 
  float pdf; 
};

struct SampleDirection
{
  vec3 direction;
  float pdf; 
};

struct SamplePoint
{
  vec3 position;
  float pdf; 
};

vec3 perpendicular(vec3 normal) {
  // Pick a reference vector different from the normal
  vec3 reference = abs(normal.x) < 0.99 ? vec3(1.0, 0.0, 0.0) : vec3(0.0, 1.0, 0.0);
  
  // Compute a vector orthogonal to the normal
  vec3 tangent = normalize(cross(normal, reference));
  return tangent;
}

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

/*Volume Sample*/
float SampleDistance(Material material) {
  float cdf =  RandomFloat();
  return -log(1 - cdf) / material.alpha;
}

vec3 SampleHenyeyGreenstein(Material material, vec3 in_direction) {
  float g = 0.9; // Henyey-Greenstein parameter
  float u1 = RandomFloat();
  float u2 = RandomFloat();

  float cos_theta;
  if (abs(g) < 1e-3) {
    cos_theta = 1.0 - 2.0 * u1;
  } else {
    float term = (1.0 - g * g) / (1.0 + g - 2.0 * g * u1);
    cos_theta = (1.0 + g * g - term * term) / (2.0 * g);
  }

  float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
  float phi = 2.0 * PI * u2;
  float cos_phi = cos(phi);
  float sin_phi = sin(phi);

  vec3 direction;
  direction.x = sin_theta * cos_phi;
  direction.y = sin_theta * sin_phi;
  direction.z = cos_theta;

  // Transform direction to the coordinate system of in_direction
  vec3 w = normalize(in_direction);
  vec3 u = normalize(cross(vec3(0.0, 1.0, 0.0), w));
  vec3 v = cross(w, u);

  return direction.x * u + direction.y * v + direction.z * w;
} 

/*Light Sample*/
LightSamplePoint SampleDirectLighting(vec3 origin) 
{
  float u = RandomFloat();
  uint entity_id = 0; 
  for(; entity_id < scene_settings.num_entity; entity_id++)
    if(metadatas[entity_id].emission_cdf > u)break; 

  float entity_pdf = metadatas[entity_id].emission_cdf - ((entity_id == 0) ? 0 : metadatas[entity_id - 1].emission_cdf); 
  Material material = materials[entity_id]; 
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

/*Shape Sample*/
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

/*Radiance Sample*/
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

SampleDirection SampleLambertianTransportDirection(vec3 normal_direction) {
  float u1 = float(RandomUint()) / float(0xFFFFFFFFu);
  float u2 = float(RandomUint()) / float(0xFFFFFFFFu);

  float r = sqrt(u1);
  float theta = 2.0 * PI * u2;

  float x = r * cos(theta);
  float y = r * sin(theta);
  float z = sqrt(1.0 - u1);

  vec3 local_dir = vec3(x, y, z);

  vec3 tangent = normalize(cross(abs(normal_direction.x) > 0.1 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0), normal_direction));
  vec3 bitangent = cross(normal_direction, tangent);

  SampleDirection ret;
  ret.direction = local_dir.x * tangent + local_dir.y * bitangent + local_dir.z * normal_direction;
  ret.direction = normalize(ret.direction);
  ret.pdf = z / PI; // Cosine-weighted hemisphere PDF
  return ret;
}

SampleDirection SampleSpecularTransportDirection(vec3 in_direction, vec3 normal_direction) {
  if(dot(in_direction, normal_direction) > 0)
    normal_direction = -normal_direction;
  SampleDirection ret;
  ret.pdf = 1;
  ret.direction = ReflectionDirection(normal_direction, in_direction);
  return ret;
}
SampleDirection SampleRetractiveTransportDirection(Material material, vec3 in_direction, vec3 normal, float inside) {
  SampleDirection ret; 
  ret.pdf = 1;

  vec3 reflection_direction = ReflectionDirection(normal, in_direction); 
  float etap = (inside < 1e-3) ? material.ior : 1.0 / material.ior;
  vec3 retraction_direction = RefractionDirection(normal, in_direction, etap); 
  float ratio = FresnelReflectionRate(in_direction, normal, etap); 
  if(RandomFloat() < ratio)
  {
    ret.pdf = ratio;
    ret.direction = reflection_direction; 
  }
  else
  {
    ret.pdf = 1 - ratio;
    ret.direction = retraction_direction;
  }
  return ret;
}

SampleDirection SampleMetalTransportDirection(Material material, vec3 in_direction, vec3 normal) {
  // Step 1: Generate random values
  float u1 = RandomFloat(); // Random number in [0, 1]
  float u2 = RandomFloat(); // Random number in [0, 1]

  // Step 2: GGX sampling (spherical coordinates)
  float alpha = material.roughness * material.roughness;
  float theta = atan(alpha * sqrt(u1 / (1.0 - u1))); // GGX theta
  float phi = 2.0 * PI * u2;                        // Uniform azimuthal angle

  // Convert spherical to Cartesian coordinates
  float sinTheta = sin(theta);
  float cosTheta = cos(theta);
  vec3 h = vec3(
      sinTheta * cos(phi),
      sinTheta * sin(phi),
      cosTheta
  );

  // Transform half-vector h to world space
  vec3 tangent = normalize(cross(abs(normal.x) > 0.1 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0), normal));  // Generate tangent vector
  vec3 bitangent = cross(normal, tangent);
  mat3 tbn = mat3(tangent, bitangent, normal);      // TBN matrix
  h = normalize(tbn * h);

  // Step 3: Reflect incoming direction around the half-vector
  vec3 outDir = ReflectionDirection(h, in_direction);

  // Step 4: Compute the PDF
  float dotNH = max(dot(normal, h), 0.0);
  float D = (alpha * alpha) / (PI * pow((dotNH * dotNH) * (alpha * alpha - 1.0) + 1.0, 2.0));
  float pdf = D * dotNH / (4.0 * max(dot(outDir, h), 0.0));

  SampleDirection ret;
  ret.pdf = pdf;
  ret.direction = normalize(outDir);

  return ret;

}

/*Handlers*/
SampleDirection SampleTransportDirection(Material material, vec3 in_direction, vec3 normal_direction, float inside) {
  if (material.type == MATERIAL_TYPE_LAMBERTIAN) {
    return SampleLambertianTransportDirection(normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_SPECULAR) {
    return SampleSpecularTransportDirection(in_direction, normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_RETRACTIVE) {
    return SampleRetractiveTransportDirection(material, in_direction, normal_direction, inside);
  }
  else if (material.type == MATERIAL_TYPE_METAL) {
    // return SampleLambertianTransportDirection(normal_direction);
    return SampleMetalTransportDirection(material, in_direction, normal_direction);
  }
}

vec3 SamplePhaseFunction(Material material, vec3 in_direction) {
  if (material.type == MATERIAL_TYPE_VOLUME) {
    return in_direction;
  }
}

#endif