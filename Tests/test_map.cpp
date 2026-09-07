#include "CppUnitTest.h"

#include "Map.h"

#include <algorithm>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace oop::projekt;

namespace Tests
{
	// Testovi mape: pocetna i zavrsna lokacija, veze medu lokacijama
	// i dostupnost svih lokacija.
	TEST_CLASS(MapTests)
	{
	private:
		static bool sadrzi(const std::vector<Location*>& lokacije, std::string naziv)
		{
			for (int i = 0; i < static_cast<int>(lokacije.size()); i++)
			{
				if (lokacije[i]->getLocationName() == naziv)
					return true;
			}
			return false;
		}

		static Location* pronadi(Map& mapa, std::string naziv)
		{
			std::vector<Location*> dostupne = mapa.getAvailableLocations();
			for (int i = 0; i < static_cast<int>(dostupne.size()); i++)
			{
				if (dostupne[i]->getLocationName() == naziv)
					return dostupne[i];
			}
			return nullptr;
		}

	public:

		TEST_METHOD(Igra_pocinje_u_Ironholdu)
		{
			Map mapa;

			Assert::IsNotNull(mapa.getCurrentLocation());
			Assert::AreEqual(std::string("Ironhold"), mapa.getCurrentLocation()->getLocationName());
			Assert::IsTrue(mapa.getCurrentLocation()->getLocationType() == Location::LocationType::City);
			Assert::IsTrue(mapa.getCurrentLocation()->getlocationdifficulty() == Location::Difficulty::Easy);
		}

		TEST_METHOD(Zavrsna_lokacija_je_Voidspire_Cave)
		{
			Map mapa;

			Assert::IsNotNull(mapa.getFinalLocation());
			Assert::AreEqual(std::string("Voidspire Cave"), mapa.getFinalLocation()->getLocationName());
			Assert::IsFalse(mapa.isAtFinalLocation());
		}

		TEST_METHOD(Srednji_bossovi_su_u_Mirkwoodu_i_Thornwoodu)
		{
			Map mapa;
			std::vector<Location*> bossovi = mapa.getMidbossLocations();

			Assert::AreEqual(size_t(2), bossovi.size());
			Assert::IsTrue(sadrzi(bossovi, "Mirkwood"));
			Assert::IsTrue(sadrzi(bossovi, "Thornwood"));
		}

		TEST_METHOD(isMidbossLocation_prepoznaje_prave_lokacije)
		{
			Map mapa;
			std::vector<Location*> bossovi = mapa.getMidbossLocations();

			Assert::IsTrue(mapa.isMidbossLocation(bossovi[0]));
			Assert::IsFalse(mapa.isMidbossLocation(mapa.getCurrentLocation()));
			Assert::IsFalse(mapa.isMidbossLocation(nullptr));
		}

		TEST_METHOD(Iz_Ironholda_vode_cetiri_veze)
		{
			Map mapa;
			std::vector<Location*> dostupne = mapa.getAvailableLocations();

			Assert::AreEqual(size_t(4), dostupne.size());
			Assert::IsTrue(sadrzi(dostupne, "Pinewood"));
			Assert::IsTrue(sadrzi(dostupne, "Ashwood"));
			Assert::IsTrue(sadrzi(dostupne, "Darkwood"));
			Assert::IsTrue(sadrzi(dostupne, "East Road"));
			Assert::IsFalse(sadrzi(dostupne, "Ashenvale"));
		}

		TEST_METHOD(Kretanje_mijenja_trenutnu_lokaciju)
		{
			Map mapa;
			Location* pinewood = pronadi(mapa, "Pinewood");
			Assert::IsNotNull(pinewood);

			mapa.moveTo(pinewood);

			Assert::AreEqual(std::string("Pinewood"), mapa.getCurrentLocation()->getLocationName());
		}

		TEST_METHOD(Veze_su_dvosmjerne)
		{
			Map mapa;
			Location* pinewood = pronadi(mapa, "Pinewood");
			mapa.moveTo(pinewood);

			Assert::IsTrue(sadrzi(mapa.getAvailableLocations(), "Ironhold"));
		}

		// Veza Darkwood - South Road je namjerno uklonjena, pa je put prema
		// jugu duzi.
		TEST_METHOD(Darkwood_nije_spojen_sa_South_Roadom)
		{
			Map mapa;
			Location* darkwood = pronadi(mapa, "Darkwood");
			Assert::IsNotNull(darkwood);

			mapa.moveTo(darkwood);
			std::vector<Location*> dostupne = mapa.getAvailableLocations();

			Assert::IsFalse(sadrzi(dostupne, "South Road"));
			Assert::IsTrue(sadrzi(dostupne, "Ironhold"));
			Assert::IsTrue(sadrzi(dostupne, "Stonehaven"));
		}

		TEST_METHOD(U_Voidspire_se_ulazi_samo_iz_Ashenvalea)
		{
			Map mapa;
			mapa.moveTo(mapa.getFinalLocation());

			std::vector<Location*> dostupne = mapa.getAvailableLocations();

			Assert::IsTrue(mapa.isAtFinalLocation());
			Assert::AreEqual(size_t(1), dostupne.size());
			Assert::AreEqual(std::string("Ashenvale"), dostupne[0]->getLocationName());
		}

		TEST_METHOD(resetToStart_vraca_igraca_u_Ironhold)
		{
			Map mapa;
			mapa.moveTo(mapa.getFinalLocation());
			Assert::IsTrue(mapa.isAtFinalLocation());

			mapa.resetToStart();

			Assert::AreEqual(std::string("Ironhold"), mapa.getCurrentLocation()->getLocationName());
		}

		// Sve lokacije moraju biti dostupne iz pocetne, inace bi dio mape
		// bio nedostizan.
		TEST_METHOD(Sve_lokacije_su_dostupne_iz_Ironholda)
		{
			Map mapa;
			std::set<std::string> posjecene;
			std::queue<Location*> red;

			posjecene.insert(mapa.getCurrentLocation()->getLocationName());
			red.push(mapa.getCurrentLocation());

			while (!red.empty())
			{
				Location* trenutna = red.front();
				red.pop();
				mapa.moveTo(trenutna);

				std::vector<Location*> susjedi = mapa.getAvailableLocations();
				for (int i = 0; i < static_cast<int>(susjedi.size()); i++)
				{
					if (posjecene.find(susjedi[i]->getLocationName()) == posjecene.end())
					{
						posjecene.insert(susjedi[i]->getLocationName());
						red.push(susjedi[i]);
					}
				}
			}

			Assert::AreEqual(size_t(15), posjecene.size());
			Assert::IsTrue(posjecene.find("Voidspire Cave") != posjecene.end());
		}

		TEST_METHOD(Tezina_lokacije_se_pretvara_u_broj)
		{
			Assert::AreEqual(0, Location::toInt(Location::Difficulty::Easy));
			Assert::AreEqual(1, Location::toInt(Location::Difficulty::Medium));
			Assert::AreEqual(2, Location::toInt(Location::Difficulty::Hard));
			Assert::AreEqual(3, Location::toInt(Location::Difficulty::VeryHard));
		}

		TEST_METHOD(Naziv_tipa_lokacije_se_cita_iz_teksta)
		{
			Assert::IsTrue(Location::stringToLocation("City") == Location::LocationType::City);
			Assert::IsTrue(Location::stringToLocation("Forest") == Location::LocationType::Forest);
			Assert::IsTrue(Location::stringToLocation("Cave") == Location::LocationType::Cave);
			Assert::IsTrue(Location::stringToLocation("Road") == Location::LocationType::Road);
		}

		// Rubni slucaj: nepoznat tip lokacije daje zadanu vrijednost.
		TEST_METHOD(Nepoznat_tip_lokacije_daje_zadanu_vrijednost)
		{
			Assert::IsTrue(Location::stringToLocation("nepostojeci") == Location::LocationType::City);
		}
	};
}
