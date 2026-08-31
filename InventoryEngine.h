#pragma once
#include <iostream>
#include "Player_Character.h"
#include "UI.h"

namespace oop::projekt
{
	class InventoryEngine
	{
	private:
		Player_Character* player;

	public:
		InventoryEngine(Player_Character* player);

		void openBackpack(UI& ui);
		void openArmor(UI& ui);
		void openWeapon(UI& ui);
	};
}