#include "CppUnitTest.h"

#include "BattleEngine.h"

#include <cstdlib>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace oop::projekt;

namespace Tests
{
	// Testovi borbe: redoslijed poteza, izracun stete i uvjeti zavrsetka.
	TEST_CLASS(BattleTests)
	{
	public:

		TEST_METHOD(Prvi_potez_pripada_brzem_sudioniku)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 10, 0, 0.5, 2);
			Battle borba(&igrac, &protivnik);

			Assert::IsTrue(borba.processTick() == Battle::TurnOwner::Player);
		}

		// Kljucna osobina turn metera: brzi lik odigra vise poteza.
		TEST_METHOD(Brzi_lik_odigra_vise_poteza)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 10, 0, 0.25, 2);
			Battle borba(&igrac, &protivnik);

			int potezaIgraca = 0;
			int potezaProtivnika = 0;
			for (int i = 0; i < 40; i++)
			{
				if (borba.processTick() == Battle::TurnOwner::Player)
					potezaIgraca += 1;
				else
					potezaProtivnika += 1;
			}

			Assert::AreEqual(40, potezaIgraca + potezaProtivnika);
			Assert::IsTrue(potezaIgraca > potezaProtivnika);
		}

		// Rubni slucaj: sporiji lik ipak mora doci na red.
		TEST_METHOD(Sporiji_lik_ipak_dobiva_poteze)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 10, 0, 0.5, 2);
			Battle borba(&igrac, &protivnik);

			int potezaProtivnika = 0;
			for (int i = 0; i < 30; i++)
			{
				if (borba.processTick() == Battle::TurnOwner::Enemy)
					potezaProtivnika += 1;
			}

			Assert::IsTrue(potezaProtivnika > 0);
		}

		// Rubni slucaj: jednaka brzina ne smije zaglaviti borbu.
		TEST_METHOD(Jednaka_brzina_ne_zaglavljuje_borbu)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 10, 0, 1.0, 2);
			Battle borba(&igrac, &protivnik);

			int potezaIgraca = 0;
			int potezaProtivnika = 0;
			for (int i = 0; i < 20; i++)
			{
				if (borba.processTick() == Battle::TurnOwner::Player)
					potezaIgraca += 1;
				else
					potezaProtivnika += 1;
			}

			Assert::AreEqual(20, potezaIgraca + potezaProtivnika);
			Assert::IsTrue(potezaProtivnika > 0);
		}

		TEST_METHOD(Napad_igraca_oduzima_protivniku_hp)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 10, 0, 0.5, 2);
			Battle borba(&igrac, &protivnik);

			float steta = borba.performAttack(Battle::TurnOwner::Player);

			Assert::AreEqual(20.0f, steta, 0.001f);
			Assert::AreEqual(30.0f, protivnik.gethp(), 0.001f);
			Assert::AreEqual(100.0f, igrac.gethp(), 0.001f);
		}

		TEST_METHOD(Napad_protivnika_umanjuje_se_za_obranu_igraca)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 30, 0, 0.5, 2);
			Battle borba(&igrac, &protivnik);

			float steta = borba.performAttack(Battle::TurnOwner::Enemy);

			Assert::AreEqual(25.0f, steta, 0.001f);
			Assert::AreEqual(75.0f, igrac.gethp(), 0.001f);
		}

		TEST_METHOD(Obrana_dodatno_umanjuje_sljedeci_napad)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 30, 0, 0.5, 2);
			Battle borba(&igrac, &protivnik);

			borba.performDefense();
			float steta = borba.performAttack(Battle::TurnOwner::Enemy);

			// 30 napad - (20 napad igraca / 2) = 20, zatim - 5 obrane = 15
			Assert::AreEqual(15.0f, steta, 0.001f);
			Assert::AreEqual(85.0f, igrac.gethp(), 0.001f);
		}

		// Obrana vrijedi samo za jedan protivnicki napad.
		TEST_METHOD(Obrana_vrijedi_samo_jedan_napad)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 30, 0, 0.5, 2);
			Battle borba(&igrac, &protivnik);

			borba.performDefense();
			float prva = borba.performAttack(Battle::TurnOwner::Enemy);
			float druga = borba.performAttack(Battle::TurnOwner::Enemy);

			Assert::AreEqual(15.0f, prva, 0.001f);
			Assert::AreEqual(25.0f, druga, 0.001f);
		}

		TEST_METHOD(Borba_traje_dok_su_oba_sudionika_ziva)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 10, 0, 0.5, 2);
			Battle borba(&igrac, &protivnik);

			Assert::IsFalse(borba.isBattleOver());
			borba.performAttack(Battle::TurnOwner::Player);
			Assert::IsFalse(borba.isBattleOver());
			borba.performAttack(Battle::TurnOwner::Player);
			Assert::IsFalse(borba.isBattleOver());
			borba.performAttack(Battle::TurnOwner::Player);

			Assert::IsFalse(protivnik.isAlive());
			Assert::IsTrue(borba.isBattleOver());
		}

		TEST_METHOD(Borba_zavrsava_i_kad_igrac_pogine)
		{
			Player_Character igrac("Igrac", 20, 20, 0, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 25, 0, 0.5, 2);
			Battle borba(&igrac, &protivnik);

			borba.performAttack(Battle::TurnOwner::Enemy);

			Assert::IsFalse(igrac.isAlive());
			Assert::IsTrue(borba.isBattleOver());
		}

		// Bijeg je nasumican, pa se kroz mnogo pokusaja moraju pojaviti
		// oba ishoda.
		TEST_METHOD(Bijeg_daje_oba_ishoda)
		{
			Player_Character igrac("Igrac", 100, 20, 5, 1.0, 5);
			Enemy protivnik("Protivnik", 1, 50, 10, 0, 0.5, 2);
			Battle borba(&igrac, &protivnik);

			srand(12345);
			bool uspjeh = false;
			bool neuspjeh = false;
			for (int i = 0; i < 300; i++)
			{
				if (borba.performEscape())
					uspjeh = true;
				else
					neuspjeh = true;
			}

			Assert::IsTrue(uspjeh);
			Assert::IsTrue(neuspjeh);
		}
	};
}
