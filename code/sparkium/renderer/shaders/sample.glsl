#ifndef SAMPLE_GLSL
#define SAMPLE_GLSL
#include "random.glsl"
#include "material.glsl"
#include "bxdf.glsl"

/*All Sample Point Struct*/
struct SampleDirection
{
  vec3 direction;
  float pdf; 
};

struct SamplePoint
{
  vec3 position;
  vec3 normal;
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
  return -log(1 - cdf) / (material.sigma_a + material.sigma_s);
}

vec3 SampleHenyeyGreenstein(Material material, vec3 in_direction) {
  float g = material.g; // Henyey-Greenstein parameter
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

// /*Radiance Sample*/
// SamplePoint CosineSampleHemisphere(vec3 normal)
// {
//     float u1 = float(RandomUint()) / float(0xFFFFFFFFu);
//     float u2 = float(RandomUint()) / float(0xFFFFFFFFu);

//     float r = sqrt(u1);
//     float theta = 2.0 * PI * u2;

//     float x = r * cos(theta);
//     float y = r * sin(theta);
//     float z = sqrt(1.0 - u1);

//     vec3 local_dir = vec3(x, y, z);

//     vec3 tangent = normalize(cross(abs(normal.x) > 0.1 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0), normal));
//     vec3 bitangent = cross(normal, tangent);

//     SamplePoint ret;
//     ret.position = local_dir.x * tangent + local_dir.y * bitangent + local_dir.z * normal;
//     ret.position = normalize(ret.position);
//     ret.pdf = z / PI; // Cosine-weighted hemisphere PDF
//     return ret;
// }
/*Radiance Sample*/
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
SampleDirection SampleRetractiveTransportDirection(Material material, vec3 in_direction, vec3 normal_direction, float inside, float wave_length) {
  SampleDirection ret; 
  ret.pdf = 1;

  vec3 reflection_direction = ReflectionDirection(normal_direction, in_direction); 
  float n = material.a + material.b / pow(wave_length, 2.0) + material.c / pow(wave_length, 4.0);
  float etap = (inside < 1e-3) ? n : 1.0 / n;
  vec3 retraction_direction = RefractionDirection(normal_direction, in_direction, etap); 
  float ratio = FresnelReflectionRate(in_direction, normal_direction, etap); 
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
SampleDirection SampleAnisotropicMicrofacet(Material material, vec3 in_direction, vec3 normal) {
    // Step 1: Generate random values
    float u1 = RandomFloat(); // Random number in [0, 1]
    float u2 = RandomFloat(); // Random number in [0, 1]

    // Step 2: Compute anisotropic roughness in tangent space
    float alpha_x = material.roughness * (1.0 - material.anisotropic);
    float alpha_y = material.roughness * (1.0 + material.anisotropic);

    // Step 3: Anisotropic GGX sampling in tangent space
    float phi = atan(alpha_y / alpha_x * tan(2.0 * PI * u2)); // Skewed azimuthal angle
    if (u2 > 0.5) {
        phi += PI;
    }

    float cosTheta = sqrt((1.0 - u1) / (1.0 + (alpha_x * alpha_y - 1.0) * u1));
    float sinTheta = sqrt(max(0.0, 1.0 - cosTheta * cosTheta));

    vec3 h_tangent = vec3(
        sinTheta * cos(phi),
        sinTheta * sin(phi),
        cosTheta
    );

    // Step 4: Apply anisotropic rotation
    float rotation = material.anisotropic_rotation;
    float cosRot = cos(rotation);
    float sinRot = sin(rotation);
    vec3 h_rotated = vec3(
        cosRot * h_tangent.x - sinRot * h_tangent.y,
        sinRot * h_tangent.x + cosRot * h_tangent.y,
        h_tangent.z
    );

    // Step 5: Transform half-vector h to world space
    vec3 tangent = normalize(cross(abs(normal.x) > 0.1 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0), normal)); // Generate tangent vector
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal); // TBN matrix
    vec3 h_world = normalize(tbn * h_rotated);

    // Step 6: Reflect incoming direction around the half-vector
    vec3 outDir = ReflectionDirection(h_world, in_direction);

    // Step 7: Compute the PDF
    float dotNH = max(dot(normal, h_world), 0.0);
    float D = 1.0 / (PI * alpha_x * alpha_y * pow(pow(dotNH, 2.0) * (alpha_x * alpha_y - 1.0) + 1.0, 2.0));
    float pdf = D * dotNH / (4.0 * max(dot(outDir, h_world), 0.0));

    // Step 8: Return result
    SampleDirection ret;
    ret.pdf = pdf;
    ret.direction = normalize(outDir);
    return ret;
}

SampleDirection SampleMultilayerTransportDirection(Material material, vec3 in_direction, vec3 normal_direction)
{
  if(RandomFloat() < 0.9)
  {
    SampleDirection ret = SampleLambertianTransportDirection(normal_direction); 
    ret.pdf *= 0.9;
    return ret;
  }
  else
  {
    Material new_material = material; 
    new_material.roughness = material.clearcoat_roughness;
    new_material.base_color = vec3(0.04);
    SampleDirection ret = SampleMetalTransportDirection(new_material, in_direction, normal_direction); 
    ret.pdf *= 0.1;
    return ret;
  }
}

SampleDirection SampleColoredRetractiveTransportDirection(Material material, vec3 in_direction, vec3 normal_direction, float inside, float wave_length) {
  if (RandomFloat() > material.alpha) {
    return SampleRetractiveTransportDirection(material, in_direction, normal_direction, inside, wave_length);
  }
  return SampleLambertianTransportDirection(normal_direction);
}

// /*LighSource Sample*/
// SampleDirection SampleDirectLighting(vec3 origin) 
// {
//   float u = RandomFloat();
//   uint entity_id = 0; 
//   for(; entity_id < scene_settings.num_entity; entity_id++)
//     if(metadatas[entity_id].emission_cdf > u)break; 

//   uint mesh_id = metadatas[entity_id].mesh_id; 
  
//   u = RandomFloat();
//   int primitive_id = 0; 
//   for(; primitive_id < mesh_metadatas[mesh_id].num_index / 3; primitive_id++)
//     if(mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id] > u)break; 
//   uint v0_id = index_buffers[mesh_id].indices[primitive_id * 3 + 0]; 
//   uint v1_id = index_buffers[mesh_id].indices[primitive_id * 3 + 1]; 
//   uint v2_id = index_buffers[mesh_id].indices[primitive_id * 3 + 2]; 
//   Vertex v0 = GetVertex(mesh_id, v0_id); 
//   Vertex v1 = GetVertex(mesh_id, v1_id); 
//   Vertex v2 = GetVertex(mesh_id, v2_id); 
//   vec3 v = UniformSampleTriangle(v0.position, v1.position, v2.position); 
//   SampleDirection ret; 
//   ret.direction = normalize(v - origin); 
//   float r = distance(v, origin);
//   float normal = normalize(cross(v1.position - v0.position, v2.position - v0.position));

//   float mesh_pdf = mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id] - ((primitive_id == 0) ? 0 : mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id - 1]); 
//   ret.pdf = mesh_pdf / TriangleArea(v0.position, v1.position, v2.position) * ; 
//   return ret; 
// }


/*Handlers*/
SampleDirection SampleTransportDirection(vec3 origin, Material material, vec3 in_direction, vec3 normal_direction, float inside, float wave_length) {
  if (material.type == MATERIAL_TYPE_LAMBERTIAN) {    
    return SampleLambertianTransportDirection(normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_SPECULAR) {
    return SampleSpecularTransportDirection(in_direction, normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_RETRACTIVE) {
    return SampleRetractiveTransportDirection(material, in_direction, normal_direction, inside, wave_length);
  }
  else if (material.type == MATERIAL_TYPE_METAL) {
    return SampleMetalTransportDirection(material, in_direction, normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_METAL_ANISOTROPIC) {
    return SampleAnisotropicMicrofacet(material, in_direction, normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_NONMETAL) {
    return SampleLambertianTransportDirection(normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_MULTILAYER) {
    return SampleMultilayerTransportDirection(material, in_direction, normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_COLORED_RETRACTIVE) {
    return SampleColoredRetractiveTransportDirection(material, in_direction, normal_direction, inside, wave_length);
  }
}

vec3 SamplePhaseFunction(Material material, vec3 in_direction) {
  if (material.type == MATERIAL_TYPE_VOLUME) {
    return SampleHenyeyGreenstein(material, in_direction);
  }
}

// SamplePoint SampleTransportPosition(Material material, uint mesh_id, vec3 normal) {
//   if (material.type == MATERIAL_TYPE_VOLUME) {
//     float u = RandomFloat();
//     int primitive_id = 0; 
//     for(; primitive_id < mesh_metadatas[mesh_id].num_index / 3; primitive_id++)
//       if(mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id] > u)break; 
//     uint v0_id = index_buffers[mesh_id].indices[primitive_id * 3 + 0]; 
//     uint v1_id = index_buffers[mesh_id].indices[primitive_id * 3 + 1]; 
//     uint v2_id = index_buffers[mesh_id].indices[primitive_id * 3 + 2]; 
//     Vertex v0 = GetVertex(mesh_id, v0_id); 
//     Vertex v1 = GetVertex(mesh_id, v1_id); 
//     Vertex v2 = GetVertex(mesh_id, v2_id); 
//     vec3 v = UniformSampleTriangle(v0.position, v1.position, v2.position);
//     vec3 new_normal =
//       normalize(transpose(inverse(entity_transform)) *
//                 cross(v1.position - v0.position, v2.position - v0.position));
//     SamplePoint ret; 
//     ret.position = v; 
//     float mesh_pdf = mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id] - ((primitive_id == 0) ? 0 : mesh_cdf_buffers[mesh_id].area_cdfs[primitive_id - 1]);
//     ret.normal = new_normal;
//     ret.pdf = mesh_pdf / TriangleArea(v0.position, v1.position, v2.position); 
//     return ret;
//   } else {
//     SamplePoint ret;
//     ret.position = origin;
//     ret.normal = normal;
//     ret.pdf = 1;
//     return ret;
//   }
// }

#endif