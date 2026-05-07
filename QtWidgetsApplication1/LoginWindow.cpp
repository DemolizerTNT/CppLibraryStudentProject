#include "LoginWindow.h"
#include "Bibliotekarz.h"
//#include "Osoba.h"
#include "Uzytkownik.h"
#include "Baza.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.loginError->hide();

	WypiszListe();

	// Przepsiz email
	connect(ui.listWidget, &QListWidget::itemDoubleClicked, this, &LoginWindow::listaLoginDbClock);
	// Wypisz listę użytkowników aktywnych
	connect(ui.refrestUserList, &QPushButton::clicked, this, &LoginWindow::WypiszListe);

	// btn do logowania - loginBtn
	connect(ui.loginBtn, &QPushButton::clicked, this, &LoginWindow::zalogujSie);
}

LoginWindow::~LoginWindow()
{}

void LoginWindow::WypiszListe()
{
	ui.listWidget->clear();

	Baza db;
	QList<DaneUzytkownika> listaKont = db.pobierzKontaLogowania();
	int i = 1;
	for(const DaneUzytkownika& osoba : listaKont)
	{
		QString tekst = QString::number(i++) + ". " + osoba.imie + " " + osoba.nazwisko + " (" + osoba.rola + ")";

		QListWidgetItem* element = new QListWidgetItem(tekst);
		element->setData(Qt::UserRole, osoba.id_u); // przypięciue ukrytego id

		ui.listWidget->addItem(element);
		
	}
}

void LoginWindow::zalogujSie()
{
	Baza db;
	DaneUzytkownika dane;

	QString mail = ui.insertEmail->text();
	QString pass = ui.insertPassword->text();
	
	// funkcja zwróci id_użytkownika
	int id_u = db.zaloguj(mail, pass);

	if (id_u != -1)
	{
		dane = db.pobierzKonto(id_u);
		ui.insertEmail->clear();
		ui.insertPassword->clear();
		emit zalogowano(id_u, dane.imie, dane.nazwisko, dane.rola);
		ui.loginError->hide();
	}
	else
	{
		ui.insertPassword->clear();
		ui.loginError->show();
	}
	
}

void LoginWindow::listaLoginDbClock(QListWidgetItem* element)
{
	Baza db;
	int idU = element->data(Qt::UserRole).toInt();

	DaneUzytkownika dane = db.pobierzKonto(idU);
	QString mail = dane.email;

	ui.insertEmail->setText(mail);


}
