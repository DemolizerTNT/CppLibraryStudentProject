#pragma once
#pragma execution_character_set("utf-8")

#include <iostream>
#include <QString>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QSqlQueryModel>

using namespace std;

struct DaneUzytkownika
{
	int id_u;
	QString imie, nazwisko, email, haslo, rola;
	QDate dodanie;
	bool aktywne;
};

struct DaneKsiazki
{
	int id_k;
	QString nazwa, autor;
	float ocena;
	QString kategoria, opis;
	int wypozyczenia, ilosc_ogolna, ilosc_dostepna;
};

struct DaneHistorii
{
	int id_h, id_u, id_k;
	QString im, naz, nazwa, akcja;
	QDate data;
};

struct DaneZwrotu
{
	int id_z, id_u, id_k;
	QString im, naz, nazwa;
	QDate oddanie;

};

struct DaneWypozyczen
{
	int id_w, id_u, id_k;
	QDate wypoz, terOdd;
	QString nazwa, Status;
};


class Baza {
private:
	QSqlDatabase db;
public:
	Baza();
	~Baza();
	bool connect();
	bool createTables();

	// Funkcje g³ównie dla U¿ytkownika
	int zaloguj(QString email, QString haslo); // zwróci ID U¿ytkownika przy poprawnym zalogowaniu siê
	QList<DaneWypozyczen> wypozyczenia(int id); // Sprawdzenie wypo¿yczeñ danego u¿ytkownika
	QList<DaneKsiazki> pobierzBiblioteke(); // Wyœwietli ca³¹ bibliotekê; sortowanie danymi polami bêdzie raczej jakoœ w Qt zrobione
	DaneKsiazki pobierzKsiazke(int id_k); // Wyœwietli dane ksi¹¿ki, potem przydatne do wpisania gdzieœ
	DaneWypozyczen pobierzWypozyczenie(int id); // Wyœwietli dane wypo¿yczenia
	QList<DaneHistorii> pobierzHistorie(int id); // Danego u¿ytkownika
	QList<DaneUzytkownika> pobierzKontaLogowania(); // U¿yjemy tego przy panelu logowania - wyœwietli tych u¿ytkowników, którzy s¹ w systemie - ich Imiê, Nazwisko
	DaneUzytkownika pobierzKonto(int id); // Wyœwietli konkretne dane dla u¿ytkownika, prawdopodobnie u¿yjemy tego w Panelu obs³ugi konta - pewnie bêdzie na samej górze po lewej
	bool edytujDaneKonta(int id, DaneUzytkownika noweDane); // Normalny u¿ytkownik bêdzie móg³ edytowaæ swoje dane - u¿yjemy tego jednak równie¿ przez Bibliotekarzy

	
	bool wypozyczKsiazke(int id_u, int id_k); // kto co wypozycza jak jest dostepne
	bool dodajZwrot(int id_u, int id_k, int id_w); // Kto co zwraca

	// Funkcje tylko dla Bibliotekarza
	bool sprawdzUprawnienia(int id); // Za pomoc¹ tej funkcji program bêdzie ustala³, czy dany u¿ytkownik ma dostêp do zaawansowanych funkcji
	QList<DaneWypozyczen>  wypozyczenia(int id, bool dostep); // Sprawdzenia ka¿dego u¿ytkownika; "bool dostep" u¿yje innej komendy po prostu
	QList<DaneHistorii> pobierzHistorie(int id, bool dostep); // Podobnie jak wy¿ej, dla ka¿dego u¿ytkownika
	QList<DaneUzytkownika> pobierzKonta(); // Wszystkich u¿ytkowników

	bool dodajKonto(DaneUzytkownika noweDane); // Stworzenie konta, trzeba bêdzie wpisaæ poprawnie 2 razy has³o - jak nie s¹ takie same, operacje zostaje odrzucona do poprawy.
	bool edytujKonto(int id, DaneUzytkownika noweDane); // Po uruchomieniu tej funkcji, program zapyta siê co trzeba zmieniæ. Jesli siê da w Qt, to wyœwietl¹ siê dane w panelach, które bêdziemy mogli edytowaæ. Jeœli tego nie zrobimy, zostan¹ takie jakie by³y.
	bool dezaktywujKonto(int id); // Usuniêcie konta - pytanie tylko, czy z jego histori¹ czy nie - bo jak tak, to trzeba bêdzie nie tyle usun¹æ konto z bazy (bo to mo¿e zepsuæ nam bazê danych, skoro id idzie po kolei) - tylko wy raczej zmieniæ status konta na "Niekatywne" z "Aktywnego" - dopiszê to (choæ pewno ju¿ dopisa³em) w tworzeniu konta. Bêdzie te¿ trzeba na podstawie tego przeszukiwaæ bazê potem.
	
	bool dodajKsiazke(DaneKsiazki noweDane);
	bool edytujKsiazke(int id_k, DaneKsiazki noweDane); // Podobnie jak w przypadku edycji konta u¿ytkownika
	bool usunKsiazke(int id_k);
	
	QList<DaneZwrotu> pobierzZwroty(); // Wypisuje wszystkie zwroty
	DaneZwrotu pobierzZwrot(int id_z); // Wypisuje konkretny zwrot
	bool akceptujZwrot(int id_u, int id_z, int id_k); // Potwierdza z ID tabeli Zwroty

	QMap<QString, int> pobierzStatystyki();
};