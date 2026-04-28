#include "Osoba.h"


Osoba::Osoba(Baza& dbRef, DaneUzytkownika dane): db(dbRef), daneKonta(dane)
{

}
Osoba:: ~Osoba()
{

}

int Osoba::getId()
{
	return daneKonta.id_u;
}

QString Osoba::getImie()
{
	return daneKonta.imie;
}

QString Osoba::getNazwisko()
{
	return daneKonta.nazwisko;
}

QString Osoba::getRola()
{
	return daneKonta.rola;
}

DaneUzytkownika Osoba::pobierzMojeDane()
{
	return db.pobierzKonto(this->getId());
}
