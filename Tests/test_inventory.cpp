#include "CppUnitTest.h"

#include "Inventory.h"

#include <memory>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace oop::projekt;

namespace Tests
{
	// Testovi ruksaka te spremnika oklopa i oruzja.
	TEST_CLASS(InventoryTests)
	{
	private:
		static std::unique_ptr<Item> junk(std::string name, int id)
		{
			return std::make_unique<Item>(name, Item_Type::Junk, Rarity::Common, id);
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

		TEST_METHOD(Ruksak_broji_samo_popunjena_mjesta)
		{
			Backpack_Inventory backpack(5);
			Assert::AreEqual(0, backpack.inventory_size());
			Assert::IsFalse(backpack.isFull());

			backpack.addItem(junk("Kamen", 1));
			backpack.addItem(junk("Grana", 2));

			Assert::AreEqual(2, backpack.inventory_size());
			Assert::IsFalse(backpack.isFull());
		}

		// Rubni slucaj: pun ruksak mora odbiti novi predmet.
		TEST_METHOD(Pun_ruksak_odbija_predmet)
		{
			Backpack_Inventory backpack(3);
			for (int i = 0; i < 3; i++)
				Assert::IsTrue(backpack.addItem(junk("Predmet", i)));

			Assert::IsTrue(backpack.isFull());
			Assert::IsFalse(backpack.addItem(junk("Visak", 99)));
			Assert::AreEqual(3, backpack.inventory_size());
		}

		TEST_METHOD(Vadenje_predmeta_vraca_taj_predmet)
		{
			Backpack_Inventory backpack(5);
			backpack.addItem(junk("Prvi", 1));
			backpack.addItem(junk("Drugi", 2));

			std::unique_ptr<Item> izvadeni = backpack.removeItem(0);

			Assert::IsNotNull(izvadeni.get());
			Assert::AreEqual(std::string("Prvi"), izvadeni->getName());
			Assert::AreEqual(1, backpack.inventory_size());
		}

		// Nakon vadenja predmeta redoslijed preostalih mora ostati isti,
		// inace bi se popis u izborniku premijesao i igrac bi opremio krivi
		// predmet.
		TEST_METHOD(Vadenje_cuva_redoslijed_preostalih_predmeta)
		{
			Backpack_Inventory backpack(5);
			backpack.addItem(junk("Prvi", 1));
			backpack.addItem(junk("Drugi", 2));
			backpack.addItem(junk("Treci", 3));
			backpack.addItem(junk("Cetvrti", 4));

			backpack.removeItem(1);

			Assert::AreEqual(3, backpack.inventory_size());
			Assert::AreEqual(std::string("Prvi"), backpack.item_from_inventory(0)->getName());
			Assert::AreEqual(std::string("Treci"), backpack.item_from_inventory(1)->getName());
			Assert::AreEqual(std::string("Cetvrti"), backpack.item_from_inventory(2)->getName());
		}

		// Rubni slucaj: nakon vadenja ne smije ostati rupa, jer bi se indeksi
		// u izborniku razisli sa stvarnim mjestima.
		TEST_METHOD(Nakon_vadenja_nema_praznina_na_pocetku)
		{
			Backpack_Inventory backpack(4);
			backpack.addItem(junk("Prvi", 1));
			backpack.addItem(junk("Drugi", 2));
			backpack.addItem(junk("Treci", 3));

			backpack.removeItem(0);

			Assert::IsNotNull(backpack.item_from_inventory(0));
			Assert::IsNotNull(backpack.item_from_inventory(1));
			Assert::IsNull(backpack.item_from_inventory(2));
		}

		// Rubni slucaj: pozicija izvan granica.
		TEST_METHOD(Vadenje_izvan_granica_vraca_nullptr)
		{
			Backpack_Inventory backpack(3);
			backpack.addItem(junk("Prvi", 1));

			Assert::IsNull(backpack.removeItem(99).get());
			Assert::IsNull(backpack.removeItem(-1).get());
			Assert::AreEqual(1, backpack.inventory_size());
		}

		TEST_METHOD(Dohvat_izvan_granica_vraca_nullptr)
		{
			Backpack_Inventory backpack(3);
			Assert::IsNull(backpack.item_from_inventory(-1));
			Assert::IsNull(backpack.item_from_inventory(100));
		}

		// Rubni slucaj: prazan ruksak.
		TEST_METHOD(Prazan_ruksak_nema_predmeta)
		{
			Backpack_Inventory backpack(5);
			Assert::AreEqual(0, backpack.inventory_size());
			Assert::AreEqual(size_t(0), backpack.getItemNames().size());
			Assert::IsNull(backpack.removeItem(0).get());
		}

		TEST_METHOD(Oklop_ide_na_mjesto_svojeg_slota)
		{
			Armor_Inventory oklopi;
			oklopi.addItem(armor("Kaciga", 5, 0.02f, ArmorSlot::Helmet));
			oklopi.addItem(armor("Cizme", 3, 0.04f, ArmorSlot::Boots));

			Assert::AreEqual(std::string("Kaciga"), oklopi.item_from_inventory(0)->getName());
			Assert::AreEqual(std::string("Cizme"), oklopi.item_from_inventory(4)->getName());
			Assert::IsNull(oklopi.item_from_inventory(1));
		}

		TEST_METHOD(Ukupna_obrana_oklopa_je_zbroj)
		{
			Armor_Inventory oklopi;
			oklopi.addItem(armor("Kaciga", 5, 0.02f, ArmorSlot::Helmet));
			oklopi.addItem(armor("Prsluk", 9, 0.04f, ArmorSlot::Chest));

			Assert::AreEqual(14.0f, oklopi.total_armor_defense(), 0.001f);
		}

		// Brzina je prosjek, a ne zbroj, kako pet komada oklopa ne bi
		// neuobicajeno ubrzalo igraca.
		TEST_METHOD(Brzina_oklopa_je_prosjek_opremljenih)
		{
			Armor_Inventory oklopi;
			oklopi.addItem(armor("Kaciga", 5, 0.02f, ArmorSlot::Helmet));
			oklopi.addItem(armor("Prsluk", 9, 0.04f, ArmorSlot::Chest));

			Assert::AreEqual(0.03f, oklopi.total_armor_speed(), 0.001f);
		}

		// Rubni slucaj: prazan spremnik oklopa ne smije dijeliti s nulom.
		TEST_METHOD(Prazan_oklop_daje_nula_bonusa)
		{
			Armor_Inventory oklopi;

			Assert::AreEqual(0.0f, oklopi.total_armor_defense(), 0.001f);
			Assert::AreEqual(0.0f, oklopi.total_armor_speed(), 0.001f);
			Assert::AreEqual(size_t(5), oklopi.getItemNames().size());
		}

		TEST_METHOD(Oruzje_ide_u_odgovarajucu_ruku)
		{
			Weapon_Inventory oruzja;
			oruzja.addItem(weapon("Stit", 1, 6, 0.0f, WeaponSlot::Left_hand));
			oruzja.addItem(weapon("Mac", 14, 1, 0.06f, WeaponSlot::Right_hand));

			Assert::AreEqual(std::string("Stit"), oruzja.item_from_inventory(0)->getName());
			Assert::AreEqual(std::string("Mac"), oruzja.item_from_inventory(1)->getName());
			Assert::AreEqual(15.0f, oruzja.total_weapon_attack(), 0.001f);
			Assert::AreEqual(7.0f, oruzja.total_weapon_defense(), 0.001f);
		}

		TEST_METHOD(Prazno_oruzje_daje_nula_bonusa)
		{
			Weapon_Inventory oruzja;

			Assert::AreEqual(0.0f, oruzja.total_weapon_attack(), 0.001f);
			Assert::AreEqual(0.0f, oruzja.total_weapon_defense(), 0.001f);
			Assert::AreEqual(0.0f, oruzja.total_weapon_speed(), 0.001f);
		}

		// Pretvorba mora prepoznati da predmet nije oklop i vratiti nullptr.
		TEST_METHOD(transferToArmor_odbija_predmet_koji_nije_oklop)
		{
			Armor_Inventory oklopi;

			std::unique_ptr<Armor> dobar = oklopi.transferToArmor(armor("Kaciga", 5, 0.02f, ArmorSlot::Helmet));
			Assert::IsNotNull(dobar.get());

			std::unique_ptr<Armor> los = oklopi.transferToArmor(junk("Kamen", 1));
			Assert::IsNull(los.get());
		}

		TEST_METHOD(transferToWeapon_odbija_predmet_koji_nije_oruzje)
		{
			Weapon_Inventory oruzja;

			std::unique_ptr<Weapon> dobro = oruzja.transferToWeapon(weapon("Mac", 14, 1, 0.06f, WeaponSlot::Right_hand));
			Assert::IsNotNull(dobro.get());

			std::unique_ptr<Weapon> lose = oruzja.transferToWeapon(junk("Kamen", 1));
			Assert::IsNull(lose.get());
		}

		// Ispis se koristi u izborniku, pa mora sadrzavati nazive predmeta
		// odnosno oznaku praznog mjesta.
		TEST_METHOD(Ispis_ruksaka_sadrzi_nazive_predmeta)
		{
			Backpack_Inventory backpack(5);
			backpack.addItem(junk("Rusty Nail", 1));

			std::string ispis = backpack.getItemNames_string();
			Assert::IsTrue(ispis.find("Rusty Nail") != std::string::npos);
		}

		TEST_METHOD(Ispis_praznog_ruksaka_javlja_da_je_prazan)
		{
			Backpack_Inventory backpack(5);
			std::string ispis = backpack.getItemNames_string();

			Assert::IsTrue(ispis.find("empty") != std::string::npos);
		}

		TEST_METHOD(Ispis_oklopa_sadrzi_nazive_slotova)
		{
			Armor_Inventory oklopi;
			oklopi.addItem(armor("Iron Helmet", 5, 0.02f, ArmorSlot::Helmet));

			std::string ispis = oklopi.getItemNames_string();
			Assert::IsTrue(ispis.find("Helmet") != std::string::npos);
			Assert::IsTrue(ispis.find("Iron Helmet") != std::string::npos);
			Assert::IsTrue(ispis.find("Boots") != std::string::npos);
		}

		// Zauzeto mjesto se ne smije prepisati, jer bi se stari predmet tiho
		// izgubio. Pozivatelj ga mora prvo skinuti.
		TEST_METHOD(Oklop_ne_prepisuje_zauzeto_mjesto)
		{
			Armor_Inventory oklopi;
			Assert::IsTrue(oklopi.addItem(armor("Kaciga", 5, 0.02f, ArmorSlot::Helmet)));

			Assert::IsFalse(oklopi.addItem(armor("Bolja kaciga", 9, 0.03f, ArmorSlot::Helmet)));
			Assert::AreEqual(std::string("Kaciga"), oklopi.item_from_inventory(0)->getName());
		}

		TEST_METHOD(Oruzje_ne_prepisuje_zauzetu_ruku)
		{
			Weapon_Inventory oruzja;
			Assert::IsTrue(oruzja.addItem(weapon("Mac", 14, 1, 0.06f, WeaponSlot::Right_hand)));

			Assert::IsFalse(oruzja.addItem(weapon("Sjekira", 20, 0, 0.03f, WeaponSlot::Right_hand)));
			Assert::AreEqual(std::string("Mac"), oruzja.item_from_inventory(1)->getName());
		}
	};
}
