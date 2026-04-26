#pragma once
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
	QString imie;
	QString nazwisko;
	QString email;
	QString haslo;
	QString rola;
	QDate dodanie;
	bool aktywne;
};

struct DaneKsiazki
{
	int id_k;
	QString nazwa;
	QString autor;
	float ocena; // bêdzie raczej zawsze ocen¹ na 5, wiêc na sztywno wpiszemy. Raczej u¿ytkownik nie bêdzie wystawia³ opinii.
	QString kategoria;
	int ilosc_ogolna;
	int ilosc_dostepna;
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
	void wypozyczenia(int id, QSqlQueryModel* model); // Sprawdzenie wypo¿yczeñ danego u¿ytkownika
	void pobierzBiblioteke(QSqlQueryModel* model); // Wyœwietli ca³¹ bibliotekê; sortowanie danymi polami bêdzie raczej jakoœ w Qt zrobione
	DaneKsiazki pobierzKsiazke(int id_k); // Wyœwietli dane ksi¹¿ki, potem przydatne do wpisania gdzieœ
	QList<QString> pobierzHistorie(int id); // Danego u¿ytkownika
	QList<DaneUzytkownika> pobierzKontaLogowania(); // U¿yjemy tego przy panelu logowania - wyœwietli tych u¿ytkowników, którzy s¹ w systemie - ich Imiê, Nazwisko
	DaneUzytkownika pobierzKonto(int id); // Wyœwietli konkretne dane dla u¿ytkownika, prawdopodobnie u¿yjemy tego w Panelu obs³ugi konta - pewnie bêdzie na samej górze po lewej
	bool edytujDaneKonta(int id, DaneUzytkownika noweDane); // Normalny u¿ytkownik bêdzie móg³ edytowaæ swoje dane - u¿yjemy tego jednak równie¿ przez Bibliotekarzy

	
	bool wypozyczKsiazke(int id_u, int id_k); // kto co wypozycza jak jest dostepne
	bool dodajZwrot(int id_u, int id_k); // Kto co zwraca

	// Funkcje tylko dla Bibliotekarza
	bool sprawdzUprawnienia(int id); // Za pomoc¹ tej funkcji program bêdzie ustala³, czy dany u¿ytkownik ma dostêp do zaawansowanych funkcji
	void wypozyczenia(int id, bool dostep, QSqlQueryModel* model); // Sprawdzenia ka¿dego u¿ytkownika; "bool dostep" u¿yje innej komendy po prostu
	QList<QString> pobierzHistorie(int id, bool dostep); // Podobnie jak wy¿ej, dla ka¿dego u¿ytkownika
	QList<DaneUzytkownika> pobierzKonta(); // Wszystkich u¿ytkowników

	bool dodajKonto(DaneUzytkownika noweDane); // Stworzenie konta, trzeba bêdzie wpisaæ poprawnie 2 razy has³o - jak nie s¹ takie same, operacje zostaje odrzucona do poprawy.
	bool edytujKonto(int id, DaneUzytkownika noweDane); // Po uruchomieniu tej funkcji, program zapyta siê co trzeba zmieniæ. Jesli siê da w Qt, to wyœwietl¹ siê dane w panelach, które bêdziemy mogli edytowaæ. Jeœli tego nie zrobimy, zostan¹ takie jakie by³y.
	bool dezaktywujKonto(int id); // Usuniêcie konta - pytanie tylko, czy z jego histori¹ czy nie - bo jak tak, to trzeba bêdzie nie tyle usun¹æ konto z bazy (bo to mo¿e zepsuæ nam bazê danych, skoro id idzie po kolei) - tylko wy raczej zmieniæ status konta na "Niekatywne" z "Aktywnego" - dopiszê to (choæ pewno ju¿ dopisa³em) w tworzeniu konta. Bêdzie te¿ trzeba na podstawie tego przeszukiwaæ bazê potem.
	
	bool dodajKsiazke(DaneKsiazki noweDane);
	bool edytujKsiazke(int id_k, DaneKsiazki noweDane); // Podobnie jak w przypadku edycji konta u¿ytkownika
	bool usunKsiazke(int id_k);
	
	QList<QString> pobierzZwroty(); // Wypisuje wszystkie zwroty
	bool akceptujZwrot(int id_z, int id_k); // Potwierdza z ID tabeli Zwroty
};