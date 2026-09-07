#include "CppUnitTest.h"

#include "Player_Character.h"
#include "Exception.h"

#include <memory>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace oop::projekt;

namespace Tests
{
	// Testovi igraceva lika: preracun statistika pri opremanju,
	// primanje stete i uzimanje predmeta.
	TEST_CLASS(PlayerCharacterTests)
	{
	private:
		// naziv, max hp, napad, obrana, brzina, velicina ruksaka
		static Player_Character noviIgrac()
		{
			return Player_Character("Ironhand", 100, 20, 10, 1.0, 10);
		}
		static std::unique_ptr<Armor> armor(std::string name, int def, float spd, ArmorSlot slot)
		{
			return std::make_unique<Armor>(name, 1, Rarity::Common, def, spd, slot);
		}
		static std::unique_ptr<Weapon> weapon(std::string name, int atk, int def, float spd, WeaponSlot slot)
		{
			return std::make_unique<Weapon>(name, 1, Rarity::Common, def, spd, atk, slot);
		}

	public:

		TEST_METHOD(Novi_igrac_ima_pun_hp_i_osnovne_statistike)
		{
			Player_Character igrac = noviIgrac();

			Assert::AreEqual(std::string("Ironhand"), igrac.getName());
			Assert::AreEqual(100.0f, igrac.gethp(), 0.001f);
			Assert::AreEqual(100.0f, igrac.getMaxHp(), 0.001f);
			Assert::IsTrue(igrac.isAlive());
			Assert::AreEqual(20.0f, igrac.deal_dmg(), 0.001f);
			Assert::AreEqual(10.0f, igrac.getDefense(), 0.001f);
		}

		TEST_METHOD(Obrana_umanjuje_primljenu_stetu)
		{
			Player_Character igrac = noviIgrac();

			float primljeno = igrac.take_dmg(30.0f);

			Assert::AreEqual(20.0f, primljeno, 0.001f);
			Assert::AreEqual(80.0f, igrac.gethp(), 0.001f);
		}

		// Rubni slucaj: napad slabiji od obrane ne smije oduzeti hp
		// ni vratiti negativnu stetu.
		TEST_METHOD(Napad_slabiji_od_obrane_ne_radi_stetu)
		{
			Player_Character igrac = noviIgrac();

			float primljeno = igrac.take_dmg(5.0f);

			Assert::AreEqual(0.0f, primljeno, 0.001f);
			Assert::AreEqual(100.0f, igrac.gethp(), 0.001f);
		}

		TEST_METHOD(Igrac_umire_kad_hp_padne_na_nulu)
		{
			Player_Character igrac = noviIgrac();
			Assert::IsTrue(igrac.isAlive());

			igrac.take_dmg(500.0f);

			Assert::IsFalse(igrac.isAlive());
		}

		TEST_METHOD(Opremanje_oruzja_povecava_napad)
		{
			Player_Character igrac = noviIgrac();
			igrac.get_backpack_inventory().addItem(weapon("Mac", 15, 0, 0.0f, WeaponSlot::Right_hand));

			Assert::IsTrue(igrac.equip(0));

			Assert::AreEqual(35.0f, igrac.deal_dmg(), 0.001f);
			Assert::AreEqual(0, igrac.get_backpack_inventory().inventory_size());
			Assert::IsNotNull(igrac.get_weapon_inventory().item_from_inventory(1));
		}

		TEST_METHOD(Opremanje_oklopa_povecava_obranu)
		{
			Player_Character igrac = noviIgrac();
			igrac.get_backpack_inventory().addItem(armor("Kaciga", 6, 0.0f, ArmorSlot::Helmet));

			Assert::IsTrue(igrac.equip(0));

			Assert::AreEqual(16.0f, igrac.getDefense(), 0.001f);
			Assert::IsNotNull(igrac.get_armor_inventory().item_from_inventory(0));
		}

		TEST_METHOD(Skidanje_oruzja_ponistava_bonus_i_vraca_u_ruksak)
		{
			Player_Character igrac = noviIgrac();
			igrac.get_backpack_inventory().addItem(weapon("Mac", 15, 0, 0.0f, WeaponSlot::Right_hand));
			igrac.equip(0);

			Assert::IsTrue(igrac.unequip_weapon(1));

			Assert::AreEqual(20.0f, igrac.deal_dmg(), 0.001f);
			Assert::IsNull(igrac.get_weapon_inventory().item_from_inventory(1));
			Assert::AreEqual(1, igrac.get_backpack_inventory().inventory_size());
		}

		TEST_METHOD(Skidanje_oklopa_ponistava_bonus_i_vraca_u_ruksak)
		{
			Player_Character igrac = noviIgrac();
			igrac.get_backpack_inventory().addItem(armor("Kaciga", 6, 0.0f, ArmorSlot::Helmet));
			igrac.equip(0);

			Assert::IsTrue(igrac.unequip_armor(0));

			Assert::AreEqual(10.0f, igrac.getDefense(), 0.001f);
			Assert::IsNull(igrac.get_armor_inventory().item_from_inventory(0));
			Assert::AreEqual(1, igrac.get_backpack_inventory().inventory_size());
		}

		TEST_METHOD(Vise_komada_opreme_zbraja_bonuse)
		{
			Player_Character igrac = noviIgrac();
			igrac.get_backpack_inventory().addItem(armor("Kaciga", 4, 0.0f, ArmorSlot::Helmet));
			igrac.get_backpack_inventory().addItem(armor("Prsluk", 8, 0.0f, ArmorSlot::Chest));
			igrac.get_backpack_inventory().addItem(weapon("Mac", 15, 2, 0.0f, WeaponSlot::Right_hand));

			igrac.equip(0);
			igrac.equip(0);
			igrac.equip(0);

			Assert::AreEqual(35.0f, igrac.deal_dmg(), 0.001f);
			Assert::AreEqual(24.0f, igrac.getDefense(), 0.001f);
		}

		// Rubni slucaj: opremanje s praznog mjesta.
		TEST_METHOD(Opremanje_s_praznog_mjesta_ne_uspijeva)
		{
			Player_Character igrac = noviIgrac();

			Assert::IsFalse(igrac.equip(0));
			Assert::AreEqual(20.0f, igrac.deal_dmg(), 0.001f);
		}

		// Junk predmet nije oprema i mora ostati u ruksaku.
		TEST_METHOD(Junk_predmet_se_ne_moze_opremiti)
		{
			Player_Character igrac = noviIgrac();
			igrac.get_backpack_inventory().addItem(
				std::make_unique<Item>("Rusty Nail", Item_Type::Junk, Rarity::Common, 1));

			Assert::IsFalse(igrac.equip(0));
			Assert::AreEqual(1, igrac.get_backpack_inventory().inventory_size());
		}

		TEST_METHOD(Heal_vraca_hp_ali_ne_iznad_maksimuma)
		{
			Player_Character igrac = noviIgrac();
			igrac.take_dmg(60.0f);
			Assert::AreEqual(50.0f, igrac.gethp(), 0.001f);

			igrac.heal(20.0f);
			Assert::AreEqual(70.0f, igrac.gethp(), 0.001f);

			igrac.heal(500.0f);
			Assert::AreEqual(100.0f, igrac.gethp(), 0.001f);
		}

		// Rubni slucaj: negativno lijecenje ne smije oduzeti hp.
		TEST_METHOD(Heal_negativnom_vrijednoscu_ne_mijenja_hp)
		{
			Player_Character igrac = noviIgrac();
			igrac.take_dmg(30.0f);
			float prije = igrac.gethp();

			igrac.heal(-50.0f);

			Assert::AreEqual(prije, igrac.gethp(), 0.001f);
		}

		TEST_METHOD(takeItem_stavlja_predmet_u_ruksak)
		{
			Player_Character igrac = noviIgrac();

			std::unique_ptr<Item> predmet = std::make_unique<Item>("Kamen", Item_Type::Junk, Rarity::Common, 1);
			igrac.takeItem(predmet);

			Assert::AreEqual(1, igrac.get_backpack_inventory().inventory_size());
		}

		// Rubni slucaj: pun ruksak mora baciti iznimku, kako bi pozivatelj
		// mogao vratiti predmet umjesto da ga tiho izgubi.
		TEST_METHOD(takeItem_baca_InventoryFullException_kad_je_ruksak_pun)
		{
			Player_Character igrac("Ironhand", 100, 20, 10, 1.0, 2);
			std::unique_ptr<Item> prvi = std::make_unique<Item>("A", Item_Type::Junk, Rarity::Common, 1);
			std::unique_ptr<Item> drugi = std::make_unique<Item>("B", Item_Type::Junk, Rarity::Common, 2);
			igrac.takeItem(prvi);
			igrac.takeItem(drugi);

			std::unique_ptr<Item> treci = std::make_unique<Item>("C", Item_Type::Junk, Rarity::Common, 3);
			Assert::ExpectException<InventoryFullException>([&igrac, &treci]()
				{
					igrac.takeItem(treci);
				});

			// Predmet ne smije nestati kad uzimanje ne uspije, inace bi se
			// izgubio i za igraca i za protivnika.
			Assert::IsNotNull(treci.get());
		}

		TEST_METHOD(InventoryFullException_cuva_naziv_predmeta)
		{
			Player_Character igrac("Ironhand", 100, 20, 10, 1.0, 1);
			std::unique_ptr<Item> prvi = std::make_unique<Item>("A", Item_Type::Junk, Rarity::Common, 1);
			igrac.takeItem(prvi);

			std::unique_ptr<Item> drugi = std::make_unique<Item>("Excalibur", Item_Type::Junk, Rarity::Common, 2);
			try
			{
				igrac.takeItem(drugi);
				Assert::Fail();
			}
			catch (const InventoryFullException& greska)
			{
				Assert::AreEqual(std::string("Excalibur"), greska.getItemName());
			}
		}

		TEST_METHOD(showStats_sadrzi_naziv_i_hp)
		{
			Player_Character igrac = noviIgrac();
			std::string stats = igrac.showStats();

			Assert::IsTrue(stats.find("Ironhand") != std::string::npos);
			Assert::IsTrue(stats.find("HP") != std::string::npos);
		}

		// Igrac nema razinu, pa je ni ispis statistika ne smije prikazivati.
		TEST_METHOD(showStats_ne_prikazuje_razinu)
		{
			Player_Character igrac = noviIgrac();
			std::string stats = igrac.showStats();

			Assert::IsTrue(stats.find("Level") == std::string::npos);
		}

		// Protivnik razinu ima jer se iz nje racunaju bodovi i plijen.
		TEST_METHOD(Protivnik_ima_razinu)
		{
			Enemy protivnik("Grub", 7, 50, 10, 0, 0.5, 2);

			Assert::AreEqual(7, protivnik.getLVL());
			Assert::AreEqual(10.0f, protivnik.deal_dmg(), 0.001f);
		}
	};
}
