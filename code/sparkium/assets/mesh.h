#pragma once

#include "sparkium/assets/texture.h"
#include "sparkium/assets/vertex.h"
#include "vector"

namespace sparkium {
class Mesh {
 public:
  Mesh(const std::vector<Vertex> &vertices = {},
       const std::vector<uint32_t> &indices = {});

  int LoadObjFile(const std::string &obj_file_path);

  int SaveObjFile(const std::string &obj_file_path) const;

  int LoadFromHeightMap(const Texture &height_map,
                        float precision = 1.0f,
                        float height_scale = 1.0f,
                        float height_offset = 0.0f);

  void CreateSphere(const glm::vec3 &position, float radius = 1.0f, uint32_t num_segments = 16, uint32_t num_rings = 16);

  const std::vector<Vertex> &Vertices() const {
    return vertices_;
  }

  const std::vector<uint32_t> &Indices() const {
    return indices_;
  }
  void scale(float scale = 1.0f) {
    float distance = 0.0f;
    for (auto &vertex : vertices_) {
      distance += glm::length(vertex.position);
    }
    distance /= vertices_.size();
    for (auto &vertex : vertices_) {
      vertex.position *= (scale / distance);
    }
  }
  void translate(const glm::vec3 &translation = glm::vec3{0.0f}) {
    for (auto &vertex : vertices_) {
      vertex.position += translation;
    }
  }
  void rotate(float angle, const glm::vec3 &axis = glm::vec3{0.0f, 1.0f, 0.0f}) {
    glm::vec3 normalizedAxis = glm::normalize(axis); 
    float radians = glm::radians(angle);            
    float c = cos(radians);
    float s = sin(radians);
    float oneMinusC = 1.0f - c;

    glm::mat3 rotationMatrix = {
        {c + normalizedAxis.x * normalizedAxis.x * oneMinusC, 
         normalizedAxis.x * normalizedAxis.y * oneMinusC - normalizedAxis.z * s, 
         normalizedAxis.x * normalizedAxis.z * oneMinusC + normalizedAxis.y * s},

        {normalizedAxis.y * normalizedAxis.x * oneMinusC + normalizedAxis.z * s, 
         c + normalizedAxis.y * normalizedAxis.y * oneMinusC, 
         normalizedAxis.y * normalizedAxis.z * oneMinusC - normalizedAxis.x * s},

        {normalizedAxis.z * normalizedAxis.x * oneMinusC - normalizedAxis.y * s, 
         normalizedAxis.z * normalizedAxis.y * oneMinusC + normalizedAxis.x * s, 
         c + normalizedAxis.z * normalizedAxis.z * oneMinusC}
    }; 
    for (auto &vertex : vertices_) {
      vertex.position = glm::vec3(rotationMatrix * glm::vec4(vertex.position, 1.0f));
    }
  }
 private:
  void MergeVertices();

  void BuildNormal();

  void BuildTangent();

  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
};

void CalculateNormals(std::vector<Vertex> &vertices,
                      const std::vector<uint32_t> &indices);
}  // namespace sparkium
