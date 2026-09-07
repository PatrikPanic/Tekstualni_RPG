#include "CppUnitTest.h"

#include "Item.h"
#include "Exception.h"

#include <cstdio>
#include <fstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace oop::projekt;

namespace Tests
{
	// Testovi ucitavanja predmeta iz tekstualnih datoteka i pretvorbe
	// pojedinog retka u objekt.
	TEST_CLASS(ItemTests)
	{
	public:

		TEST_METHOD(Armor_fromLine_cita_sva_polja)
		{
			Armor armor = Armor::fromLine("7;Iron Chest;common;5;0.02;chest");

			Assert::AreEqual(std::string("Iron Chest"), armor.getName());
			Assert::AreEqual(7, armor.getID());
			Assert::IsTrue(armor.getRarity() == Rarity::Common);
			Assert::IsTrue(armor.getType() == Item_Type::Armor);
			Assert::AreEqual(5, armor.getArmorDefense());
			Assert::AreEqual(0.02f, armor.getArmorSpeed(), 0.0001f);
			Assert::IsTrue(armor.getArmorSlot() == ArmorSlot::Chest);
		}

		TEST_METHOD(Armor_fromLine_prepoznaje_sve_rijetkosti)
		{
			Assert::IsTrue(Armor::fromLine("1;A;common;1;0.0;helmet").getRarity() == Rarity::Common);
			Assert::IsTrue(Armor::fromLine("2;B;rare;1;0.0;helmet").getRarity() == Rarity::Rare);
			Assert::IsTrue(Armor::fromLine("3;C;epic;1;0.0;helmet").getRarity() == Rarity::Epic);
			Assert::IsTrue(Armor::fromLine("4;D;legendary;1;0.0;helmet").getRarity() == Rarity::Legendary);
		}

		TEST_METHOD(Armor_fromLine_prepoznaje_sve_slotove)
		{
			Assert::IsTrue(Armor::fromLine("1;A;common;1;0.0;helmet").getArmorSlot() == ArmorSlot::Helmet);
			Assert::IsTrue(Armor::fromLine("2;B;common;1;0.0;chest").getArmorSlot() == ArmorSlot::Chest);
			Assert::IsTrue(Armor::fromLine("3;C;common;1;0.0;gloves").getArmorSlot() == ArmorSlot::Gloves);
			Assert::IsTrue(Armor::fromLine("4;D;common;1;0.0;pants").getArmorSlot() == ArmorSlot::Pants);
			Assert::IsTrue(Armor::fromLine("5;E;common;1;0.0;boots").getArmorSlot() == ArmorSlot::Boots);
		}

		// Rubni slucaj: nepoznata vrijednost u datoteci ne smije srusiti program,
		// nego se koristi zadana vrijednost.
		TEST_METHOD(Armor_fromLine_nepoznate_vrijednosti_daju_zadane)
		{
			Armor armor = Armor::fromLine("9;X;nepostojeca;1;0.0;nepostojeci");

			Assert::IsTrue(armor.getRarity() == Rarity::Common);
			Assert::IsTrue(armor.getArmorSlot() == ArmorSlot::Helmet);
		}

		TEST_METHOD(Weapon_fromLine_cita_napad_i_ruku)
		{
			Weapon weapon = Weapon::fromLine("12;Iron Sword;rare;1;0.05;14;right_hand");

			Assert::AreEqual(std::string("Iron Sword"), weapon.getName());
			Assert::IsTrue(weapon.getType() == Item_Type::Weapon);
			Assert::AreEqual(14, weapon.getWeaponAttack());
			Assert::AreEqual(1, weapon.getWeaponDefense());
			Assert::IsTrue(weapon.getWeaponSlot() == WeaponSlot::Right_hand);

			Weapon shield = Weapon::fromLine("13;Wooden Shield;common;6;0.0;1;left_hand");
			Assert::IsTrue(shield.getWeaponSlot() == WeaponSlot::Left_hand);
		}

		TEST_METHOD(Item_fromLine_daje_junk_predmet)
		{
			Item item = Item::fromLine("3;Rusty Nail;common");

			Assert::AreEqual(std::string("Rusty Nail"), item.getName());
			Assert::IsTrue(item.getType() == Item_Type::Junk);
			Assert::IsTrue(item.getRarity() == Rarity::Common);
		}

		TEST_METHOD(seeStats_sadrzi_naziv_i_napad)
		{
			Weapon weapon("Test Sword", 1, Rarity::Epic, 2, 0.03f, 25, WeaponSlot::Right_hand);
			std::string stats = weapon.seeStats();

			Assert::IsTrue(stats.find("Test Sword") != std::string::npos);
			Assert::IsTrue(stats.find("25") != std::string::npos);
		}

		// Ucitavanje predlozaka iz datoteke. Broj predlozaka se provjerava kao
		// razlika prije i poslije, jer je skladiste staticko i moze vec
		// sadrzavati predloske ucitane u drugom testu.
		TEST_METHOD(loadFromFile_ucitava_predloske_i_stvara_predmete)
		{
			const std::string path = "test_armor_templates.txt";
			{
				std::ofstream file(path);
				file << "id;name;rarity;defense;speed;slot\n";
				file << "1;Test Helmet;common;5;0.02;helmet\n";
				file << "2;Test Chest;rare;9;0.01;chest\n";
			}

			int prije = ItemTemplateStore<Armor>::templateCount();
			ItemTemplateStore<Armor>::loadFromFile(path);
			int poslije = ItemTemplateStore<Armor>::templateCount();

			Assert::AreEqual(2, poslije - prije);

			int id = ItemTemplateStore<Armor>::getRandomId(Rarity::Common);
			Assert::IsTrue(id >= 0);

			std::unique_ptr<Armor> armor = ItemTemplateStore<Armor>::createItem(id);
			Assert::IsNotNull(armor.get());

			std::remove(path.c_str());
		}

		// createItem mora vratiti novu kopiju, a ne pokazivac na sam predlozak,
		// inace bi izmjena jednog predmeta u igri promijenila i predlozak.
		TEST_METHOD(createItem_vraca_neovisne_kopije)
		{
			const std::string path = "test_copy_templates.txt";
			{
				std::ofstream file(path);
				file << "id;name;rarity;defense;speed;slot\n";
				file << "1;Copy Helmet;epic;5;0.02;helmet\n";
			}
			ItemTemplateStore<Armor>::loadFromFile(path);

			int id = ItemTemplateStore<Armor>::getRandomId(Rarity::Epic);
			Assert::IsTrue(id >= 0);

			std::unique_ptr<Armor> prvi = ItemTemplateStore<Armor>::createItem(id);
			std::unique_ptr<Armor> drugi = ItemTemplateStore<Armor>::createItem(id);

			Assert::IsNotNull(prvi.get());
			Assert::IsNotNull(drugi.get());
			Assert::IsTrue(prvi.get() != drugi.get());
			Assert::AreEqual(prvi->getName(), drugi->getName());

			std::remove(path.c_str());
		}

		TEST_METHOD(createItem_za_nepostojecu_oznaku_vraca_nullptr)
		{
			Assert::IsNull(ItemTemplateStore<Armor>::createItem(-1).get());
			Assert::IsNull(ItemTemplateStore<Armor>::createItem(999999).get());
		}

		// Rubni slucaj: datoteka ne postoji.
		TEST_METHOD(loadFromFile_baca_FileNotFoundException)
		{
			Assert::ExpectException<FileNotFoundException>([]()
				{
					ItemTemplateStore<Weapon>::loadFromFile("ova_datoteka_ne_postoji.txt");
				});
		}

		// Rubni slucaj: datoteka postoji, ali sadrzi samo zaglavlje.
		TEST_METHOD(loadFromFile_baca_InvalidDataException_za_praznu_datoteku)
		{
			const std::string path = "test_empty_templates.txt";
			{
				std::ofstream file(path);
				file << "id;name;rarity;defense;speed;attack;slot\n";
			}

			Assert::ExpectException<InvalidDataException>([path]()
				{
					ItemTemplateStore<Weapon>::loadFromFile(path);
				});

			std::remove(path.c_str());
		}

		// Sve greske u igri moraju se moci uhvatiti i kao GameException.
		TEST_METHOD(iznimke_se_hvataju_kao_GameException)
		{
			Assert::ExpectException<GameException>([]()
				{
					ItemTemplateStore<Item>::loadFromFile("nema_me.txt");
				});
		}

		TEST_METHOD(FileNotFoundException_cuva_putanju)
		{
			try
			{
				ItemTemplateStore<Item>::loadFromFile("putanja_test.txt");
				Assert::Fail();
			}
			catch (const FileNotFoundException& greska)
			{
				Assert::AreEqual(std::string("putanja_test.txt"), greska.getFilePath());
			}
		}
	};
}
