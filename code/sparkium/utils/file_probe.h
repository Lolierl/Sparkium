#pragma once
#include "sparkium/utils/common.h"

namespace sparkium {
class FileProbe {
 public:
  FileProbe();
  ~FileProbe();

  void AddSearchPath(const std::string &path);

  std::string FindFile(const std::string &filename) const;

  static FileProbe &GetInstance();

  friend std::ostream &operator<<(std::ostream &os, const FileProbe &probe);

 private:
  std::vector<std::string> search_paths_;
};

std::string FindAssetsFile(const std::string &filename);
}  // namespace sparkium
