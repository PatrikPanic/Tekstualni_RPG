#include "CppUnitTest.h"

#include "Score.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace oop::projekt;

namespace Tests
{
	// Testovi bodovanja, brojaca dana i liste najboljih rezultata.
	TEST_CLASS(ScoreTests)
	{
	private:
		static Location lokacija(Location::Difficulty tezina)
		{
			return Location("Test", Location::LocationType::Cave, tezina);
		}

	public:

		TEST_METHOD(Nova_partija_pocinje_bez_bodova_na_prvom_danu)
		{
			Score score;

			Assert::AreEqual(0, score.getScore());
			Assert::AreEqual(1, score.getDays());
			Assert::AreEqual(0, score.getDayPenalty());
			Assert::AreEqual(0, score.getFinalScore());
		}

		// Bodovi rastu s razinom protivnika i tezinom lokacije.
		TEST_METHOD(Bodovi_ovise_o_razini_protivnika_i_tezini)
		{
			Enemy protivnik("Grub", 3, 50, 10, 0, 0.5, 2);
			Location easy = lokacija(Location::Difficulty::Easy);

			Score score;
			score.addEnemyScore(protivnik, &easy);

			// 10 * 3 * (1 + 0) = 30
			Assert::AreEqual(30, score.getScore());
		}

		TEST_METHOD(Teza_lokacija_nosi_vise_bodova)
		{
			Enemy protivnik("Grub", 3, 50, 10, 0, 0.5, 2);
			Location veryhard = lokacija(Location::Difficulty::VeryHard);

			Score score;
			score.addEnemyScore(protivnik, &veryhard);

			// 10 * 3 * (1 + 3) = 120
			Assert::AreEqual(120, score.getScore());
		}

		TEST_METHOD(Bodovi_se_zbrajaju_kroz_vise_pobjeda)
		{
			Enemy prvi("A", 2, 50, 10, 0, 0.5, 2);
			Enemy drugi("B", 5, 50, 10, 0, 0.5, 2);
			Location medium = lokacija(Location::Difficulty::Medium);

			Score score;
			score.addEnemyScore(prvi, &medium);    // 10*2*2 = 40
			score.addEnemyScore(drugi, &medium);   // 10*5*2 = 100

			Assert::AreEqual(140, score.getScore());
		}

		TEST_METHOD(Srednji_i_zavrsni_boss_nose_dodatne_bodove)
		{
			Score score;
			score.addMidbossScore();
			Assert::AreEqual(200, score.getScore());

			score.addBossScore();
			Assert::AreEqual(700, score.getScore());
		}

		TEST_METHOD(Preostali_hp_se_dodaje_kao_bodovi)
		{
			Score score;
			score.addRemainingHpScore(42.7f);

			Assert::AreEqual(42, score.getScore());
		}

		// Rubni slucaj: negativan hp ne smije oduzeti bodove.
		TEST_METHOD(Negativan_hp_ne_donosi_bodove)
		{
			Score score;
			score.addRemainingHpScore(-30.0f);

			Assert::AreEqual(0, score.getScore());
		}

		TEST_METHOD(Svaki_dan_umanjuje_konacni_rezultat)
		{
			Score score;
			score.addBossScore();          // 500
			Assert::AreEqual(500, score.getFinalScore());

			score.nextDay();               // dan 2
			Assert::AreEqual(2, score.getDays());
			Assert::AreEqual(25, score.getDayPenalty());
			Assert::AreEqual(475, score.getFinalScore());

			score.nextDay();
			score.nextDay();               // dan 4
			Assert::AreEqual(75, score.getDayPenalty());
			Assert::AreEqual(425, score.getFinalScore());
		}

		// Rubni slucaj: kazna veca od osvojenih bodova ne smije dati
		// negativan rezultat.
		TEST_METHOD(Konacni_rezultat_ne_moze_biti_negativan)
		{
			Score score;
			score.addRemainingHpScore(20.0f);
			for (int i = 0; i < 20; i++)
				score.nextDay();

			Assert::IsTrue(score.getDayPenalty() > score.getScore());
			Assert::AreEqual(0, score.getFinalScore());
		}

		TEST_METHOD(Reset_vraca_bodove_i_dane_na_pocetak)
		{
			Score score;
			score.addBossScore();
			score.nextDay();
			score.nextDay();

			score.reset();

			Assert::AreEqual(0, score.getScore());
			Assert::AreEqual(1, score.getDays());
			Assert::AreEqual(0, score.getDayPenalty());
		}

		// Rubni slucaj: datoteka jos ne postoji, lista mora biti prazna,
		// a ne baciti iznimku.
		TEST_METHOD(Nepostojeca_datoteka_daje_praznu_listu)
		{
			std::remove("test_hs_prazna.txt");

			Highscore highscore("test_hs_prazna.txt");

			Assert::AreEqual(0, highscore.entryCount());
			Assert::AreEqual(0, highscore.getBestScore());
			Assert::AreEqual(size_t(0), highscore.getScoreNames().size());
		}

		TEST_METHOD(Rezultati_se_sortiraju_od_najveceg)
		{
			std::remove("test_hs_sort.txt");
			Highscore highscore("test_hs_sort.txt");

			highscore.addScore("Ana", 300);
			highscore.addScore("Bruno", 900);
			highscore.addScore("Ceco", 100);

			Assert::AreEqual(3, highscore.entryCount());
			Assert::AreEqual(900, highscore.getBestScore());

			std::vector<std::string> popis = highscore.getScoreNames();
			Assert::IsTrue(popis[0].find("Bruno") != std::string::npos);
			Assert::IsTrue(popis[2].find("Ceco") != std::string::npos);

			std::remove("test_hs_sort.txt");
		}

		// Rubni slucaj: lista zadrzava najvise deset rezultata.
		TEST_METHOD(Lista_zadrzava_najboljih_deset)
		{
			std::remove("test_hs_deset.txt");
			Highscore highscore("test_hs_deset.txt");

			for (int i = 1; i <= 18; i++)
				highscore.addScore("Igrac", i * 10);

			Assert::AreEqual(10, highscore.entryCount());
			Assert::AreEqual(180, highscore.getBestScore());

			std::remove("test_hs_deset.txt");
		}

		TEST_METHOD(Rezultati_prezivljavaju_ponovno_ucitavanje)
		{
			std::remove("test_hs_spremanje.txt");
			{
				Highscore highscore("test_hs_spremanje.txt");
				highscore.addScore("Ana", 500);
				highscore.addScore("Bruno", 700);
			}

			Highscore ponovo("test_hs_spremanje.txt");

			Assert::AreEqual(2, ponovo.entryCount());
			Assert::AreEqual(700, ponovo.getBestScore());

			std::remove("test_hs_spremanje.txt");
		}

		// Rubni slucaj: ostecena datoteka. Neispravni redci se preskacu,
		// a ispravni se i dalje ucitavaju.
		TEST_METHOD(Neispravni_redci_se_preskacu)
		{
			const std::string path = "test_hs_neispravna.txt";
			{
				std::ofstream file(path);
				file << "Ana;500\n";
				file << "redak bez tocka zareza\n";
				file << "Bruno;nijebroj\n";
				file << ";\n";
				file << "\n";
				file << "Ceco;700\n";
			}

			Highscore highscore(path);

			Assert::AreEqual(2, highscore.entryCount());
			Assert::AreEqual(700, highscore.getBestScore());

			std::remove(path.c_str());
		}

		// Znak ';' u imenu bi pokvario zapis u datoteci, pa se uklanja.
		TEST_METHOD(Tocka_zarez_u_imenu_se_uklanja)
		{
			const std::string path = "test_hs_ime.txt";
			std::remove(path.c_str());
			{
				Highscore highscore(path);
				highscore.addScore("Ana;Bruno", 400);
			}

			Highscore ponovo(path);

			Assert::AreEqual(1, ponovo.entryCount());
			Assert::AreEqual(400, ponovo.getBestScore());

			std::remove(path.c_str());
		}

		TEST_METHOD(Ispis_liste_sadrzi_mjesto_ime_i_bodove)
		{
			const std::string path = "test_hs_ispis.txt";
			std::remove(path.c_str());
			Highscore highscore(path);
			highscore.addScore("Ana", 350);

			std::vector<std::string> popis = highscore.getScoreNames();

			Assert::AreEqual(size_t(1), popis.size());
			Assert::IsTrue(popis[0].find("1.") != std::string::npos);
			Assert::IsTrue(popis[0].find("Ana") != std::string::npos);
			Assert::IsTrue(popis[0].find("350") != std::string::npos);

			std::remove(path.c_str());
		}
	};
}
