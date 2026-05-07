#include "Uzytkownik.h"

Uzytkownik::Uzytkownik()
{
}

Uzytkownik::~Uzytkownik()
{
}

Uzytkownik::Uzytkownik(Baza& bazaRef, DaneUzytkownika noweDane) : Osoba(bazaRef, noweDane)
{

}

QList<DaneWypozyczen> Uzytkownik::wypozyczLista(int id, bool dostep)
{
	return db.wypozyczenia(id, dostep);
}

QList<DaneHistorii> Uzytkownik::historiaLista(int id)
{
	return db.pobierzHistorie(id);
}

bool Uzytkownik::edycjaKonta(DaneUzytkownika noweDane)
{
	return db.edytujDaneKonta(this->getId(), noweDane);
}

bool Uzytkownik::wypoKsiazke(int id_ksiazki)
{
	return db.wypozyczKsiazke(this->getId(), id_ksiazki);
}

bool Uzytkownik::dodZwrot(int id_u, int id_ksiazki, int id_w)
{
	return db.dodajZwrot(id_u, id_ksiazki, id_w);
}