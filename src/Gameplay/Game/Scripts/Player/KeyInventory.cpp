#include "KeyInventory.h"

KeyInventory* KeyInventory::instance = nullptr;

void KeyInventory::Start() {
	instance = this;
}
void KeyInventory::Update() {
	std::cout << "Has Key: " << hasKey << std::endl;
}

bool KeyInventory::HasKey()
{
	return KeyInventory::instance->hasKey;
}