#pragma once
#include <iostream>
#include "Baza.h"


using namespace std;

class Osoba
{
protected:
	Baza& db;
	DaneUzytkownika daneKonta;

public:
	Osoba(Baza& dbRef, DaneUzytkownika daneKonta);
	virtual ~Osoba();

	int getId();
	QString getImie();
	QString getNazwisko();
	QString getRola();

	// Funkcje dziedziczone u Uzytkownika i Bibliotekarz
	DaneUzytkownika pobierzMojeDane();
};
