#pragma once
#include <iostream>
#include "Osoba.h"

class Uzytkownik : public Osoba
{
public:
	Uzytkownik();
	~Uzytkownik();

	Uzytkownik(Baza& bazaRef, DaneUzytkownika noweDane);

	QList<DaneWypozyczen> wypozyczLista(int id, bool dostep);
	QList<DaneHistorii> historiaLista(int id);
	bool edycjaKonta(DaneUzytkownika noweDane);
	bool wypoKsiazke(int id_ksiazki);
	bool dodZwrot(int id_u, int id_ksiazki, int id_w);
};