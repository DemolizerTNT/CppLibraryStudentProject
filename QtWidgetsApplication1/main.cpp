#include "MainWindow.h"
#include "LoginWindow.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include "Baza.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Baza db;
    db.connect();

    LoginWindow l;
    MainWindow w;

    // Jak logowanie siê uda, wyœwietli siê panel MainWindow
    QObject::connect(&l, &LoginWindow::zalogowano, &w, &MainWindow::poprawnyInterfejs);
    QObject::connect(&l, &LoginWindow::zalogowano, &l, &LoginWindow::hide);

    // Jak wylogowanie siê uda, wyœwietli siê panel LoginWindow
    QObject::connect(&w, &MainWindow::wylogowano, &w, &MainWindow::hide);
    QObject::connect(&w, &MainWindow::wylogowano, &l, &LoginWindow::show);

    //l.WypiszListe();
    l.show();
  

    

    return app.exec();
}
