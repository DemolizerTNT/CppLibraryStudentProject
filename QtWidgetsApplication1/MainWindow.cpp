#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPushButton>
#include <QDate>
#include "Baza.h"
#include "Bibliotekarz.h"
#include "Uzytkownik.h"

#include <QStringList>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
#include <QDialog>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QProgressBar>
#include <QMap>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// Dodanie użytkownika
	connect(ui.addUserBtn, &QPushButton::clicked, this, &MainWindow::addUserBtn);

	// Btn do wylogowania się
	connect(ui.logOut, &QPushButton::clicked, this, &MainWindow::wylogujSie);

	ui.stackedWidget->setCurrentWidget(ui.addUserData);

	// Podpięcie guzików panelu bocznego
	connect(ui.AllAccounts, &QPushButton::clicked, this, &MainWindow::allAccountsBtn);
	connect(ui.allBorrows, &QPushButton::clicked, this, &MainWindow::allBorrowsBtn);
	connect(ui.allHistory, &QPushButton::clicked, this, &MainWindow::allHistoryBtn);
	connect(ui.books, &QPushButton::clicked, this, &MainWindow::booksBtn);
	connect(ui.borrows, &QPushButton::clicked, this, &MainWindow::borrowsBtn);
	connect(ui.history, &QPushButton::clicked, this, &MainWindow::historyBtn);
	connect(ui.library, &QPushButton::clicked, this, &MainWindow::libraryBtn);
	connect(ui.myAccount, &QPushButton::clicked, this, &MainWindow::myAccountBtn);
	connect(ui.returns, &QPushButton::clicked, this, &MainWindow::returnsBtn);

	connect(ui.addNewBookBtn, &QPushButton::clicked, this, &MainWindow::addBookDataBtn);
	connect(ui.addNewUserBtn, &QPushButton::clicked, this, &MainWindow::addUserDataBtn);

	connect(ui.editAllBooksBtn, &QPushButton::clicked, this, &MainWindow::editAllBooksBtnClicked);
	connect(ui.editAllAccountsBtn, &QPushButton::clicked, this, &MainWindow::editAllAccountsBtnClicked);
	connect(ui.myEditBtn, &QPushButton::clicked, this, &MainWindow::editMyAccountBtnClicked);

	// Guziki w panelach
	// allAccountsPanel
	connect(ui.allAccountsList, &QTableWidget::cellDoubleClicked, this, &MainWindow::allAccountListDblClicked);
	connect(ui.editUserBtn_2, &QPushButton::clicked, this, &MainWindow::saveEditedUserData);
	connect(ui.closeAllAccountsBtn, &QPushButton::clicked, this, &MainWindow::closeEditUserPanel);
	/*connect(ui.deactiveAllAccountsBtn, &QPushButton::clicked, this, &MainWindow::openDeactivateUserPanel);
	connect(ui.deactiveAllAccountsDeclineBtn, &QPushButton::clicked, this, &MainWindow::closeDeactivateUserPanel);
	connect(ui.deactiveAllAccountsAcceptBtn, &QPushButton::clicked, this, &MainWindow::deactivateUser);*/

	// allHistory
	connect(ui.allHistorySpecificUserBtn, &QPushButton::clicked, this, &MainWindow::allHistorySpecificBtn);
	// addBookData
	connect(ui.addBookBtn, &QPushButton::clicked, this, &MainWindow::addBookDataNew);
	connect(ui.addNewBookFilterBtn, &QPushButton::clicked, this, &MainWindow::booksFilter);

	// library
	connect(ui.allLibraryListFilterBtn, &QPushButton::clicked, this, &MainWindow::libraryFilter);
	connect(ui.allLibraryList, &QTableWidget::cellDoubleClicked, this, &MainWindow::libraryBorrowsData);
	connect(ui.allLibraryCloseBtn, &QPushButton::clicked, this, &MainWindow::libraryBorrowsCloseBtn);
	connect(ui.allLibraryOrderBtn, &QPushButton::clicked, this, &MainWindow::libraryBorrowsConfirm);

	// books
	connect(ui.allBooksList, &QTableWidget::cellDoubleClicked, this, &MainWindow::booksData);
	connect(ui.allBooksEditClose, &QPushButton::clicked, this, &MainWindow::closeEditBook);
	//connect(ui.historyFilterBtn, &QPushButton::clicked, this, &MainWindow::booksFiltler);
	connect(ui.booksStatistics, &QPushButton::clicked, this, &MainWindow::booksStatisticsBtn);
	// editBookData
	connect(ui.editBookBtn, &QPushButton::clicked, this, &MainWindow::editBookDateBtn);
	
	//allBorrowsPanel
	connect(ui.allBorrowListSpecificUserBtn, &QPushButton::clicked, this, &MainWindow::allBorrowsSpecificBtn);
	connect(ui.closeBorrowListBtn, &QPushButton::clicked, this, &MainWindow::borrowsReturnsCloseBtn);
	connect(ui.borrowList, &QTableWidget::cellDoubleClicked, this, &MainWindow::borrowsReturns);
	connect(ui.returnBorrowListBtn, &QPushButton::clicked, this, &MainWindow::borrowsReturnsAccept);

	//returns
	connect(ui.returnsList, &QTableWidget::cellDoubleClicked, this, &MainWindow::returnsPanel);
	connect(ui.returnListCloseBtn, &QPushButton::clicked, this, &MainWindow::returnsPanelCloseBtn);
	connect(ui.returnListAcceptBtn, &QPushButton::clicked, this, &MainWindow::borrowsReturnsAcceptBtn);


	//myData
	connect(ui.saveMyBtn, &QPushButton::clicked, this, &MainWindow::updateData);
	
	

	

	// Panele Frame, wyskakujące z errorami
	ui.errPanelAddUser->hide();
}

MainWindow::~MainWindow()
{
}

void MainWindow::userBtns()
{
	ui.borrows->show();
	ui.library->show();
	ui.history->show();
	ui.myAccount->show();

	ui.allBorrows->hide();
	ui.books->hide();
	ui.allHistory->hide();
	ui.returns->hide();
	ui.AllAccounts->hide();
}

void MainWindow::librarianBtns()
{
	ui.borrows->hide();
	ui.library->hide();
	ui.history->hide();
	ui.myAccount->hide();

	ui.allBorrows->show();
	ui.books->show();
	ui.allHistory->show();
	ui.returns->show();
	ui.AllAccounts->show();
}

void MainWindow::wylogujSie()
{
	emit wylogowano();
}


void MainWindow::addUserBtn()
{
	DaneUzytkownika noweDane;
	noweDane.imie = ui.addName->text();
	noweDane.nazwisko = ui.addLast->text();
	noweDane.email = ui.addEmail->text();
	noweDane.haslo = ui.addPassword->text();
	QString pass = ui.addPassword2->text();
	noweDane.rola = ui.addRole->currentText();
	noweDane.dodanie = QDate::currentDate();

	if (pass != noweDane.haslo || ui.addName->text().isEmpty() || ui.addLast->text().isEmpty() || ui.addEmail->text().isEmpty() || ui.addPassword->text().isEmpty() || ui.addPassword2->text().isEmpty())
	{
		ui.errPanelAddUser->show();
	}
	else
	{
		ui.errPanelAddUser->hide();
		Bibliotekarz bib;

		if (bib.stworzKonto(noweDane))
		{
			qDebug() << "Konto dodany";
		}
		else
		{
			qDebug() << "Nie dodano konta";
		}

		ui.addName->clear();
		ui.addLast->clear();
		ui.addEmail->clear();
		ui.addPassword->clear();
		ui.addPassword2->clear();

	}

}

void MainWindow::allAccountsBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.AllAccountsPanel);
	//ui.deactiveAllAccountsFrame->hide();
	ui.editAllAccountsPanel->hide();

	Bibliotekarz bib;
	QList<DaneUzytkownika> listaUzytkownikow = bib.kontaInformacje();
	//db.pobierzKonta();
	ui.allAccountsList->clearContents();
	ui.allAccountsList->setRowCount(0);

	ui.allAccountsList->setColumnCount(7);
	ui.allAccountsList->setHorizontalHeaderLabels({ "Id_u", "Nazwa", "Email", "Hasło", "Rola", "Dodano", "Aktywne" });

	//ui.allAccountsList->setColumnHidden(0, true);
	//ui.allAccountsList->setColumnHidden(3, true);
	ui.allAccountsList->setColumnHidden(3, true);
	ui.allAccountsList->setEditTriggers(QAbstractItemView::NoEditTriggers);


	for (int i = 0; i < listaUzytkownikow.size(); i++)
	{
		const DaneUzytkownika& user = listaUzytkownikow[i];

		int row = ui.allAccountsList->rowCount();
		ui.allAccountsList->insertRow(row);
		ui.allAccountsList->setItem(row, 0, new QTableWidgetItem(QString::number(user.id_u)));
		QTableWidgetItem* eleName = new QTableWidgetItem(user.imie + " " + user.nazwisko);
		eleName->setToolTip(user.imie + " " + user.nazwisko);
		ui.allAccountsList->setItem(row, 1, eleName);
		//ui.allAccountsList->setItem(row, 1, new QTableWidgetItem(user.imie + " "+ user.nazwisko));
		QTableWidgetItem* eleEmail = new QTableWidgetItem(user.email);
		eleEmail->setToolTip(user.email);
		ui.allAccountsList->setItem(row, 2, eleEmail);
		ui.allAccountsList->setItem(row, 3, new QTableWidgetItem(user.haslo));
		ui.allAccountsList->setItem(row, 4, new QTableWidgetItem(user.rola));
		ui.allAccountsList->setItem(row, 5, new QTableWidgetItem(user.dodanie.toString("dd-MM-yyyy")));
		QString act = "Aktywne";
		if (user.aktywne == 0) act = "Nieaktywne";
		ui.allAccountsList->setItem(row, 6, new QTableWidgetItem(act));
	}
	ui.allAccountsList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::allBorrowsBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.allBorrowsPanel);

	Bibliotekarz bib;

	QList<DaneWypozyczen> dane = bib.wypoLista(0, true);

	ui.allBorrowList->clearContents();
	ui.allBorrowList->setRowCount(0);
	ui.allBorrowList->setColumnCount(6);
	ui.allBorrowList->setHorizontalHeaderLabels({ "Id_w", "Id_u", "Id_k", "Wypożyczona", "Termin" , "Status"});

	ui.allBorrowList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < dane.size(); i++)
	{
		const DaneWypozyczen& wypo = dane[i];


		int row = ui.allBorrowList->rowCount();
		ui.allBorrowList->insertRow(row);
		ui.allBorrowList->setItem(row, 0, new QTableWidgetItem(QString::number(wypo.id_w)));
		ui.allBorrowList->setItem(row, 1, new QTableWidgetItem(QString::number(wypo.id_u)));
		ui.allBorrowList->setItem(row, 2, new QTableWidgetItem(QString::number(wypo.id_k)));
		ui.allBorrowList->setItem(row, 3, new QTableWidgetItem(wypo.wypoz.toString("dd-MM-yyyy")));
		ui.allBorrowList->setItem(row, 4, new QTableWidgetItem(wypo.terOdd.toString("dd-MM-yyyy")));
		QString status = wypo.Status;
		if (QDate::currentDate() > wypo.terOdd) status = "Zaległe!";
		ui.allBorrowList->setItem(row, 5, new QTableWidgetItem(status));
	}
	ui.allBorrowList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::allHistoryBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.allHistoryPanel);
	Bibliotekarz bib;

	QList<DaneHistorii> dane = bib.histoLista(0, true);

	ui.allHistoryList->clearContents();
	ui.allHistoryList->setRowCount(0);
	ui.allHistoryList->setColumnCount(7);
	ui.allHistoryList->setHorizontalHeaderLabels({ "Id_h", "Id_u", "Użytkownik", "Id_k", "Książka", "Akcja", "Data" });

	ui.allHistoryList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < dane.size(); i++)
	{
		const DaneHistorii& hist = dane[i];

		int row = ui.allHistoryList->rowCount();
		ui.allHistoryList->insertRow(row);
		ui.allHistoryList->setItem(row, 0, new QTableWidgetItem(QString::number(hist.id_h)));
		ui.allHistoryList->setItem(row, 1, new QTableWidgetItem(QString::number(hist.id_u)));
		QTableWidgetItem* eleName = new QTableWidgetItem(hist.im + " " + hist.naz);
		eleName->setToolTip(hist.im + " " + hist.naz);
		ui.allHistoryList->setItem(row, 2, eleName);
		//ui.allHistoryList->setItem(row, 2, new QTableWidgetItem(hist.im + " " + hist.naz));
		ui.allHistoryList->setItem(row, 3, new QTableWidgetItem(QString::number(hist.id_k)));
		QTableWidgetItem* eleTitle = new QTableWidgetItem(hist.nazwa);
		eleTitle->setToolTip(hist.nazwa);
		ui.allHistoryList->setItem(row, 4, eleTitle);
		//ui.allHistoryList->setItem(row, 4, new QTableWidgetItem(hist.nazwa));
		QTableWidgetItem* eleAction = new QTableWidgetItem(hist.akcja);
		eleAction->setToolTip(hist.akcja);
		ui.allHistoryList->setItem(row, 5, eleAction);
		//ui.allHistoryList->setItem(row, 5, new QTableWidgetItem(hist.akcja));
		ui.allHistoryList->setItem(row, 6, new QTableWidgetItem(hist.data.toString("dd-MM-yyyy")));
	}
	ui.allHistoryList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::allHistorySpecificBtn()
{
	//ui.stackedWidget->setCurrentWidget(ui.allHistoryPanel);
	Bibliotekarz bib;
	QList<DaneHistorii> dane = bib.histoLista(ui.allHistorySpecificUserNumer->value(), true);

	ui.allHistoryList->clearContents();
	ui.allHistoryList->setRowCount(0);
	ui.allHistoryList->setColumnCount(7);
	ui.allHistoryList->setHorizontalHeaderLabels({ "Id_h", "Id_u", "Użytkownik", "Id_k", "Książka", "Akcja", "Data" });

	ui.allHistoryList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < dane.size(); i++)
	{
		const DaneHistorii& hist = dane[i];


		int row = ui.allHistoryList->rowCount();
		ui.allHistoryList->insertRow(row);
		ui.allHistoryList->setItem(row, 0, new QTableWidgetItem(QString::number(hist.id_h)));
		ui.allHistoryList->setItem(row, 1, new QTableWidgetItem(QString::number(hist.id_u)));
		ui.allHistoryList->setItem(row, 2, new QTableWidgetItem(hist.im + " " + hist.naz));
		ui.allHistoryList->setItem(row, 3, new QTableWidgetItem(QString::number(hist.id_k)));
		ui.allHistoryList->setItem(row, 4, new QTableWidgetItem(hist.nazwa));
		ui.allHistoryList->setItem(row, 5, new QTableWidgetItem(hist.akcja));
		ui.allHistoryList->setItem(row, 6, new QTableWidgetItem(hist.data.toString("dd-MM-yyyy")));
	}
	ui.allHistoryList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::addBookDataNew()
{
	DaneKsiazki dane;

	dane.nazwa = ui.addBookName->text();
	dane.autor = ui.addBookAuthor->text();
	dane.ocena = ui.addBookReview->text().toFloat();
	dane.kategoria = ui.addBookCategory->currentText();
	dane.opis = ui.addBookDesc->toPlainText();
	dane.ilosc_dostepna = ui.accBookAmount->text().toInt();
	dane.ilosc_ogolna = ui.accBookAmount->text().toInt();

	if (dane.nazwa == "" || dane.autor == "" || dane.kategoria == "-Wybierz-" || dane.opis == "")
	{
		ui.addBookError->show();
		qDebug() << "Błąd wprowadzenia nowej książki";
	}
	else
	{
		ui.addBookError->hide();
		Bibliotekarz bib;
		bib.stworzKsiazke(dane);
		addBookDataBtn();		

		ui.addBookName->clear();
		ui.addBookAuthor->clear();
		ui.addBookReview->clear();
		ui.addBookDesc->clear();
		ui.accBookAmount->setValue(0);
	}
}

void MainWindow::closeEditBook()
{
	ui.allBooksPanel->hide();
}

void MainWindow::editBookDateBtn()
{
	Baza db;
	DaneKsiazki dane;
	//= db.pobierzKsiazke(bookActiveId);


	dane.nazwa = ui.editBookName->text();
	dane.autor = ui.editBookAuthor->text();
	dane.ocena = ui.editBookReview->text().toFloat();
	dane.kategoria = ui.editBookCategory->currentText();
	dane.opis = ui.editBookDesc->toPlainText();
	dane.ilosc_dostepna = ui.editBookAvailable->text().toInt();
	dane.ilosc_ogolna = ui.editBookAmount->text().toInt();

	if (dane.nazwa == "" || dane.autor == "" || dane.ocena < 0 || dane.ocena >10 || dane.kategoria == "-Wybierz" || dane.opis == "" || (dane.ilosc_dostepna > dane.ilosc_ogolna))
	{
		ui.errPanelEditBook->show();
		qDebug() << "Błąd edycji danych książki";
	}
	else
	{
		ui.errPanelEditBook->hide();
		Bibliotekarz bib;
		bib.zmodyfikujKsiazke(bookActiveId, dane);
	}


}

void MainWindow::allBorrowsSpecificBtn()
{
	//ui.stackedWidget->setCurrentWidget(ui.allBorrowsPanel);
	Bibliotekarz bib;
	QList<DaneWypozyczen> dane = bib.wypoLista(ui.allBorrowListSpecificUser->value(), true);

	ui.allBorrowList->clearContents();
	ui.allBorrowList->setRowCount(0);
	ui.allBorrowList->setColumnCount(6);
	ui.allBorrowList->setHorizontalHeaderLabels({ "Id_w", "Id_u", "Id_k", "Wypożyczona", "Termin" , "Status" });

	ui.allBorrowList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < dane.size(); i++)
	{
		const DaneWypozyczen& wypo = dane[i];


		int row = ui.allBorrowList->rowCount();
		ui.allBorrowList->insertRow(row);
		ui.allBorrowList->setItem(row, 0, new QTableWidgetItem(QString::number(wypo.id_w)));
		ui.allBorrowList->setItem(row, 1, new QTableWidgetItem(QString::number(wypo.id_u)));
		ui.allBorrowList->setItem(row, 2, new QTableWidgetItem(QString::number(wypo.id_k)));
		ui.allBorrowList->setItem(row, 3, new QTableWidgetItem(wypo.wypoz.toString("dd-MM-yyyy")));
		ui.allBorrowList->setItem(row, 4, new QTableWidgetItem(wypo.terOdd.toString("dd-MM-yyyy")));
		ui.allBorrowList->setItem(row, 5, new QTableWidgetItem(wypo.Status));
	}
	ui.allBorrowList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}


void MainWindow::booksStatisticsBtn()
{
	Bibliotekarz bib;
	QMap<QString, int> statystyka = bib.statystyki();

	if (statystyka.isEmpty())
	{
		qDebug() << "Brak danych do wyświetlenia";
		return;
	}

	QDialog* statsDialog = new QDialog(this);
	statsDialog->setWindowTitle("Statystyki wypożyczonych kategorii");
	statsDialog->resize(600, 450);

	// Kolory
	QStringList kolory = { "#e74c3c", "#3498db", "#2ecc71", "#f1c40f", "#9b59b6", "#e67e22", "#1abc9c", "#34495e" };

	int maxWartosc = 0;
	int sumaWypozyczen = 0;
	for (auto it = statystyka.begin(); it != statystyka.end(); ++it)
	{
		sumaWypozyczen += it.value();
		if (it.value() > maxWartosc)
		{
			maxWartosc = it.value();
		}
	}

	// Wspólny panel
	QWidget* topBarWidget = new QWidget(statsDialog);
	topBarWidget->setFixedHeight(35); 
	QHBoxLayout* topBarLayout = new QHBoxLayout(topBarWidget);
	topBarLayout->setContentsMargins(0, 0, 0, 0); 
	topBarLayout->setSpacing(0); 

	QTableWidget* table = new QTableWidget(statsDialog);
	table->setColumnCount(3);
	table->setHorizontalHeaderLabels({ "Kategoria", "Ilość", "Udział" });
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->setRowCount(statystyka.size());

	int row = 0;
	int colorIndex = 0;
	for (auto it = statystyka.begin(); it != statystyka.end(); ++it)
	{
		QString aktualnyKolor = kolory[colorIndex % kolory.size()];

		double procent = ((double)it.value() / sumaWypozyczen) * 100.0;
		QLabel* chunk = new QLabel();
		chunk->setStyleSheet(QString("background-color: %1; color: white; font-weight: bold;").arg(aktualnyKolor));
		chunk->setAlignment(Qt::AlignCenter);
		chunk->setToolTip(QString("%1: %2%").arg(it.key()).arg(QString::number(procent, 'f', 1)));

		if (procent > 5.0) {
			chunk->setText(QString::number(procent, 'f', 1) + "%");
		}

		topBarLayout->addWidget(chunk, it.value());
		QTableWidgetItem* catItem = new QTableWidgetItem(it.key());
		QPixmap pixmap(15, 15);
		pixmap.fill(QColor(aktualnyKolor));
		catItem->setIcon(QIcon(pixmap));
		table->setItem(row, 0, catItem);

		QTableWidgetItem* valItem = new QTableWidgetItem(QString::number(it.value()));
		valItem->setTextAlignment(Qt::AlignCenter);
		table->setItem(row, 1, valItem);

		QProgressBar* bar = new QProgressBar();
		bar->setRange(0, maxWartosc);
		bar->setValue(it.value());
		bar->setTextVisible(false);

		bar->setStyleSheet(QString("QProgressBar { border: 1px solid #bdc3c7; border-radius: 3px; background-color: #ecf0f1; }"
			"QProgressBar::chunk { background-color: %1; width: 10px; }").arg(aktualnyKolor));

		table->setCellWidget(row, 2, bar);

		row++;
		colorIndex++;
	}

	table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

	QVBoxLayout* layout = new QVBoxLayout(statsDialog);

	QLabel* titleLabel = new QLabel("<b>Całkowity udział wypożyczeń:</b>");
	layout->addWidget(titleLabel);
	layout->addWidget(topBarWidget);
	layout->addSpacing(10); 
	layout->addWidget(table);

	statsDialog->setLayout(layout);

	statsDialog->exec();
	delete statsDialog;
}

void MainWindow::booksBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.booksPanel);
	ui.addNewBookFilter->clear();
	//ui.allBooksFrame->hide();
	ui.allBooksPanel->hide();

	Baza db;
	QList<DaneKsiazki> listaKsiazek = db.pobierzBiblioteke();
	ui.allBooksList->clearContents();
	ui.allBooksList->setRowCount(0);

	ui.allBooksList->setColumnCount(7);
	ui.allBooksList->setHorizontalHeaderLabels({ "Id_k", "Nazwa", "Autor", "Ocena", "Kategoria", "Pożyczona", "Ilosc" });

	//ui.allAccountsList->setColumnHidden(0, true);
	//ui.allAccountsList->setColumnHidden(3, true);
	//ui.allBooksList->setColumnHidden(4, true);
	ui.allBooksList->setEditTriggers(QAbstractItemView::NoEditTriggers);


	for (int i = 0; i < listaKsiazek.size(); i++)
	{
		const DaneKsiazki& book = listaKsiazek[i];

		int row = ui.allBooksList->rowCount();
		ui.allBooksList->insertRow(row);
		ui.allBooksList->setItem(row, 0, new QTableWidgetItem(QString::number(book.id_k)));
		QTableWidgetItem* listaKsiNazwa = new QTableWidgetItem(book.nazwa);
		listaKsiNazwa->setToolTip(book.nazwa);
		ui.allBooksList->setItem(row, 1, listaKsiNazwa);
		//ui.allBooksList->setItem(row, 1, new QTableWidgetItem(book.nazwa));
		QTableWidgetItem* listaKsiAutor = new QTableWidgetItem(book.autor);
		listaKsiAutor->setToolTip(book.autor);
		ui.allBooksList->setItem(row, 2, listaKsiAutor);
		//ui.allBooksList->setItem(row, 2, new QTableWidgetItem(book.autor));
		ui.allBooksList->setItem(row, 3, new QTableWidgetItem(QString::number(book.ocena)));
		QTableWidgetItem* listaKsiKategoria = new QTableWidgetItem(book.kategoria);
		listaKsiKategoria->setToolTip(book.kategoria);
		ui.allBooksList->setItem(row, 4, listaKsiKategoria);
		//ui.allBooksList->setItem(row, 4, new QTableWidgetItem(book.kategoria));
		ui.allBooksList->setItem(row, 5, new QTableWidgetItem(QString::number(book.wypozyczenia)));
		ui.allBooksList->setItem(row, 6, new QTableWidgetItem(QString::number(book.ilosc_dostepna)+"/"+QString::number(book.ilosc_ogolna)));
	}
	ui.allBooksList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::borrowsBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.borrowsPanel);
	ui.borrowListPanel->hide();


	Uzytkownik user;
	QList<DaneWypozyczen> listaWypozyczen = user.wypozyczLista(activeId, false);
	ui.borrowList->setRowCount(0);

	ui.borrowList->setColumnCount(7);
	ui.borrowList->setHorizontalHeaderLabels({ "Id_w", "Id_u", "Id_k", "Nazwa", "Wypożyczona", "Termin", "Status",});

	ui.borrowList->setColumnHidden(1, true);
	//ui.allAccountsList->setColumnHidden(3, true);
	//ui.allBooksList->setColumnHidden(4, true);
	ui.borrowList->setEditTriggers(QAbstractItemView::NoEditTriggers);


	for (int i = 0; i < listaWypozyczen.size(); i++)
	{
		const DaneWypozyczen& wypo = listaWypozyczen[i];

		int row = ui.borrowList->rowCount();
		ui.borrowList->insertRow(row);
		ui.borrowList->setItem(row, 0, new QTableWidgetItem(QString::number(wypo.id_w)));
		ui.borrowList->setItem(row, 1, new QTableWidgetItem(QString::number(wypo.id_u)));
		ui.borrowList->setItem(row, 2, new QTableWidgetItem(QString::number(wypo.id_k)));
		QTableWidgetItem* listaWypoNazwa = new QTableWidgetItem(wypo.nazwa);
		listaWypoNazwa->setToolTip(wypo.nazwa);
		ui.borrowList->setItem(row, 3, listaWypoNazwa);
		ui.borrowList->setItem(row, 4, new QTableWidgetItem(wypo.wypoz.toString("dd-MM-yyyy")));
		ui.borrowList->setItem(row, 5, new QTableWidgetItem(wypo.terOdd.toString("dd-MM-yyyy")));
		QString status = wypo.Status;
		if (QDate::currentDate() > wypo.terOdd) status = "Zaległe!";
		ui.borrowList->setItem(row, 6, new QTableWidgetItem(status));
	}
	ui.borrowList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::borrowsReturns(int row, int column)
{
	int id_w = ui.borrowList->item(row, 0)->text().toInt();
	borrowActiveId = id_w;
	Baza db;
	DaneWypozyczen wypo = db.pobierzWypozyczenie(id_w);

	ui.borrowListPanel->show();
	ui.borrowListTitle->setText("Identyfikator: " + QString::number(wypo.id_w) + " --- " + wypo.nazwa);

	bookActiveId = wypo.id_k;
}

void MainWindow::borrowsReturnsAccept()
{
	Uzytkownik user;

	if (!user.dodZwrot(activeId,bookActiveId, borrowActiveId))
	{
		qDebug() << "Błąd zwrotu!";
	}
	else
	{
		qDebug() << "Zwrócono!";
		borrowsReturnsCloseBtn();
		borrowsBtn();
	}

	}


	



void MainWindow::borrowsReturnsCloseBtn()
{
	ui.borrowListPanel->hide();
}

void MainWindow::historyBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.historyPanel);
	Uzytkownik uzy;
	QList<DaneHistorii> dane = uzy.historiaLista(activeId);

	ui.historyList->clearContents();
	ui.historyList->setRowCount(0);
	ui.historyList->setColumnCount(7);
	ui.historyList->setHorizontalHeaderLabels({ "Id_h", "Id_u", "Użytkownik", "Id_k", "Książka", "Akcja", "Data" });

	ui.historyList->setColumnHidden(1, true);
	ui.historyList->setColumnHidden(2, true);


	ui.historyList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < dane.size(); i++)
	{
		const DaneHistorii& hist = dane[i];

		int row = ui.historyList->rowCount();
		ui.historyList->insertRow(row);
		ui.historyList->setItem(row, 0, new QTableWidgetItem(QString::number(hist.id_h)));
		ui.historyList->setItem(row, 1, new QTableWidgetItem(QString::number(hist.id_u)));
		ui.historyList->setItem(row, 2, new QTableWidgetItem(hist.im + " " + hist.naz));
		ui.historyList->setItem(row, 3, new QTableWidgetItem(QString::number(hist.id_k)));
		QTableWidgetItem* listaHistoriaNazwa = new QTableWidgetItem(hist.nazwa);
		listaHistoriaNazwa->setToolTip(hist.nazwa);
		ui.historyList->setItem(row, 4, listaHistoriaNazwa);
		//ui.historyList->setItem(row, 4, new QTableWidgetItem(hist.nazwa));
		QTableWidgetItem* listaHistoriaAkcja = new QTableWidgetItem(hist.akcja);
		listaHistoriaAkcja->setToolTip(hist.akcja);
		ui.historyList->setItem(row, 5, listaHistoriaAkcja);
		//ui.historyList->setItem(row, 5, new QTableWidgetItem(hist.akcja));
		ui.historyList->setItem(row, 6, new QTableWidgetItem(hist.data.toString("dd-MM-yyyy")));
	}
	ui.historyList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::libraryBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.libraryPanel);
	ui.allLibraryListFilter->clear();
	ui.allLibraryPanel->hide();
	//allLibraryList

	Baza db;
	QList<DaneKsiazki> listaKsiazek = db.pobierzBiblioteke();
	ui.allLibraryList->clearContents();
	ui.allLibraryList->setRowCount(0);

	ui.allLibraryList->setColumnCount(7);
	ui.allLibraryList->setHorizontalHeaderLabels({ "Id_b", "Nazwa", "Autor", "Ocena", "Kategoria", "Pożyczona", "Ilosc" });

	//ui.allAccountsList->setColumnHidden(0, true);
	//ui.allAccountsList->setColumnHidden(3, true);
	//ui.allBooksList->setColumnHidden(4, true);
	ui.allLibraryList->setEditTriggers(QAbstractItemView::NoEditTriggers);


	for (int i = 0; i < listaKsiazek.size(); i++)
	{
		const DaneKsiazki& book = listaKsiazek[i];

		int row = ui.allLibraryList->rowCount();
		ui.allLibraryList->insertRow(row);
		ui.allLibraryList->setItem(row, 0, new QTableWidgetItem(QString::number(book.id_k)));
		QTableWidgetItem* listaKsiNazwa = new QTableWidgetItem(book.nazwa);
		listaKsiNazwa->setToolTip(book.nazwa);
		ui.allLibraryList->setItem(row, 1, listaKsiNazwa);
		//ui.allBooksList->setItem(row, 1, new QTableWidgetItem(book.nazwa));
		QTableWidgetItem* listaKsiAutor = new QTableWidgetItem(book.autor);
		listaKsiAutor->setToolTip(book.autor);
		ui.allLibraryList->setItem(row, 2, listaKsiAutor);
		//ui.allBooksList->setItem(row, 2, new QTableWidgetItem(book.autor));
		ui.allLibraryList->setItem(row, 3, new QTableWidgetItem(QString::number(book.ocena)));
		QTableWidgetItem* listaKsiKategoria = new QTableWidgetItem(book.kategoria);
		listaKsiKategoria->setToolTip(book.kategoria);
		ui.allLibraryList->setItem(row, 4, listaKsiKategoria);
		//ui.allBooksList->setItem(row, 4, new QTableWidgetItem(book.kategoria));
		ui.allLibraryList->setItem(row, 5, new QTableWidgetItem(QString::number(book.wypozyczenia)));
		ui.allLibraryList->setItem(row, 6, new QTableWidgetItem(QString::number(book.ilosc_dostepna) + "/" + QString::number(book.ilosc_ogolna)));
	}
	ui.allLibraryList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::myAccountBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.myAccountPanel);
	Baza db;
	DaneUzytkownika dane = db.pobierzKonto(activeId);
	ui.myId->setText(QString::number(dane.id_u));
	ui.myName->setText(dane.imie);
	ui.myLast->setText(dane.nazwisko);
	ui.myEmail->setText(dane.email);
	ui.myPassword->setText(dane.haslo);
	ui.myRole->setText(dane.rola);
	ui.myJoinData->setText(dane.dodanie.toString("dd-MM-yyyy"));
	QString act = "Aktywne";
	if (dane.aktywne == 0) act = "Nieaktywne";
	ui.myActive->setText(act);
}

void MainWindow::returnsBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.returnList);
	ui.returnListPanel->hide();

	Bibliotekarz bib;
	QList<DaneZwrotu> dane = bib.zwrotyLista();

	ui.returnsList->clearContents();
	ui.returnsList->setRowCount(0);

	ui.returnsList->setColumnCount(6);
	ui.returnsList->setHorizontalHeaderLabels({ "Id_z", "Id_u", "Użytkownik", "Id_k", "Tytuł", "Oddanie" });

	ui.returnsList->setEditTriggers(QAbstractItemView::NoEditTriggers);


	for (int i = 0; i < dane.size(); i++)
	{
		const DaneZwrotu& zwr = dane[i];

		int row = ui.returnsList->rowCount();
		ui.returnsList->insertRow(row);

		ui.returnsList->setItem(row, 0, new QTableWidgetItem(QString::number(zwr.id_z)));
		ui.returnsList->setItem(row, 1, new QTableWidgetItem(QString::number(zwr.id_u)));
		QTableWidgetItem* zwrotyNazwa = new QTableWidgetItem(zwr.im + " " + zwr.naz);
		zwrotyNazwa->setToolTip(zwr.im + " " + zwr.naz);
		ui.returnsList->setItem(row, 2, zwrotyNazwa);
		//ui.allAccountsList->setItem(row, 2, new QTableWidgetItem(zwr.im + " " + zwr.naz));
		ui.returnsList->setItem(row, 3, new QTableWidgetItem(QString::number(zwr.id_k)));
		QTableWidgetItem* zwrotyTytul = new QTableWidgetItem(zwr.nazwa);
		zwrotyTytul->setToolTip(zwr.nazwa);
		ui.returnsList->setItem(row, 4, zwrotyTytul);
		//ui.allAccountsList->setItem(row, 4, new QTableWidgetItem(zwr.nazwa));
		ui.returnsList->setItem(row, 5, new QTableWidgetItem(zwr.oddanie.toString("dd-MM-yyyy")));
	
	}
	ui.returnsList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::returnsPanel(int row, int column)
{
	int id_z = ui.returnsList->item(row, 0)->text().toInt();
	returnActiveId = id_z;

	Bibliotekarz bib;
	DaneZwrotu dane = bib.pobZwr(id_z);

	ui.returnListPanel->show();
	ui.returnListTitle->setText("Identyfikator: " + QString::number(dane.id_z) + " --- Użytkownik: " + dane.im + " " + dane.naz + "\nKsiążka: " + dane.nazwa);
}

void MainWindow::returnsPanelCloseBtn()
{
	ui.returnListPanel->hide();
}

void MainWindow::borrowsReturnsAcceptBtn()
{
	Bibliotekarz bib;
	DaneZwrotu dane = bib.pobZwr(returnActiveId);

	bib.akceptZwroty(dane.id_u, dane.id_z, dane.id_k);
	returnsBtn();
}


void MainWindow::addBookDataBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.addBookData);
	ui.addBookError->hide();
}

void MainWindow::addUserDataBtn()
{
	ui.stackedWidget->setCurrentWidget(ui.addUserData);
	ui.errPanelAddUser->hide();
}

void MainWindow::editBookDataBtn(const DaneKsiazki& dane)
{
	ui.stackedWidget->setCurrentWidget(ui.editBookData);
	ui.editBookName->setText(dane.nazwa);
	ui.editBookAuthor->setText(dane.autor);
	ui.editBookReview->setText(QString::number(dane.ocena));
	ui.editBookCategory->setCurrentText(dane.kategoria);
	ui.editBookDesc->setText(dane.opis);
	ui.editBookAvailable->setValue(dane.ilosc_dostepna);
	ui.editBookAmount->setValue(dane.ilosc_ogolna);
	bookActiveId = dane.id_k;
}

void MainWindow::editUserDataBtn(const DaneUzytkownika& dane)
{
	ui.errorEditUser->hide();
	ui.stackedWidget->setCurrentWidget(ui.editUserData);
	ui.editUserName->setText(dane.imie);
	ui.editUserLast->setText(dane.nazwisko);
	ui.editUserEmail->setText(dane.email);
	ui.editUserPassword->setText(dane.haslo);
	ui.editUserPassword2->setText(dane.haslo);
	ui.editUserRole->setCurrentText(dane.rola);
	QString act = "Aktywne";
	if (dane.aktywne == 0) act = "Nieaktywne";
	ui.editUserActive->setCurrentText(act);
	viewedId = dane.id_u;
}

void MainWindow::userDataBtn(const DaneUzytkownika& dane)
{
	ui.stackedWidget->setCurrentWidget(ui.userData);
	ui.editMyName->setText(dane.imie);
	ui.editMyLast->setText(dane.nazwisko);
	ui.editMyEmail->setText(dane.email);
	ui.editMyPassword->setText(dane.haslo);
	ui.editMyPassword2->setText(dane.haslo);
}

void MainWindow::saveEditedUserData()
{
	DaneUzytkownika dane;

	dane.imie = ui.editUserName->text();
	dane.nazwisko = ui.editUserLast->text();
	dane.email = ui.editUserEmail->text();
	dane.haslo = ui.editUserPassword->text();
	QString pass = ui.editUserPassword2->text();
	dane.rola = ui.editUserRole->currentText();
	dane.aktywne = 1;
	if (ui.editUserActive->currentText() == "Nieaktywne")
		dane.aktywne = 0;

	if (dane.imie == "" || dane.nazwisko == "" || dane.email == "" || dane.haslo == "" || pass == "" || dane.haslo != pass)
	{
		ui.errorEditUser->show();
	}
	else
	{
		Bibliotekarz bib;
		bib.zmodyfikujKonto(viewedId, dane);
		allAccountsBtn();
	}

}

void MainWindow::closeEditUserPanel()
{
	ui.editAllAccountsPanel->hide();
}

//void MainWindow::historyPanelShow()
//{
//
//}

//void MainWindow::openDeactivateUserPanel()
//{
//	
//	ui.deactiveAllAccountsFrame->show();
//}

void MainWindow::editAllBooksBtnClicked()
{
	ui.errPanelEditBook->hide();
	int currentRow = ui.allBooksList->currentRow();
	int id_k = ui.allBooksList->item(currentRow, 0)->text().toInt();
	Baza db;
	DaneKsiazki dane = db.pobierzKsiazke(id_k);

	editBookDataBtn(dane);
}

void MainWindow::editAllAccountsBtnClicked()
{
	int currentRow = ui.allAccountsList->currentRow();
	int id_u = ui.allAccountsList->item(currentRow, 0)->text().toInt();
	Baza db;
	DaneUzytkownika dane = db.pobierzKonto(id_u);

	editUserDataBtn(dane);
}

void MainWindow::editMyAccountBtnClicked()
{
	Baza db;
	DaneUzytkownika dane = db.pobierzKonto(activeId);

	userDataBtn(dane);
}

//void MainWindow::allAccountsPanel1(DaneUzytkownika dane)
//{
//	
//}

void MainWindow::allAccountListDblClicked(int row, int column)
{
	int id_u = ui.allAccountsList->item(row, 0)->text().toInt();
	Baza db;
	DaneUzytkownika dane = db.pobierzKonto(id_u);

	ui.editAllAccountsPanel->show();
	ui.allAccountsName->setText("Identyfikator: " + QString::number(dane.id_u) + " --- " + dane.imie + " " + dane.nazwisko);
}

//void MainWindow::closeDeactivateUserPanel()
//{
//	ui.deactiveAllAccountsFrame->hide();
//}
//
//void MainWindow::deactivateUser()
//{
//	Baza db;
//
//}

void MainWindow::booksFilter()
{
	const QString& text = ui.addNewBookFilter->text();

	for (int row = 0; row < ui.allBooksList->rowCount(); row++)
	{
		bool match = false;

		QTableWidgetItem* nameItem = ui.allBooksList->item(row, 1); //  Nazwa
		QTableWidgetItem* authorItem = ui.allBooksList->item(row, 2); // Autor
		QTableWidgetItem* categoryItem = ui.allBooksList->item(row, 4); // Kategoria

		QString name = nameItem ? nameItem->text() : "";
		QString author = authorItem ? authorItem->text() : "";
		QString category = categoryItem ? categoryItem->text() : "";

		if (name.contains(text, Qt::CaseInsensitive) || //Insensitive - wielkość obojętna
			author.contains(text, Qt::CaseInsensitive) ||
			category.contains(text, Qt::CaseInsensitive))
		{
			match = true;
		}

		ui.allBooksList->setRowHidden(row, !match); // ukrywamy niepotrzebne
	}
}

void MainWindow::libraryFilter()
{
	const QString& text = ui.allLibraryListFilter->text();

	for (int row = 0; row < ui.allLibraryList->rowCount(); row++)
	{
		bool match = false;

		QTableWidgetItem* nameItem = ui.allLibraryList->item(row, 1); //  Nazwa
		QTableWidgetItem* authorItem = ui.allLibraryList->item(row, 2); // Autor
		QTableWidgetItem* categoryItem = ui.allLibraryList->item(row, 4); // Kategoria

		QString name = nameItem ? nameItem->text() : "";
		QString author = authorItem ? authorItem->text() : "";
		QString category = categoryItem ? categoryItem->text() : "";

		if (name.contains(text, Qt::CaseInsensitive) || //Insensitive - wielkość obojętna
			author.contains(text, Qt::CaseInsensitive) ||
			category.contains(text, Qt::CaseInsensitive))
		{
			match = true;
		}

		ui.allLibraryList->setRowHidden(row, !match); // ukrywamy niepotrzebne
	}
}

void MainWindow::libraryBorrowsData(int row, int column)
{
	ui.allLibraryPanel->show();


	bookActiveId = ui.allLibraryList->item(row, 0)->text().toInt();
	
	Baza db;
	DaneKsiazki dane = db.pobierzKsiazke(bookActiveId);

	ui.allLibraryName->setText("Identyfikator: " + QString::number(dane.id_k) + " --- " + dane.nazwa);
	ui.allLibraryDesc->setText(dane.opis);
}

void MainWindow::libraryBorrowsCloseBtn()
{
	ui.allLibraryPanel->hide();
}

void MainWindow::libraryBorrowsConfirm()
{
	Baza db;

	db.wypozyczKsiazke(activeId, bookActiveId);
	libraryBorrowsCloseBtn(); 
	libraryBtn();
}

void MainWindow::booksData(int row, int column)
{
	int id_k = ui.allBooksList->item(row, 0)->text().toInt();
	Baza db;
	DaneKsiazki dane = db.pobierzKsiazke(id_k);
	ui.allBooksPanel->show();
	ui.allBooksName_2->setText("Identyfikator: " + QString::number(dane.id_k) + " --- " + dane.nazwa);
	ui.editAllBooksDesc->setText(dane.opis);
}


void MainWindow::updateData()
{
	Baza db;
	DaneUzytkownika noweDane = db.pobierzKonto(activeId);
	
	noweDane.imie = ui.editMyName->text();
	noweDane.nazwisko = ui.editMyLast->text();
	noweDane.email = ui.editMyEmail->text();

	
	if (ui.editMyPassword->text() != ui.editMyPassword2->text()) qDebug() << "Błędne hasło";
	else
	{
		noweDane.haslo = ui.editMyPassword->text();
	}

	Uzytkownik uzy(db, noweDane);

	if (uzy.edycjaKonta(noweDane))
	{
		qDebug() << "Udało się zaktualizować dane konta!";
	}
	else
	{
		qDebug() << "Błąd zapisu danych.";
	}

}

