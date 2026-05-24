#include "Baza.h"
#include <QDebug> // błędy do konsoli

Baza::Baza() {};

Baza::~Baza()
{

}

bool Baza::createTables()
{
	QSqlQuery kwerenda;

	// Uzytkownicy
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS Uzytkownicy("
		"Id_dane INTEGER PRIMARY KEY AUTOINCREMENT, Imie TEXT, Nazwisko TEXT, "
		"Email TEXT, Haslo TEXT, Rola TEXT, DataDodania TEXT, Aktywne INTEGER)")) {
		qDebug() << "Błąd tworzenia tabeli Uzytkownicy: " << kwerenda.lastError().text();
		return false;
	}

	// Wypozyczenia
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS Wypozyczenia("
		"Id_wyp INTEGER PRIMARY KEY AUTOINCREMENT, Id_uzyt INTEGER, "
		"Id_ksiazki INTEGER, DataWypozyczenia TEXT, TerminOddania TEXT, Status TEXT)")) {
		qDebug() << "Błąd tworzenia tabeli Wypozyczenia: " << kwerenda.lastError().text();
		return false;
	}

	// BazaKsiazek
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS BazaKsiazek("
		"Id_ksiazki INTEGER PRIMARY KEY AUTOINCREMENT, Nazwa TEXT, "
		"Autor TEXT, Ocena REAL, Kategoria TEXT, Opis TEXT, Wypozyczenia INT, IloscDostepna INTEGER, IloscOgolna INTEGER)")) {
		qDebug() << "Błąd tworzenia tabeli BazaKsiazek: " << kwerenda.lastError().text();
		return false;
	}

	// Historia
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS Historia("
		"Id_historia INTEGER PRIMARY KEY AUTOINCREMENT, Id_uzyt INTEGER, "
		"Id_ksiazki INTEGER, Akcja TEXT, Data TEXT)")) {
		qDebug() << "Błąd tworzenia tabeli Historia: " << kwerenda.lastError().text();
		return false;
	}

	// Zwrot
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS Zwrot("
		"Id_zwrot INTEGER PRIMARY KEY AUTOINCREMENT, Id_uzyt INTEGER, Id_ksiazki INTEGER, DataOddania TEXT)")) {
		qDebug() << "Błąd tworzenia tabeli Zwrot: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Wszystkie tabele zostały pomyślnie utworzone lub już istniały";
	return true;
}

bool Baza::connect()
{
	if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) // sprawdzenie, czy przedtem było już połączone
	{
		db = QSqlDatabase::database(QSqlDatabase::defaultConnection);
	}
	else
	{
		// Jeśli nie, tworzymy nowe
		db = QSqlDatabase::addDatabase("QSQLITE");
	}
	db.setDatabaseName("dbLibrary.db"); // Nazwa pliku bazy 

	if (!db.open()) {
		qDebug() << "Błąd łączenia z bazą: " << db.lastError().text(); // W przypadku problemu z połączeniem, w konsoli wyświetla się błąd
		return false;
	}

	qDebug() << "Połączono się pomyślnie z bazą!";

	if (createTables()) {
		qDebug() << "Struktura bazy danych jest gotowa.";
	}

	return true;
}

QList<DaneUzytkownika> Baza::pobierzKontaLogowania()
{
	QSqlQuery kwerenda;
	QList<DaneUzytkownika> listaKont;

	if (!kwerenda.exec("SELECT Id_dane, Imie, Nazwisko, Email, Rola FROM Uzytkownicy WHERE Aktywne = 1")){
		qDebug() << "Błąd pobierania danych konta: " << kwerenda.lastError().text();
		return listaKont;
	}

	while (kwerenda.next()) {
		DaneUzytkownika temp;
		
		temp.id_u = kwerenda.value("Id_dane").toInt();
		temp.imie = kwerenda.value("Imie").toString();
		temp.nazwisko = kwerenda.value("Nazwisko").toString();
		temp.email = kwerenda.value("Email").toString();
		temp.rola = kwerenda.value("Rola").toString();

		listaKont.append(temp); // Dodanie do listy
	}

	return listaKont;
}

int Baza::zaloguj(QString email, QString haslo)
{
	QSqlQuery kwerenda;

	kwerenda.prepare("SELECT Id_dane FROM Uzytkownicy WHERE Email = :email AND Haslo = :haslo AND Aktywne = 1");
	kwerenda.bindValue(":email", email);
	kwerenda.bindValue(":haslo", haslo);

	if (!kwerenda.exec()) {
		qDebug() << "Błąd logowania " << kwerenda.lastError().text();
		return -1;
	}
	if (kwerenda.next()) return kwerenda.value("Id_dane").toInt();

	return -1;
}

bool Baza::sprawdzUprawnienia(int id)
{
	QSqlQuery kwerenda;
	kwerenda.prepare("SELECT Id_dane FROM Uzytkownicy WHERE Id_dane = :id and Rola = 'Bibliotekarz'");
	kwerenda.bindValue(":id", id);

	if (!kwerenda.exec()) {
		qDebug() << "Błąd bazy podczas sprawdzania uprawnień: " << kwerenda.lastError().text();
		;
		return false;
	}
	if (kwerenda.next()) return true;

	return false;
}

QList<DaneWypozyczen> Baza::wypozyczenia(int id)
{
	return wypozyczenia(id, false);
}

QList<DaneWypozyczen> Baza::wypozyczenia(int id, bool dostep)
{
	QSqlQuery kwerenda;
	QList<DaneWypozyczen> wypo;

	if (!dostep || id!=0)
	{
		kwerenda.prepare("SELECT w.Id_wyp, w.Id_uzyt, w.Id_ksiazki, k.Nazwa, w.DataWypozyczenia, w.TerminOddania, w.Status FROM Wypozyczenia w join BazaKsiazek k on w.Id_ksiazki = k.Id_ksiazki WHERE w.Status != 'Zwrócona' AND w.id_uzyt = :id");
		kwerenda.bindValue(":id", id);
	}
	else
	{
		kwerenda.prepare("SELECT w.Id_wyp, w.Id_uzyt, w.Id_ksiazki, k.Nazwa, w.DataWypozyczenia, w.TerminOddania, w.Status FROM Wypozyczenia w join BazaKsiazek k on w.Id_ksiazki = k.Id_ksiazki WHERE w.Status != 'Zwrócona'");
	}

	if (!kwerenda.exec()) {
		qDebug() << "Błąd pobierania listy wypożyczeń: " << kwerenda.lastError().text();
		return wypo;
	}

	while (kwerenda.next())
	{
		DaneWypozyczen temp;

		temp.id_w = kwerenda.value("Id_wyp").toInt();
		temp.id_u = kwerenda.value("Id_uzyt").toInt();
		temp.id_k = kwerenda.value("Id_ksiazki").toInt();
		temp.nazwa = kwerenda.value("Nazwa").toString();
		temp.wypoz = kwerenda.value("DataWypozyczenia").toDate();
		temp.terOdd = kwerenda.value("TerminOddania").toDate();
		temp.Status = kwerenda.value("Status").toString();

		wypo.append(temp);
	}

	return wypo;
}

QList<DaneKsiazki> Baza::pobierzBiblioteke()
{
	QSqlQuery kwerenda;
	QList<DaneKsiazki> listaBiblioteki;

	kwerenda.prepare("SELECT Id_ksiazki, Nazwa, Autor, Ocena, Kategoria, Wypozyczenia, IloscOgolna, IloscDostepna FROM BazaKsiazek");

	if (!kwerenda.exec()) {
		qDebug() << "Błąd pobierania biblioteki: " << kwerenda.lastError().text();
		return listaBiblioteki;
	}
	
	while (kwerenda.next())
	{
		DaneKsiazki temp;

		temp.id_k = kwerenda.value("Id_ksiazki").toInt();
		temp.nazwa = kwerenda.value("Nazwa").toString();
		temp.autor = kwerenda.value("Autor").toString();
		temp.ocena = kwerenda.value("Ocena").toFloat();
		temp.kategoria = kwerenda.value("Kategoria").toString();
		temp.wypozyczenia = kwerenda.value("Wypozyczenia").toInt();
		temp.ilosc_ogolna = kwerenda.value("IloscOgolna").toInt();
		temp.ilosc_dostepna = kwerenda.value("IloscDostepna").toInt();


		listaBiblioteki.append(temp);
	}

	return listaBiblioteki;
}

DaneKsiazki Baza::pobierzKsiazke(int id_k)
{
	QSqlQuery kwerenda;
	DaneKsiazki temp;

	kwerenda.prepare("SELECT Id_ksiazki, Nazwa, Autor, Ocena, Kategoria, Opis, Wypozyczenia, IloscOgolna, IloscDostepna FROM BazaKsiazek WHERE id_ksiazki = :id");
	kwerenda.bindValue(":id", id_k);

	if (!kwerenda.exec()) {
		qDebug() << "Błąd pobierania danych książki: " << kwerenda.lastError().text();
		return temp;
	}

	if (kwerenda.next()) {
		temp.id_k = kwerenda.value("Id_ksiazki").toInt();
		temp.nazwa = kwerenda.value("Nazwa").toString();
		temp.autor = kwerenda.value("Autor").toString();
		temp.ocena = kwerenda.value("Ocena").toFloat();
		temp.kategoria = kwerenda.value("Kategoria").toString();
		temp.opis = kwerenda.value("Opis").toString();
		temp.ilosc_ogolna = kwerenda.value("IloscOgolna").toInt();
		temp.ilosc_dostepna = kwerenda.value("IloscDostepna").toInt();
	}

	return temp;
}

DaneWypozyczen Baza::pobierzWypozyczenie(int id)
{
	QSqlQuery kwerenda;
	DaneWypozyczen temp;

	kwerenda.prepare("SELECT w.Id_wyp, w.Id_uzyt, w.Id_ksiazki, k.Nazwa, w.DataWypozyczenia, w.TerminOddania, w.Status FROM Wypozyczenia w join BazaKsiazek k on w.Id_ksiazki = k.Id_ksiazki WHERE w.Id_wyp = :id");
	kwerenda.bindValue(":id", id);

	if (!kwerenda.exec()) {
		qDebug() << "Błąd pobierania danych konta: " << kwerenda.lastError().text();
		return temp;
	}

	if (kwerenda.next()) {
		temp.id_w = kwerenda.value("Id_wyp").toInt();
		temp.id_u = kwerenda.value("Id_uzyt").toInt();
		temp.id_k = kwerenda.value("Id_ksiazki").toInt();
		temp.nazwa = kwerenda.value("Nazwa").toString();
		temp.wypoz = kwerenda.value("DataWypozyczenia").toDate();
		temp.terOdd = kwerenda.value("TerminOddania").toDate();
		temp.Status = kwerenda.value("Status").toString();
	}

	return temp;
}

DaneUzytkownika Baza::pobierzKonto(int id)
{
	QSqlQuery kwerenda;
	DaneUzytkownika temp;

	kwerenda.prepare("SELECT Id_dane, Imie, Nazwisko, Email, Haslo, Rola, DataDodania, Aktywne FROM Uzytkownicy WHERE Id_dane = :id");
	kwerenda.bindValue(":id", id);

	if (!kwerenda.exec()) {
		qDebug() << "Błąd pobierania danych konta: " << kwerenda.lastError().text();
		return temp;
	}

	if(kwerenda.next()) {
		temp.id_u = kwerenda.value("Id_dane").toInt();
		temp.imie = kwerenda.value("Imie").toString();
		temp.nazwisko = kwerenda.value("Nazwisko").toString();
		temp.email = kwerenda.value("Email").toString();
		temp.haslo = kwerenda.value("Haslo").toString();
		temp.rola = kwerenda.value("Rola").toString();
		temp.dodanie = kwerenda.value("DataDodania").toDate();
		temp.aktywne = kwerenda.value("Aktywne").toBool();
	}

	return temp;
}

QList<DaneUzytkownika> Baza::pobierzKonta()
{
	QSqlQuery kwerenda;
	QList<DaneUzytkownika> listaKont;

	kwerenda.prepare("SELECT Id_dane, Imie, Nazwisko, Email, Haslo, Rola, DataDodania, Aktywne FROM Uzytkownicy");

	if (!kwerenda.exec()) {
		qDebug() << "Błąd pobierania danych konta: " << kwerenda.lastError().text();
		return listaKont;
	}

	while (kwerenda.next()) {
		DaneUzytkownika temp;

		temp.id_u = kwerenda.value("Id_dane").toInt();
		temp.imie = kwerenda.value("Imie").toString();
		temp.nazwisko = kwerenda.value("Nazwisko").toString();
		temp.email = kwerenda.value("Email").toString();
		temp.haslo = kwerenda.value("Haslo").toString();
		temp.rola = kwerenda.value("Rola").toString();
		temp.dodanie = kwerenda.value("DataDodania").toDate();
		temp.aktywne = kwerenda.value("Aktywne").toBool();

		listaKont.append(temp);
	}

	return listaKont;
}

QList<DaneHistorii> Baza::pobierzHistorie(int id)
{
	return pobierzHistorie(id, false);
}

QList<DaneHistorii> Baza::pobierzHistorie(int id, bool dostep)
{
	QSqlQuery kwerenda;
	QList<DaneHistorii> historia;

	if (!dostep || id != 0)
	{
		kwerenda.prepare("SELECT h.Id_historia, u.Id_dane, u.Imie, u.Nazwisko, k.Id_ksiazki, k.Nazwa, h.Akcja, h.Data FROM Historia h join Uzytkownicy u on u.Id_dane = h.Id_uzyt join BazaKsiazek k on k.Id_ksiazki = h.Id_ksiazki WHERE h.Id_uzyt = :id");
		kwerenda.bindValue(":id", id);
	}
	else
	{
		kwerenda.prepare("SELECT h.Id_historia, u.Id_dane, u.Imie, u.Nazwisko, k.Id_ksiazki, k.Nazwa, h.Akcja, h.Data FROM Historia h join Uzytkownicy u on u.Id_dane = h.Id_uzyt join BazaKsiazek k on k.Id_ksiazki = h.Id_ksiazki");
	}

	if (!kwerenda.exec()) {
		qDebug() << "Błąd pobierania historii: " << kwerenda.lastError().text();
		return historia;
	}

	while (kwerenda.next())
	{	
		DaneHistorii temp;

		temp.id_h = kwerenda.value("Id_historia").toInt();
		temp.id_u = kwerenda.value("Id_dane").toInt();
		temp.id_k = kwerenda.value("Id_ksiazki").toInt();
		temp.im = kwerenda.value("Imie").toString();
		temp.naz = kwerenda.value("Nazwisko").toString();
		temp.nazwa = kwerenda.value("Nazwa").toString();
		temp.akcja = kwerenda.value("Akcja").toString();
		temp.data = kwerenda.value("Data").toDate();

		historia.append(temp);
	}

	return historia;
}

bool Baza::edytujDaneKonta(int id, DaneUzytkownika noweDane) // Użytkownik
{
	QSqlQuery kwerenda;

	kwerenda.prepare("UPDATE Uzytkownicy SET Imie = :imie, Nazwisko = :nazwisko, Email = :email, Haslo = :haslo WHERE Id_dane = :id");
	kwerenda.bindValue(":imie", noweDane.imie);
	kwerenda.bindValue(":nazwisko", noweDane.nazwisko);
	kwerenda.bindValue(":email", noweDane.email);
	kwerenda.bindValue(":haslo", noweDane.haslo);
	kwerenda.bindValue(":id", id);

	if (!kwerenda.exec())
	{
		qDebug() << "Bład aktualizowania danych użytkownika: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyślnie zaktualizowano!";
	return true;
}

bool Baza::edytujKonto(int id, DaneUzytkownika noweDane) // Bibliotekarz
{
	QSqlQuery kwerenda;

	kwerenda.prepare("UPDATE Uzytkownicy SET Imie = :imie, Nazwisko = :nazwisko, Email = :email, Haslo = :haslo, Rola = :rola, Aktywne = :aktywne WHERE Id_dane = :id");
	kwerenda.bindValue(":imie", noweDane.imie);
	kwerenda.bindValue(":nazwisko", noweDane.nazwisko);
	kwerenda.bindValue(":email", noweDane.email);
	kwerenda.bindValue(":haslo", noweDane.haslo);
	kwerenda.bindValue(":rola", noweDane.rola);
	kwerenda.bindValue(":aktywne", noweDane.aktywne);
	kwerenda.bindValue(":id", id);

	if (!kwerenda.exec())
	{
		qDebug() << "Bład aktualizowania danych użytkownika: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyślnie zaktualizowano!";
	return true;
}

bool Baza::dodajKonto(DaneUzytkownika noweDane)
{
	QSqlQuery kwerenda;

	kwerenda.prepare("INSERT INTO Uzytkownicy(Imie, Nazwisko, Email, Haslo, Rola, DataDodania, Aktywne) "
		"VALUES (:imie, :nazwisko, :email, :haslo, :rola, :data, :active);");
	kwerenda.bindValue(":imie", noweDane.imie);
	kwerenda.bindValue(":nazwisko", noweDane.nazwisko);
	kwerenda.bindValue(":email", noweDane.email);
	kwerenda.bindValue(":haslo", noweDane.haslo);
	kwerenda.bindValue(":rola", noweDane.rola);
	kwerenda.bindValue(":data", noweDane.dodanie);
	kwerenda.bindValue(":active", 1);

	if (!kwerenda.exec())
	{
		qDebug() << "Bład dodawania konta: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyślnie dodano!";
	return true;
}

bool Baza::dezaktywujKonto(int id)
{
	QSqlQuery kwerenda;

	kwerenda.prepare("UPDATE Uzytkownicy SET Aktywne = 0 WHERE Id_dane = :id");
	kwerenda.bindValue(":id", id);

	if (!kwerenda.exec())
	{
		qDebug() << "Bład dezaktywacji konta: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyślnie dezaktywowano!";
	return true;
}

bool Baza::dodajKsiazke(DaneKsiazki noweDane)
{
	QSqlQuery kwerenda;
	
	kwerenda.prepare("INSERT INTO BazaKsiazek(Nazwa, Autor, Ocena, Kategoria, Opis, Wypozyczenia, IloscDostepna, IloscOgolna) "
		"VALUES (:nazwa, :autor, :ocena, :kat, :opi, :wyp, :dos, :ogo);");
	kwerenda.bindValue(":nazwa", noweDane.nazwa);
	kwerenda.bindValue(":autor", noweDane.autor);
	kwerenda.bindValue(":ocena", noweDane.ocena);
	kwerenda.bindValue(":kat", noweDane.kategoria);
	kwerenda.bindValue(":opi", noweDane.opis);
	kwerenda.bindValue(":wyp", 0);
	kwerenda.bindValue(":dos", noweDane.ilosc_ogolna);
	kwerenda.bindValue(":ogo", noweDane.ilosc_ogolna);

	if (!kwerenda.exec())
	{
		qDebug() << "Bład dodawania książki: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyślnie dodano!";
	return true;
}

bool Baza::edytujKsiazke(int id_k, DaneKsiazki noweDane)
{
	QSqlQuery kwerenda;

	kwerenda.prepare("UPDATE BazaKsiazek SET Nazwa = :nazwa, Autor = :autor, Ocena = :ocena, Kategoria = :kat, Opis = :opis, IloscDostepna = :dos, IloscOgolna = :ogo WHERE Id_ksiazki = :id");
	kwerenda.bindValue(":nazwa", noweDane.nazwa);
	kwerenda.bindValue(":autor", noweDane.autor);
	kwerenda.bindValue(":ocena", noweDane.ocena);
	kwerenda.bindValue(":kat", noweDane.kategoria);
	kwerenda.bindValue(":opis", noweDane.opis);
	kwerenda.bindValue(":dos", noweDane.ilosc_dostepna);
	kwerenda.bindValue(":ogo", noweDane.ilosc_ogolna);
	kwerenda.bindValue(":id", id_k);

	if (!kwerenda.exec())
	{
		qDebug() << "Bład edycji książki: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyślnie edytowano!";
	return true;
}

bool Baza::usunKsiazke(int id_k)
{
	QSqlQuery kwerenda;

	kwerenda.prepare("UPDATE BazaKsiazek SET IloscDostepna = 0, IloscOgolna = 0 WHERE Id_ksiazki = :id");
	kwerenda.bindValue(":id", id_k);

	if (!kwerenda.exec())
	{
		qDebug() << "Bład wyłączenia książki: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyślnie wyłączono!";
	return true;
}

bool Baza::wypozyczKsiazke(int id_u, int id_k) 
{
	QSqlDatabase::database().transaction();

	QSqlQuery kwerenda1, kwerenda2, kwerenda3;

	kwerenda1.prepare("UPDATE BazaKsiazek SET IloscDostepna = IloscDostepna - 1, Wypozyczenia = Wypozyczenia + 1 WHERE Id_ksiazki = :id AND IloscDostepna > 0");
	kwerenda1.bindValue(":id", id_k);

	kwerenda2.prepare("INSERT INTO Wypozyczenia(Id_uzyt, Id_ksiazki, DataWypozyczenia, TerminOddania, Status)"
		"VALUES(:id_u, :id_k, :wyp, :odd, 'Wypożyczona');");
	kwerenda2.bindValue(":id_u", id_u);
	kwerenda2.bindValue(":id_k", id_k);
	kwerenda2.bindValue(":wyp", QDate::currentDate());
	kwerenda2.bindValue(":odd", QDate::currentDate().addDays(14));

	kwerenda3.prepare("INSERT INTO Historia(Id_uzyt, Id_ksiazki, Akcja, Data) "
		"VALUES(:id_u, :id_k, 'Wypożyczenie książki', :data);");
	kwerenda3.bindValue(":id_u", id_u);
	kwerenda3.bindValue(":id_k", id_k);
	kwerenda3.bindValue(":data", QDate::currentDate());

	if (kwerenda1.exec() && kwerenda1.numRowsAffected() > 0 && kwerenda2.exec() && kwerenda3.exec())
	{
		QSqlDatabase::database().commit(); // Zatwierdzenie zmian
		qDebug() << "Pomyślnie wypozyczono!";
		return true;
	}
	else
	{
		QSqlDatabase::database().rollback(); // W razie erroru wycofanie
		qDebug() << "Bład wypożyczenia książki: " << kwerenda1.lastError().text() << kwerenda2.lastError().text() << kwerenda3.lastError().text();
		return false;
	}

}

bool Baza::dodajZwrot(int id_u, int id_k, int id_w)
{
	QSqlDatabase::database().transaction();

	QSqlQuery kwerenda1, kwerenda2, kwerenda3;

	kwerenda1.prepare("UPDATE Wypozyczenia SET Status = 'Zwrócona' WHERE Id_wyp = :id_w");
	kwerenda1.bindValue(":id_w", id_w);

	kwerenda2.prepare("INSERT INTO Zwrot(id_uzyt, Id_ksiazki, DataOddania)"
		"VALUES(:id_u, :id_k, :odd);");
	kwerenda2.bindValue(":id_u", id_u);
	kwerenda2.bindValue(":id_k", id_k);
	kwerenda2.bindValue(":odd", QDate::currentDate());

	kwerenda3.prepare("INSERT INTO Historia(Id_uzyt, Id_ksiazki, Akcja, Data) "
		"VALUES(:id_u, :id_k, 'Zwrot książki', :data);");
	kwerenda3.bindValue(":id_u", id_u);
	kwerenda3.bindValue(":id_k", id_k);
	kwerenda3.bindValue(":data", QDate::currentDate());

	if (kwerenda1.exec() && kwerenda1.numRowsAffected() > 0 && kwerenda2.exec() && kwerenda3.exec())
	{
		QSqlDatabase::database().commit(); // Zatwierdzenie zmian
		qDebug() << "Pomyślnie zwrócono!";
		return true;
	}
	else
	{
		QSqlDatabase::database().rollback(); // W razie erroru wycofanie
		qDebug() << "Bład zwrotu książki: " << kwerenda1.lastError().text() << kwerenda2.lastError().text() << kwerenda3.lastError().text();
		return false;
	}
}

QList<DaneZwrotu> Baza::pobierzZwroty()
{
	QSqlQuery kwerenda;
	QList<DaneZwrotu> zwroty;

	if (!kwerenda.exec("SELECT z.Id_zwrot, z.Id_uzyt, z.id_ksiazki, u.Imie, u.Nazwisko, k.Nazwa, z.DataOddania FROM Zwrot z JOIN Uzytkownicy u on u.Id_dane = z.Id_uzyt join "
		"BazaKsiazek k on k.Id_ksiazki = z.Id_Ksiazki")) {
		qDebug() << "Błąd pobierania zwrotów: " << kwerenda.lastError().text();
		return zwroty;
	}

	while (kwerenda.next())
	{
		DaneZwrotu temp;
		temp.id_z = kwerenda.value("Id_zwrot").toInt();
		temp.id_u = kwerenda.value("Id_uzyt").toInt();
		temp.id_k = kwerenda.value("Id_ksiazki").toInt();
		temp.im = kwerenda.value("Imie").toString();
		temp.naz = kwerenda.value("Nazwisko").toString();
		temp.nazwa = kwerenda.value("Nazwa").toString();
		temp.oddanie = kwerenda.value("DataOddania").toDate();
	
		zwroty.append(temp);
	}

	return zwroty;
}

DaneZwrotu Baza::pobierzZwrot(int id_z)
{
	QSqlQuery kwerenda;
	DaneZwrotu zwrot;

	kwerenda.prepare("SELECT z.Id_zwrot, z.Id_uzyt, z.Id_ksiazki, u.Imie, u.Nazwisko, k.Nazwa, z.DataOddania FROM Zwrot z JOIN Uzytkownicy u on u.Id_dane = z.Id_uzyt join "
		"BazaKsiazek k on k.Id_ksiazki = z.Id_Ksiazki WHERE Id_zwrot = :id_z");
	kwerenda.bindValue(":id_z", id_z);


	if (!kwerenda.exec()) {
		qDebug() << "Błąd pobierania zwrotu: " << kwerenda.lastError().text();
		return zwrot;
	}

	if (kwerenda.next())
	{
		zwrot.id_z = kwerenda.value("Id_zwrot").toInt();
		zwrot.id_u = kwerenda.value("Id_uzyt").toInt();
		zwrot.id_k = kwerenda.value("Id_ksiazki").toInt();
		zwrot.im = kwerenda.value("Imie").toString();
		zwrot.naz = kwerenda.value("Nazwisko").toString();
		zwrot.nazwa = kwerenda.value("Nazwa").toString();
		zwrot.oddanie = kwerenda.value("DataOddania").toDate();
	}

	return zwrot;
}

bool Baza::akceptujZwrot(int id_u, int id_z, int id_k)
{
	QSqlDatabase::database().transaction();

	QSqlQuery kwerenda1, kwerenda2, kwerenda3;

	kwerenda1.prepare("DELETE FROM Zwrot WHERE Id_zwrot =:id");
	kwerenda1.bindValue(":id", id_z);

	kwerenda2.prepare("UPDATE BazaKsiazek SET IloscDostepna = IloscDostepna + 1 WHERE Id_ksiazki = :id");
	kwerenda2.bindValue(":id", id_k);

	kwerenda3.prepare("INSERT INTO Historia(Id_uzyt, Id_ksiazki, Akcja, Data) VALUES (:id_u, :id_k, 'Potwierdzenie Oddania', :date);");
	kwerenda3.bindValue(":id_u", id_u);
	kwerenda3.bindValue(":id_k", id_k);
	kwerenda3.bindValue(":date", QDate::currentDate());

	if (kwerenda1.exec() && kwerenda1.numRowsAffected() > 0 && kwerenda2.exec() && kwerenda3.exec())
	{
		QSqlDatabase::database().commit();
		qDebug() << "Pomyślnie przyjęto!";
		return true;
	}
	else
	{
		QSqlDatabase::database().rollback();
		qDebug() << "Bład przyjęcia książki: " << kwerenda1.lastError().text() << kwerenda2.lastError().text();
		return false;
	}
}

QMap<QString, int> Baza::pobierzStatystyki()
{
	QMap<QString, int> dane;
	QSqlQuery kwerenda;

	kwerenda.prepare("SELECT Kategoria, SUM(Wypozyczenia) FROM BazaKsiazek group by kategoria having SUM(Wypozyczenia) > 0");

	if (!kwerenda.exec())
	{
		qDebug() << "Błąd pobierania statystyk: " << kwerenda.lastError().text();
		return dane;
	}

	while (kwerenda.next())
	{
		QString kategoria = kwerenda.value(0).toString();
		int ilosc = kwerenda.value(1).toInt();
		dane.insert(kategoria, ilosc);
	}

	return dane;
}
