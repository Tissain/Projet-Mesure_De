// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>
#include <QSqlDatabase>
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void AppliquerModification();
    void modifyGatewayButtonClicked();
    void acquisitionModelButtonClicked();
    void measureModelButtonClicked();
    void returnButtonClicked();
    void connectToDatabase();
    void returnButtonClicked2();
    void returnButtonClicked3();
    void resetInputFields();

private:
    void setupUi();

    Ui::MainWindow *ui;
    QString filePathPaserelle;
    QString filePathModele;
    QString filePathMesure;
    QSqlDatabase db;
    QNetworkAccessManager *networkManager;
};

#endif // MAINWINDOW_H
