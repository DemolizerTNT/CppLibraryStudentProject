#pragma once
#include <iostream>
#include "Osoba.h"

class Uzytkownik : public Osoba
{
public:
	Uzytkownik(Baza& bazaRef, DaneUzytkownika noweDane);

	void wypozyczLista(QSqlQueryModel* model);
	QList<QString> historiaLista();
	bool edycjaKonta(DaneUzytkownika noweDane);
	bool wypoKsiazke(int id_ksiazki);
	bool dodZwrot(int id_ksiazki);
};