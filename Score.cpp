#include "Score.h"

namespace oop::projekt
{
	// Usporedba za sortiranje liste od najboljeg prema najlosijem rezultatu.
	static bool compareEntries(const ScoreEntry& first, const ScoreEntry& second)
	{
		return first.getEntryScore() > second.getEntryScore();
	}

	// Bodovi za pobjedu rastu s razinom protivnika i tezinom lokacije, pa
	// borbe na opasnijim mjestima nose do cetiri puta vise bodova.
	void Score::addEnemyScore(Enemy& enemy, Location* location)
	{
		int difficulty = Location::toInt(location->getlocationdifficulty());
		total_score += 10 * enemy.getLVL() * (1 + difficulty);
	}

	void Score::addBossScore()
	{
		total_score += boss_score;
	}

	void Score::addMidbossScore()
	{
		total_score += midboss_score;
	}

	void Score::addRemainingHpScore(float remaining_hp)
	{
		if (remaining_hp > 0)
			total_score += static_cast<int>(remaining_hp);
	}

	// Iz imena mice znak ';' jer se koristi kao razdjelnik u datoteci.
	// Konacni rezultat umanjen je za svaki dan proveden u igri, pa se
	// odmaranje isplati samo kad je stvarno potrebno.
	int Score::getFinalScore() const
	{
		int final_score = total_score - getDayPenalty();
		if (final_score < 0)
			final_score = 0;
		return final_score;
	}

	std::string Highscore::cleanName(std::string name) const
	{
		for (int i = 0; i < static_cast<int>(name.size()); i++)
		{
			if (name[i] == ';' || name[i] == '\n')
				name[i] = ' ';
		}
		if (name.empty())
			name = "Player";
		return name;
	}

	// Cita zapise iz datoteke. Neispravni redci se preskacu umjesto da se baci
	// iznimka, kako jedan ostecen redak ne bi onemogucio prikaz cijele liste.
	void Highscore::loadFromFile()
	{
		entries.clear();
		std::fstream file(file_path, std::ios::in);
		if (file.is_open())
		{
			std::string line;
			while (getline(file, line))
			{
				if (line.empty())
					continue;

				std::stringstream ss(line);
				std::string name, score_text;
				getline(ss, name, ';');
				getline(ss, score_text);

				int score_value = 0;
				std::stringstream score_stream(score_text);
				if (!(score_stream >> score_value))
					continue;

				entries.push_back(ScoreEntry(name, score_value));
			}
		}
		std::sort(entries.begin(), entries.end(), compareEntries);
		if (static_cast<int>(entries.size()) > max_entries)
			entries.erase(entries.begin() + max_entries, entries.end());
	}

	void Highscore::saveToFile() const
	{
		std::fstream file(file_path, std::ios::out);
		if (file.is_open())
		{
			for (int i = 0; i < static_cast<int>(entries.size()); i++)
			{
				file << entries[i].getEntryName() << ";" << entries[i].getEntryScore() << "\n";
			}
		}
	}

	// Dodaje rezultat, sortira listu i odmah je sprema, kako rezultat ne bi
	// bio izgubljen ako se program zatvori bez urednog izlaza.
	void Highscore::addScore(std::string name, int score)
	{
		entries.push_back(ScoreEntry(cleanName(name), score));
		std::sort(entries.begin(), entries.end(), compareEntries);
		if (static_cast<int>(entries.size()) > max_entries)
			entries.erase(entries.begin() + max_entries, entries.end());
		saveToFile();
	}

	std::vector<std::string> Highscore::getScoreNames() const
	{
		std::vector<std::string> score_names;
		for (int i = 0; i < static_cast<int>(entries.size()); i++)
		{
			score_names.push_back(std::to_string(i + 1) + ". " + entries[i].getEntryName() +
				" - " + std::to_string(entries[i].getEntryScore()));
		}
		return score_names;
	}

	int Highscore::getBestScore() const
	{
		if (entries.empty())
			return 0;
		return entries[0].getEntryScore();
	}
}
