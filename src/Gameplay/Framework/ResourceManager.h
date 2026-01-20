#pragma once

#include <Engine.h>
#include <Audio.h>

#define RESOURCE_MANAGER ResourceManager::Get()

#define RES_PATH "../../res"

class ResourceManager {
public:
	static ResourceManager& Get();

	gce::Texture* GetTexture(const std::string& _name);
	gce::Geometry* GetGeometry(const std::string& _name);
	std::string GetSound(const std::string& _name);

private:
	bool LoadTexture(const std::string& _name);
	bool LoadGeometry(const std::string& _name);

	void LoadAllTextures();
	void LoadAllGeometries();
	void LoadAllSounds();

	std::unordered_map<std::string, gce::Texture*> m_textures;
	std::unordered_map<std::string, gce::Geometry*> m_geometries;

private:
	ResourceManager(bool _preload = false);
	~ResourceManager();
	static ResourceManager* m_instance;
};