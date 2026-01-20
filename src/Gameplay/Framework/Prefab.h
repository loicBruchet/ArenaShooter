#pragma once

#include "PrefabRegistry.h"

#include "Utils/Json.hpp"

#include <Engine.h>

//namespace gce {
//    class Scene;
//	class GameObject;
//}

class Prefab {
public:
    Prefab(gce::Scene& _scene, json* _properties = nullptr);
	virtual ~Prefab();

	static PrefabRegistry& GetRegistry();

    gce::GameObject& GetRootObject() const;
protected:
    gce::GameObject& m_root;
private:
	gce::Scene& m_scene;
};

#pragma warning(push)
#pragma warning(disable : 4068)

//using PrefabFactory = Prefab* (*)(gce::Scene&);
using PrefabFactory = Prefab * (*)(gce::Scene&, json*);

#pragma warning(pop)

#define REGISTER_PREFAB(subtype)										        \
    extern "C" Prefab* Create_##subtype(gce::Scene& _scene, json* _properties); \
    inline Prefab* Create_##subtype(gce::Scene& _scene, json* _properties) {   \
        return new subtype(_scene, _properties);                                \
    }                                                                           \
    struct subtype##Registrar {                                                 \
        subtype##Registrar() {                                                  \
             PrefabRegistry::Get().RegisterPrefab(#subtype, Create_##subtype);	\
        }                                                                       \
    };                                                                          \
    static subtype##Registrar subtype##_registrar;