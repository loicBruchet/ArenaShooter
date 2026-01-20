#include "pch.h"
#include "Scene.h"

#include <ranges>

#include "GameObject.h"
#include "GameManager.h"

namespace gce {
	Scene& Scene::Create()
	{
		Scene* const pNew = new Scene();
		GameManager::s_pInstance->m_scenes.push_back(pNew);
		return *pNew;
	}

	GameObject* Scene::GetGameObject(uint32 id) const
	{
		if (m_gameObjects.contains(id) == false)
			return nullptr;

		return m_gameObjects.at(id);
	}

	GameObject* Scene::GetObjectByName(std::string_view name) const
	{
		for (GameObject* const pGameObject : m_gameObjects | std::views::values)
		{
			if (pGameObject->GetName() == name)
				return pGameObject;
		}
		return nullptr;
	}

	Scene::~Scene()
	{
		for (auto& kv : m_gameObjects) {
			GameObject* p = kv.second;
			if (p) 
			{
				p->m_pScene = nullptr;
				p->Destroy();
			}
		}

		m_gameObjects.clear();
	}

	void Scene::UpdateMatrix()
	{
		for (GameObject* const pGameObject : m_gameObjects | std::views::values)
		{
			if (pGameObject->IsActive() == false) continue;

			pGameObject->transform.UpdateMatrix();
		}
	}
}