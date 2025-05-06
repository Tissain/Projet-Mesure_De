#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    networkManager = new QNetworkAccessManager(this);

    ui->setupUi(this);
    setupUi();
    connectToDatabase();

    ui->gatewayNameEdit->setVisible(false);
    ui->ipAddressEdit->setVisible(false);
    ui->gatewayNameLabel->setVisible(false);
    ui->ipAddressLabel->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->ipAddressPaserelleLabel->setVisible(false);
    ui->ipAddressDNSLabel->setVisible(false);
    ui->ipAddressMaskLabel->setVisible(false);
    ui->DHCPLabel->setVisible(false);
    ui->comboBoxDHCP->setVisible(false);
    ui->EtatModbusLabel->setVisible(false);
    ui->comboBoxModbus->setVisible(false);
    ui->TimeZoneLabel->setVisible(false);
    ui->ipAddressPaserelleEdit->setVisible(false);
    ui->comboBoxTimeZone->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressDNSEdit->setVisible(false);
    ui->returnButton->setVisible(false);
    ui->NameModeleAcquisitionEdit->setVisible(false);
    ui->NameModeleAquisitionLabel->setVisible(false);
    ui->returnButton2->setVisible(false);
    ui->returnButton3->setVisible(false);
    ui->status1->setVisible(false);
    ui->status2->setVisible(false);
    ui->status3->setVisible(false);
    ui->comboBoxStatus1->setVisible(false);
    ui->comboBoxStatus2->setVisible(false);
    ui->comboBoxStatus3->setVisible(false);
    ui->TypeCharge->setVisible(false);
    ui->comboBoxCharge->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(false);
    ui->PeriodeIntegrationInstEdit->setVisible(false);
    ui->PeriodeIntegrationMoy->setVisible(false);
    ui->PeriodeIntegrationMoyEdit->setVisible(false);
    ui->PeriodeIntegrationCourbe->setVisible(false);
    ui->PeriodeIntegrationCourbeEdit->setVisible(false);
    ui->NameDomaineEdit->setVisible(false);
    ui->NameDomaine->setVisible(false);
}

void MainWindow::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Bdd");
    db.setUserName("admin");
    db.setPassword("admin");
    QMessageBox::information(this, "Succes", QString("Connection à la base de donnée résussit"));

    if (!db.open()) {
        QSqlError error = db.lastError();
        QMessageBox::critical(this, "Erreur", QString("Impossible de se connecter à la base de données. Erreur: %1").arg(error.text()));
    }
}

void MainWindow::setupUi() {
    connect(ui->AppliquerModification, &QPushButton::clicked, this, &MainWindow::AppliquerModification);
    connect(ui->modifyGatewayButton, &QPushButton::clicked, this, &MainWindow::modifyGatewayButtonClicked);
    connect(ui->acquisitionModelButton, &QPushButton::clicked, this, &MainWindow::acquisitionModelButtonClicked);
    connect(ui->measureModelButton, &QPushButton::clicked, this, &MainWindow::measureModelButtonClicked);
    connect(ui->returnButton, &QPushButton::clicked, this, &MainWindow::returnButtonClicked);
    connect(ui->returnButton2, &QPushButton::clicked, this, &MainWindow::returnButtonClicked2);
    connect(ui->returnButton3, &QPushButton::clicked, this, &MainWindow::returnButtonClicked3);
    ui->comboBoxTimeZone->addItem("", -1);
    ui->comboBoxTimeZone->addItem("États-Unis New York UTC-5", 27);
    ui->comboBoxTimeZone->addItem("États-Unis Los Angeles UTC-8", 24);
    ui->comboBoxTimeZone->addItem("Chine UTC+8", 40);
    ui->comboBoxTimeZone->addItem("France UTC+1", 33);
    ui->comboBoxTimeZone->addItem("Russie (Moscou) UTC+3", 35);
    ui->comboBoxTimeZone->addItem(" Japon UTC+9", 41);
    ui->comboBoxTimeZone->addItem("Allemagne UTC+1", 33);
    ui->comboBoxDHCP->addItem("", -1);
    ui->comboBoxDHCP->addItem("Activé", 1);
    ui->comboBoxDHCP->addItem("Desactivé", 0);
    ui->comboBoxModbus->addItem("", -1);
    ui->comboBoxModbus->addItem("Activé", 1);
    ui->comboBoxModbus->addItem("Desactivé", 0);
    ui->comboBoxStatus1->addItem("", -1);
    ui->comboBoxStatus1->addItem("Activé", 1);
    ui->comboBoxStatus1->addItem("Desactivé", 0);
    ui->comboBoxStatus2->addItem("", -1);
    ui->comboBoxStatus2->addItem("Activé", 1);
    ui->comboBoxStatus2->addItem("Desactivé", 0);
    ui->comboBoxStatus3->addItem("", -1);
    ui->comboBoxStatus3->addItem("Activé", 1);
    ui->comboBoxStatus3->addItem("Desactivé", 0);
    ui->comboBoxCharge->addItem("", -1);
    ui->comboBoxCharge->addItem("monophasé 1P+N-1TC (0)", 0);
    ui->comboBoxCharge->addItem("triphasé 3P+N-3TC (1)", 1);
    ui->PrimaryKey->addItem("", -1);
    ui->PrimaryKey->addItem("Modbus (0)", 0);
    ui->PrimaryKey->addItem("MQTT (1)", 1);
    ui->PrimaryKey->addItem("ALL (2)", 2);
}


void MainWindow::AppliquerModification() {
    QString newName = ui->gatewayNameEdit->text();
    QString newIp = ui->ipAddressEdit->text();
    QString ipGateway = ui->ipAddressPaserelleEdit->text();
    QString ipMask = ui->ipAddressMaskEdit->text();
    QString ipDNS = ui->ipAddressDNSEdit->text();
    QString newNameModeleAcquisition = ui->NameModeleAcquisitionEdit->text();
    QString PeriodeIntInst = ui->PeriodeIntegrationInstEdit->text();
    QString PeriodeIntMoy = ui->PeriodeIntegrationMoyEdit->text();
    QString PeriodeIntCourbe = ui->PeriodeIntegrationCourbeEdit->text();
    QString DomaineName = ui->NameDomaineEdit->text();

    int timeZoneValue = ui->comboBoxTimeZone->currentData().toInt();
    int DHCP = ui->comboBoxDHCP->currentData().toInt();
    int Modbus = ui->comboBoxModbus->currentData().toInt();
    int status1 = ui->comboBoxStatus1->currentData().toInt();
    int status2 = ui->comboBoxStatus2->currentData().toInt();
    int status3 = ui->comboBoxStatus3->currentData().toInt();
    int phase = ui->comboBoxCharge->currentData().toInt();
    int idDispositif = ui->PrimaryKey->currentData().toInt();

    if (idDispositif == -1)
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un ID de Dispositif.");
        return;
    }

        if (idDispositif == 0) {
        QSqlQuery queryProtocole(db);
        queryProtocole.prepare("UPDATE `Dispositif_Passerelle` SET `ID_Protocole_FK` = '0' WHERE `Dispositif_Passerelle`.`ID_Dispositif_PK` = 0;");
        queryProtocole.addBindValue(idDispositif == -1 ? QVariant() : idDispositif);

        if (!queryProtocole.exec()) {
            QSqlError error = queryProtocole.lastError();
            QMessageBox::critical(this, "Erreur SQL", QString("Erreur lors de la modification de l'ID Protocole: %1").arg(error.text()));
            qDebug() << "Erreur SQL (ID Protocole):" << error.text() << " Requête: " << queryProtocole.lastQuery();
        } else {
            QMessageBox::information(this, "Succès", "Type de Protocole envoyer");
        }
    }

        if (idDispositif == 1) {
            QSqlQuery queryProtocole(db);
            queryProtocole.prepare("UPDATE `Dispositif_Passerelle` SET `ID_Protocole_FK` = '1' WHERE `Dispositif_Passerelle`.`ID_Dispositif_PK` = 1;");
            queryProtocole.addBindValue(idDispositif == -1 ? QVariant() : idDispositif);

            if (!queryProtocole.exec()) {
                QSqlError error = queryProtocole.lastError();
                QMessageBox::critical(this, "Erreur SQL", QString("Erreur lors de la modification de l'ID Protocole: %1").arg(error.text()));
                qDebug() << "Erreur SQL (ID Protocole):" << error.text() << " Requête: " << queryProtocole.lastQuery();
            } else {
                QMessageBox::information(this, "Succès", "Type de Protocole envoyer");
            }
        }

    if (!db.isOpen())
    {
        QMessageBox::critical(this, "Erreur", "La base de données n'est pas ouverte.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO Dispositif_Passerelle (ID_Dispositif_PK,"
                  "Nom_dispositif,"
                  "Adresse_IP_Passerelle,"
                  "Adresse_gateway_Passerelle,"
                  "Adresse_mask_gateway_Passerelle,"
                  "Adresse_DNS_Passerelle,"
                  "time_zone_passerelle,"
                  "Adresse_DHCP_Passerelle,"
                  "Imodbus,"
                  "Domaine_Passerelle,"
                  "Nom_model_acquisition,"
                  "Statut_Capteur_1,"
                  "Statut_Capteur_2,"
                  "Statut_Capteur_3,"
                  "Type_charge,"
                  "Periode_inst,"
                  "Periode_moyenne,"
                  "Periode_courbe) "
                  "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) "
                  "ON DUPLICATE KEY UPDATE "
                  "Nom_dispositif = IFNULL(?, Nom_dispositif),"
                  "Adresse_IP_Passerelle = IFNULL(?, Adresse_IP_Passerelle),"
                  "Adresse_gateway_Passerelle = IFNULL(?, Adresse_gateway_Passerelle),"
                  "Adresse_mask_gateway_Passerelle = IFNULL(?, Adresse_mask_gateway_Passerelle),"
                  "Adresse_DNS_Passerelle = IFNULL(?, Adresse_DNS_Passerelle),"
                  "time_zone_passerelle = IFNULL(?, time_zone_passerelle),"
                  "Adresse_DHCP_Passerelle = IFNULL(?, Adresse_DHCP_Passerelle),"
                  "Imodbus = IFNULL(?, Imodbus),"
                  "Domaine_Passerelle = IFNULL(?, Domaine_Passerelle),"
                  "Nom_model_acquisition = IFNULL(?, Nom_model_acquisition),"
                  "Statut_Capteur_1 = IFNULL(?, Statut_Capteur_1),"
                  "Statut_Capteur_2 = IFNULL(?, Statut_Capteur_2),"
                  "Statut_Capteur_3 = IFNULL(?, Statut_Capteur_3),"
                  "Type_charge = IFNULL(?, Type_charge),"
                  "Periode_inst = IFNULL(?, Periode_inst),"
                  "Periode_moyenne = IFNULL(?, Periode_moyenne),"
                  "Periode_courbe = IFNULL(?, Periode_courbe)");

    // Première série de addBindValue()
    query.addBindValue(idDispositif);
    query.addBindValue(newName.isEmpty() ? QVariant() : newName);
    query.addBindValue(newIp.isEmpty() ? QVariant() : newIp);
    query.addBindValue(ipGateway.isEmpty() ? QVariant() : ipGateway);
    query.addBindValue(ipMask.isEmpty() ? QVariant() : ipMask);
    query.addBindValue(ipDNS.isEmpty() ? QVariant() : ipDNS);
    query.addBindValue(timeZoneValue == -1 ? QVariant() : timeZoneValue);
    query.addBindValue(DHCP == -1 ? QVariant() : DHCP);
    query.addBindValue(Modbus == -1 ? QVariant() : Modbus);
    query.addBindValue(DomaineName.isEmpty() ? QVariant() : DomaineName);
    query.addBindValue(newNameModeleAcquisition.isEmpty() ? QVariant() : newNameModeleAcquisition);
    query.addBindValue(status1 == -1 ? QVariant() : status1);
    query.addBindValue(status2 == -1 ? QVariant() : status2);
    query.addBindValue(status3 == -1 ? QVariant() : status3);
    query.addBindValue(phase == -1 ? QVariant() : phase);
    query.addBindValue(PeriodeIntInst.isEmpty() ? QVariant() : PeriodeIntInst);
    query.addBindValue(PeriodeIntMoy.isEmpty() ? QVariant() : PeriodeIntMoy);
    query.addBindValue(PeriodeIntCourbe.isEmpty() ? QVariant() : PeriodeIntCourbe);

    // Seconde série de addBindValue()
    query.addBindValue(newName.isEmpty() ? QVariant() : newName);
    query.addBindValue(newIp.isEmpty() ? QVariant() : newIp);
    query.addBindValue(ipGateway.isEmpty() ? QVariant() : ipGateway);
    query.addBindValue(ipMask.isEmpty() ? QVariant() : ipMask);
    query.addBindValue(ipDNS.isEmpty() ? QVariant() : ipDNS);
    query.addBindValue(timeZoneValue == -1 ? QVariant() : timeZoneValue);
    query.addBindValue(DHCP == -1 ? QVariant() : DHCP);
    query.addBindValue(Modbus == -1 ? QVariant() : Modbus);
    query.addBindValue(DomaineName.isEmpty() ? QVariant() : DomaineName);
    query.addBindValue(newNameModeleAcquisition.isEmpty() ? QVariant() : newNameModeleAcquisition);
    query.addBindValue(status1 == -1 ? QVariant() : status1);
    query.addBindValue(status2 == -1 ? QVariant() : status2);
    query.addBindValue(status3 == -1 ? QVariant() : status3);
    query.addBindValue(phase == -1 ? QVariant() : phase);
    query.addBindValue(PeriodeIntInst.isEmpty() ? QVariant() : PeriodeIntInst);
    query.addBindValue(PeriodeIntMoy.isEmpty() ? QVariant() : PeriodeIntMoy);
    query.addBindValue(PeriodeIntCourbe.isEmpty() ? QVariant() : PeriodeIntCourbe);

    if (!query.exec()) {
        QSqlError error = query.lastError();
        QMessageBox::critical(this, "Erreur SQL", QString("requete échoué"));
        qDebug() << "Erreur SQL (AppliquerModification):" << error.text() << " Requête: " << query.lastQuery();
    } else {
        QMessageBox::information(this, "Succès", "Données mises à jour avec succès.");
    }
}

void MainWindow::modifyGatewayButtonClicked() {
    ui->gatewayNameEdit->setVisible(true);
    ui->ipAddressEdit->setVisible(true);
    ui->gatewayNameLabel->setVisible(true);
    ui->ipAddressLabel->setVisible(true);
    ui->AppliquerModification->setVisible(true);
    ui->returnButton->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->ipAddressPaserelleLabel->setVisible(true);
    ui->ipAddressDNSLabel->setVisible(true);
    ui->ipAddressMaskLabel->setVisible(true);
    ui->DHCPLabel->setVisible(true);
    ui->comboBoxDHCP->setVisible(true);
    ui->EtatModbusLabel->setVisible(true);
    ui->comboBoxModbus->setVisible(true);
    ui->TimeZoneLabel->setVisible(true);
    ui->ipAddressPaserelleEdit->setVisible(true);
    ui->comboBoxTimeZone->setVisible(true);
    ui->ipAddressMaskEdit->setVisible(true);
    ui->ipAddressDNSEdit->setVisible(true);
    ui->NameDomaineEdit->setVisible(true);
    ui->NameDomaine->setVisible(true);
}

void MainWindow::returnButtonClicked() {
    ui->gatewayNameEdit->setVisible(false);
    ui->ipAddressEdit->setVisible(false);
    ui->gatewayNameLabel->setVisible(false);
    ui->ipAddressLabel->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->returnButton->setVisible(false);
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->ipAddressPaserelleLabel->setVisible(false);
    ui->ipAddressDNSLabel->setVisible(false);
    ui->ipAddressMaskLabel->setVisible(false);
    ui->DHCPLabel->setVisible(false);
    ui->comboBoxDHCP->setVisible(false);
    ui->EtatModbusLabel->setVisible(false);
    ui->comboBoxModbus->setVisible(false);
    ui->TimeZoneLabel->setVisible(false);
    ui->ipAddressPaserelleEdit->setVisible(false);
    ui->comboBoxTimeZone->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressDNSEdit->setVisible(false);
    ui->NameDomaineEdit->setVisible(false);
    ui->NameDomaine->setVisible(false);
    resetInputFields(); // Appel de la fonction de réinitialisation
}

void MainWindow::acquisitionModelButtonClicked() {
    ui->NameModeleAcquisitionEdit->setVisible(true);
    ui->NameModeleAquisitionLabel->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->returnButton2->setVisible(true);
    ui->AppliquerModification->setVisible(true);
    ui->status1->setVisible(true);
    ui->status2->setVisible(true);
    ui->status3->setVisible(true);
    ui->comboBoxStatus1->setVisible(true);
    ui->comboBoxStatus2->setVisible(true);
    ui->comboBoxStatus3->setVisible(true);
    ui->TypeCharge->setVisible(true);
    ui->comboBoxCharge->setVisible(true);
}

void MainWindow::returnButtonClicked2()
{
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->NameModeleAcquisitionEdit->setVisible(false);
    ui->NameModeleAquisitionLabel->setVisible(false);
    ui->returnButton2->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->status1->setVisible(false);
    ui->status2->setVisible(false);
    ui->status3->setVisible(false);
    ui->comboBoxStatus1->setVisible(false);
    ui->comboBoxStatus2->setVisible(false);
    ui->comboBoxStatus3->setVisible(false);
    ui->TypeCharge->setVisible(false);
    ui->comboBoxCharge->setVisible(false);
    resetInputFields(); // Appel de la fonction de réinitialisation

}

void MainWindow::measureModelButtonClicked() {
    ui->returnButton3->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->AppliquerModification->setVisible(true);
    ui->PeriodeIntegrationInst->setVisible(true);
    ui->PeriodeIntegrationInstEdit->setVisible(true);
    ui->PeriodeIntegrationMoy->setVisible(true);
    ui->PeriodeIntegrationMoyEdit->setVisible(true);
    ui->PeriodeIntegrationCourbe->setVisible(true);
    ui->PeriodeIntegrationCourbeEdit->setVisible(true);
}
void MainWindow::returnButtonClicked3()
{
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->returnButton3->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(false);
    ui->PeriodeIntegrationInstEdit->setVisible(false);
    ui->PeriodeIntegrationMoy->setVisible(false);
    ui->PeriodeIntegrationMoyEdit->setVisible(false);
    ui->PeriodeIntegrationCourbe->setVisible(false);
    ui->PeriodeIntegrationCourbeEdit->setVisible(false);
    resetInputFields(); // Appel de la fonction de réinitialisation
}

void MainWindow::resetInputFields()
{
    ui->gatewayNameEdit->clear();
    ui->ipAddressEdit->clear();
    ui->ipAddressPaserelleEdit->clear();
    ui->ipAddressMaskEdit->clear();
    ui->ipAddressDNSEdit->clear();
    ui->NameModeleAcquisitionEdit->clear();
    ui->PeriodeIntegrationInstEdit->clear();
    ui->PeriodeIntegrationMoyEdit->clear();
    ui->PeriodeIntegrationCourbeEdit->clear();
    ui->NameDomaineEdit->clear();

    ui->comboBoxTimeZone->setCurrentIndex(0);
    ui->comboBoxDHCP->setCurrentIndex(0);
    ui->comboBoxModbus->setCurrentIndex(0);
    ui->comboBoxStatus1->setCurrentIndex(0);
    ui->comboBoxStatus2->setCurrentIndex(0);
    ui->comboBoxStatus3->setCurrentIndex(0);
    ui->comboBoxCharge->setCurrentIndex(0);
    ui->PrimaryKey->setCurrentIndex(0);
}
