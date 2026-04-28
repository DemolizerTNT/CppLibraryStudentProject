#include "Uzytkownik.h"

Uzytkownik::Uzytkownik(Baza& bazaRef, DaneUzytkownika noweDane) : Osoba(bazaRef, noweDane)
{

}

void Uzytkownik::wypozyczLista(QSqlQueryModel* model)
{
	db.wypozyczenia(this->getId(), model);
}

QList<QString> Uzytkownik::historiaLista()
{
	return db.pobierzHistorie(this->getId());
}

bool Uzytkownik::edycjaKonta(DaneUzytkownika noweDane)
{
	return db.edytujDaneKonta(this->getId(), noweDane);
}

bool Uzytkownik::wypoKsiazke(int id_ksiazki)
{
	return db.wypozyczKsiazke(this->getId(), id_ksiazki);
}

bool Uzytkownik::dodZwrot(int id_ksiazki)
{
	return db.dodajZwrot(this->getId(), id_ksiazki);
}