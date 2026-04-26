#include "Baza.h"
#include <QDebug> // b³êdy do konsoli

Baza::Baza() {};

Baza::~Baza()
{
	if (db.isOpen()) db.close();
}

bool Baza::connect()
{
	db = QSqlDatabase::addDatabase("SQLITE"); // Silnik bazy
	db.setDatabaseName("dbLibrary.db"); // Nazwa pliku bazy 

	if (!db.open()) {
		qDebug() << "B³¹d ³¹czenia z baz¹: " << db.lastError().text(); // W przypadku problemu z po³¹czeniem, w konsoli wyœwietla siê b³¹d
		return false;
	}

	qDebug() << "Po³¹czono siê pomyœlnie z baz¹!";
	return true;
}

bool Baza::createTables()
{
	QSqlQuery kwerenda;

	// Uzytkownicy
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS Uzytkownicy("
		"Id_dane INTEGER PRIMARY KEY AUTOINCREMENT, Imie TEXT, Nazwisko TEXT, "
		"Email TEXT, Haslo TEXT, Rola TEXT, DataDodania TEXT, Aktywne INTEGER)")) {
		qDebug() << "B³¹d tworzenia tabeli Uzytkownicy: " << kwerenda.lastError().text();
		return false;
	}

	// Wypozyczenia
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS Wypozyczenia("
		"Id_wyp INTEGER PRIMARY KEY AUTOINCREMENT, Id_uzyt INTEGER, "
		"Id_ksiazki INTEGER, DataWypozyczenia TEXT, TerminOddania TEXT, Status TEXT)")) {
		qDebug() << "B³¹d tworzenia tabeli Wypozyczenia: " << kwerenda.lastError().text();
		return false;
	}

	// BazaKsiazek
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS BazaKsiazek("
		"Id_ksiazki INTEGER PRIMARY KEY AUTOINCREMENT, Nazwa TEXT, "
		"Autor TEXT, Ocena REAL, Kategoria TEXT, IloscDostepna INTEGER, IloscOgolna INTEGER)")) {
		qDebug() << "B³¹d tworzenia tabeli BazaKsiazek: " << kwerenda.lastError().text();
		return false;
	}

	// Historia
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS Historia("
		"Id_historia INTEGER PRIMARY KEY AUTOINCREMENT, Id_uzyt INTEGER, "
		"Id_ksiazki INTEGER, Akcja TEXT, Data TEXT)")) {
		qDebug() << "B³¹d tworzenia tabeli Historia: " << kwerenda.lastError().text();
		return false;
	}

	// Zwrot
	if (!kwerenda.exec("CREATE TABLE IF NOT EXISTS Zwrot("
		"Id_zwrot INTEGER PRIMARY KEY AUTOINCREMENT, Id_uzyt INTEGER, Id_ksiazki INTEGER, DataOddania TEXT)")) {
		qDebug() << "B³¹d tworzenia tabeli Zwrot: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Wszystkie tabele zosta³y pomyœlnie utworzone lub ju¿ istnia³y";
	return true;
}

QList<DaneUzytkownika> Baza::pobierzKontaLogowania()
{
	QSqlQuery kwerenda;
	QList<DaneUzytkownika> listaKont;

	if (!kwerenda.exec("SELECT Id_dane, Imie, Nazwisko, Email FROM Uzytkownicy WHERE Aktywne = 1")){
		qDebug() << "B³¹d pobierania danych konta: " << kwerenda.lastError().text();
		return listaKont;
	}

	while (kwerenda.next()) {
		DaneUzytkownika temp;
		
		temp.id_u = kwerenda.value("Id_dane").toInt();
		temp.imie = kwerenda.value("Imie").toString();
		temp.nazwisko = kwerenda.value("Nazwisko").toString();
		temp.email = kwerenda.value("Email").toString();

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
		qDebug() << "B³¹d logowania " << kwerenda.lastError().text();
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
		qDebug() << "B³¹d bazy podczas sprawdzania uprawnieñ: " << kwerenda.lastError().text();
		return false;
	}
	if (kwerenda.next()) return true;

	return false;
}

void Baza::wypozyczenia(int id, QSqlQueryModel* model)
{
	wypozyczenia(id, false, model);
}

void Baza::wypozyczenia(int id, bool dostep, QSqlQueryModel* model)
{
	if (!model) return;

	QSqlQuery kwerenda;

	if (!dostep || id!=-1)
	{
		kwerenda.prepare("SELECT Id_wyp, Id_uzyt, Id_ksiazki, DataWypozyczenia, TerminOddania, Status FROM Wypozyczenia WHERE id_uzyt = :id");
		kwerenda.bindValue(":id", id);
	}
	else
	{
		kwerenda.prepare("SELECT Id_wyp, Id_uzyt, Id_ksiazki, DataWypozyczenia, TerminOddania, Status FROM Wypozyczenia");
	}

	if (!kwerenda.exec()) {
		qDebug() << "B³¹d pobierania listy wypo¿yczeñ: " << kwerenda.lastError().text();
	}

	model->setQuery(std::move(kwerenda));

}

void Baza::pobierzBiblioteke(QSqlQueryModel* model)
{
	if (!model) return; // zabezpieczenie przed crashem

	QSqlQuery kwerenda;
	//QSqlQueryModel* model = new QSqlQueryModel();

	kwerenda.prepare("SELECT Id_ksiazki, Nazwa, Autor, Ocena, Kategoria, IloscOgolna, IloscDostepna FROM BazaKsiazek");

	if (!kwerenda.exec()) {
		qDebug() << "B³¹d pobierania biblioteki: " << kwerenda.lastError().text();
		return;
	}

	model->setQuery(std::move(kwerenda));

	return;
}

DaneKsiazki Baza::pobierzKsiazke(int id_k)
{
	QSqlQuery kwerenda;
	DaneKsiazki temp;

	kwerenda.prepare("SELECT Id_ksiazki, Nazwa, Autor, Ocena, Kategoria, IloscOgolna, IloscDostepna FROM BazaKsiazek WHERE id_ksiazki = :id");
	kwerenda.bindValue(":id", id_k);

	if (!kwerenda.exec()) {
		qDebug() << "B³¹d pobierania danych ksi¹¿ki: " << kwerenda.lastError().text();
		return temp;
	}

	if (kwerenda.next()) {
		temp.id_k = kwerenda.value("Id_ksiazki").toInt();
		temp.nazwa = kwerenda.value("Nazwa").toString();
		temp.autor = kwerenda.value("Autor").toString();
		temp.ocena = kwerenda.value("Ocena").toFloat();
		temp.kategoria = kwerenda.value("Kategoria").toString();
		temp.ilosc_ogolna = kwerenda.value("IloscOgolna").toInt();
		temp.ilosc_dostepna = kwerenda.value("IloscDostepna").toInt();
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
		qDebug() << "B³¹d pobierania danych konta: " << kwerenda.lastError().text();
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
		qDebug() << "B³¹d pobierania danych konta: " << kwerenda.lastError().text();
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

QList<QString> Baza::pobierzHistorie(int id)
{
	return pobierzHistorie(id, false);
}

QList<QString> Baza::pobierzHistorie(int id, bool dostep)
{
	QSqlQuery kwerenda;
	QList<QString> historia;

	if (!dostep || id != -1)
	{
		kwerenda.prepare("SELECT h.Id_historia, u.Imie, u.Nazwisko, k.Nazwa, h.Akcja, h.Data FROM Historia h join Uzytkownicy u on u.Id_dane = h.Id_uzyt join BazaKsiazek k on k.Id_ksiazki = h.Id_ksiazki WHERE h.Id_uzyt = :id");
		kwerenda.bindValue(":id", id);
	}
	else
	{
		kwerenda.prepare("SELECT h.Id_historia, u.Imie, u.Nazwisko, k.Nazwa, h.Akcja, h.Data FROM Historia h join Uzytkownicy u on u.Id_dane = h.Id_uzyt join BazaKsiazek k on k.Id_ksiazki = h.Id_ksiazki");
	}

	if (!kwerenda.exec()) {
		qDebug() << "B³¹d pobierania historii: " << kwerenda.lastError().text();
		return historia;
	}

	while (kwerenda.next())
	{
		int id_h = kwerenda.value("Id_historia").toInt();
		/*int id_u = kwerenda.value("Id_uzyt").toInt();
		int id_k = kwerenda.value("Id_ksiazki").toInt();*/
		QString im = kwerenda.value("Imie").toString();
		QString naz = kwerenda.value("Nazwisko").toString();
		QString ksi = kwerenda.value("Nazwa").toString();
		QString akcja = kwerenda.value("Akcja").toString();
		QString data = kwerenda.value("Data").toString();

		/*DaneUzytkownika temp2 = pobierzKonto(id_u);
		DaneKsiazki temp3 = pobierzKsiazke(id_k);*/
		QString temp = QString("%1. U¿ytkownik %2 %3 dokona³: %4 z: %5 w dniu %6").arg(id_h).arg(im).arg(naz).arg(akcja).arg(ksi).arg(data);
		historia.append(temp);
	}

	return historia;
}

bool Baza::edytujDaneKonta(int id, DaneUzytkownika noweDane) // U¿ytkownik
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
		qDebug() << "B³ad aktualizowania danych u¿ytkownika: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyœlnie zaktualizowano!";
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
		qDebug() << "B³ad aktualizowania danych u¿ytkownika: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyœlnie zaktualizowano!";
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
	kwerenda.bindValue(":active", noweDane.aktywne);

	if (!kwerenda.exec())
	{
		qDebug() << "B³ad dodawania konta: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyœlnie dodano!";
	return true;
}

bool Baza::dezaktywujKonto(int id)
{
	QSqlQuery kwerenda;

	kwerenda.prepare("UPDATE Uzytkownicy SET Aktywne = 0 WHERE Id_dane = :id");
	kwerenda.bindValue(":id", id);

	if (!kwerenda.exec())
	{
		qDebug() << "B³ad dezaktywacji konta: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyœlnie dezaktywowano!";
	return true;
}

bool Baza::dodajKsiazke(DaneKsiazki noweDane)
{
	QSqlQuery kwerenda;

	kwerenda.prepare("INSERT INTO BazaKsiazek(Nazwa, Autor, Ocena, Kategoria, IloscDostepna, IloscOgolna) "
		"VALUES (:nazwa, :autor, :ocena, :kat, :dos, :ogo);");
	kwerenda.bindValue(":nazwa", noweDane.nazwa);
	kwerenda.bindValue(":autor", noweDane.autor);
	kwerenda.bindValue(":ocena", noweDane.ocena);
	kwerenda.bindValue(":kat", noweDane.kategoria);
	kwerenda.bindValue(":dos", noweDane.ilosc_dostepna);
	kwerenda.bindValue(":ogo", noweDane.ilosc_ogolna);

	if (!kwerenda.exec())
	{
		qDebug() << "B³ad dodawania ksi¹¿ki: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyœlnie dodano!";
	return true;
}

bool Baza::edytujKsiazke(int id_k, DaneKsiazki noweDane)
{
	QSqlQuery kwerenda;

	kwerenda.prepare("UPDATE BazaKsiazek SET Nazwa = :nazwa, Autor = :autor, Ocena = :ocena, Kategoria = :kat, IloscDostepna = :dos, IloscOgolna = :ogo WHERE Id_ksiazki = :id");
	kwerenda.bindValue(":nazwa", noweDane.nazwa);
	kwerenda.bindValue(":autor", noweDane.autor);
	kwerenda.bindValue(":ocena", noweDane.ocena);
	kwerenda.bindValue(":kat", noweDane.kategoria);
	kwerenda.bindValue(":dos", noweDane.ilosc_dostepna);
	kwerenda.bindValue(":ogo", noweDane.ilosc_ogolna);
	kwerenda.bindValue(":id", id_k);

	if (!kwerenda.exec())
	{
		qDebug() << "B³ad edycji ksi¹¿ki: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyœlnie edytowano!";
	return true;
}

bool Baza::usunKsiazke(int id_k)
{
	QSqlQuery kwerenda;

	kwerenda.prepare("UPDATE BazaKsiazek SET IloscDostepna = 0, IloscOgolna = 0 WHERE Id_ksiazki = :id");
	kwerenda.bindValue(":id", id_k);

	if (!kwerenda.exec())
	{
		qDebug() << "B³ad wy³¹czenia ksi¹¿ki: " << kwerenda.lastError().text();
		return false;
	}

	qDebug() << "Pomyœlnie wy³¹czono!";
	return true;
}

bool Baza::wypozyczKsiazke(int id_u, int id_k) 
{
	QSqlDatabase::database().transaction();

	QSqlQuery kwerenda1, kwerenda2, kwerenda3;

	kwerenda1.prepare("UPDATE BazaKsiazek SET IloscDostepna = IloscDostepna - 1 WHERE Id_ksiazki = :id AND IloscDostepna > 0");
	kwerenda1.bindValue(":id", id_k);

	kwerenda2.prepare("INSERT INTO Wypozyczenia(Id_uzyt, Id_ksiazki, DataWypozyczenia, TerminOddania, Status)"
		"VALUES(:id_u, :id_k, :wyp, :odd, 'Wypo¿yczona');");
	kwerenda2.bindValue(":id_u", id_u);
	kwerenda2.bindValue(":id_k", id_k);
	kwerenda2.bindValue(":wyp", QDate::currentDate());
	kwerenda2.bindValue(":odd", QDate::currentDate().addDays(14));

	kwerenda3.prepare("INSERT INTO Historia(Id_uzyt, Id_ksiazki, Akcja, Data) "
		"VALUES(:id_u, :id_k, 'Wypo¿yczenie ksi¹¿ki', :data);");
	kwerenda3.bindValue(":id_u", id_u);
	kwerenda3.bindValue(":id_k", id_k);
	kwerenda3.bindValue(":data", QDate::currentDate());

	if (kwerenda1.exec() && kwerenda1.numRowsAffected() > 0 && kwerenda2.exec() && kwerenda3.exec())
	{
		QSqlDatabase::database().commit(); // Zatwierdzenie zmian
		qDebug() << "Pomyœlnie wypozyczono!";
		return true;
	}
	else
	{
		QSqlDatabase::database().rollback(); // W razie erroru wycofanie
		qDebug() << "B³ad wypo¿yczenia ksi¹¿ki: " << kwerenda1.lastError().text() << kwerenda2.lastError().text() << kwerenda3.lastError().text();
		return false;
	}

}

bool Baza::dodajZwrot(int id_u, int id_k)
{
	QSqlDatabase::database().transaction();

	QSqlQuery kwerenda1, kwerenda2, kwerenda3;

	kwerenda1.prepare("UPDATE Wypozyczenia SET Status = 'Zwrócona' WHERE Id_uzyt = :id_u AND Id_ksiazki = :id_k and Status = 'Wypo¿yczona'");
	kwerenda1.bindValue(":id_u", id_u);
	kwerenda1.bindValue(":id_k", id_k);

	kwerenda2.prepare("INSERT INTO Zwrot(id_uzyt, Id_ksiazki, DataOddania)"
		"VALUES(:id_u, :id_k, :odd);");
	kwerenda2.bindValue(":id_u", id_u);
	kwerenda2.bindValue(":id_k", id_k);
	kwerenda2.bindValue(":odd", QDate::currentDate());

	kwerenda3.prepare("INSERT INTO Historia(Id_uzyt, Id_ksiazki, Akcja, Data) "
		"VALUES(:id_u, :id_k, 'Zwrot ksi¹¿ki', :data);");
	kwerenda3.bindValue(":id_u", id_u);
	kwerenda3.bindValue(":id_k", id_k);
	kwerenda3.bindValue(":data", QDate::currentDate());

	if (kwerenda1.exec() && kwerenda1.numRowsAffected() > 0 && kwerenda2.exec() && kwerenda3.exec())
	{
		QSqlDatabase::database().commit(); // Zatwierdzenie zmian
		qDebug() << "Pomyœlnie zwrócono!";
		return true;
	}
	else
	{
		QSqlDatabase::database().rollback(); // W razie erroru wycofanie
		qDebug() << "B³ad zwrotu ksi¹¿ki: " << kwerenda1.lastError().text() << kwerenda2.lastError().text() << kwerenda3.lastError().text();
		return false;
	}
}

QList<QString> Baza::pobierzZwroty()
{
	QSqlQuery kwerenda;
	QList<QString> zwroty;

	if (!kwerenda.exec("SELECT z.Id_zwrot, u.Imie, u.Nazwisko, k.Nazwa, z.DataOddania FROM Zwrot z JOIN Uzytkownicy u on u.Id_dane = z.Id_uzyt join "
		"BazaKsiazek k on k.Id_ksiazki = z.Id_Ksiazki")) {
		qDebug() << "B³¹d pobierania zwrotów: " << kwerenda.lastError().text();
		return zwroty;
	}

	while (kwerenda.next())
	{
		int id_z = kwerenda.value("Id_zwrot").toInt();
		/*int id_u = kwerenda.value("Id_uzyt").toInt();
		int id_k = kwerenda.value("Id_ksiazki").toInt();*/
		QString im = kwerenda.value("Imie").toString();
		QString naz = kwerenda.value("Nazwisko").toString();
		QString ksi = kwerenda.value("Nazwa").toString();
		QString odd = kwerenda.value("DataOddania").toString();

		/*DaneUzytkownika temp0 = pobierzKonto(id_u);
		DaneKsiazki temp1 = pobierzKsiazke(id_k);*/
		
		QString temp = QString("%1. U¿ytkownik %2 %3 zwróci³: %4 w dniu %5").arg(id_z).arg(im).arg(naz).arg(ksi).arg(odd);
		zwroty.append(temp);
	}

	return zwroty;
}

bool Baza::akceptujZwrot(int id_z, int id_k)
{
	QSqlDatabase::database().transaction();

	QSqlQuery kwerenda1, kwerenda2;

	kwerenda1.prepare("DELETE FROM Zwrot WHERE Id_zwrot =:id");
	kwerenda1.bindValue(":id", id_z);

	kwerenda2.prepare("UPDATE BazaKsiazek SET IloscDostepna = IloscDostepna + 1 WHERE Id_ksiazki = :id");
	kwerenda2.bindValue(":id", id_k);

	if (kwerenda1.exec() && kwerenda1.numRowsAffected() > 0 && kwerenda2.exec())
	{
		QSqlDatabase::database().commit();
		qDebug() << "Pomyœlnie przyjêto!";
		return true;
	}
	else
	{
		QSqlDatabase::database().rollback();
		qDebug() << "B³ad przyjêcia ksi¹¿ki: " << kwerenda1.lastError().text() << kwerenda2.lastError().text();
		return false;
	}
}
