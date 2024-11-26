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
