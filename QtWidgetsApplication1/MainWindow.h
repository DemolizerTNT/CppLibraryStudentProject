#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "Bibliotekarz.h"
//#include "Osoba.h"
#include "Uzytkownik.h"
//#include "Baza.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	// Widoki
	void userBtns();
	void librarianBtns();

	// Obs³uga guzików na panelu g³ównym
	void allAccountsBtn();
	void allBorrowsBtn();
	void allHistoryBtn();
	void booksBtn();
	void borrowsBtn();
	void historyBtn();
	void libraryBtn();
	void myAccountBtn();
	void returnsBtn();

	// Obs³uga guzików pod konkretne, wewnêtrznie odpalanie panele
	void addBookDataBtn();
	void addUserDataBtn();
	void editBookDataBtn(const DaneKsiazki& dane);
	void editUserDataBtn(const DaneUzytkownika& dane);
	void userDataBtn(const DaneUzytkownika& dane);

	// wyskakuj¹ce guziki do akceptacji itd - do dodania
	//void allAccountsPanel1(DaneUzytkownika dane);
	void saveEditedUserData();
	void closeEditUserPanel();
	/*void openDeactivateUserPanel();
	void closeDeactivateUserPanel();
	void deactivateUser();*/
	// allHistory
	void allHistorySpecificBtn();

	// addBookData
	void addBookDataNew();
	void closeEditBook();

	// editBookData
	void editBookDateBtn();

	// allBorrows
	void allBorrowsSpecificBtn();

	// books
	void booksStatisticsBtn();

	




private slots:
	void addUserBtn();
	void wylogujSie();
	void editAllBooksBtnClicked();
	void editAllAccountsBtnClicked();
	void editMyAccountBtnClicked();
	void booksFilter();
	void libraryFilter();
	void libraryBorrowsData(int row, int column);
	void libraryBorrowsCloseBtn();
	void libraryBorrowsConfirm();
	void booksData(int row, int column);
	//void booksFiltler
	void borrowsReturns(int row, int column);
	void borrowsReturnsCloseBtn();
	void borrowsReturnsAccept();

	//returns
	void returnsPanel(int row, int column);
	void returnsPanelCloseBtn();
	void borrowsReturnsAcceptBtn();

	//myData
	void updateData();

private:
	Ui::MainWindowClass ui;
	int activeId, viewedId;
	int bookActiveId;
	int borrowActiveId;
	int returnActiveId;

signals:
	void wylogowano();

public slots:
	void poprawnyInterfejs(int id, QString name, QString last, QString rola) {
		activeId = id;
		this->show();
		ui.userName->setText(name + " " + last + " ("+rola+")");

		if (rola == "Bibliotekarz") {
			librarianBtns();
			//ui.stackedWidget->setCurrentWidget(ui.allBorrowsPanel);
			allBorrowsBtn();
		}
		else
		{
			userBtns();
			//ui.stackedWidget->setCurrentWidget(ui.borrowsPanel);
			borrowsBtn();
		}
	}
	void allAccountListDblClicked(int row, int column);
	
};

