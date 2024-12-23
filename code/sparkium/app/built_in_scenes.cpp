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
// (LR, UD, FB)
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

  vertices.push_back(make_vertex({343.0f, 848.7f, 27.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({343.0f, 848.7f, 132.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({213.0f, 848.7f, 132.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({213.0f, 848.7f, 127.0f}, {0.0f, 1.0f}));
  int light_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "LightMesh");
  Material light_material;
  light_material.base_color = {0.0f, 0.0f, 0.0f};
  light_material.emission = {1.0f, 1.0f, 1.0f};
  light_material.emission_strength = 70.0f;
  light_material.spectrum_type = SPECTRUM_TYPE_D65;
  light_material.illuminant_type = ILLUMINANT_TYPE_LAMBERTIAN;
  int light_id = scene->CreateEntity();
  scene->SetEntityMesh(light_id, light_mesh_id);
  scene->SetEntityMaterial(light_id, light_material);

  vertices.clear(); 
  vertices.push_back(make_vertex({243.0f, 2248.7f, 4432.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({-1600.0f, 2248.7f, 4432.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({-1600.0f, 1400.0f, 4432.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({243.0f, 1400.0f, 4432.0f}, {0.0f, 1.0f}));
  int light2_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "Light2Mesh");
  Material light2_material;
  light2_material.base_color = {0.0f, 0.0f, 0.0f};
  light2_material.emission = {1.0f, 1.0f, 1.0f};
  light2_material.emission_strength = 300;
  light2_material.spectrum_type = SPECTRUM_TYPE_D65;
  light2_material.illuminant_type = ILLUMINANT_TYPE_LAMBERTIAN;
  int light2_id = scene->CreateEntity();
  scene->SetEntityMesh(light2_id, light2_mesh_id);
  scene->SetEntityMaterial(light2_id, light2_material);

  // int plate_mesh_id =
  //     asset_manager->LoadMesh(plate_mesh, "PlateMesh"); 

  // Material plate_material;
  // plate_material.base_color = {1.000f, 1.000f, 1.000f};

	// plate_material.type=MATERIAL_TYPE_MULTILAYER;
  // plate_material.roughness = 0.1;
  // plate_material.specular = 0.5;
  // plate_material.specular_tint = 0.5;

  // int plate_id = scene->CreateEntity();
  // scene->SetEntityMesh(plate_id, plate_mesh_id);

  Mesh hat_mesh;
	hat_mesh.LoadObjFile(FindAssetsFile("mesh/Hat/Hat.obj"));
	hat_mesh.scale(50.0f);
  // hat_mesh.rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	hat_mesh.translate(glm::vec3(-150.0f, 115.0f, 0.0f));
  int hat_mesh_id =
      asset_manager->LoadMesh(hat_mesh, "HatMesh"); 

  Material hat_material;
  hat_material.base_color = {1.000f, 1.000f, 1.000f};
	hat_material.type=MATERIAL_TYPE_LAMBERTIAN;

  Texture hat_texture;
  hat_texture.LoadFromFile(
      FindAssetsFile("texture/Hat.jpg"),
      LDRColorSpace::UNORM);

  auto hat_texture_id =
      asset_manager->LoadTexture(hat_texture, "HatTexture");
  int hat_id = scene->CreateEntity();
  scene->SetEntityMesh(hat_id, hat_mesh_id);
  scene->SetEntityMaterial(hat_id, hat_material);
  scene->SetEntityAlbedoTexture(hat_id, hat_texture_id);
  Mesh bed_mesh;
	bed_mesh.LoadObjFile(FindAssetsFile("mesh/bed/bed.obj"));
	bed_mesh.scale(220.0f);  
  bed_mesh.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  bed_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	bed_mesh.translate(glm::vec3(-200.0f, 0.0f, 350.0f));
  int bed_mesh_id =
      asset_manager->LoadMesh(bed_mesh, "BedMesh"); 

  Material bed_material;
  bed_material.base_color = {1.000f, 1.000f, 1.000f};
	bed_material.type=MATERIAL_TYPE_MULTILAYER;
  bed_material.roughness = 0.4;
  bed_material.specular = 0.5;
  bed_material.specular_tint = 0.5;
  
  Texture bed_texture;
  bed_texture.LoadFromFile(
      FindAssetsFile("texture/bed/bed.png"),
      LDRColorSpace::UNORM);

  auto bed_texture_id =
      asset_manager->LoadTexture(bed_texture, "BedTexture");
  int bed_id = scene->CreateEntity();
  scene->SetEntityMesh(bed_id, bed_mesh_id);
  scene->SetEntityMaterial(bed_id, bed_material);
  scene->SetEntityAlbedoTexture(bed_id, bed_texture_id);

  Mesh pillow_mesh;
	pillow_mesh.LoadObjFile(FindAssetsFile("mesh/pillow/pillow.obj"));
	pillow_mesh.scale(80.0f);  
  //pillow_mesh.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  //pillow_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	pillow_mesh.translate(glm::vec3(-150.0f, 105.0f, 290.0f));
  int pillow_mesh_id =
      asset_manager->LoadMesh(pillow_mesh, "pillowMesh"); 

  Material pillow_material;
  pillow_material.base_color = {1.000f, 1.000f, 1.000f};
	pillow_material.type=MATERIAL_TYPE_MULTILAYER;
  pillow_material.roughness = 0.6;
  pillow_material.specular = 0.2;
  pillow_material.specular_tint = 0.5;
  
  Texture pillow_texture;
  pillow_texture.LoadFromFile(
      FindAssetsFile("texture/pillow.jpg"),
      LDRColorSpace::UNORM);

  auto pillow_texture_id =
      asset_manager->LoadTexture(pillow_texture, "pillowTexture");
  int pillow_id = scene->CreateEntity();
  scene->SetEntityMesh(pillow_id, pillow_mesh_id);
  scene->SetEntityMaterial(pillow_id, pillow_material);
  scene->SetEntityAlbedoTexture(pillow_id, pillow_texture_id);

  Mesh pillow2_mesh;
	pillow2_mesh.LoadObjFile(FindAssetsFile("mesh/pillow/pillow.obj"));
	pillow2_mesh.scale(80.0f);  
  pillow2_mesh.rotate(20.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  //pillow2_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	pillow2_mesh.translate(glm::vec3(-290.0f, 105.0f, 280.0f));
  int pillow2_mesh_id =
      asset_manager->LoadMesh(pillow2_mesh, "pillow2Mesh"); 

  Material pillow2_material;
  pillow2_material.base_color = {1.000f, 1.000f, 1.000f};
	pillow2_material.type=MATERIAL_TYPE_MULTILAYER;
  pillow2_material.roughness = 0.6;
  pillow2_material.specular = 0.2;
  pillow2_material.specular_tint = 0.5;
  
  int pillow2_id = scene->CreateEntity();
  scene->SetEntityMesh(pillow2_id, pillow2_mesh_id);
  scene->SetEntityMaterial(pillow2_id, pillow2_material);
  scene->SetEntityAlbedoTexture(pillow2_id, pillow_texture_id);

  Mesh mattress_mesh;
	mattress_mesh.LoadObjFile(FindAssetsFile("mesh/bed/bed2.obj"));
  mattress_mesh.scalebyindex(glm::vec3(285.0f, 290.0f, 290.0f));  
  mattress_mesh.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  mattress_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mattress_mesh.translate(glm::vec3(-195.0f, 10.0f, 345.0f));
  int mattress_mesh_id =
      asset_manager->LoadMesh(mattress_mesh, "BedMesh"); 

  Material mattress_material;
	mattress_material.type=MATERIAL_TYPE_LAMBERTIAN; 
  mattress_material.roughness = 0.8;
  mattress_material.sheen = 0.3; 
  mattress_material.sheen_tint = 0.5;
  mattress_material.specular = 0.2; 
  mattress_material.specular_tint = 0.3;
  
  Texture mattress_texture;
  mattress_texture.LoadFromFile(
      FindAssetsFile("texture/bed/mattress.png"),
      LDRColorSpace::UNORM);

  auto mattress_texture_id =
      asset_manager->LoadTexture(mattress_texture, "MattressTexture");
  int mattress_id = scene->CreateEntity();
  scene->SetEntityMesh(mattress_id, mattress_mesh_id);
  scene->SetEntityMaterial(mattress_id, mattress_material);
  scene->SetEntityAlbedoTexture(mattress_id, mattress_texture_id);

  Mesh frame1_mesh;
	frame1_mesh.LoadObjFile(FindAssetsFile("mesh/frame/frame.obj"));
	frame1_mesh.scalebyindex(glm::vec3(70.0f, 117.0f, 137.0f));  
  frame1_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  //frame1_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	frame1_mesh.translate(glm::vec3(-400.0f, 357.0f, 286.0f));
  int frame1_mesh_id =
      asset_manager->LoadMesh(frame1_mesh, "frame1Mesh"); 

  Material frame1_material;
  frame1_material.base_color = {1.000f, 1.000f, 1.000f};
	frame1_material.type=MATERIAL_TYPE_LAMBERTIAN;
  frame1_material.roughness = 0.4;
  frame1_material.specular = 0.2;
  frame1_material.specular_tint = 0.5;
  
  Texture frame1_texture;
  frame1_texture.LoadFromFile(
      FindAssetsFile("texture/floor.jpg"),
      LDRColorSpace::UNORM);

  auto frame1_texture_id =
      asset_manager->LoadTexture(frame1_texture, "frame1Texture");
  int frame1_id = scene->CreateEntity();
  scene->SetEntityMesh(frame1_id, frame1_mesh_id);
  scene->SetEntityMaterial(frame1_id, frame1_material);
  scene->SetEntityAlbedoTexture(frame1_id, frame1_texture_id);
/*
  Mesh frame2_mesh;
	frame2_mesh.LoadObjFile(FindAssetsFile("mesh/frame/frame.obj"));
	frame2_mesh.scale(70.0f);  
  frame2_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  //frame2_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	frame2_mesh.translate(glm::vec3(-400.0f, 500.0f, 400.0f));
  int frame2_mesh_id =
      asset_manager->LoadMesh(frame2_mesh, "frame2Mesh"); 

  Material frame2_material;
  frame2_material.base_color = {1.000f, 1.000f, 1.000f};
	frame2_material.type=MATERIAL_TYPE_MULTILAYER;
  frame2_material.roughness = 0.4;
  frame2_material.specular = 0.2;
  frame2_material.specular_tint = 0.5;
  
  Texture frame2_texture;
  frame2_texture.LoadFromFile(
      FindAssetsFile("texture/floor.jpg"),
      LDRColorSpace::UNORM);

  auto frame2_texture_id =
      asset_manager->LoadTexture(frame2_texture, "frame2Texture");
  int frame2_id = scene->CreateEntity();
  scene->SetEntityMesh(frame2_id, frame2_mesh_id);
  scene->SetEntityMaterial(frame2_id, frame2_material);
  scene->SetEntityAlbedoTexture(frame2_id, frame2_texture_id);

  Mesh frame4_mesh;
	frame4_mesh.LoadObjFile(FindAssetsFile("mesh/frame/frame.obj"));
	frame4_mesh.scale(70.0f);  
  frame4_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  //frame4_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	frame4_mesh.translate(glm::vec3(-400.0f, 300.0f, 400.0f));
  int frame4_mesh_id =
      asset_manager->LoadMesh(frame4_mesh, "frame4Mesh"); 

  Material frame4_material;
  frame4_material.base_color = {1.000f, 1.000f, 1.000f};
	frame4_material.type=MATERIAL_TYPE_MULTILAYER;
  frame4_material.roughness = 0.4;
  frame4_material.specular = 0.2;
  frame4_material.specular_tint = 0.5;
  
  Texture frame4_texture;
  frame4_texture.LoadFromFile(
      FindAssetsFile("texture/floor.jpg"),
      LDRColorSpace::UNORM);

  auto frame4_texture_id =
      asset_manager->LoadTexture(frame4_texture, "frame4Texture");
  int frame4_id = scene->CreateEntity();
  scene->SetEntityMesh(frame4_id, frame4_mesh_id);
  scene->SetEntityMaterial(frame4_id, frame4_material);
  scene->SetEntityAlbedoTexture(frame4_id, frame4_texture_id);
*/
  Mesh frame5_mesh;
	frame5_mesh.LoadObjFile(FindAssetsFile("mesh/frame/frame.obj"));
	frame5_mesh.scalebyindex(glm::vec3(120.0f, 145.0f, 150.0f));  
  frame5_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  frame5_mesh.rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
  //frame5_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	frame5_mesh.translate(glm::vec3(-200.0f, 360.0f, 550.0f));
  int frame5_mesh_id =
      asset_manager->LoadMesh(frame5_mesh, "frame5Mesh"); 

  Material frame5_material;
  frame5_material.base_color = {1.000f, 1.000f, 1.000f};
	frame5_material.type=MATERIAL_TYPE_LAMBERTIAN;
  frame5_material.roughness = 0.4;
  frame5_material.specular = 0.2;
  frame5_material.specular_tint = 0.5;
  
  Texture frame5_texture;
  frame5_texture.LoadFromFile(
      FindAssetsFile("texture/floor.jpg"),
      LDRColorSpace::UNORM);

  auto frame5_texture_id =
      asset_manager->LoadTexture(frame5_texture, "frame5Texture");
  int frame5_id = scene->CreateEntity();
  scene->SetEntityMesh(frame5_id, frame5_mesh_id);
  scene->SetEntityMaterial(frame5_id, frame5_material);
  scene->SetEntityAlbedoTexture(frame5_id, frame5_texture_id);

  vertices.clear();
  vertices.push_back(make_vertex({-74.0f, 455.0f, 530.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({-326.0f, 455.0f, 530.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({-326.0f, 255.0f, 530.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({-74.0f, 255.0f, 530.0f}, {0.0f, 1.0f}));
  int starry_night_mesh_id = asset_manager->LoadMesh(Mesh(vertices, indices), "StarryNightMesh");
  Material starry_night_material;
  starry_night_material.type = MATERIAL_TYPE_LAMBERTIAN;
  Texture starry_night_texture;
  starry_night_texture.LoadFromFile(
      FindAssetsFile("texture/starry_night.jpg"),
      LDRColorSpace::UNORM);
  auto starry_night_texture_id =
      asset_manager->LoadTexture(starry_night_texture, "StarryNightTexture");
  int starry_night_id = scene->CreateEntity();
  scene->SetEntityMesh(starry_night_id, starry_night_mesh_id);
  scene->SetEntityMaterial(starry_night_id, starry_night_material);
  scene->SetEntityAlbedoTexture(starry_night_id, starry_night_texture_id);

  vertices.clear();
  vertices.push_back(make_vertex({-388.0f, 455.0f, 360.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({-388.0f, 455.0f, 201.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({-388.0f, 255.0f, 201.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({-388.0f, 255.0f, 360.0f}, {0.0f, 1.0f}));
  int sunflower_mesh_id = asset_manager->LoadMesh(Mesh(vertices, indices), "SunflowerMesh");
  Material sunflower_material;
  sunflower_material.type = MATERIAL_TYPE_LAMBERTIAN;
  Texture sunflower_texture;
  sunflower_texture.LoadFromFile(
      FindAssetsFile("texture/sunflower.jpg"),
      LDRColorSpace::UNORM);
  auto sunflower_texture_id =
      asset_manager->LoadTexture(sunflower_texture, "SunflowerTexture");
  int sunflower_id = scene->CreateEntity();
  scene->SetEntityMesh(sunflower_id, sunflower_mesh_id);
  scene->SetEntityMaterial(sunflower_id, sunflower_material);
  scene->SetEntityAlbedoTexture(sunflower_id, sunflower_texture_id);

  Mesh mirror_frame_mesh;
	mirror_frame_mesh.LoadObjFile(FindAssetsFile("mesh/mirror/mirror_frame.obj"));
	mirror_frame_mesh.scale(80.0f);  
  //mirror_frame_mesh.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  //mirror_frame_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mirror_frame_mesh.translate(glm::vec3(420.0f, 305.0f, 550.0f));
  int mirror_frame_mesh_id =
      asset_manager->LoadMesh(mirror_frame_mesh, "mirror_frameMesh"); 

  Material mirror_frame_material;
  mirror_frame_material.base_color = {0.5, 0.25, 0.1};
	mirror_frame_material.type=MATERIAL_TYPE_MULTILAYER;
  mirror_frame_material.roughness = 0.6;
  mirror_frame_material.specular = 0.2;
  mirror_frame_material.specular_tint = 0.5;
  mirror_frame_material.sheen = 1.0;
  mirror_frame_material.sheen_tint = 0.5; 
  mirror_frame_material.clearcoat = 0.7; 
  mirror_frame_material.clearcoat_roughness = 0.1;

  Texture mirror_frame_texture;
  mirror_frame_texture.LoadFromFile(
      FindAssetsFile("texture/mirror/mirror_frame.png"),
      LDRColorSpace::UNORM);

  auto mirror_frame_texture_id =
    asset_manager->LoadTexture(mirror_frame_texture, "mirror_frameTexture");
  int mirror_frame_id = scene->CreateEntity();
  scene->SetEntityMesh(mirror_frame_id, mirror_frame_mesh_id);
  scene->SetEntityMaterial(mirror_frame_id, mirror_frame_material);
  scene->SetEntityAlbedoTexture(mirror_frame_id, mirror_frame_texture_id);

  Mesh mirror_mesh;
	mirror_mesh.LoadObjFile(FindAssetsFile("mesh/mirror/mirror.obj"));
	mirror_mesh.scale(80.0f);  
  mirror_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  mirror_mesh.rotate(5.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  //mirror_mesh.rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	mirror_mesh.translate(glm::vec3(420.0f, 305.0f, 548.0f));
  int mirror_mesh_id =
      asset_manager->LoadMesh(mirror_mesh, "mirrorMesh"); 

  Material mirror_material;
  mirror_material.base_color = {1.000f, 1.000f, 1.000f};
	mirror_material.type=MATERIAL_TYPE_SPECULAR;
  
  Texture mirror_texture;
  mirror_texture.LoadFromFile(
      FindAssetsFile("texture/mirror/mirror.jpg"),
      LDRColorSpace::UNORM);

  auto mirror_texture_id =
      asset_manager->LoadTexture(mirror_texture, "mirrorTexture");
  int mirror_id = scene->CreateEntity();
  scene->SetEntityMesh(mirror_id, mirror_mesh_id);
  scene->SetEntityMaterial(mirror_id, mirror_material);
  scene->SetEntityAlbedoTexture(mirror_id, mirror_texture_id);

  Mesh lantern_frame_mesh;
	lantern_frame_mesh.LoadObjFile(FindAssetsFile("mesh/lantern/lantern_frame.obj"));
  lantern_frame_mesh.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  lantern_frame_mesh.scale(100.0f);  
  // lantern_frame_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	lantern_frame_mesh.translate(glm::vec3(0.0f, 400.0f, 550.0f));
  int lantern_frame_mesh_id =
      asset_manager->LoadMesh(lantern_frame_mesh, "lantern_frameMesh"); 

  Material lantern_frame_material;
  lantern_frame_material.base_color = {1.000f, 1.000f, 1.000f};
	lantern_frame_material.type=MATERIAL_TYPE_METAL;
  lantern_frame_material.roughness = 0.4;

  Texture lantern_frame_texture;
  lantern_frame_texture.LoadFromFile(
      FindAssetsFile("texture/lamp.png"),
      LDRColorSpace::UNORM);

  auto lantern_frame_texture_id =
      asset_manager->LoadTexture(lantern_frame_texture, "lantern_frameTexture");
  int lantern_frame_id = scene->CreateEntity();
  scene->SetEntityMesh(lantern_frame_id, lantern_frame_mesh_id);
  scene->SetEntityMaterial(lantern_frame_id, lantern_frame_material);
  scene->SetEntityAlbedoTexture(lantern_frame_id, lantern_frame_texture_id);

  Mesh lantern_glass_mesh;
	lantern_glass_mesh.LoadObjFile(FindAssetsFile("mesh/lantern/lantern_glass.obj"));
  lantern_glass_mesh.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  lantern_glass_mesh.scale(100.0f);  
  // lantern_glass_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	lantern_glass_mesh.translate(glm::vec3(0.0f, 400.0f, 550.0f));
  int lantern_glass_mesh_id =
      asset_manager->LoadMesh(lantern_glass_mesh, "lantern_glassMesh"); 

  Material lantern_glass_material;
  lantern_glass_material.base_color = {1.000f, 1.000f, 1.000f};
	lantern_glass_material.type=MATERIAL_TYPE_RETRACTIVE;
  lantern_glass_material.ior = 1.5; 
  
  int lantern_glass_id = scene->CreateEntity();
  scene->SetEntityMesh(lantern_glass_id, lantern_glass_mesh_id);
  scene->SetEntityMaterial(lantern_glass_id, lantern_glass_material);

  Mesh lantern_lamp_mesh;
	lantern_lamp_mesh.LoadObjFile(FindAssetsFile("mesh/lantern/lamp.obj"));
  lantern_lamp_mesh.rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  lantern_lamp_mesh.scale(100.0f);  
  // lantern_lamp_mesh.rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	lantern_lamp_mesh.translate(glm::vec3(0.0f, 400.0f, 550.0f));
  int lantern_lamp_mesh_id =
      asset_manager->LoadMesh(lantern_lamp_mesh, "lantern_lampMesh"); 

  Material lantern_lamp_material;
  lantern_lamp_material.base_color = {1.000f, 1.000f, 1.000f};
	lantern_lamp_material.type=MATERIAL_TYPE_VOLUME; 
  lantern_lamp_material.a = 1.5046;
  lantern_lamp_material.b = 4200;
  lantern_lamp_material.c = 7650;
  lantern_lamp_material.g = 0.9;
  lantern_lamp_material.sigma_a = 1e-1 * 0.7;
  lantern_lamp_material.sigma_s = 1e-1 * 0.3;
  lantern_lamp_material.spectrum_type = SPECTRUM_TYPE_D65;
  lantern_lamp_material.emission_strength = 100.0f;
  
  int lantern_lamp_id = scene->CreateEntity();
  scene->SetEntityMesh(lantern_lamp_id, lantern_lamp_mesh_id);
  scene->SetEntityMaterial(lantern_lamp_id, lantern_lamp_material);

  Mesh table_mesh;
	table_mesh.LoadObjFile(FindAssetsFile("mesh/Table/Wood_Table.obj"));

	table_mesh.scalebyindex(glm::vec3(120.0f, 150.0f, 120.0f));
  table_mesh.rotate(75.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	table_mesh.translate(glm::vec3(230.0f, 0.0f, 250.0f));

  int table_mesh_id =
      asset_manager->LoadMesh(table_mesh, "TableMesh"); 

  Material table_material;
  table_material.base_color = {1.000f, 1.000f, 1.000f};
	table_material.type=MATERIAL_TYPE_MULTILAYER;
  table_material.roughness = 0.4;
  table_material.specular = 0.5;
  table_material.specular_tint = 0.5;
  
  Texture table_texture;
  table_texture.LoadFromFile(
      FindAssetsFile("texture/table.jpg"),
      LDRColorSpace::UNORM);

  auto table_texture_id =
      asset_manager->LoadTexture(table_texture, "TableTexture");
  int table_id = scene->CreateEntity();
  scene->SetEntityMesh(table_id, table_mesh_id);
  scene->SetEntityMaterial(table_id, table_material);
  scene->SetEntityAlbedoTexture(table_id, table_texture_id);

  Mesh chair_mesh;
	chair_mesh.LoadObjFile(FindAssetsFile("mesh/chair/chair.obj"));
	chair_mesh.scale(120.0f);
  chair_mesh.rotate(145.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	chair_mesh.translate(glm::vec3(70.0f, 0.0f, 250.0f));

  int chair_mesh_id =
      asset_manager->LoadMesh(chair_mesh, "ChairMesh"); 

  Material chair_material;
  chair_material.base_color = {0.5f, 0.3f, 0.1f};
	chair_material.type=MATERIAL_TYPE_MULTILAYER; 
  chair_material.roughness = 1.0;
  chair_material.sheen = 1.0; 
  chair_material.sheen_tint = 0.5;
  chair_material.clearcoat = 0.5;
  chair_material.clearcoat_roughness = 0.1;
  chair_material.specular = 0.5; 
  chair_material.specular_tint = 0.5;
  chair_material.metallic = 0.5; 
  
  Texture chair_texture;
  chair_texture.LoadFromFile(
      FindAssetsFile("texture/chair/chair_01_Base_Color.png"),
      LDRColorSpace::UNORM);

  Texture chair_roughness_texture;
  chair_roughness_texture.LoadRoughnessTextureFromFile(
      FindAssetsFile("texture/chair/chair_01_Roughness.png"));

  Texture chair_normal_texture;
  chair_normal_texture.LoadNormalTextureFromFile(
      FindAssetsFile("texture/chair/chair_01_Normal_DirectX.png"));
    
  auto chair_texture_id =
      asset_manager->LoadTexture(chair_texture, "ChairTexture");
  auto chair_roughness_texture_id =
      asset_manager->LoadTexture(chair_roughness_texture, "ChairRoughnessTexture");
  auto chair_normal_texture_id =
      asset_manager->LoadTexture(chair_normal_texture, "ChairNormalTexture");

  int chair_id = scene->CreateEntity();
  scene->SetEntityMesh(chair_id, chair_mesh_id);
  scene->SetEntityMaterial(chair_id, chair_material);
  scene->SetEntityAlbedoTexture(chair_id, chair_texture_id);
  scene->SetEntityRoughnessTexture(chair_id, chair_roughness_texture_id);
  scene->SetEntityNormalTexture(chair_id, chair_normal_texture_id);

  Mesh second_chair_mesh;
	second_chair_mesh.LoadObjFile(FindAssetsFile("mesh/chair/chair.obj"));
	second_chair_mesh.scale(120.0f);
  second_chair_mesh.rotate(125.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	second_chair_mesh.translate(glm::vec3(480.0f, 0.0f, 50.0f));

  int second_chair_mesh_id =
      asset_manager->LoadMesh(second_chair_mesh, "ChairMesh"); 

  Material second_chair_material;
  second_chair_material.base_color = {0.5f, 0.3f, 0.1f};
	second_chair_material.type=MATERIAL_TYPE_MULTILAYER; 
  second_chair_material.roughness = 1.0;
  second_chair_material.sheen = 1.0; 
  second_chair_material.sheen_tint = 0.5;
  second_chair_material.clearcoat = 0.5;
  second_chair_material.clearcoat_roughness = 0.1;
  second_chair_material.specular = 0.5; 
  second_chair_material.specular_tint = 0.5;
  second_chair_material.metallic = 0.5; 
  
  Texture second_chair_texture;
  second_chair_texture.LoadFromFile(
      FindAssetsFile("texture/chair/chair_01_Base_Color.png"),
      LDRColorSpace::UNORM);

  Texture second_chair_roughness_texture;
  second_chair_roughness_texture.LoadRoughnessTextureFromFile(
      FindAssetsFile("texture/chair/chair_01_Roughness.png"));

  Texture second_chair_normal_texture;
  second_chair_normal_texture.LoadNormalTextureFromFile(
      FindAssetsFile("texture/chair/chair_01_Normal_DirectX.png"));
    
  auto second_chair_texture_id =
      asset_manager->LoadTexture(second_chair_texture, "ChairTexture");
  auto second_chair_roughness_texture_id =
      asset_manager->LoadTexture(second_chair_roughness_texture, "ChairRoughnessTexture");
  auto second_chair_normal_texture_id =
      asset_manager->LoadTexture(second_chair_normal_texture, "ChairNormalTexture");

  int second_chair_id = scene->CreateEntity();
  scene->SetEntityMesh(second_chair_id, second_chair_mesh_id);
  scene->SetEntityMaterial(second_chair_id, second_chair_material);
  scene->SetEntityAlbedoTexture(second_chair_id, second_chair_texture_id);
  scene->SetEntityRoughnessTexture(second_chair_id, second_chair_roughness_texture_id);
  scene->SetEntityNormalTexture(second_chair_id, second_chair_normal_texture_id);

  Mesh teapot1_mesh;
	teapot1_mesh.LoadObjFile(FindAssetsFile("mesh/teapot1/Teapot1.obj"));
	teapot1_mesh.scale(30.0f);
  //teapot1_mesh.rotate(75.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	teapot1_mesh.translate(glm::vec3(205.0f, 180.0f, 310.0f));

  int teapot1_mesh_id =
      asset_manager->LoadMesh(teapot1_mesh, "teapot1Mesh"); 

  Material teapot1_material;
  teapot1_material.base_color = {1.000f, 1.000f, 1.000f};
	teapot1_material.type=MATERIAL_TYPE_METAL;
  teapot1_material.roughness = 0.4;
  
  Texture teapot1_texture;
  teapot1_texture.LoadFromFile(
      FindAssetsFile("texture/teapot1.png"),
      LDRColorSpace::UNORM);

  auto teapot1_texture_id =
      asset_manager->LoadTexture(teapot1_texture, "teapot1Texture");
  int teapot1_id = scene->CreateEntity();
  scene->SetEntityMesh(teapot1_id, teapot1_mesh_id);
  scene->SetEntityMaterial(teapot1_id, teapot1_material);
  scene->SetEntityAlbedoTexture(teapot1_id, teapot1_texture_id);

  Mesh vase_mesh;
	vase_mesh.LoadObjFile(FindAssetsFile("mesh/vase/vase.obj"));
	vase_mesh.scale(40.0f);
  //vase_mesh.rotate(75.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	vase_mesh.translate(glm::vec3(205.0f, 145.0f, 210.0f));

  int vase_mesh_id =
      asset_manager->LoadMesh(vase_mesh, "vaseMesh"); 

  Material vase_material;
  vase_material.base_color = {1.0f, 0.5f, 0.3f};
	vase_material.type=MATERIAL_TYPE_RETRACTIVE;
  vase_material.ior = 1.5; 

  int vase_id = scene->CreateEntity();
  scene->SetEntityMesh(vase_id, vase_mesh_id);
  scene->SetEntityMaterial(vase_id, vase_material);

  Mesh pot_mesh;
	pot_mesh.LoadObjFile(FindAssetsFile("mesh/pot/pot.obj"));
	pot_mesh.scale(50.0f);
  pot_mesh.rotate(115.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	pot_mesh.translate(glm::vec3(265.0f, 145.0f, 240.0f));

  int pot_mesh_id =
      asset_manager->LoadMesh(pot_mesh, "potMesh"); 

  Material pot_material;
  pot_material.base_color = {1.000f, 1.000f, 1.000f};
	pot_material.type=MATERIAL_TYPE_MULTILAYER; 
  pot_material.roughness = 0.05;
  pot_material.sheen = 0.3; 
  pot_material.sheen_tint = 0.5;
  pot_material.clearcoat = 1.0; 
  pot_material.clearcoat_roughness = 0.1; 
  pot_material.specular = 0.8;
  pot_material.specular_tint = 0.8;
  int pot_id = scene->CreateEntity();
  scene->SetEntityMesh(pot_id, pot_mesh_id);
  scene->SetEntityMaterial(pot_id, pot_material);

  Mesh window_mesh;
  window_mesh.LoadObjFile(FindAssetsFile("mesh/Window/window.obj"));
  window_mesh.scale(200.0f);
  window_mesh.rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  window_mesh.translate(glm::vec3(200.0f, 400.0f, 575.2f));

  int window_mesh_id =
      asset_manager->LoadMesh(window_mesh, "WindowMesh"); 

  Material window_material;
  window_material.type=MATERIAL_TYPE_MULTILAYER; 
  window_material.roughness = 0.1; 
  window_material.specular = 0.7; 
  window_material.specular_tint = 0.5; 
  window_material.clearcoat = 0.7;
  window_material.clearcoat_roughness = 0.1;
  Texture window_texture;
  window_texture.LoadFromFile(
      FindAssetsFile("texture/Window/window.png"),
      LDRColorSpace::UNORM);

  auto window_texture_id =
      asset_manager->LoadTexture(window_texture, "WindowTexture");

  int window_id = scene->CreateEntity();
  scene->SetEntityMesh(window_id, window_mesh_id);
  scene->SetEntityMaterial(window_id, window_material);
  //scene->SetEntityAlbedoTexture(window_id, window_texture_id);
  
  vertices.clear();
  vertices.push_back(make_vertex({605.9f, 0.0f, -800.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 0.0f, -800.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 0.0f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({605.9f, 0.0f, 559.2f}, {0.0f, 1.0f}));
  int floor_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "FloorMesh");
  Material floor_material;
  floor_material.base_color = {0.8, 0.8, 0.8}; 
  Texture floor_texture;
  floor_texture.LoadFromFile(
      FindAssetsFile("texture/floor.jpg"),
      LDRColorSpace::UNORM);
  floor_material.type = MATERIAL_TYPE_MULTILAYER; 
  floor_material.roughness = 0.4; 
  floor_material.sheen = 0.5;
  floor_material.sheen_tint = 0.5;
  floor_material.clearcoat = 0.7;
  floor_material.clearcoat_roughness = 0.1;
  floor_material.specular = 0.2;
  floor_material.specular_tint = 1.0;
  //floor_material.metallic = 0.5;
  auto floor_texture_id =
      asset_manager->LoadTexture(floor_texture, "FloorTexture");
  int floor_id = scene->CreateEntity();
  scene->SetEntityMesh(floor_id, floor_mesh_id);
  scene->SetEntityMaterial(floor_id, floor_material);
  scene->SetEntityAlbedoTexture(floor_id, floor_texture_id);

  Mesh door_mesh;
	door_mesh.LoadObjFile(FindAssetsFile("mesh/door/door.obj"));
	door_mesh.scale(250.0f);
  door_mesh.rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  //door_mesh.rotate(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	door_mesh.translate(glm::vec3(617.0f, -15.0f, 400.0f));

  int door_mesh_id =
      asset_manager->LoadMesh(door_mesh, "doorMesh"); 

  Material door_material;
  door_material.base_color = {1.000f, 1.000f, 1.000f};
	door_material.type=MATERIAL_TYPE_MULTILAYER; 
  door_material.roughness = 0.5;
  door_material.clearcoat = 1.0; 
  door_material.clearcoat_roughness = 0.1; 
  door_material.specular = 0.2;
  door_material.specular_tint = 0.8;

  Texture door_texture;
  door_texture.LoadFromFile(
      FindAssetsFile("texture/door/door.png"),
      LDRColorSpace::UNORM);

  auto door_texture_id =
      asset_manager->LoadTexture(door_texture, "doorTexture");

  int door_id = scene->CreateEntity();
  scene->SetEntityMesh(door_id, door_mesh_id);
  scene->SetEntityMaterial(door_id, door_material);
  scene->SetEntityAlbedoTexture(door_id, door_texture_id);

  Texture wall_texture;
  wall_texture.LoadFromFile(
      FindAssetsFile("texture/wall.jpg"),
      LDRColorSpace::UNORM);

  auto wall_texture_id =
      asset_manager->LoadTexture(wall_texture, "WallTexture");  
  vertices.clear();
  vertices.push_back(make_vertex({605.9f, 848.8f, -800.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({605.9f, 848.8f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 848.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 848.8f, -800.0f}, {0.0f, 1.0f}));
  int ceiling_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "CeilingMesh");
  Material ceiling_material;
  ceiling_material.base_color = {0.2, 0.6, 1.0};
  int ceiling_id = scene->CreateEntity();
  scene->SetEntityMesh(ceiling_id, ceiling_mesh_id);
  scene->SetEntityMaterial(ceiling_id, ceiling_material);

  vertices.clear();
  vertices.push_back(make_vertex({605.9f, 848.8f, 559.2f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 848.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({605.9f, 0.0f, 559.2f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({316.0f, 600.0f, 559.2f}, {0.4870f, 0.5896f}));
  vertices.push_back(make_vertex({84.0f, 600.0f, 559.2f}, {0.7177f, 0.5896f}));
  vertices.push_back(make_vertex({84.0f, 190.0f, 559.2f}, {0.7177f, 0.1061f}));
  vertices.push_back(make_vertex({316.0f, 190.0f, 559.2f}, {0.4870f, 0.1061f}));
  std::vector<uint32_t> new_indices = {0, 1, 4, 4, 1, 5, 5, 1, 2, 5, 2, 6, 6, 2, 3, 6, 3, 7, 7, 3, 0, 7, 0, 4};
  int back_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, new_indices), "BackWallMesh");
  Material back_wall_material;
  back_wall_material.base_color = {0.2, 0.6, 1.0};
  int back_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(back_wall_id, back_wall_mesh_id);
  scene->SetEntityMaterial(back_wall_id, back_wall_material);
  scene->SetEntityAlbedoTexture(back_wall_id, wall_texture_id);

  // Clear previous vertices
  vertices.clear();
  float depth = 2.0f;
    vertices.push_back(make_vertex({306.0f, 600.0f, 570.2f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 600.0f, 570.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 190.0f, 570.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({306.0f, 190.0f, 570.2f}, {0.0f, 0.0f}));
  int window_glass_mesh_id = asset_manager->LoadMesh(Mesh(vertices, indices), "WindowGlassMesh");
  Material window_glass_material;
  window_glass_material.type = MATERIAL_TYPE_VOLUME;
  window_glass_material.base_color = {0.8f, 0.8f, 0.8f};
  window_glass_material.ior = 1.5;
  window_glass_material.sigma_s = 7e-2;
  window_glass_material.g = 0.9;
  int window_glass_id = scene->CreateEntity();
  scene->SetEntityMesh(window_glass_id, window_glass_mesh_id);
  scene->SetEntityMaterial(window_glass_id, window_glass_material);

  // Back face
  vertices.clear();
  vertices.push_back(make_vertex({306.0f, 600.0f, 570.2f - depth}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 600.0f, 570.2f - depth}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 190.0f, 570.2f - depth}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({306.0f, 190.0f, 570.2f - depth}, {0.0f, 0.0f}));
  window_glass_mesh_id = asset_manager->LoadMesh(Mesh(vertices, indices), "WindowGlassMeshBack");
  window_glass_id = scene->CreateEntity();
  scene->SetEntityMesh(window_glass_id, window_glass_mesh_id);
  scene->SetEntityMaterial(window_glass_id, window_glass_material);

  // Top face
  vertices.clear();
  vertices.push_back(make_vertex({306.0f, 600.0f, 570.2f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 600.0f, 570.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 600.0f, 570.2f - depth}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({306.0f, 600.0f, 570.2f - depth}, {0.0f, 0.0f}));
  window_glass_mesh_id = asset_manager->LoadMesh(Mesh(vertices, indices), "WindowGlassMeshTop");
  window_glass_id = scene->CreateEntity();
  scene->SetEntityMesh(window_glass_id, window_glass_mesh_id);
  scene->SetEntityMaterial(window_glass_id, window_glass_material);

  // Bottom face
  vertices.clear();
  vertices.push_back(make_vertex({306.0f, 190.0f, 570.2f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 190.0f, 570.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 190.0f, 570.2f - depth}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({306.0f, 190.0f, 570.2f - depth}, {0.0f, 0.0f}));
  window_glass_mesh_id = asset_manager->LoadMesh(Mesh(vertices, indices), "WindowGlassMeshBottom");
  window_glass_id = scene->CreateEntity();
  scene->SetEntityMesh(window_glass_id, window_glass_mesh_id);
  scene->SetEntityMaterial(window_glass_id, window_glass_material);

  // Left face
  vertices.clear();
  vertices.push_back(make_vertex({94.0f, 600.0f, 570.2f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 190.0f, 570.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({94.0f, 190.0f, 570.2f - depth}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({94.0f, 600.0f, 570.2f - depth}, {0.0f, 0.0f}));
  window_glass_mesh_id = asset_manager->LoadMesh(Mesh(vertices, indices), "WindowGlassMeshLeft");
  window_glass_id = scene->CreateEntity();
  scene->SetEntityMesh(window_glass_id, window_glass_mesh_id);
  scene->SetEntityMaterial(window_glass_id, window_glass_material);

  // Right face
  vertices.clear();
  vertices.push_back(make_vertex({306.0f, 600.0f, 570.2f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({306.0f, 190.0f, 570.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({306.0f, 190.0f, 570.2f - depth}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({306.0f, 600.0f, 570.2f - depth}, {0.0f, 0.0f}));
  window_glass_mesh_id = asset_manager->LoadMesh(Mesh(vertices, indices), "WindowGlassMeshRight");
  window_glass_id = scene->CreateEntity();
  scene->SetEntityMesh(window_glass_id, window_glass_mesh_id);
  scene->SetEntityMaterial(window_glass_id, window_glass_material);

  vertices.clear();
  vertices.push_back(make_vertex({-400.0f, 0.0f, 570.2f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 0.0f, -800.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({-400.0f, 848.8f, -800.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 848.8f, 570.2f}, {0.0f, 1.0f}));
  int right_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "RightWallMesh");
  Material right_wall_material;
  right_wall_material.base_color = {0.2, 0.6, 1.0};
  int right_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(right_wall_id, right_wall_mesh_id);
  scene->SetEntityMaterial(right_wall_id, right_wall_material);
  scene->SetEntityAlbedoTexture(right_wall_id, wall_texture_id);
  scene->SetEntityAlbedoDetailTexture(right_wall_id, wall_texture_id);

  vertices.clear();
  vertices.push_back(make_vertex({605.9f, 0.0f, -800.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({605.9f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({605.9f, 848.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({605.9f, 848.8f, -800.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({605.9f, 0.0f, 200.2f}, {0.7359f, 0.0f}));
  vertices.push_back(make_vertex({605.9f, 0.0f, 385.2f}, {0.8720f, 0.0f}));
  vertices.push_back(make_vertex({605.9f, 370.5f, 385.2f}, {0.8720f, 0.4365f}));
  vertices.push_back(make_vertex({605.9f, 370.5f, 200.2f}, {0.7359f, 0.4365f}));
  new_indices = {5, 1, 2, 5, 2, 6, 6, 2, 3, 6, 3, 7, 7, 3, 0, 7, 0, 4};
  int left_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, new_indices), "LeftWallMesh");
  Material left_wall_material;
  left_wall_material.base_color = {0.2, 0.6, 1.0};
  int left_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(left_wall_id, left_wall_mesh_id);
  scene->SetEntityMaterial(left_wall_id, left_wall_material);
  scene->SetEntityAlbedoTexture(left_wall_id, wall_texture_id);

  vertices.clear();
  vertices.push_back(make_vertex({605.9f, 848.8f, -800.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 848.8f, -800.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({-400.0f, 0.0f,  -800.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({605.9f, 0.0f,  -800.0f}, {0.0f, 0.0f}));
  int front_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "FrontWallMesh");
  Material front_wall_material;
  front_wall_material.base_color = {0.2, 0.6, 1.0};
  int front_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(front_wall_id, front_wall_mesh_id);
  scene->SetEntityMaterial(front_wall_id, front_wall_material);
  //(LR, UD, FB)

  auto envmap = scene->GetEnvMap();

  Texture envmap_texture;
  envmap_texture.LoadFromFile(FindAssetsFile("texture/sky.hdr"),
                              LDRColorSpace::UNORM);
  auto envmap_id = asset_manager->LoadTexture(envmap_texture, "Envmap");
  envmap->SetEnvmapTexture(envmap_id);

  scene->SetEnvmapSettings({0.0f, 1.0f, uint32_t(envmap_id), 0});
  scene->Camera()->SetPosition({108.0f, 343.0f, -600.0f});
  scene->Camera()->SetEulerAngles({glm::radians(-10.0f), glm::radians(180.0f), 0.0f});
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

	// // Create a pointlight

	// Mesh light_sphere_mesh;
  // glm::vec3 light_sphere_position = glm::vec3(20.0f, 20.0f, 20.0f);  // Position of the light sphere
  // light_sphere_mesh.CreateSphere(light_sphere_position, 10.0f, 4, 4);  // Small sphere with radius 10
  // int light_sphere_mesh_id = asset_manager->LoadMesh(light_sphere_mesh, "LightSphereMesh");

  // Material light_sphere_material;
  // light_sphere_material.base_color = {1.0f, 1.0f, 1.0f};  // White light
  // light_sphere_material.emission = {1.0f, 1.0f, 1.0f};  // Emissive material
  // light_sphere_material.emission_strength = 50.0f;  // Adjust the strength as needed
  // light_sphere_material.spectrum_type = SPECTRUM_TYPE_D75;
  // light_sphere_material.illuminant_type = ILLUMINANT_TYPE_LAMBERTIAN;
  // int light_sphere_id = scene->CreateEntity();
  // scene->SetEntityMesh(light_sphere_id, light_sphere_mesh_id);
  // scene->SetEntityMaterial(light_sphere_id, light_sphere_material);

	// Create a sphere
	Mesh sphere_mesh;
	glm::vec3 sphere_position = glm::vec3(203.0f, 268.7f, 187.0f);  // Position of the sphere
	sphere_mesh.CreateSphere(sphere_position, 100.0f, 256, 256);
	int sphere_mesh_id = asset_manager->LoadMesh(sphere_mesh, "SphereMesh");

	Material sphere_material;
	sphere_material.base_color = {1.000, 1.000, 1.000};
	sphere_material.type = MATERIAL_TYPE_RETRACTIVE;
  sphere_material.roughness = 0.4; 
  sphere_material.anisotropic = 0;
  sphere_material.anisotropic_rotation = 0;
  sphere_material.a = 1.5046;
  sphere_material.b = 4200;
  sphere_material.c = 7650;
  sphere_material.sigma_a = 3e-3 * 0.2;
  sphere_material.sigma_s = 3e-3 * 0.8;
  // sphere_material.spectrum_type = SPECTRUM_TYPE_D50;
  // sphere_material.emission_strength = 1.0f;
	int sphere_id = scene->CreateEntity();
	scene->SetEntityMesh(sphere_id, sphere_mesh_id);
	scene->SetEntityMaterial(sphere_id, sphere_material);

	Mesh bunny_mesh;
	bunny_mesh.LoadObjFile(FindAssetsFile("mesh/bunny.obj"));
	bunny_mesh.scale(100.0f);
	bunny_mesh.translate(glm::vec3(400.0f, -40.0f, 100.0f));

  int bunny_mesh_id =
      asset_manager->LoadMesh(bunny_mesh, "BunnyMesh"); 

  Material bunny_material;
  bunny_material.base_color = {0.8f, 0.8f, 0.8f};
  bunny_material.type=MATERIAL_TYPE_SPECULAR;
  int bunny_id = scene->CreateEntity();
  scene->SetEntityMesh(bunny_id, bunny_mesh_id);
  scene->SetEntityMaterial(bunny_id, bunny_material);

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
  light_material.spectrum_type = SPECTRUM_TYPE_D75;
  light_material.illuminant_type = ILLUMINANT_TYPE_PARALLEL;
  light_material.illuminant_dir = {0.0f, -1.0f, 0.0f};
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
      // asset_manager->LoadTexture(terrain_texture, "TerrainTexture");

//   auto terrain_detail_texture_id = asset_manager->LoadTexture(
//       terrain_detail_texture, "TerrainDetailTexture");


  // ceiling
  // <vertex position="556.0 548.8 0.0  " tex_coord="0 0"/>
  // <vertex position="556.0 548.8 559.2" tex_coord="1 0"/>
  // <vertex position="0.0 548.8 559.2" tex_coord="1 1"/>
  // <vertex position="0.0 548.8   0.0" tex_coord="0 1"/>
  vertices.clear();
  vertices.push_back(make_vertex({556.0f, 548.8f, 0.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({556.0f, 548.8f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 0.0f}, {0.0f, 1.0f}));
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
  // <vertex position="0.0 548.8 559.2" tex_coord="1 1"/>
  // <vertex position="556.0 548.8 559.2" tex_coord="0 1"/>
  vertices.clear();
  vertices.push_back(make_vertex({549.6f, 0.0f, 559.2f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({556.0f, 548.8f, 559.2f}, {0.0f, 1.0f}));
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
  // <vertex position = "0.0 548.8   0.0" tex_coord = "1 1" />
  // <vertex position = "0.0 548.8 559.2" tex_coord = "0 1" />
  vertices.clear();
  vertices.push_back(make_vertex({0.0f, 0.0f, 559.2f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 0.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 559.2f}, {0.0f, 1.0f}));
  int right_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "RightWallMesh");
  Material right_wall_material;
  right_wall_material.base_color = {0.0, 1.0, 0.0};
  int right_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(right_wall_id, right_wall_mesh_id);
  scene->SetEntityMaterial(right_wall_id, right_wall_material);

  // left_wall
  // <vertex position="552.8   0.0   0.0 " tex_coord="0 0"/>
  // <vertex position = "549.6   0.0 559.2" tex_coord = "1 0" />
  // <vertex position = "556.0 548.8 559.2" tex_coord = "1 1" />
  // <vertex position = "556.0 548.8   0.0" tex_coord = "0 1" />
  vertices.clear();
  vertices.push_back(make_vertex({552.8f, 0.0f, 0.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({549.6f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({556.0f, 548.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({556.0f, 548.8f, 0.0f}, {0.0f, 1.0f}));
  int left_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "LeftWallMesh");
  Material left_wall_material;
  left_wall_material.base_color = {1.0f, 0.0f, 0.0f};
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
  short_box_material.type = MATERIAL_TYPE_MULTILAYER;   
  short_box_material.base_color = {0.8f, 0.8f, 0.8f};
  short_box_material.roughness = 0.1;   
  short_box_material.sheen = 1.0; 
  short_box_material.sheen_tint = 0.5;
  short_box_material.clearcoat = 1.0;
  short_box_material.clearcoat_roughness = 0.5; 
  short_box_material.metallic = 0; 
  short_box_material.specular = 0.5;
  short_box_material.specular_tint = 0.5;
  short_box_material.a = 1.5046;
  short_box_material.b = 4200;
  short_box_material.c = 7650;
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
  tall_box_material.type = MATERIAL_TYPE_RETRACTIVE;
  tall_box_material.base_color = {0.8f, 0.8f, 0.8f};
  tall_box_material.a = 1.5046;
  tall_box_material.b = 4200;
  tall_box_material.c = 7650;
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
  scene->SetEnvmapSettings({0.0f, 0.1f, uint32_t(envmap_id), 0});

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
                            glm::scale(glm::mat4{1.0f}, glm::vec3{605.9f}));
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
