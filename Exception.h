#pragma once
#include <iostream>
#include <string>
#include <exception>

namespace oop::projekt
{
	// GameException - zajednicki predak svih gresaka koje igra sama prijavljuje.
	// Nasljeduje std::exception, pa se cijela hijerarhija moze uhvatiti jednim
	// catch blokom, bilo kao GameException bilo kao std::exception.
	class GameException : public std::exception
	{
	protected:
		std::string message;
	public:
		GameException(std::string message) { this->message = message; }
		virtual ~GameException() { ; }

		// what() je metoda iz std::exception i mora biti noexcept, pa se poruka
		// sprema u clan i vraca kao pokazivac na vec postojeci sadrzaj.
		const char* what() const noexcept override { return message.c_str(); }
		std::string getMessage() const { return message; }
	};

	// Trazena datoteka ne postoji ili se ne moze otvoriti. Baca se pri ucitavanju
	// predlozaka predmeta i imena protivnika, jer bez njih igra ne moze poceti.
	class FileNotFoundException : public GameException
	{
	private:
		std::string file_path;
	public:
		FileNotFoundException(std::string path)
			: GameException("File not found: " + path) {
			file_path = path;
		}
		std::string getFilePath() const { return file_path; }
	};

	// Datoteka postoji, ali iz nje nije procitan nijedan valjan zapis.
	// Cuva i naziv datoteke i opis problema, kako bi poruka bila upotrebljiva.
	class InvalidDataException : public GameException
	{
	private:
		std::string file_path;
		std::string reason;
	public:
		InvalidDataException(std::string path, std::string reason)
			: GameException("Invalid data in " + path + ": " + reason) {
			file_path = path;
			this->reason = reason;
		}
		std::string getFilePath() const { return file_path; }
		std::string getReason() const { return reason; }
	};

	// Predmet se ne moze dodati jer u ruksaku nema slobodnog mjesta.
	// Za razliku od gornjih dviju, ova greska nije kobna - igra je hvata
	// tijekom uzimanja plijena i samo obavijesti igraca.
	class InventoryFullException : public GameException
	{
	private:
		std::string item_name;
	public:
		InventoryFullException(std::string item_name)
			: GameException("Backpack is full, could not pick up: " + item_name) {
			this->item_name = item_name;
		}
		std::string getItemName() const { return item_name; }
	};
}
