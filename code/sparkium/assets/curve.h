#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace sparkium {

struct Curve {
  glm::vec3 p0;  // Start point
  glm::vec3 p1;  // Control point 1
  glm::vec3 p2;  // Control point 2
  glm::vec3 p3;  // End point
};

class Hair {
 public:
  Hair(const std::vector<Curve> &curves = {});

  void AddCurve(const Curve &curve);

  const std::vector<Curve> &Curves() const {
    return curves_;
  }

 private:
  std::vector<Curve> curves_;
};

}  // namespace sparkium