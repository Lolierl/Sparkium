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

float SampleWavelength() {
  const float wavelengths[81] = float[](
    380.0, 385.0, 390.0, 395.0, 400.0, 405.0, 410.0, 415.0, 420.0, 425.0,
    430.0, 435.0, 440.0, 445.0, 450.0, 455.0, 460.0, 465.0, 470.0, 475.0,
    480.0, 485.0, 490.0, 495.0, 500.0, 505.0, 510.0, 515.0, 520.0, 525.0,
    530.0, 535.0, 540.0, 545.0, 550.0, 555.0, 560.0, 565.0, 570.0, 575.0,
    580.0, 585.0, 590.0, 595.0, 600.0, 605.0, 610.0, 615.0, 620.0, 625.0,
    630.0, 635.0, 640.0, 645.0, 650.0, 655.0, 660.0, 665.0, 670.0, 675.0,
    680.0, 685.0, 690.0, 695.0, 700.0, 705.0, 710.0, 715.0, 720.0, 725.0,
    730.0, 735.0, 740.0, 745.0, 750.0, 755.0, 760.0, 765.0, 770.0, 775.0,
    780.0
  );
  const float D65_spectrum[81] = float[](
    0.000174, 0.000244, 0.000339, 0.000464, 0.000634, 0.000854, 0.001133, 0.001478, 0.001896, 0.002393,
    0.002973, 0.003638, 0.004391, 0.005237, 0.006182, 0.007234, 0.008394, 0.009676, 0.011083, 0.012616,
    0.014276, 0.016070, 0.018000, 0.020066, 0.022259, 0.024572, 0.027000, 0.029538, 0.032177, 0.034924,
    0.037775, 0.040730, 0.043785, 0.046933, 0.050171, 0.053493, 0.056890, 0.060355, 0.063881, 0.067457,
    0.071078, 0.074731, 0.078406, 0.082090, 0.085774, 0.089444, 0.093085, 0.096682, 0.100220, 0.103685,
    0.107070, 0.110359, 0.113554, 0.116653, 0.119653, 0.122552, 0.125347, 0.128037, 0.130621, 0.133097,
    0.135463, 0.137718, 0.139859, 0.141884, 0.143792, 0.145581, 0.147250, 0.148796, 0.150219, 0.151516,
    0.152687, 0.153731, 0.154646, 0.155430, 0.156080, 0.156597, 0.157000, 0.157286, 0.157436, 0.157426,
    0.157222
  );
  // Generate a random number between 0 and 1
  float rand = RandomFloat() * 0.157222;

  // Find the wavelength corresponding to the random number
  for (int i = 0; i < 81; ++i) {
    if (rand <= D65_spectrum[i]) {
      return wavelengths[i];
    }
  }

  // Fallback in case of numerical issues
  return wavelengths[80];
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
SampleDirection SampleRetractiveTransportDirection(Material material, vec3 in_direction, vec3 normal, float inside, float wavelength) {
  SampleDirection ret; 
  ret.pdf = 1;

  vec3 reflection_direction = ReflectionDirection(normal, in_direction); 
  float n = material.a + material.b / pow(wavelength, 2.0) + material.c / pow(wavelength, 4.0);
  float etap = (inside < 1e-3) ? n : 1.0 / n;
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
/*Handlers*/
SampleDirection SampleTransportDirection(vec3 origin, Material material, vec3 in_direction, vec3 normal_direction, float inside, float wavelength) {
  if (material.type == MATERIAL_TYPE_LAMBERTIAN) {
    LightSamplePoint light_sample = SampleDirectLighting(origin);
    
    return SampleLambertianTransportDirection(normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_SPECULAR) {
    return SampleSpecularTransportDirection(in_direction, normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_RETRACTIVE) {
    return SampleRetractiveTransportDirection(material, in_direction, normal_direction, inside, wavelength);
  }
  else if (material.type == MATERIAL_TYPE_METAL) {
    return SampleMetalTransportDirection(material, in_direction, normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_METAL_ANISOTROPIC) {
    return SampleAnisotropicMicrofacet(material, in_direction, normal_direction);
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