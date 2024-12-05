#include "sparkium/app/built_in_scenes.h"

#include "sparkium/asset_manager/asset_manager.h"
#include "sparkium/renderer/renderer.h"

namespace sparkium {

std::vector<std::pair<std::string, std::function<void(Scene *scene)>>>
BuiltInSceneList() {
  return {
      {"SunFlower Dog", LoadSunFlowerDog},
      {"Cornell Box", LoadCornellBox},
      {"Island Scene", LoadIslandScene},
  };
}
//(LR, UD, FB)
void LoadSunFlowerDog(Scene *scene)
{
  AssetManager *asset_manager = scene->Renderer()->AssetManager();

  auto make_vertex = [](const glm::vec3 &pos, const glm::vec2 &tex_coord) {
    Vertex vertex;
    vertex.position = pos;
    vertex.tex_coord = tex_coord;
    return vertex;
  };
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices = {0, 1, 3, 1, 2, 3};

  // light
  // <vertex position="343.0 548.7 227.0" tex_coord="0 0"/>
  // <vertex position="343.0 548.7 332.0" tex_coord="1 0"/>
  // <vertex position="213.0 548.7 332.0" tex_coord="1 1"/>
  // <vertex position="213.0 548.7 227.0" tex_coord="0 1"/>
  //light
  vertices.push_back(make_vertex({343.0f, 648.7f, 227.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({343.0f, 648.7f, 332.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({213.0f, 648.7f, 332.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({213.0f, 648.7f, 227.0f}, {0.0f, 1.0f}));
  int light_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "LightMesh");
  Material light_material;
  light_material.base_color = {0.0f, 0.0f, 0.0f};
  light_material.emission = {1.0f, 1.0f, 0.6f};
  light_material.emission_strength = 30.0f;
  int light_id = scene->CreateEntity();
  scene->SetEntityMesh(light_id, light_mesh_id);
  scene->SetEntityMaterial(light_id, light_material);


  Mesh plate_mesh;
	plate_mesh.LoadObjFile(FindAssetsFile("mesh/Plate/Plate OBJ.obj"));
	plate_mesh.scale(50.0f);
	plate_mesh.translate(glm::vec3(210.0f, 190.0f, 100.0f));

  int plate_mesh_id =
      asset_manager->LoadMesh(plate_mesh, "PlateMesh"); 

  Material plate_material;
  plate_material.base_color = {1.000f, 1.000f, 1.000f};
	plate_material.type=MATERIAL_TYPE_LAMBERTIAN;
  plate_material.roughness = 0.4;
  plate_material.ior = 1.5; 
  
  int plate_id = scene->CreateEntity();
  scene->SetEntityMesh(plate_id, plate_mesh_id);
  scene->SetEntityMaterial(plate_id, plate_material);

  Mesh table_mesh;
	table_mesh.LoadObjFile(FindAssetsFile("mesh/Table/Wood_Table.obj"));
	table_mesh.scale(200.0f);
	table_mesh.translate(glm::vec3(200.0f, 0.0f, 100.0f));

  int table_mesh_id =
      asset_manager->LoadMesh(table_mesh, "TableMesh"); 

  Material table_material;
  table_material.base_color = {1.000f, 0.766f, 0.336f};
	table_material.type=MATERIAL_TYPE_LAMBERTIAN;
  table_material.roughness = 0.4;
  table_material.ior = 1.5; 
  
  Texture table_texture;
  table_texture.LoadFromFile(
      FindAssetsFile("texture/Wood_Table_C_2.jpg"),
      LDRColorSpace::UNORM);

  auto table_texture_id =
      asset_manager->LoadTexture(table_texture, "TableTexture");
  int table_id = scene->CreateEntity();
  scene->SetEntityMesh(table_id, table_mesh_id);
  scene->SetEntityMaterial(table_id, table_material);
  scene->SetEntityAlbedoTexture(table_id, table_texture_id);

  Mesh chair_mesh;
	chair_mesh.LoadObjFile(FindAssetsFile("mesh/chair/chair.obj"));
	chair_mesh.scale(200.0f);
	chair_mesh.translate(glm::vec3(200.0f, 0.0f, -100.0f));

  int chair_mesh_id =
      asset_manager->LoadMesh(chair_mesh, "ChairMesh"); 

  Material chair_material;
  //chair_material.base_color = {1.000f, 0.766f, 0.336f};
	chair_material.type=MATERIAL_TYPE_LAMBERTIAN;
  chair_material.roughness = 0.4;
  chair_material.ior = 1.5; 
  
  Texture chair_texture;
  chair_texture.LoadFromFile(
      FindAssetsFile("texture/chair/chair_01_Base_Color.png"),
      LDRColorSpace::UNORM);

  auto chair_texture_id =
      asset_manager->LoadTexture(chair_texture, "ChairTexture");
  int chair_id = scene->CreateEntity();
  scene->SetEntityMesh(chair_id, chair_mesh_id);
  scene->SetEntityMaterial(chair_id, chair_material);
  scene->SetEntityAlbedoTexture(chair_id, chair_texture_id);

  Mesh glass_mesh;
	glass_mesh.LoadObjFile(FindAssetsFile("mesh/Glass/Glass OBJ.obj"));
	glass_mesh.scale(40.0f);
	glass_mesh.translate(glm::vec3(300.0f, 260.0f, 100.0f));

  int glass_mesh_id =
      asset_manager->LoadMesh(glass_mesh, "GlassMesh"); 

  Material glass_material;
  glass_material.base_color = {1.000f, 1.000f, 1.000f};
	glass_material.type=MATERIAL_TYPE_RETRACTIVE;
  glass_material.roughness = 0.4;
  glass_material.ior = 1.5; 
  
  int glass_id = scene->CreateEntity();
  scene->SetEntityMesh(glass_id, glass_mesh_id);
  scene->SetEntityMaterial(glass_id, glass_material);

  Mesh bread_mesh;
	bread_mesh.LoadObjFile(FindAssetsFile("mesh/Bread/Bread.obj"));
	bread_mesh.scale(30.0f);
	bread_mesh.translate(glm::vec3(210.0f, 210.0f, 85.0f));

  int bread_mesh_id =
      asset_manager->LoadMesh(bread_mesh, "BreadMesh"); 

  Material bread_material;
  //bread_material.base_color = {1.000f, 1.000f, 0.336f};
	bread_material.type=MATERIAL_TYPE_LAMBERTIAN;
  bread_material.roughness = 0.4;
  bread_material.ior = 1.5; 
  
  Texture bread_texture;
  bread_texture.LoadFromFile(
      FindAssetsFile("texture/Bread.png"),
      LDRColorSpace::UNORM);

  auto bread_texture_id =
      asset_manager->LoadTexture(bread_texture, "BreadTexture");
  int bread_id = scene->CreateEntity();
  scene->SetEntityMesh(bread_id, bread_mesh_id);
  scene->SetEntityMaterial(bread_id, bread_material);
  scene->SetEntityAlbedoTexture(bread_id, bread_texture_id);

  Mesh spoon_mesh;
	spoon_mesh.LoadObjFile(FindAssetsFile("mesh/Spoon/spoon.obj"));
  spoon_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  //spoon_mesh.rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  //spoon_mesh.rotate(-90.0f, glm::vec3(0.0f, 0.0f, 1.0f));

  //spoon_mesh.rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	spoon_mesh.scale(40.0f);
	spoon_mesh.translate(glm::vec3(90.0f, 200.0f, 70.0f));
  
  int spoon_mesh_id =
      asset_manager->LoadMesh(spoon_mesh, "SpoonMesh"); 

  Material spoon_material;
  spoon_material.base_color = {1.000f, 1.000f, 1.000f};
	spoon_material.type=MATERIAL_TYPE_METAL;
  spoon_material.roughness = 0.1;
  //spoon_material.ior = 1.5; 
  
  int spoon_id = scene->CreateEntity();
  scene->SetEntityMesh(spoon_id, spoon_mesh_id);
  scene->SetEntityMaterial(spoon_id, spoon_material);

  Mesh fork_mesh;
	fork_mesh.LoadObjFile(FindAssetsFile("mesh/Fork/Fork.obj"));
  fork_mesh.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  fork_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	fork_mesh.scale(40.0f);
	fork_mesh.translate(glm::vec3(130.0f, 200.0f,  73.0f));

  int fork_mesh_id =
      asset_manager->LoadMesh(fork_mesh, "ForkMesh"); 

  Material fork_material;
  fork_material.base_color = {1.000f, 1.000f, 1.000f};
	fork_material.type=MATERIAL_TYPE_METAL;
  fork_material.roughness = 0.1;
  fork_material.ior = 1.0; 
  
  Texture fork_texture;
  fork_texture.LoadFromFile(
      FindAssetsFile("texture/Fork.jpg"),
      LDRColorSpace::UNORM);

  int fork_id = scene->CreateEntity();
  scene->SetEntityMesh(fork_id, fork_mesh_id);
  scene->SetEntityMaterial(fork_id, fork_material);

  Mesh knife_mesh;
	knife_mesh.LoadObjFile(FindAssetsFile("mesh/Knife/Knife.obj"));
  knife_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	knife_mesh.scale(40.0f);
	knife_mesh.translate(glm::vec3(120.0f, 200.0f, 85.0f));

  int knife_mesh_id =
      asset_manager->LoadMesh(knife_mesh, "KnifeMesh"); 

  Material knife_material;
  knife_material.base_color = {1.000f, 1.000f, 1.000f};
	knife_material.type=MATERIAL_TYPE_METAL;
  knife_material.roughness = 0.1;
  knife_material.ior = 1.5; 
  
  Texture knife_texture;
  knife_texture.LoadFromFile(
      FindAssetsFile("texture/Knife.jpg"),
      LDRColorSpace::UNORM);

  auto knife_texture_id =
      asset_manager->LoadTexture(knife_texture, "KnifeTexture");
  int knife_id = scene->CreateEntity();
  scene->SetEntityMesh(knife_id, knife_mesh_id);
  scene->SetEntityMaterial(knife_id, knife_material);

  vertices.clear();
  vertices.push_back(make_vertex({1000.0f, 0.0f, -400.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 0.0f, -400.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 0.0f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({1000.0f, 0.0f, 559.2f}, {0.0f, 1.0f}));
  int floor_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "FloorMesh");
  Material floor_material;
  //floor_material.base_color = {0.0f, 0.0f, 0.8f};
  Texture floor_texture;
  floor_texture.LoadFromFile(
      FindAssetsFile("texture/floor.jpg"),
      LDRColorSpace::UNORM);

  auto floor_texture_id =
      asset_manager->LoadTexture(floor_texture, "FloorTexture");
  int floor_id = scene->CreateEntity();
  scene->SetEntityMesh(floor_id, floor_mesh_id);
  scene->SetEntityMaterial(floor_id, floor_material);
  scene->SetEntityAlbedoTexture(floor_id, floor_texture_id);

  Texture terrain_texture;
  terrain_texture.LoadFromFile(
      FindAssetsFile("texture/dog.jpg"),
      LDRColorSpace::UNORM);

  auto terrain_texture_id =
      asset_manager->LoadTexture(terrain_texture, "TerrainTexture");

  vertices.clear();
  vertices.push_back(make_vertex({1000.0f, 648.8f, -400.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({1000.0f, 648.8f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 648.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 648.8f, -400.0f}, {0.0f, 1.0f}));
  int ceiling_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "CeilingMesh");
  Material ceiling_material;
  ceiling_material.base_color = {0.8f, 0.8f, 0.8f};
  int ceiling_id = scene->CreateEntity();
  scene->SetEntityMesh(ceiling_id, ceiling_mesh_id);
  scene->SetEntityMaterial(ceiling_id, ceiling_material);

  vertices.clear();
  vertices.push_back(make_vertex({1000.0f, 648.8f, 559.2f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 648.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({1000.0f, 0.0f, 559.2f}, {0.0f, 0.0f}));
  int back_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "BackWallMesh");
  Material back_wall_material;
  back_wall_material.base_color = {0.8f, 0.8f, 0.8f};
  int back_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(back_wall_id, back_wall_mesh_id);
  scene->SetEntityMaterial(back_wall_id, back_wall_material);
  scene->SetEntityAlbedoTexture(back_wall_id, terrain_texture_id);

  vertices.clear();
  vertices.push_back(make_vertex({-400.0f, 0.0f, 559.2f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 0.0f, -400.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 648.8f, -400.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 648.8f, 559.2f}, {0.0f, 1.0f}));
  int right_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "RightWallMesh");
  Material right_wall_material;
  right_wall_material.base_color = {0.8, 0.8, 0.8};
  int right_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(right_wall_id, right_wall_mesh_id);
  scene->SetEntityMaterial(right_wall_id, right_wall_material);

  vertices.clear();
  vertices.push_back(make_vertex({1000.0f, 0.0f, -400.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({1000.0f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({1000.0f, 648.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({1000.0f, 648.8f, -400.0f}, {0.0f, 1.0f}));
  int left_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "LeftWallMesh");
  Material left_wall_material;
  left_wall_material.base_color = {0.8f, 0.8f, 0.8f};
  int left_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(left_wall_id, left_wall_mesh_id);
  scene->SetEntityMaterial(left_wall_id, left_wall_material);
  //(LR, UD, FB)
  scene->SetEnvmapSettings({0.0f, 0.0f, 0, 0});
  scene->Camera()->SetPosition({308.0f, 563.0f, -500.0f});
  scene->Camera()->SetEulerAngles({glm::radians(-30.0f), glm::radians(180.0f), 0.0f});
  scene->Camera()->SetFov(glm::radians(40.0f));
  scene->Camera()->SetFar(2000.0f);
  scene->Camera()->SetCameraSpeed(100.0f);
}
void LoadCornellBox(Scene *scene) {
  AssetManager *asset_manager = scene->Renderer()->AssetManager();

  auto make_vertex = [](const glm::vec3 &pos, const glm::vec2 &tex_coord) {
    Vertex vertex;
    vertex.position = pos;
    vertex.tex_coord = tex_coord;
    return vertex;
  };

	// Create a pointlight
  
	// Mesh light_sphere_mesh;
  // glm::vec3 light_sphere_position = glm::vec3(20.0f, 20.0f, 20.0f);  // Position of the light sphere
  // light_sphere_mesh.CreateSphere(light_sphere_position, 1.0f, 4, 4);  // Small sphere with radius 10
  // int light_sphere_mesh_id = asset_manager->LoadMesh(light_sphere_mesh, "LightSphereMesh");

  // Material light_sphere_material;
  // light_sphere_material.base_color = {1.0f, 1.0f, 1.0f};  // White light
  // light_sphere_material.emission = {1.0f, 1.0f, 1.0f};  // Emissive material
  // light_sphere_material.emission_strength = 1000.0f;  // Adjust the strength as needed
  // light_sphere_material.type = MATERIAL_TYPE_LAMBERTIAN;
  // int light_sphere_id = scene->CreateEntity();
  // scene->SetEntityMesh(light_sphere_id, light_sphere_mesh_id);
  // scene->SetEntityMaterial(light_sphere_id, light_sphere_material);

	//  // Create a sphere
	// Mesh sphere_mesh;
	// glm::vec3 sphere_position = glm::vec3(203.0f, 268.7f, 187.0f);  // Position of the sphere
	// sphere_mesh.CreateSphere(sphere_position, 100.0f, 16, 16);
	// int sphere_mesh_id = asset_manager->LoadMesh(sphere_mesh, "SphereMesh");

	// Material sphere_material;
	// sphere_material.base_color = {1.000f, 0.766f, 0.336f};
	// sphere_material.type=MATERIAL_TYPE_METAL;
  // sphere_material.roughness = 0.4;
  // sphere_material.ior = 1.5; 
	// int sphere_id = scene->CreateEntity();
	// scene->SetEntityMesh(sphere_id, sphere_mesh_id);
	// scene->SetEntityMaterial(sphere_id, sphere_material);

	// Mesh bunny_mesh;
	// bunny_mesh.LoadObjFile(FindAssetsFile("mesh/bunny.obj"));
	// bunny_mesh.scale(100.0f);
	// bunny_mesh.translate(glm::vec3(400.0f, -40.0f, 100.0f));

  // int bunny_mesh_id =
  //     asset_manager->LoadMesh(bunny_mesh, "BunnyMesh"); 

  // Material bunny_material;
  // bunny_material.base_color = {0.8f, 0.8f, 0.8f};
  // bunny_material.type=MATERIAL_TYPE_SPECULAR;
  // int bunny_id = scene->CreateEntity();
  // scene->SetEntityMesh(bunny_id, bunny_mesh_id);
  // scene->SetEntityMaterial(bunny_id, bunny_material);

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices = {0, 1, 3, 1, 2, 3};

  // light
  // <vertex position="343.0 548.7 227.0" tex_coord="0 0"/>
  // <vertex position="343.0 548.7 332.0" tex_coord="1 0"/>
  // <vertex position="213.0 548.7 332.0" tex_coord="1 1"/>
  // <vertex position="213.0 548.7 227.0" tex_coord="0 1"/>
  vertices.push_back(make_vertex({343.0f, 548.7f, 227.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({343.0f, 548.7f, 332.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({213.0f, 548.7f, 332.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({213.0f, 548.7f, 227.0f}, {0.0f, 1.0f}));
  int light_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "LightMesh");
  Material light_material;
  light_material.base_color = {0.0f, 0.0f, 0.0f};
  light_material.emission = {1.0f, 1.0f, 1.0f};
  light_material.emission_strength = 30.0f;
  int light_id = scene->CreateEntity();
  scene->SetEntityMesh(light_id, light_mesh_id);
  scene->SetEntityMaterial(light_id, light_material);

  // floor
  // <vertex position="552.8 0.0   0.0" tex_coord="0 0"/>
  // <vertex position="0.0 0.0   0.0" tex_coord="1 0"/>
  // <vertex position="0.0 0.0 559.2" tex_coord="1 1"/>
  // <vertex position="549.6 0.0 559.2" tex_coord="0 1"/>
  vertices.clear();
  vertices.push_back(make_vertex({552.8f, 0.0f, 0.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({549.6f, 0.0f, 559.2f}, {0.0f, 1.0f}));
  int floor_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "FloorMesh");

  Material floor_material;
  floor_material.base_color = {0.8f, 0.8f, 0.8f};
  int floor_id = scene->CreateEntity();
  scene->SetEntityMesh(floor_id, floor_mesh_id);
  scene->SetEntityMaterial(floor_id, floor_material);

  // Texture terrain_texture;
  // terrain_texture.LoadFromFile(
  //     FindAssetsFile("texture/earth.jpg"),
  //     LDRColorSpace::UNORM);

//   Texture terrain_detail_texture;
// //   terrain_detail_texture.LoadFromFile(
// //       FindAssetsFile("texture/earth_clouds.jpg"), LDRColorSpace::UNORM);
  // auto terrain_texture_id =
  //     asset_manager->LoadTexture(terrain_texture, "TerrainTexture");

//   auto terrain_detail_texture_id = asset_manager->LoadTexture(
//       terrain_detail_texture, "TerrainDetailTexture");


  // ceiling
  // <vertex position="556.0 648.8 0.0  " tex_coord="0 0"/>
  // <vertex position="556.0 648.8 559.2" tex_coord="1 0"/>
  // <vertex position="0.0 648.8 559.2" tex_coord="1 1"/>
  // <vertex position="0.0 648.8   0.0" tex_coord="0 1"/>
  vertices.clear();
  vertices.push_back(make_vertex({556.0f, 648.8f, 0.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({556.0f, 648.8f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 648.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({0.0f, 648.8f, 0.0f}, {0.0f, 1.0f}));
  int ceiling_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "CeilingMesh");
  Material ceiling_material;
  ceiling_material.base_color = {0.8f, 0.8f, 0.8f};
  int ceiling_id = scene->CreateEntity();
  scene->SetEntityMesh(ceiling_id, ceiling_mesh_id);
  scene->SetEntityMaterial(ceiling_id, ceiling_material);

  // back_wall
  // <vertex position="549.6   0.0 559.2" tex_coord="0 0"/>
  // <vertex position="0.0   0.0 559.2" tex_coord="1 0"/>
  // <vertex position="0.0 648.8 559.2" tex_coord="1 1"/>
  // <vertex position="556.0 648.8 559.2" tex_coord="0 1"/>
  vertices.clear();
  vertices.push_back(make_vertex({549.6f, 0.0f, 559.2f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 648.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({556.0f, 648.8f, 559.2f}, {0.0f, 1.0f}));
  int back_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "BackWallMesh");
  Material back_wall_material;
  back_wall_material.base_color = {0.8f, 0.8f, 0.8f};
  int back_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(back_wall_id, back_wall_mesh_id);
  scene->SetEntityMaterial(back_wall_id, back_wall_material);
  // scene->SetEntityAlbedoTexture(back_wall_id, terrain_texture_id);
//   scene->SetEntityAlbedoDetailTexture(back_wall_id, terrain_detail_texture_id);
  // right_wall
  // <vertex position="0.0   0.0 559.2 " tex_coord="0 0"/>
  // <vertex position = "0.0   0.0   0.0" tex_coord = "1 0" />
  // <vertex position = "0.0 648.8   0.0" tex_coord = "1 1" />
  // <vertex position = "0.0 648.8 559.2" tex_coord = "0 1" />
  vertices.clear();
  vertices.push_back(make_vertex({0.0f, 0.0f, 559.2f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 648.8f, 0.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({0.0f, 648.8f, 559.2f}, {0.0f, 1.0f}));
  int right_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "RightWallMesh");
  Material right_wall_material;
  right_wall_material.base_color = {0.0, 0.8, 0.0};
  int right_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(right_wall_id, right_wall_mesh_id);
  scene->SetEntityMaterial(right_wall_id, right_wall_material);

  // left_wall
  // <vertex position="552.8   0.0   0.0 " tex_coord="0 0"/>
  // <vertex position = "549.6   0.0 559.2" tex_coord = "1 0" />
  // <vertex position = "556.0 648.8 559.2" tex_coord = "1 1" />
  // <vertex position = "556.0 648.8   0.0" tex_coord = "0 1" />
  vertices.clear();
  vertices.push_back(make_vertex({552.8f, 0.0f, 0.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({549.6f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({556.0f, 648.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({556.0f, 648.8f, 0.0f}, {0.0f, 1.0f}));
  int left_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "LeftWallMesh");
  Material left_wall_material;
  left_wall_material.base_color = {0.8f, 0.0f, 0.0f};
  int left_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(left_wall_id, left_wall_mesh_id);
  scene->SetEntityMaterial(left_wall_id, left_wall_material);

  // short_box
  // <vertex position="130.0 165.0  65.0" tex_coord="0 0"/>
  // <vertex position = "82.0 165.0 225.0" tex_coord = "1 0" />
  // <vertex position = "240.0 165.0 272.0" tex_coord = "1 1" />
  // <vertex position = "290.0 165.0 114.0" tex_coord = "0 1" />
  //
  // <vertex position = "290.0   0.0 114.0" tex_coord = "0 0" />
  // <vertex position = "290.0 165.0 114.0" tex_coord = "1 0" />
  // <vertex position = "240.0 165.0 272.0" tex_coord = "1 1" />
  // <vertex position = "240.0   0.0 272.0" tex_coord = "0 1" />
  //
  // <vertex position = "130.0   0.0  65.0" tex_coord = "0 0" />
  // <vertex position = "130.0 165.0  65.0" tex_coord = "1 0" />
  // <vertex position = "290.0 165.0 114.0" tex_coord = "1 1" />
  // <vertex position = "290.0   0.0 114.0" tex_coord = "0 1" />
  //
  // <vertex position = "82.0   0.0 225.0" tex_coord = "0 0" />
  // <vertex position = "82.0 165.0 225.0" tex_coord = "1 0" />
  // <vertex position = "130.0 165.0  65.0" tex_coord = "1 1" />
  // <vertex position = "130.0   0.0  65.0" tex_coord = "0 1" />
  //
  // <vertex position = "240.0   0.0 272.0" tex_coord = "0 0" />
  // <vertex position = "240.0 165.0 272.0" tex_coord = "1 0" />
  // <vertex position = " 82.0 165.0 225.0" tex_coord = "1 1" />
  // <vertex position = "82.0   0.0 225.0" tex_coord = "0 1" />
  //
  // <index value = "0" /><index value = "1" /><index value = "3" />
  // <index value = "1" /><index value = "2" /><index value = "3" />
  // <index value = "4" /><index value = "5" /><index value = "7" />
  // <index value = "5" /><index value = "6" /><index value = "7" />
  // <index value = "8" /><index value = "9" /><index value = "11" />
  // <index value = "9" /><index value = "10" /><index value = "11" />
  // <index value = "12" /><index value = "13" /><index value = "15" />
  // <index value = "13" /><index value = "14" /><index value = "15" />
  // <index value = "16" /><index value = "17" /><index value = "19" />
  // <index value = "17" /><index value = "18" /> <index value = "19" />
  indices = {0, 1,  3,  1,  2,  3,  4,  5,  7,  5,  6,  7,  8,  9,  11,
             9, 10, 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19};
  vertices.clear();
  vertices.push_back(make_vertex({130.0f, 165.0f, 65.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 165.0f, 225.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 165.0f, 272.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 165.0f, 114.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 0.0f, 114.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({290.0f, 165.0f, 114.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 165.0f, 272.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({240.0f, 0.0f, 272.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({130.0f, 0.0f, 65.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({130.0f, 165.0f, 65.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({290.0f, 165.0f, 114.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 0.0f, 114.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({82.0f, 0.0f, 225.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 165.0f, 225.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({130.0f, 165.0f, 65.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({130.0f, 0.0f, 65.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({240.0f, 0.0f, 272.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 165.0f, 272.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 165.0f, 225.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({82.0f, 0.0f, 225.0f}, {0.0f, 1.0f}));
  int short_box_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "ShortBoxMesh");
  Material short_box_material;
  //short_box_material.type = MATERIAL_TYPE_LAMBERTIAN; 
  //short_box_material.base_color = {0.8f, 0.8f, 0.8f};
  short_box_material.type = MATERIAL_TYPE_LAMBERTIAN; 
  short_box_material.base_color = {1.0f, 1.0f, 1.0f};
  short_box_material.alpha = 0.1; 
  int short_box_id = scene->CreateEntity();
  scene->SetEntityMesh(short_box_id, short_box_mesh_id);
  scene->SetEntityMaterial(short_box_id, short_box_material);

  // tall_box
  // <vertex position="423.0 330.0 247.0" tex_coord="0 0"/>
  // <vertex position="265.0 330.0 296.0" tex_coord="1 0"/>
  // <vertex position="314.0 330.0 456.0" tex_coord="1 1"/>
  // <vertex position="472.0 330.0 406.0" tex_coord="0 1"/>
  //
  // <vertex position="423.0   0.0 247.0" tex_coord="0 0"/>
  // <vertex position="423.0 330.0 247.0" tex_coord="1 0"/>
  // <vertex position="472.0 330.0 406.0" tex_coord="1 1"/>
  // <vertex position="472.0   0.0 406.0" tex_coord="0 1"/>
  //
  // <vertex position="472.0   0.0 406.0" tex_coord="0 0"/>
  // <vertex position="472.0 330.0 406.0" tex_coord="1 0"/>
  // <vertex position="314.0 330.0 456.0" tex_coord="1 1"/>
  // <vertex position="314.0   0.0 456.0" tex_coord="0 1"/>
  //
  // <vertex position="314.0   0.0 456.0" tex_coord="0 0"/>
  // <vertex position="314.0 330.0 456.0" tex_coord="1 0"/>
  // <vertex position="265.0 330.0 296.0" tex_coord="1 1"/>
  // <vertex position="265.0   0.0 296.0" tex_coord="0 1"/>
  //
  // <vertex position="265.0   0.0 296.0" tex_coord="0 0"/>
  // <vertex position="265.0 330.0 296.0" tex_coord="1 0"/>
  // <vertex position="423.0 330.0 247.0" tex_coord="1 1"/>
  // <vertex position="423.0   0.0 247.0" tex_coord="0 1"/>
  //
  //
  // <index value="0"/>
  // <index value="1"/>
  // <index value="3"/>
  // <index value="1"/>
  // <index value="2"/>
  // <index value="3"/>
  // <index value="4"/>
  // <index value="5"/>
  // <index value="7"/>
  // <index value="5"/>
  // <index value="6"/>
  // <index value="7"/>
  // <index value="8"/>
  // <index value="9"/>
  // <index value="11"/>
  // <index value="9"/>
  // <index value="10"/>
  // <index value="11"/>
  // <index value="12"/>
  // <index value="13"/>
  // <index value="15"/>
  // <index value="13"/>
  // <index value="14"/>
  // <index value="15"/>
  // <index value="16"/>
  // <index value="17"/>
  // <index value="19"/>
  // <index value="17"/>
  // <index value="18"/>
  // <index value="19"/>
  indices = {0, 1,  3,  1,  2,  3,  4,  5,  7,  5,  6,  7,  8,  9,  11,
             9, 10, 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19};
  vertices.clear();
  vertices.push_back(make_vertex({423.0f, 330.0f, 247.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({265.0f, 330.0f, 296.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({314.0f, 330.0f, 456.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({472.0f, 330.0f, 406.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({423.0f, 0.0f, 247.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({423.0f, 330.0f, 247.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({472.0f, 330.0f, 406.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({472.0f, 0.0f, 406.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({472.0f, 0.0f, 406.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({472.0f, 330.0f, 406.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({314.0f, 330.0f, 456.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({314.0f, 0.0f, 456.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({314.0f, 0.0f, 456.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({314.0f, 330.0f, 456.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({265.0f, 330.0f, 296.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({265.0f, 0.0f, 296.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({265.0f, 0.0f, 296.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({265.0f, 330.0f, 296.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({423.0f, 330.0f, 247.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({423.0f, 0.0f, 247.0f}, {0.0f, 1.0f}));

  int tall_box_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "TallBoxMesh");
  Material tall_box_material;
  tall_box_material.type = MATERIAL_TYPE_SPECULAR;
  tall_box_material.base_color = {0.8f, 0.8f, 0.8f};
  tall_box_material.ior = 1.8f;
  int tall_box_id = scene->CreateEntity();
  scene->SetEntityMesh(tall_box_id, tall_box_mesh_id);
  scene->SetEntityMaterial(tall_box_id, tall_box_material);

  scene->SetEnvmapSettings({0.0f, 0.0f, 0, 0});

  scene->Camera()->SetPosition({278.0f, 273.0f, -800.0f});
  scene->Camera()->SetEulerAngles({0.0f, glm::radians(180.0f), 0.0f});
  scene->Camera()->SetFov(glm::radians(40.0f));
  scene->Camera()->SetFar(2000.0f);
  scene->Camera()->SetCameraSpeed(100.0f);
}

void LoadIslandScene(Scene *scene) {
  AssetManager *asset_manager = scene->Renderer()->AssetManager();
  scene->Camera()->GetPosition() = glm::vec3{0.0f, 0.0f, 5.0f};

  auto envmap = scene->GetEnvMap();

  Texture envmap_texture;
  envmap_texture.LoadFromFile(FindAssetsFile("texture/envmap_clouds_4k.hdr"),
                              LDRColorSpace::UNORM);
  auto envmap_id = asset_manager->LoadTexture(envmap_texture, "Envmap");
  envmap->SetEnvmapTexture(envmap_id);
  scene->SetEnvmapSettings({0.0f, 1.0f, uint32_t(envmap_id), 0});

  int entity_id = scene->CreateEntity();

  Texture terrain_texture;
  terrain_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/terrain-texture3.bmp"),
      LDRColorSpace::UNORM);

  Texture terrain_detail_texture;
  terrain_detail_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/detail.bmp"), LDRColorSpace::UNORM);
  auto terrain_texture_id =
      asset_manager->LoadTexture(terrain_texture, "TerrainTexture");
  auto terrain_detail_texture_id = asset_manager->LoadTexture(
      terrain_detail_texture, "TerrainDetailTexture");

  scene->SetEntityAlbedoTexture(entity_id, terrain_texture_id);
  scene->SetEntityAlbedoDetailTexture(entity_id, terrain_detail_texture_id);

  Texture heightmap_texture;
  heightmap_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/heightmap.bmp"), LDRColorSpace::UNORM);
  Mesh terrain_mesh;
  terrain_mesh.LoadFromHeightMap(heightmap_texture, 1.0f, 0.2f, 0.0f);
  auto terrain_mesh_id = asset_manager->LoadMesh(terrain_mesh, "TerrainMesh");
  Material terrain_material;
  terrain_material.sheen = 1.0f;
  scene->SetEntityMesh(entity_id, terrain_mesh_id);
  scene->SetEntityMaterial(entity_id, terrain_material);
  scene->SetEntityTransform(
      entity_id,
      glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, -0.06f, 0.0f}));
  scene->SetEntityDetailScaleOffset(entity_id, {20.0f, 20.0f, 0.0f, 0.0f});

  Mesh plane_mesh;
  std::vector<Vertex> plane_vertices;
  std::vector<uint32_t> plane_indices;
  const int precision = 500;
  const float inv_precision = 1.0f / static_cast<float>(precision);
  for (int i = 0; i <= precision; i++) {
    for (int j = 0; j <= precision; j++) {
      Vertex vertex;
      vertex.position = {static_cast<float>(i) * inv_precision - 0.5f, 0.0f,
                         static_cast<float>(j) * inv_precision - 0.5f};
      vertex.normal = {0.0f, 1.0f, 0.0f};
      vertex.tangent = {1.0f, 0.0f, 0.0f};
      vertex.tex_coord = {static_cast<float>(i) * inv_precision,
                          1.0f - static_cast<float>(j) * inv_precision};
      vertex.signal = 1.0f;
      plane_vertices.push_back(vertex);
    }
  }
  for (int i = 0; i < precision; i++) {
    for (int j = 0; j < precision; j++) {
      plane_indices.push_back(i * (precision + 1) + j);
      plane_indices.push_back(i * (precision + 1) + j + 1);
      plane_indices.push_back((i + 1) * (precision + 1) + j);
      plane_indices.push_back((i + 1) * (precision + 1) + j);
      plane_indices.push_back(i * (precision + 1) + j + 1);
      plane_indices.push_back((i + 1) * (precision + 1) + j + 1);
    }
  }
  plane_mesh = {plane_vertices, plane_indices};

  auto plane_mesh_id = asset_manager->LoadMesh(plane_mesh, "PlaneMesh");

  Texture water_texture;
  water_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/SkyBox/SkyBox5.bmp"),
      LDRColorSpace::UNORM);
  auto water_texture_id =
      asset_manager->LoadTexture(water_texture, "WaterTexture");
  int water_entity_id = scene->CreateEntity();
  Material water_material;
  water_material.base_color = {1.0f, 1.0f, 1.0f};
  water_material.metallic = 0.1f;
  water_material.specular = 1.0f;
  water_material.alpha = 1.0f;
  scene->SetEntityAlbedoDetailTexture(water_entity_id, water_texture_id);
  scene->SetEntityMesh(water_entity_id, plane_mesh_id);
  scene->SetEntityMaterial(water_entity_id, water_material);
  scene->SetEntityTransform(water_entity_id,
                            glm::scale(glm::mat4{1.0f}, glm::vec3{1000.0f}));
  scene->SetEntityDetailScaleOffset(water_entity_id,
                                    {10000.0f, 10000.0f, 0.0f, 0.0f});

  scene->Camera()->SetFar(500.0f);
  scene->Camera()->SetNear(0.05f);
  scene->Camera()->SetPosition({0.0f, 0.1f, 1.2f});

  scene->SetUpdateCallback([=](Scene *scene, float delta_time) {
    glm::vec2 speed{0.3f, 1.0f};
    speed *= delta_time * 0.1f;
    glm::vec4 detail_scale_offset;
    scene->GetEntityDetailScaleOffset(water_entity_id, detail_scale_offset);
    detail_scale_offset.z += speed.x;
    detail_scale_offset.w += speed.y;
    detail_scale_offset.z = glm::mod(detail_scale_offset.z, 1.0f);
    detail_scale_offset.w = glm::mod(detail_scale_offset.w, 1.0f);
    scene->SetEntityDetailScaleOffset(water_entity_id, detail_scale_offset);
  });
}

}  // namespace sparkium
