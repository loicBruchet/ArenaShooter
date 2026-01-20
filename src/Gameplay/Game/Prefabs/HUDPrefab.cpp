#include "HUDPrefab.h"

#include "PlayerPrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/ResourceManager.h"

#include "Game/Tags.h"

#include "../Scripts/UI/UISwitch.h"

HUDPrefab::HUDPrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties) {

	////////////////// UI //////////////////


		 ////////// BORDURE ////////////
	gce::BitMapBrush* bordureTexture = new gce::BitMapBrush("res/ArenaShooter/UI/bordure.png");

	gce::GameObject& uiBordure = m_root;
	uiBordure.SetName("UI_Bordure");
	/*uiBordure.transform.SetLocalPosition({ 855.0f, 540.0f, 0.0f });
	uiBordure.transform.SetLocalScale({ 1710.0f,920.0f,0.0f });*/
	uiBordure.transform.SetLocalPosition({ 960.0f, 540.0f, 0.0f });
	uiBordure.transform.SetLocalScale({ 1920.0f,1080.0f,0.0f });
	gce::UiImage& uiImage = *uiBordure.AddComponent<gce::UiImage>();
	uiImage.btmBrush = bordureTexture;
	uiImage.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	////////// VISEUR ////////////

	gce::BitMapBrush* viseurSimpleTexture = new gce::BitMapBrush("res/ArenaShooter/UI/Viseur.png");


	gce::GameObject& uiViseur1 = gce::GameObject::Create(_scene);
	uiViseur1.SetName("UI_Bordure");
	uiViseur1.transform.SetLocalPosition({ 960.0f, 540.0f, 0.0f });
	uiViseur1.transform.SetLocalScale({ 100.0f,100.0f,0.0f });

	gce::UiImage& uiImageViseurSimple = *uiViseur1.AddComponent<gce::UiImage>();	//Brush 1
	uiImageViseurSimple.btmBrush = viseurSimpleTexture;
	uiImageViseurSimple.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	UISwitch& switchViseur1 = *uiViseur1.AddScript<UISwitch>();
	switchViseur1.ScaleFactor = 100.0f;
	switchViseur1.ChangeActive(false);

	gce::BitMapBrush* viseurTChercheuseTexture = new gce::BitMapBrush("res/ArenaShooter/UI/viseur_ttcherc.png");

	gce::GameObject& uiViseur2 = gce::GameObject::Create(_scene);
	uiViseur2.SetName("UI_Bordure");
	uiViseur2.transform.SetLocalPosition({ 960.0f, 540.0f, 0.0f });
	uiViseur2.transform.SetLocalScale({ 100.0f,100.0f,0.0f });

	gce::UiImage& uiImageViseurTChercheuse = *uiViseur2.AddComponent<gce::UiImage>(); //Brush 2
	uiImageViseurTChercheuse.btmBrush = viseurTChercheuseTexture;
	uiImageViseurTChercheuse.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	UISwitch& switchViseur2 = *uiViseur2.AddScript<UISwitch>();
	switchViseur2.ScaleFactor = 100.0f;
	switchViseur2.ChangeActive(true);

	////////////////////////////////////////

		/////////// MUNITION /////////////
	gce::BitMapBrush* munitionTexture = new gce::BitMapBrush("res/ArenaShooter/UI/Nombre_Munitions.png");

	gce::GameObject& uimunition = gce::GameObject::Create(_scene);
	uimunition.SetName("UI_Clef");
	uimunition.transform.SetLocalPosition({ 1785.0f, 815.0f, 0.0f });
	uimunition.transform.SetLocalScale({ 142.5f,142.5f,0.0f });
	gce::UiImage& uiImageMunition = *uimunition.AddComponent<gce::UiImage>();
	uiImageMunition.btmBrush = munitionTexture;
	uiImageMunition.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);


	////////////////////////////////////////

	////////////////////////////////////////

		/////////// TYPE ARME /////////////
	gce::BitMapBrush* Arme_TirTexture = new gce::BitMapBrush("res/ArenaShooter/UI/Arme_Tir.png");


	gce::GameObject& uiArme1 = gce::GameObject::Create(_scene);
	uiArme1.SetName("UI_Clef");
	uiArme1.transform.SetLocalPosition({ 1730.0f, 950.0f, 0.0f });
	uiArme1.transform.SetLocalScale({ 250.0f,250.0f,0.0f });

	gce::UiImage& uiImageArmeSimple = *uiArme1.AddComponent<gce::UiImage>();
	uiImageArmeSimple.btmBrush = Arme_TirTexture;
	uiImageArmeSimple.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	UISwitch& switchArme1 = *uiArme1.AddScript<UISwitch>();
	switchArme1.ScaleFactor = 250.0f;
	switchArme1.ChangeActive(false);



	gce::BitMapBrush* Arme_ttCherchTexture = new gce::BitMapBrush("res/ArenaShooter/UI/Arme_ttCherc.png");

	gce::GameObject& uiArme2 = gce::GameObject::Create(_scene);
	uiArme2.SetName("UI_Bordure");
	uiArme2.transform.SetLocalPosition({ 1730.0f, 950.0f, 0.0f });
	uiArme2.transform.SetLocalScale({ 250.0f,250.0f,0.0f });;

	gce::UiImage& uiImageArmeTChercheuse = *uiArme2.AddComponent<gce::UiImage>(); //Brush 2
	uiImageArmeTChercheuse.btmBrush = Arme_ttCherchTexture;
	uiImageArmeTChercheuse.InitializeImage({ 100.0f,100.0f }, { 0.0f,0.0f }, 1.0f);

	UISwitch& switchArme2 = *uiArme2.AddScript<UISwitch>();
	switchArme2.ScaleFactor = 250.0f;
	switchArme2.ChangeActive(true);

	//AJOUTER UN COMPONENT

	////////////////////////////////////////


}
HUDPrefab::~HUDPrefab() {}