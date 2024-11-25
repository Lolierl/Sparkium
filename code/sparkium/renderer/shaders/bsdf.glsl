#ifndef BSDF_GLSL
#define BSDF_GLSL

#include "constants.glsl"
#include "material.glsl"

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

#endif