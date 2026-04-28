#pragma once
#include <iostream>
#include "Osoba.h"

class Bibliotekarz : public Osoba
{
public:
	Bibliotekarz(Baza& bazaRef, DaneUzytkownika noweDane);

	void wypoLista(int id, bool dostep, QSqlQueryModel* model);
	QList<QString> histoLista(int id, bool dostep);
	QList<DaneUzytkownika> kontaInformacje();

	bool stworzKonto(DaneUzytkownika noweDane);
	bool zmodyfikujKonto(int id, DaneUzytkownika noweDane);
	bool wylaczKonto(int id);

	bool stworzKsiazke(DaneKsiazki noweDane);
	bool zmodyfikujKsiazke(int id_k, DaneKsiazki noweDane); 
	bool wyrzucKsiazke(int id_k);

	QList<QString> zwrotyLista(); 
	bool akceptZwroty(int id_u, int id_z, int id_k);
};