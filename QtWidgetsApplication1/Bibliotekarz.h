#pragma once
#include <iostream>
#include "Osoba.h"

class Bibliotekarz : public Osoba
{
public:
	Bibliotekarz();
	~Bibliotekarz();

	Bibliotekarz(Baza& bazaRef, DaneUzytkownika noweDane);

	QList<DaneWypozyczen> wypoLista(int id, bool dostep);
	QList<DaneHistorii> histoLista(int id, bool dostep);
	QList<DaneUzytkownika> kontaInformacje();

	bool stworzKonto(DaneUzytkownika noweDane);
	bool zmodyfikujKonto(int id, DaneUzytkownika noweDane);
	bool wylaczKonto(int id);

	bool stworzKsiazke(DaneKsiazki noweDane);
	bool zmodyfikujKsiazke(int id_k, DaneKsiazki noweDane); 
	bool wyrzucKsiazke(int id_k);

	QList<DaneZwrotu> zwrotyLista(); 
	DaneZwrotu pobZwr(int id_z);
	bool akceptZwroty(int id_u, int id_z, int id_k);

	QMap<QString, int> statystyki();
};