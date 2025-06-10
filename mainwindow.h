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

    void on_ClientWebButton_clicked();

    void on_returnButton4_clicked();

    void on_AppliquerModification_2_clicked();

    void showHideButton();

private:
    void setupUi();

    Ui::MainWindow *ui;
    QString filePathPaserelle;
    QString filePathModele;
    QString filePathMesure;
    QSqlDatabase db;
    QNetworkAccessManager *networkManager;
    bool passwordVisible; // Pour suivre l'état du mot de passe

    const std::string MASTER_AES_KEY_HEX = "0123456789abcdef0123456789abcdef"; // 16 octets / 32 hex chars
    const std::string MASTER_AES_IV_HEX = "fedcba9876543210fedcba9876543210";   // 16 octets / 32 hex chars
};

#endif // MAINWINDOW_H
