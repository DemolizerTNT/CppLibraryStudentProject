#pragma once

#include <QWidget>
#include "ui_LoginWindow.h"

class LoginWindow : public QWidget
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

private slots:
	void listaLoginDbClock(QListWidgetItem* element);
	void WypiszListe();
	void zalogujSie();

private:
	Ui::LoginWindowClass ui;

signals:
	void zalogowano(int id, QString name, QString last, QString rola);
};

