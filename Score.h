#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Player_Character.h"
#include "Location.h"

namespace oop::projekt
{
	// Prati bodove osvojene tijekom jedne odigrane partije.
	// Score - prati bodove i protekle dane jedne odigrane partije.
	// Bodovi se dobivaju za pobjede, a dani se trose odmaranjem u gradu i
	// umanjuju konacni rezultat, pa se odmor ne isplati bez potrebe.
	class Score
	{
	private:
		int total_score;
		int days;
	public:
		static const int boss_score = 500;
		static const int midboss_score = 200;
		static const int day_penalty = 25;

		Score() { total_score = 0; days = 1; }
		void addEnemyScore(Enemy& enemy, Location* location);
		void addBossScore();
		void addMidbossScore();
		void addRemainingHpScore(float remaining_hp);
		void nextDay() { days += 1; }
		int getDays() const { return days; }
		int getScore() const { return total_score; }
		int getDayPenalty() const { return (days - 1) * day_penalty; }
		int getFinalScore() const;
		void reset() { total_score = 0; days = 1; }
	};

	// Jedan zapis u listi najboljih rezultata.
	// ScoreEntry - jedan zapis u listi najboljih rezultata.
	class ScoreEntry
	{
	private:
		std::string entry_name;
		int entry_score;
	public:
		ScoreEntry(std::string name, int score) { entry_name = name; entry_score = score; }
		std::string getEntryName() const { return entry_name; }
		int getEntryScore() const { return entry_score; }
	};

	// Lista najboljih rezultata, trajno spremljena u tekstualnu datoteku.
	// Highscore - lista najboljih rezultata, trajno spremljena u tekstualnu
	// datoteku. Cuva se najboljih deset rezultata, sortiranih od najveceg.
	// Ako datoteka ne postoji, lista je samo prazna - to nije greska, jer se
	// datoteka stvara tek nakon prve odigrane partije.
	class Highscore
	{
	private:
		std::vector<ScoreEntry> entries;
		std::string file_path;
		std::string cleanName(std::string name) const;
	public:
		static const int max_entries = 10;

		Highscore(std::string path = "highscore.txt") { file_path = path; loadFromFile(); }
		void loadFromFile();
		void saveToFile() const;
		void addScore(std::string name, int score);
		std::vector<std::string> getScoreNames() const;
		int entryCount() const { return static_cast<int>(entries.size()); }
		int getBestScore() const;
	};
}
