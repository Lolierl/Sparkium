#include "sparkium/assets/curve.h"

namespace sparkium {

Hair::Hair(const std::vector<Curve> &curves) : curves_(curves) {}

void Hair::AddCurve(const Curve &curve) {
  curves_.push_back(curve);
}

}  // namespace sparkium