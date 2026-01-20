#include "ResourceManager.h"

#include "Framework/Utils/OS.h"

#include <iostream>

std::wstring StringToWString(const std::string& str) {
	std::wstring wstr(str.begin(), str.end());
	return wstr;
}

ResourceManager* ResourceManager::m_instance = nullptr;

ResourceManager& ResourceManager::Get() {
	if (m_instance == nullptr) {
		m_instance = new ResourceManager();
	}
	return *m_instance;
}

ResourceManager::ResourceManager(bool _preload) {
	if(!_preload)
		return;

	LoadAllTextures();
	LoadAllGeometries();

	gce::Console::Log("ResourceManager: Preloaded " + std::to_string(m_textures.size())
		+ " textures and " + std::to_string(m_geometries.size()) + " geometries successfully.",
		gce::Console::LogType::SUCCESS);
}
ResourceManager::~ResourceManager() {
	for (auto& texture : m_textures) {
		delete texture.second;
	}
	m_textures.clear();
	for (auto& geometry : m_geometries) {
		delete geometry.second;
	}
	m_geometries.clear();
}

gce::Texture* ResourceManager::GetTexture(const std::string& _name) {
	bool exists = m_textures.count(_name);
	if (!exists) {
		if (LoadTexture(_name))
			return m_textures[_name];
		else
			return nullptr;
	}
	return m_textures[_name];
}

gce::Geometry* ResourceManager::GetGeometry(const std::string& _name) {
	bool exists = m_geometries.count(_name);
	if (!exists) {
		if (LoadGeometry(_name))
			return m_geometries[_name];
		else
			return nullptr;
	}
	return m_geometries[_name];
}

std::string ResourceManager::GetSound(const std::string& _name) {
    if (!gce::AudioUse::IsSoundLoaded(_name)) {
        std::string path = OS::getFileInDirectory(RES_PATH, _name + ".wav");
        if (path.empty())
            return "";
        gce::AudioUse::LoadSound(_name, StringToWString(path).c_str());
    }
    return _name;
}

bool ResourceManager::LoadTexture(const std::string& _name) {
	std::string path = OS::getFileInDirectory(RES_PATH, _name + ".png");
	if (path.empty())
		return false;

	gce::Texture* texture = new gce::Texture(OS::getAbsolutePath(path));
	if(texture == nullptr)
		return false;

	m_textures[_name] = texture;

	return true;
}

bool ResourceManager::LoadGeometry(const std::string& _name) {
	std::string path = OS::getFileInDirectory(RES_PATH, _name + ".obj");
	if (path.empty())
		return false;

	gce::Geometry* geometry = gce::GeometryFactory::LoadGeometry(path);
	if (geometry == nullptr)
		return false;

	m_geometries[_name] = geometry;

	return true;
}

void ResourceManager::LoadAllTextures() {
	std::vector<std::string> paths = OS::getFilesInDirectory(RES_PATH, ".png");
	for (const std::string& path : paths) {
		std::string name = OS::getFileName(path, true);
		gce::Texture* texture = new gce::Texture(path);
		if (texture != nullptr) {
			m_textures[name] = texture;
		}
	}
}

void ResourceManager::LoadAllGeometries() {
	std::vector<std::string> paths = OS::getFilesInDirectory(RES_PATH, ".obj");
	for (const std::string& path : paths) {
		std::string name = OS::getFileName(path, true);
		gce::Geometry* geometry = gce::GeometryFactory::LoadGeometry(path);
		if (geometry != nullptr) {
			m_geometries[name] = geometry;
		}
	}
}