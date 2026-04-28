#include "Bibliotekarz.h"

Bibliotekarz::Bibliotekarz(Baza& bazaRef, DaneUzytkownika noweDane) : Osoba(bazaRef, noweDane)
{

}

void Bibliotekarz::wypoLista(int id, bool dostep, QSqlQueryModel* model)
{
	db.wypozyczenia(id, dostep, model);
}

QList<QString> Bibliotekarz::histoLista(int id, bool dostep)
{
	return db.pobierzHistorie(id, dostep);
}

QList<DaneUzytkownika>  Bibliotekarz::kontaInformacje()
{
	return db.pobierzKonta();
}

bool Bibliotekarz::stworzKonto(DaneUzytkownika noweDane)
{
	return db.dodajKonto(noweDane);
}

bool Bibliotekarz::zmodyfikujKonto(int id, DaneUzytkownika noweDane)
{
	return db.edytujKonto(id, noweDane);
}

bool Bibliotekarz::wylaczKonto(int id)
{
	return db.dezaktywujKonto(id);
}

bool Bibliotekarz::stworzKsiazke(DaneKsiazki noweDane)
{
	return db.dodajKsiazke(noweDane);
}

bool Bibliotekarz::zmodyfikujKsiazke(int id_k, DaneKsiazki noweDane)
{
	return db.edytujKsiazke(id_k, noweDane);
}

bool Bibliotekarz::wyrzucKsiazke(int id_k)
{
	return db.usunKsiazke(id_k);
}

QList<QString> Bibliotekarz::zwrotyLista()
{
	return db.pobierzZwroty();
}

bool Bibliotekarz::akceptZwroty(int id_u, int id_z, int id_k)
{
	return db.akceptujZwrot(id_u, id_z, id_k);
}
