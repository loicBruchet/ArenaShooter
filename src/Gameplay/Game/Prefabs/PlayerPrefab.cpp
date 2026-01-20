#include "PlayerPrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/ResourceManager.h"

#include "Game/Scripts/Player/PlayerCamera.h"
#include "Game/Scripts/Stats/HealthStat.h"
#include "Game/Scripts/Stats/ShieldStat.h"
#include "Game/Scripts/Stats/UIGestion.h"
#include "Game/Scripts/Stats/UIGestionShield.h"

#include "Game/Scripts/Player/PlayerMovements.h"
#include "Game/Scripts/Player/PlayerInputs.h"
#include "Game/Scripts/Player/PlayerAnimations.h"
#include "Game/Scripts/Player/KeyInventory.h"

#include "Framework/PrefabRegistry.h"								
#include "Game/Scripts/Weapon/Weapon.h"
#include "Game/Tags.h"

#include "Game/Levels.h"

PlayerPrefab::PlayerPrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties) {
	gce::Geometry* SHOTGUN_GEO_BODY = RESOURCE_MANAGER.GetGeometry("Shotgun_BodyObject");
	gce::Geometry* SHOTGUN_GEO_BARILLET = RESOURCE_MANAGER.GetGeometry("Shotgun_BarilletObject");

	gce::Texture* SHOTGUN_TEXTURE = RESOURCE_MANAGER.GetTexture("metalR_albedo");

	//////////////// PLAYER ////////////////

	gce::GameObject& player_object = m_root;
	player_object.SetName("Player");
	player_object.SetTags(TAG_DEFAULT | TAG_ENTITY | TAG_PLAYER);
	player_object.transform.SetWorldPosition({ -2.0f, 0.0f, -2.0f });

	gce::BoxCollider& player_collider = *player_object.AddComponent<gce::BoxCollider>();
	player_collider.m_localOffSet = { 0.0f, 1.0f, 0.0f };
	player_collider.localScale = { 1.0f, 2.0f, 1.0f };

	gce::PhysicComponent& player_physics = *player_object.AddComponent<gce::PhysicComponent>();
	player_physics.SetGravityScale(4.0f);
	player_physics.SetBounciness(0.0f);
	player_physics.SetMass(70.0f);

	HealthStat& player_health = *player_object.AddScript<HealthStat>();
	player_health.Init(10);

	ShieldStat& player_shield = *player_object.AddScript<ShieldStat>();
	player_shield.Init(5);

	PlayerMovements& player_movements = *player_object.AddScript<PlayerMovements>();

	PlayerInputs& player_inputs = *player_object.AddScript<PlayerInputs>();

	KeyInventory& player_keys = *player_object.AddScript<KeyInventory>();

	PlayerAnimations& player_animations = *player_object.AddScript<PlayerAnimations>();

	player_health.OnDeath.AddListener([](gce::GameObject* _obj) {
		gce::Scene& newScene = gce::Scene::Create();
		gce::GameManager::ChangeScene(&newScene);
		GameOver(newScene);
	});

	////////////////////////////////////////

	//////////////// CAMERA ////////////////

	gce::GameObject& camera_object = gce::GameObject::Create(_scene);
	camera_object.transform.LocalTranslate({ 0.0f, 1.5f, 0.0f });
	camera_object.SetTags(TAG_DEFAULT);
	camera_object.SetParent(player_object);
	camera_object.transform.SetLocalPosition({ 0.0f, 1.5f, 0.0f });
	camera_object.transform.SetLocalRotation({ 0.0f, 0.0f, 0.0f });
	camera_object.SetTags(TAG_DEFAULT);
	camera_object.SetName("MainCamera");

	gce::Camera& camera_view = *camera_object.AddComponent<gce::Camera>();
	camera_view.perspective.aspectRatio = 16.0f / 9.0f;
	camera_view.perspective.up = { 0.0f, 1.0f, 0.0f };
	camera_view.perspective.nearPlane = 0.001f;
	camera_view.perspective.farPlane = 500.0f;
	camera_view.perspective.fov = gce::PI / 4;
	camera_view.SetType(gce::PERSPECTIVE);
	camera_view.SetMainCamera();

	PlayerCamera& camera_controller = *camera_object.AddScript<PlayerCamera>();

	////////////////////////////////////////

	//////////// GROUND CHECKER ////////////

	gce::GameObject& groundchecker_object = gce::GameObject::Create(_scene);
	groundchecker_object.transform.SetLocalPosition({ 0.0f, 0.0f, 0.0f });
	groundchecker_object.SetParent(player_object);
	groundchecker_object.SetName("GroundChecker");
	groundchecker_object.SetTags(TAG_OTHER);

	gce::SphereCollider& groundchecker_collider = *groundchecker_object.AddComponent<gce::SphereCollider>();
	groundchecker_collider.localScale = 0.1f;
	groundchecker_collider.isTrigger = true;
	groundchecker_collider.localRay = 0.3f;

	////////////////////////////////////////

	///////////// ROOF CHECKER /////////////

	gce::GameObject& roofchecker_object = gce::GameObject::Create(_scene);
	roofchecker_object.transform.SetLocalPosition({ 0.0f, 1.5f, 0.0f });
	roofchecker_object.SetParent(player_object);
	roofchecker_object.SetName("RoofChecker");
	groundchecker_object.SetTags(TAG_OTHER);

	gce::SphereCollider& roofchecker_collider = *roofchecker_object.AddComponent<gce::SphereCollider>();
	roofchecker_collider.localScale = 1.0f;
	roofchecker_collider.isTrigger = true;
	roofchecker_collider.localRay = 0.3f;

	////////////////////////////////////////

	//////////////// WEAPON ////////////////

	gce::GameObject& weapon_object = gce::GameObject::Create(_scene);
	weapon_object.SetName("Weapon");
	weapon_object.SetTags(TAG_DEFAULT);
	weapon_object.SetParent(camera_object);
	weapon_object.transform.SetLocalPosition({ 0.18f, -0.15f, 0.5f });

	gce::MeshRenderer& weeapon_mesh = *weapon_object.AddComponent<gce::MeshRenderer>();
	weeapon_mesh.SetAlbedoTexture(SHOTGUN_TEXTURE);
	weeapon_mesh.SetGeometry(SHOTGUN_GEO_BODY);

	Weapon& weapon_controller = *weapon_object.AddScript<Weapon>();
	weapon_controller.bulletPrefabName = "GuidedShotgunProjectilePrefab";
	weapon_controller.reloadTime = 0.85f;

	gce::GameObject& weapon_barillet_object = gce::GameObject::Create(_scene);
	weapon_barillet_object.SetName("BarilletWeapon");
	weapon_barillet_object.SetTags(TAG_DEFAULT);
	weapon_barillet_object.SetParent(weapon_object);
	weapon_barillet_object.transform.SetLocalPosition({ 0.0f, 0.0f, 0.0f });

	gce::MeshRenderer& weapon_barillet_mesh = *weapon_barillet_object.AddComponent<gce::MeshRenderer>();
	weapon_barillet_mesh.SetAlbedoTexture(SHOTGUN_TEXTURE);
	weapon_barillet_mesh.SetGeometry(SHOTGUN_GEO_BARILLET);


	gce::GameObject& ammo_Text_object = gce::GameObject::Create(_scene);
	ammo_Text_object.SetName("Ammo_Text");
	ammo_Text_object.SetTags(TAG_DEFAULT);
	ammo_Text_object.SetParent(weapon_object);
	ammo_Text_object.transform.SetLocalPosition({ 1785.0f, 815.0f, 0.0f });
	gce::TextRenderer& ammo_text_renderer = *ammo_Text_object.AddComponent<gce::TextRenderer>();
 
	ammo_text_renderer.SetFont(new gce::Font(L"BatmanForeverAlternate"));
	ammo_text_renderer.text = { L"8/1" };
	ammo_text_renderer.rectPosF = new gce::RectanglePosF(1755.0f, 790.0f, 0.0f, 0.0f);
	ammo_text_renderer.textSize = { 0.75f, 0.75f };
	ammo_Text_object.SetParent(player_object);
	////////////////////////////////////////

	////////////////// UI //////////////////
	
	gce::GameObject& ui_object = gce::GameObject::Create(_scene);
	ui_object.SetName("ui-test");
	ui_object.transform.SetLocalPosition({ 100.0f, 100.0f, 0.0f });
	ui_object.transform.SetLocalScale({ 100.0f, 100.0f, 0.0f });
	UIGestionHealth& ui_gestion = *ui_object.AddScript<UIGestionHealth>();
	ui_gestion.LoadFullBrush("res/ArenaShooter/UI/2PV.png");
	ui_gestion.LoadEmptyBrush("res/ArenaShooter/UI/0PV.png");
	ui_gestion.InitializeUI();
	
	gce::GameObject& ui_object_shield = gce::GameObject::Create(_scene);
	ui_object_shield.SetName("ui-shield-test");
	ui_object_shield.transform.SetLocalPosition({ 100.0f, 150.0f, 0.0f });
	ui_object_shield.transform.SetLocalScale({ 100.0f, 100.0f, 0.0f });
	UIGestionShield& ui_gestion_shield = *ui_object_shield.AddScript<UIGestionShield>();
	ui_gestion_shield.LoadFullBrush("res/ArenaShooter/UI/Shield_dark.png");
	ui_gestion_shield.LoadEmptyBrush("res/ArenaShooter/UI/Shield_light.png");
	ui_gestion_shield.InitializeUI();

	//for (int j = 0; j < 5; j++)
	//{
	//	Prefab* healthUi = PREFAB_REGISTRY.CreatePrefab("HealthPrefab", scene);

	//	gce::Vector3f32 shielpos = healthUi->GetRootObject().transform.GetLocalPosition();
	//	healthUi->GetRootObject().transform.SetLocalPosition({ shielpos.x + j * 40,shielpos.y ,shielpos.z });
	//}

	PREFAB_REGISTRY.CreatePrefab("HUDPrefab", _scene);

	////////////////////////////////////////

}
PlayerPrefab::~PlayerPrefab() {}