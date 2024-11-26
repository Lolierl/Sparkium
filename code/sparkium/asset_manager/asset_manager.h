#pragma once

#include "sparkium/asset_manager/asset_manager_utils.h"
#include "sparkium/asset_manager/mesh_asset.h"
#include "sparkium/asset_manager/texture_asset.h"
#include "sparkium/asset_manager/hair_asset.h"
#include "sparkium/asset_manager/pointlight_asset.h"

namespace sparkium {
class AssetManager {
 public:
  AssetManager(vulkan::Core *core, uint32_t max_textures, uint32_t max_meshes, uint32_t max_point_lights);

  ~AssetManager();

  int LoadTexture(const Texture &texture, std::string name = "Unnamed Texture");

  int LoadHair(const Hair &hair, std::string name = "Unnamed Hair");

  int LoadMesh(const Mesh &mesh, std::string name = "Unnamed Mesh");

  int LoadPointLight(const PointLight &light, std::string name = "Unnamed Light"); 

  TextureAsset *GetTexture(uint32_t id);

  MeshAsset *GetMesh(uint32_t id);

  PointLightAsset *GetPointLight(uint32_t id);

  uint32_t GetTextureBindingId(uint32_t id);

  uint32_t GetMeshBindingId(uint32_t id);

  void DestroyTexture(uint32_t id);

  void DestroyMesh(uint32_t id);

  void DestroyHair(uint32_t id);

  void DestroyPointLight(uint32_t id);

  void ImGui();

  vulkan::Core *Core() {
    return core_;
  }

  std::set<uint32_t> GetTextureIds();

  std::set<uint32_t> GetMeshIds();

  std::set<uint32_t> GetHairIds();

  std::set<uint32_t> GetPointLightIds();

  vulkan::DescriptorSetLayout *DescriptorSetLayout() {
    return descriptor_set_layout_.get();
  }

  VkDescriptorSet DescriptorSet(uint32_t frame_id) {
    return descriptor_sets_[frame_id]->Handle();
  }

  void Update(uint32_t frame_id);

  bool ComboForTextureSelection(const char *label, uint32_t *id);

  bool ComboForMeshSelection(const char *label, uint32_t *id);

  bool ComboForPointLightSelection(const char *label, uint32_t *id);

  void SyncData(VkCommandBuffer cmd_buffer, int frame_id);

  void Clear();

 private:
  void CreateDefaultAssets();
  void CreateDescriptorObjects();

  void DestroyDefaultAssets();
  void DestroyDescriptorObjects();

  void UpdateMeshDataBindings(uint32_t frame_id);
  void UpdateTextureBindings(uint32_t frame_id);
  void UpdatePointLightBindings(uint32_t frame_id);
  
  vulkan::Core *core_;
  uint32_t next_mesh_id_{};
  uint32_t next_texture_id_{};
  uint32_t next_hair_id_{};
  uint32_t next_point_light_id_{}; 

  // Using map instead of vector here to reserve space for deleting assets
  // The first element in pairs is the binding index, will be updated every
  // frame in function GetXXXIds
  std::map<uint32_t, std::pair<uint32_t, std::unique_ptr<TextureAsset>>>
      textures_;
  std::map<uint32_t, std::pair<uint32_t, std::unique_ptr<MeshAsset>>> meshes_;
  std::map<uint32_t, std::pair<uint32_t, std::unique_ptr<PointLightAsset>>> point_lights_;
  std::unique_ptr<vulkan::DynamicBuffer<MeshMetadata>> mesh_metadata_buffer_;

  std::unique_ptr<vulkan::DescriptorSetLayout> descriptor_set_layout_;
  std::unique_ptr<vulkan::DescriptorPool> descriptor_pool_;
  std::vector<std::unique_ptr<vulkan::DescriptorSet>> descriptor_sets_;

  std::unique_ptr<vulkan::Sampler> linear_sampler_;
  std::unique_ptr<vulkan::Sampler> nearest_sampler_;

  std::vector<uint32_t> last_frame_bound_texture_num_;
  std::vector<uint32_t> last_frame_bound_mesh_num_;
  std::vector<uint32_t> last_frame_bound_pointlight_num_;
  std::vector<uint32_t> last_frame_bound_light_num_;

  uint32_t max_textures_{};
  uint32_t max_meshes_{};
  uint32_t max_point_lights_{};
};
}  // namespace sparkium
