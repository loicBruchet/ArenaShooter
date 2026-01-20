#include "ColliderUtils.h"

bool IsTrigger(gce::GameObject* _object) {
	gce::BoxCollider* boxCollider = _object->GetComponent<gce::BoxCollider>();
	if (boxCollider) {
		if (boxCollider->isTrigger)
			return true;
	}

	gce::SphereCollider* sphereCollider = _object->GetComponent<gce::SphereCollider>();
	if (sphereCollider) {
		if (sphereCollider->isTrigger)
			return true;
	}

	return false;
}