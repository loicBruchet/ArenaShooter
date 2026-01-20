#include "MachineGunProjectile.h"

#include "Game/Tags.h"
#include "Game/Scripts/Stats/ShieldStat.h"

void MachineGunProjectile::Start() {
	Projectile::Start();
}

void MachineGunProjectile::OnHit(gce::GameObject* _other) {
	if (!_other->HasTag(TAG_PLAYER))
		return;

	ShieldStat* shield = _other->GetScript<ShieldStat>();

	if (shield == nullptr)	
		return;

	shield->TakeDammage(1);
};