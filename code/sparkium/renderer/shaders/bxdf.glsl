#ifndef BXDF_GLSL
#define BXDF_GLSL

#include "constants.glsl"
#include "material.glsl"

vec3 ReflectionDirection(vec3 normal, vec3 direction)
{
  if(dot(normal, direction) > 0)normal = -normal; 
  return -dot(normal, direction) * normal * 2 + direction; 
}
vec3 RefractionDirection(vec3 normal, vec3 direction, float eta) 
{    
    vec3 wi = -normalize(direction); 
    float cosTheta_i = dot(normal, wi); 
    float sin2Theta_i = 1 - cosTheta_i * cosTheta_i; 
    float sin2Theta_t = sin2Theta_i / (eta * eta);
    float cosTheta_t = ((1 - sin2Theta_t) >= 0) ? sqrt(1 - sin2Theta_t) : 0; 
    vec3 wt = -wi / eta + (cosTheta_i / eta - cosTheta_t) * normal;
    return wt; 
}
float FresnelReflectionRate(vec3 in_direction, vec3 half_direction, float eta) {
  float reflect_rate;
  float c = abs(dot(in_direction, half_direction));
  float gsquare = c*c - 1 + eta*eta;
  if (gsquare > 0) {
    float g = sqrt(gsquare);
    reflect_rate = 0.5 * (g-c) * (g-c) / (g+c) / (g+c) * (1 + (c*g + c*c -1) * (c*g + c*c - 1) / (c*g - c*c + 1) / (c*g - c*c + 1));
  } else {
    reflect_rate = 1;
  }

  return reflect_rate;
}

float Shadow_term(vec3 in_direction, vec3 out_direction, vec3 normal_direction, vec3 half_direction, float alpha) {
  float cos_theta_in = dot(-in_direction, normal_direction);
  float cos_theta_on = dot(out_direction, normal_direction);
  float cos_theta_im = dot(-in_direction, half_direction);
  float cos_theta_om = dot(out_direction, half_direction);
  float g1;
  if (cos_theta_im * cos_theta_in > 0)
    g1 = 2 / (1+sqrt(1 + alpha*alpha * (1/cos_theta_in/cos_theta_in - 1)));
  else 
    g1 = 0;

  float g2;
  if (cos_theta_om * cos_theta_on > 0)
    g2 = 2 / (1+sqrt(1 + alpha*alpha * (1/cos_theta_on/cos_theta_on - 1)));
  else 
    g2 = 0;
  float shadow = g1*g2;
  return shadow;
}
float lum(vec3 color) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}
vec3 CalculateLambertianBRDF(Material material, vec3 in_direction, vec3 out_direction, vec3 normal_direction) {
  return material.base_color * INV_PI;
}
vec3 CalculateNonMetalBRDF(Material material, vec3 in_direction, vec3 out_direction, vec3 normal_direction) {
  vec3 h = normalize(-in_direction + out_direction);
  float cosThetaI = max(dot(normal_direction, -in_direction), 0.0);
  float cosThetaO = max(dot(normal_direction, out_direction), 0.0);
  float cosThetaH = max(dot(-in_direction, h), 0.0);
  float FD90 = 0.5 + 2.0 * cosThetaH * cosThetaH * material.roughness;
  vec3 res = material.base_color * INV_PI * (1.0 + (FD90 - 1.0) * pow(1.0 - cosThetaI, 5.0)) * (1.0 + (FD90 - 1.0) * pow(1.0 - cosThetaO, 5.0))
         + mix(vec3(1.0), material.base_color / lum(material.base_color), material.sheen_tint) * material.sheen * pow(1.0 - cosThetaH, 5.0); 
  return max(res, vec3(0.0)); 
}

vec3 CalculateSpecularBRDF(Material material, vec3 in_direction, vec3 out_direction, vec3 normal_direction) {
  // Calculate the perfect reflection direction
  vec3 truth =  ReflectionDirection(normal_direction, in_direction);
  
  // Check if the view direction matches the reflection direction
  float match = max(dot(truth, out_direction), 0.0);

  vec3 specular = (match > 0.9999) ? material.base_color : vec3(0.0);

  // For perfect specular, we only reflect in the perfect mirror direction
  return specular / abs(dot(normal_direction, -in_direction));
}

vec3 CalculateRetractiveBSDF(Material material, vec3 in_direction, vec3 out_direction, vec3 normal, float inside, float wave_length) {
  // Calculate the perfect reflection direction

  vec3 reflection_direction = ReflectionDirection(normal, in_direction);
  float cos_theta = -dot(in_direction, normal);
  float n = material.a + material.b / pow(wave_length, 2.0) + material.c / pow(wave_length, 4.0);
  float etap = (inside < 1e-3) ? n : 1.0 / n;
  vec3 retraction_direction = RefractionDirection(normal, in_direction, etap); 
  float ratio = FresnelReflectionRate(in_direction, -normal, etap); 
  float match1 = max(dot(reflection_direction, out_direction), 0.0);
  float match2 = max(dot(retraction_direction, out_direction), 0.0);
  
  if(match1 > 0.9999)
  {
    return vec3(ratio / abs(dot(normal, -in_direction)));
  }
  else if(match2 > 0.9999)
  {
    return vec3((1 - ratio) / etap / etap / abs(dot(normal, -in_direction)));
  }
  else
  {
    return vec3(0.0);
  }
}
vec3 CalculateMetalBRDF(Material material, vec3 in_direction, vec3 out_direction, vec3 normal_direction) {
  // Normalize input vectors
  vec3 N = normalize(normal_direction);
  vec3 V = normalize(out_direction);
  vec3 L = normalize(-in_direction);
    
  // Calculate the half vector
  vec3 H = normalize(V + L);
    
    // Calculate the Fresnel term using Schlick's approximation
  vec3 F0 = material.base_color;
  vec3 F = F0 + (1.0 - F0) * pow(1.0 - dot(H, V), 5.0);
  
  // Calculate the geometric attenuation term
  float NdotV = max(dot(N, V), 1e-4);
  float NdotL = max(dot(N, L), 1e-4);
  float NdotH = max(dot(N, H), 1e-4);
  float VdotH = max(dot(V, H), 1e-4);
  
  float k = (material.roughness + 1.0) * (material.roughness + 1.0) / 8.0;
  float G_V = NdotV / (NdotV * (1.0 - k) + k);
  float G_L = NdotL / (NdotL * (1.0 - k) + k);
  float G = G_V * G_L;
  
  // Calculate the normal distribution function (NDF) using GGX/Trowbridge-Reitz
  float alpha = material.roughness * material.roughness;
  float alpha2 = alpha * alpha;
  float denom = (NdotH * NdotH * (alpha2 - 1.0) + 1.0);
  float D = alpha2 / (PI * denom * denom);
  
  // Combine terms to get the specular BRDF
  vec3 specular = (F * G * D) / (4.0 * NdotV * NdotL);
  return max(specular, vec3(0.0));
}
vec3 CalculateMultilayerBRDF(Material material, vec3 in_direction, vec3 out_direction, vec3 normal_direction) {
  Material new_material = material;
  Material specular_material = material;

  new_material.roughness = material.clearcoat_roughness;
  new_material.base_color = vec3(0.04);
  specular_material.base_color = mix(0.08 * material.specular * mix(vec3(1.0), material.base_color / lum(material.base_color), material.specular_tint), material.base_color, material.metallic);
  return CalculateNonMetalBRDF(material, in_direction, out_direction, normal_direction)
  + CalculateMetalBRDF(specular_material, in_direction, out_direction, normal_direction)
  + material.clearcoat * CalculateMetalBRDF(new_material, in_direction, out_direction, normal_direction);
}
vec3 CalculateAnisotropicMetalBRDF(Material material, vec3 in_direction, vec3 out_direction, vec3 normal_direction) {
    // Normalize input vectors
    vec3 N = normalize(normal_direction);
    vec3 V = normalize(out_direction);
    vec3 L = normalize(-in_direction);
    
    // Calculate the half vector
    vec3 H = normalize(V + L);
    
    // Calculate the Fresnel term using Schlick's approximation
    vec3 F0 = material.base_color;
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - dot(H, V), 5.0);
    
    // Calculate the geometric attenuation term
    float NdotV = max(dot(N, V), 1e-4);
    float NdotL = max(dot(N, L), 1e-4);
    float NdotH = max(dot(N, H), 1e-4);
    float VdotH = max(dot(V, H), 1e-4);
    
    // Anisotropic roughness
    float alpha_x = material.roughness * (1.0 - material.anisotropic);
    float alpha_y = material.roughness * (1.0 + material.anisotropic);
    
    // Anisotropic rotation
    float rotation = material.anisotropic_rotation;
    float cosRot = cos(rotation);
    float sinRot = sin(rotation);
    
    // Transform the half-vector to account for anisotropy
    vec3 h_tangent = vec3(
        NdotH * cosRot - NdotH * sinRot,
        NdotH * sinRot + NdotH * cosRot,
        NdotH
    );

    // Anisotropic GGX normal distribution function (NDF) using alpha_x and alpha_y
    float alpha2_x = alpha_x * alpha_x;
    float alpha2_y = alpha_y * alpha_y;
    float denom = (NdotH * NdotH * (alpha2_x - 1.0) + 1.0);
    float D_x = alpha2_x / (PI * denom * denom);
    
    denom = (NdotH * NdotH * (alpha2_y - 1.0) + 1.0);
    float D_y = alpha2_y / (PI * denom * denom);
    
    // Average the distributions in the two directions
    float D = (D_x + D_y) / 2.0;
    
    // Calculate the geometric attenuation term (G)
    float k = (material.roughness + 1.0) * (material.roughness + 1.0) / 8.0;
    float G_V = NdotV / (NdotV * (1.0 - k) + k);
    float G_L = NdotL / (NdotL * (1.0 - k) + k);
    float G = G_V * G_L;
    
    // Combine the terms to get the final specular BRDF
    vec3 specular = (F * G * D) / (4.0 * NdotV * NdotL);
    
    return specular;
}
vec3 CalculateBxDF(Material material, vec3 in_direction, vec3 out_direction, vec3 normal_direction, float inside, float wave_length) {
  if (material.type == MATERIAL_TYPE_LAMBERTIAN) {
    return CalculateLambertianBRDF(material, in_direction, out_direction, normal_direction);
  }
  else if (material.type == MATERIAL_TYPE_SPECULAR) {
    return CalculateSpecularBRDF(material, in_direction, out_direction, normal_direction);
  }
  else if(material.type == MATERIAL_TYPE_RETRACTIVE) {
    return CalculateRetractiveBSDF(material, in_direction, out_direction, normal_direction, inside, wave_length);
  }
  else if(material.type == MATERIAL_TYPE_METAL) {
    return CalculateMetalBRDF(material, in_direction, out_direction, normal_direction);
  }
  else if(material.type == MATERIAL_TYPE_METAL_ANISOTROPIC) {
    return CalculateAnisotropicMetalBRDF(material, in_direction, out_direction, normal_direction);
  }
  else if(material.type == MATERIAL_TYPE_NONMETAL) {
    return CalculateNonMetalBRDF(material, in_direction, out_direction, normal_direction);
  }
  else if(material.type == MATERIAL_TYPE_MULTILAYER) {
    return CalculateMultilayerBRDF(material, in_direction, out_direction, normal_direction);
  }

}

#endif