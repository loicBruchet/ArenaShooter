#include "ShotgunProjectile.h"

#include "Game/Tags.h"
#include "Game/Scripts/Stats/HealthStat.h"

void ShotgunProjectile::Start() {
	Projectile::Start();
}

void ShotgunProjectile::OnHit(gce::GameObject* _other) {
	if (!_other->HasTag(TAG_ENEMY))
		return;

	HealthStat* health = _other->GetScript<HealthStat>();

	if (health == nullptr)
		return;

	health->TakeDammage(10);
};