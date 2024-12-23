#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL

#include "constants.glsl"
#include "random.glsl"
#include "material.glsl"

struct LightSamplePoint
{
  vec3 position; 
  uint mesh_id, primitive_id; 
  float pdf; 
};

struct WaveLengthWithPDF
{
	float wave_length;
	float pdf;
};

/*Spectrum Sample*/
WaveLengthWithPDF SampleWaveLength() {
	WaveLengthWithPDF ret;
	ret.pdf = 1;
	ret.wave_length = 550;
	float rand = RandomFloat();

  // Find the wave_length corresponding to the random number
  for (int i = 0; i < 81; ++i) {
    if (rand <= spectrum_cdf[i]) {
      ret.wave_length = 380 + i * 5;
			ret.pdf = (i == 0) ? spectrum_cdf[i] : (spectrum_cdf[i] - spectrum_cdf[i-1]);
			return ret;
    }
  }
	return ret;
}


float CalculateLightStrength(Material material, vec3 origin, vec3 direction, vec3 normal, float wave_length) {
	float geometry_coeff = 1.0;
	float spectrum_coeff = 1.0;
	if (material.spectrum_type == SPECTRUM_TYPE_D65) {
		uint i = (int(wave_length) - 380) / 5;
		spectrum_coeff = (i == 0) ? D65_spectrum[i] : (D65_spectrum[i] - D65_spectrum[i-1]);
		spectrum_coeff /= D65_spectrum[80];
	}
	else if (material.spectrum_type == SPECTRUM_TYPE_D75) {
		uint i = (int(wave_length) - 380) / 5;
		spectrum_coeff = (i == 0) ? D75_spectrum[i] : (D75_spectrum[i] - D75_spectrum[i-1]);
		spectrum_coeff /= D75_spectrum[80];
	}
	else if (material.spectrum_type == SPECTRUM_TYPE_D50)  {
		uint i = (int(wave_length) - 380) / 5;
		spectrum_coeff = (i == 0) ? D50_spectrum[i] : (D50_spectrum[i] - D50_spectrum[i-1]);
		spectrum_coeff /= D50_spectrum[80];
	}
	else if (material.spectrum_type == SPECTRUM_TYPE_SODIUM)  {
		uint i = (int(wave_length) - 380) / 5;
		spectrum_coeff = (i == 0) ? sodium_spectrum[i] : (sodium_spectrum[i] - sodium_spectrum[i-1]);
		spectrum_coeff /= sodium_spectrum[80];
	}

	if (material.illuminant_type == ILLUMINANT_TYPE_LAMBERTIAN) {
		geometry_coeff = 1.0;
	}
	else if (material.type == MATERIAL_TYPE_VOLUME) {
		geometry_coeff = 1.0;
	}
	else if (material.illuminant_type == ILLUMINANT_TYPE_PARALLEL) {
		if (max(abs(dot(material.illuminant_dir, direction)), 0.0) < 1 - 1e-1) {
			return 0.0;
		}
		geometry_coeff = 1.0;
    }
	return geometry_coeff * spectrum_coeff * material.emission_strength;
}

#endif