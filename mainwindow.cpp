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
#include <QCryptographicHash>
#include <QRandomGenerator>

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
    ui->AppliquerModification_2->setVisible(false);
    ui->ipAddressPasserelleLabel->setVisible(false);
    ui->ipAddressDNSLabel->setVisible(false);
    ui->ipAddressMaskLabel->setVisible(false);
    ui->DHCPLabel->setVisible(false);
    ui->comboBoxDHCP->setVisible(false);
    ui->ipAddressPasserelleEdit->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressDNSEdit->setVisible(false);
    ui->returnButton->setVisible(false);
    ui->NameModeleAcquisitionEdit->setVisible(false);
    ui->NameModeleAquisitionLabel->setVisible(false);
    ui->returnButton2->setVisible(false);
    ui->returnButton3->setVisible(false);
    ui->returnButton4->setVisible(false);
    ui->TypeCharge->setVisible(false);
    ui->comboBoxCharge->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(false);
    ui->PeriodeIntegrationInstEdit->setVisible(false);
    ui->PeriodeIntegrationMoy->setVisible(false);
    ui->PeriodeIntegrationMoyEdit->setVisible(false);
    ui->NameDomaineEdit->setVisible(false);
    ui->NameDomaine->setVisible(false);
    ui->idClientEdit->setVisible(false);
    ui->idClientLabel->setVisible(false);
    ui->MDPEdit->setVisible(false);
    ui->MDPLabel->setVisible(false);
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->showHideButton->setVisible(false);
    connect(ui->showHideButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    ui->showHideButton->setIcon(QIcon("C:/Users/EtudiantIR2/Documents/Appli_BDD/preview-show-interface-icon-free-vector.jpg"));
    ui->showHideButton->setIconSize(QSize(26, 26)); // Définir la taille de l'icône (optionnel)
}

void MainWindow::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Bdd");
    db.setUserName("admin");
    db.setPassword("admin");
    QMessageBox::information(this, "Succes", QString("Connection à la base de donnée réussit"));

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
    ui->comboBoxDHCP->addItem("", -1);
    ui->comboBoxDHCP->addItem("Activé", 1);
    ui->comboBoxDHCP->addItem("Desactivé", 0);
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
    QString ipGateway = ui->ipAddressPasserelleEdit->text();
    QString ipMask = ui->ipAddressMaskEdit->text();
    QString ipDNS = ui->ipAddressDNSEdit->text();
    QString newNameModeleAcquisition = ui->NameModeleAcquisitionEdit->text();
    QString DomaineName = ui->NameDomaineEdit->text();
    QString idClient = ui->idClientEdit->text();
    QString periodeInst = ui->PeriodeIntegrationInstEdit->text(); // ou .value() si c'est un QSpinBox
    QString periodeMoy = ui->PeriodeIntegrationMoyEdit->text();   // idem

    int DHCP = ui->comboBoxDHCP->currentData().toInt();
    int phase = ui->comboBoxCharge->currentData().toInt();
    int idDispositif = ui->PrimaryKey->currentData().toInt();

    if (idDispositif == -1)
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un ID de Dispositif.");
        return;
    }

    if (idDispositif == 0)
    {
        QSqlQuery queryProtocole(db);
        queryProtocole.prepare("UPDATE `Dispositif_Passerelle` SET `ID_Protocole_FK` = '0' WHERE `Dispositif_Passerelle`.`ID_Dispositif_PK` = 0;");
        queryProtocole.addBindValue(idDispositif == -1 ? QVariant() : idDispositif);

        if (!queryProtocole.exec())
        {
            QSqlError error = queryProtocole.lastError();
            QMessageBox::critical(this, "Erreur SQL", QString("Erreur lors de la modification de l'ID Protocole: %1").arg(error.text()));
            qDebug() << "Erreur SQL (ID Protocole):" << error.text() << " Requête: " << queryProtocole.lastQuery();
        } else {
            QMessageBox::information(this, "Succès", "Type de Protocole envoyer");
        }
    }

    if (idDispositif == 1)
    {
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
                  "ID_Client_FK,"
                  "Adresse_IP_Passerelle,"
                  "Adresse_gateway_Passerelle,"
                  "Adresse_mask_gateway_Passerelle,"
                  "Adresse_DNS_Passerelle,"
                  "Adresse_DHCP_Passerelle,"
                  "Domaine_Passerelle,"
                  "Nom_model_acquisition,"
                  "Type_charge,"
                  "Periode_inst,"
                  "Periode_moyenne)"
                  "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) "
                  "ON DUPLICATE KEY UPDATE "
                  "Nom_dispositif = IFNULL(?, Nom_dispositif),"
                  "ID_Client_FK = IFNULL(?, ID_Client_FK),"
                  "Adresse_IP_Passerelle = IFNULL(?, Adresse_IP_Passerelle),"
                  "Adresse_gateway_Passerelle = IFNULL(?, Adresse_gateway_Passerelle),"
                  "Adresse_mask_gateway_Passerelle = IFNULL(?, Adresse_mask_gateway_Passerelle),"
                  "Adresse_DNS_Passerelle = IFNULL(?, Adresse_DNS_Passerelle),"
                  "Adresse_DHCP_Passerelle = IFNULL(?, Adresse_DHCP_Passerelle),"
                  "Domaine_Passerelle = IFNULL(?, Domaine_Passerelle),"
                  "Nom_model_acquisition = IFNULL(?, Nom_model_acquisition),"
                  "Type_charge = IFNULL(?, Type_charge),"
                  "Periode_inst = IFNULL(?, Periode_inst),"
                  "Periode_moyenne = IFNULL(?, Periode_moyenne);");

    // Partie INSERT
    query.addBindValue(idDispositif);
    query.addBindValue(newName.isEmpty() ? QVariant() : newName);
    query.addBindValue(idClient.isEmpty() ? QVariant() : idClient);
    query.addBindValue(newIp.isEmpty() ? QVariant() : newIp);
    query.addBindValue(ipGateway.isEmpty() ? QVariant() : ipGateway);
    query.addBindValue(ipMask.isEmpty() ? QVariant() : ipMask);
    query.addBindValue(ipDNS.isEmpty() ? QVariant() : ipDNS);
    query.addBindValue(DHCP == -1 ? QVariant() : DHCP);
    query.addBindValue(DomaineName.isEmpty() ? QVariant() : DomaineName);
    query.addBindValue(newNameModeleAcquisition.isEmpty() ? QVariant() : newNameModeleAcquisition);
    query.addBindValue(phase == -1 ? QVariant() : phase);
    query.addBindValue(periodeInst.isEmpty() ? QVariant() : periodeInst);
    query.addBindValue(periodeMoy.isEmpty() ? QVariant() : periodeMoy);

    // Partie UPDATE
    query.addBindValue(newName.isEmpty() ? QVariant() : newName);
    query.addBindValue(idClient.isEmpty() ? QVariant() : idClient);
    query.addBindValue(newIp.isEmpty() ? QVariant() : newIp);
    query.addBindValue(ipGateway.isEmpty() ? QVariant() : ipGateway);
    query.addBindValue(ipMask.isEmpty() ? QVariant() : ipMask);
    query.addBindValue(ipDNS.isEmpty() ? QVariant() : ipDNS);
    query.addBindValue(DHCP == -1 ? QVariant() : DHCP);
    query.addBindValue(DomaineName.isEmpty() ? QVariant() : DomaineName);
    query.addBindValue(newNameModeleAcquisition.isEmpty() ? QVariant() : newNameModeleAcquisition);
    query.addBindValue(phase == -1 ? QVariant() : phase);
    query.addBindValue(periodeInst.isEmpty() ? QVariant() : periodeInst);
    query.addBindValue(periodeMoy.isEmpty() ? QVariant() : periodeMoy);

    if (!query.exec()) {
        QSqlError error = query.lastError();
        QMessageBox::critical(this, "Erreur SQL", QString("requete échoué"));
        qDebug() << "Erreur SQL (AppliquerModification):" << error.text() << " Requête: " << query.lastQuery();
    } else {
        QMessageBox::information(this, "Succès", "Données mises à jour avec succès.");
    }
}

void MainWindow::on_AppliquerModification_2_clicked()
{
    QString idClient = ui->idClientEdit->text();
    QString MDP = ui->MDPEdit->text(); // Mot de passe en clair
    int idDispositif = ui->PrimaryKey->currentData().toInt();

    // Vérifications d'entrée
    if (idDispositif == -1) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un ID de Dispositif.");
        return;
    }

    if (idClient.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Veuillez saisir un ID client.");
        return;
    }

    if (MDP.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Veuillez saisir un Mot de passe.");
        return;
    }
    quint64 randValue = QRandomGenerator::system()->generate64();
    QByteArray sel = QByteArray::number(randValue).toHex();

    QByteArray mdpSale = MDP.toUtf8() + sel;
    QByteArray hash = QCryptographicHash::hash(mdpSale, QCryptographicHash::Sha256).toHex();

    QSqlQuery queryClientWeb(db);
    queryClientWeb.prepare("INSERT INTO Client_Web (`ID_Client_PK`, `Mot_de_passe`, `Salt`) VALUES (?, ?, ?) "
                           "ON DUPLICATE KEY UPDATE "
                           "Mot_de_passe = ?, Salt = ?;");

    // Liaison des valeurs pour INSERT et UPDATE
    queryClientWeb.addBindValue(idClient);
    queryClientWeb.addBindValue(hash);
    queryClientWeb.addBindValue(sel);

    queryClientWeb.addBindValue(hash);
    queryClientWeb.addBindValue(sel);

    qDebug() << "ID:" << idClient << "HASH:" << hash << "SEL:" << sel;

    if (!queryClientWeb.exec()) {
        QSqlError error = queryClientWeb.lastError();
        QMessageBox::critical(this, "Erreur SQL", QString("Erreur lors de la modification du Client Web: %1").arg(error.text()));
        qDebug() << "Erreur SQL (Client Web):" << error.text() << " Requête: " << queryClientWeb.lastQuery();
        return;
    } else {
        QMessageBox::information(this, "Succès", "Information Client mise à jour dans Client_Web.");
    }
/*
    QSqlQuery queryDispositifPasserelle(db);
    queryDispositifPasserelle.prepare("INSERT INTO Dispositif_Passerelle (ID_Dispositif_PK, ID_Client_FK) "
                                      "VALUES (?, ?) "
                                      "ON DUPLICATE KEY UPDATE "
                                      "ID_Client_FK = VALUES(ID_Client_FK);"); // Met à jour ID_Client_FK avec la nouvelle valeur

    // Liaison des valeurs pour Dispositif_Passerelle
    queryDispositifPasserelle.addBindValue(idDispositif); // L'ID du dispositif
    queryDispositifPasserelle.addBindValue(idClient);     // L'ID du client à lier

    if (!queryDispositifPasserelle.exec()) {
        QSqlError error = queryDispositifPasserelle.lastError();
        QMessageBox::critical(this, "Erreur SQL", QString("Erreur lors de la mise à jour de Dispositif_Passerelle avec l'ID client: %1").arg(error.text()));
        qDebug() << "Erreur SQL (Dispositif_Passerelle):" << error.text() << " Requête: " << queryDispositifPasserelle.lastQuery();
    }*/
}

void MainWindow::on_pushButton_clicked()
{
    if (passwordVisible) {
        // Le mot de passe est actuellement visible, masquez-le
        ui->MDPEdit->setEchoMode(QLineEdit::Password);
        ui->showHideButton->setIcon(QIcon("C:/Users/EtudiantIR2/Documents/Appli_BDD/preview-show-interface-icon-free-vector.jpg")); // Icône "œil fermé"
        passwordVisible = false;
    } else {
        // Le mot de passe est actuellement masqué, affichez-le
        ui->MDPEdit->setEchoMode(QLineEdit::Normal);
        ui->showHideButton->setIcon(QIcon("C:/Users/EtudiantIR2/Documents/Appli_BDD/eye-slash-icon-symbol-design-illustration-vector.jpg")); // Icône "œil ouvert" ou "œil barré"
        passwordVisible = true;
    }
}

void MainWindow::modifyGatewayButtonClicked() {
    ui->PrimaryKey->setVisible(true);
    ui->PrimaryKeylabel->setVisible(true);
    ui->gatewayNameEdit->setVisible(true);
    ui->ipAddressEdit->setVisible(true);
    ui->gatewayNameLabel->setVisible(true);
    ui->ipAddressLabel->setVisible(true);
    ui->AppliquerModification->setVisible(true);
    ui->returnButton->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->ClientWebButton->setVisible(false);
    ui->ipAddressPasserelleLabel->setVisible(true);
    ui->ipAddressDNSLabel->setVisible(true);
    ui->ipAddressMaskLabel->setVisible(true);
    ui->DHCPLabel->setVisible(true);
    ui->comboBoxDHCP->setVisible(true);
    ui->ipAddressPasserelleEdit->setVisible(true);
    ui->ipAddressMaskEdit->setVisible(true);
    ui->ipAddressDNSEdit->setVisible(true);
    ui->NameDomaineEdit->setVisible(true);
    ui->NameDomaine->setVisible(true);
}

void MainWindow::returnButtonClicked() {
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->gatewayNameEdit->setVisible(false);
    ui->ipAddressEdit->setVisible(false);
    ui->gatewayNameLabel->setVisible(false);
    ui->ipAddressLabel->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->returnButton->setVisible(false);
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->ClientWebButton->setVisible(true);
    ui->ipAddressPasserelleLabel->setVisible(false);
    ui->ipAddressDNSLabel->setVisible(false);
    ui->ipAddressMaskLabel->setVisible(false);
    ui->DHCPLabel->setVisible(false);
    ui->comboBoxDHCP->setVisible(false);
    ui->ipAddressPasserelleEdit->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressDNSEdit->setVisible(false);
    ui->NameDomaineEdit->setVisible(false);
    ui->NameDomaine->setVisible(false);
    resetInputFields();
}

void MainWindow::acquisitionModelButtonClicked() {
    ui->PrimaryKey->setVisible(true);
    ui->PrimaryKeylabel->setVisible(true);
    ui->NameModeleAcquisitionEdit->setVisible(true);
    ui->NameModeleAquisitionLabel->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->ClientWebButton->setVisible(false);
    ui->returnButton2->setVisible(true);
    ui->AppliquerModification->setVisible(true);
    ui->TypeCharge->setVisible(true);
    ui->comboBoxCharge->setVisible(true);
}

void MainWindow::returnButtonClicked2()
{
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->ClientWebButton->setVisible(true);
    ui->NameModeleAcquisitionEdit->setVisible(false);
    ui->NameModeleAquisitionLabel->setVisible(false);
    ui->returnButton2->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->TypeCharge->setVisible(false);
    ui->comboBoxCharge->setVisible(false);
    resetInputFields();

}

void MainWindow::measureModelButtonClicked() {
    ui->PrimaryKey->setVisible(true);
    ui->PrimaryKeylabel->setVisible(true);
    ui->returnButton3->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->ClientWebButton->setVisible(false);
    ui->AppliquerModification->setVisible(true);
    ui->PeriodeIntegrationInst->setVisible(true);
    ui->PeriodeIntegrationInstEdit->setVisible(true);
    ui->PeriodeIntegrationMoy->setVisible(true);
    ui->PeriodeIntegrationMoyEdit->setVisible(true);
}
void MainWindow::returnButtonClicked3()
{
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->ClientWebButton->setVisible(true);
    ui->returnButton3->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(false);
    ui->PeriodeIntegrationInstEdit->setVisible(false);
    ui->PeriodeIntegrationMoy->setVisible(false);
    ui->PeriodeIntegrationMoyEdit->setVisible(false);
    resetInputFields();
}

void MainWindow::resetInputFields()
{
    ui->gatewayNameEdit->clear();
    ui->ipAddressEdit->clear();
    ui->ipAddressPasserelleEdit->clear();
    ui->ipAddressMaskEdit->clear();
    ui->ipAddressDNSEdit->clear();
    ui->NameModeleAcquisitionEdit->clear();
    ui->PeriodeIntegrationInstEdit->clear();
    ui->PeriodeIntegrationMoyEdit->clear();
    ui->NameDomaineEdit->clear();
    ui->idClientEdit->clear();

    ui->comboBoxCharge->setCurrentIndex(0);
    ui->PrimaryKey->setCurrentIndex(0);
}

void MainWindow::on_ClientWebButton_clicked()
{
    ui->idClientEdit->setVisible(true);
    ui->idClientLabel->setVisible(true);
    ui->MDPEdit->setVisible(true);
    ui->MDPLabel->setVisible(true);
    ui->MDPEdit->setEchoMode(QLineEdit::Password);
    ui->returnButton4->setVisible(true);
    ui->AppliquerModification_2->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->ClientWebButton->setVisible(false);
    ui->showHideButton->setVisible(true);
}



void MainWindow::on_returnButton4_clicked()
{
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->ClientWebButton->setVisible(true);
    ui->AppliquerModification_2->setVisible(false);
    ui->idClientEdit->setVisible(false);
    ui->idClientLabel->setVisible(false);
    ui->MDPEdit->setVisible(false);
    ui->MDPLabel->setVisible(false);
    ui->returnButton4->setVisible(false);
    ui->showHideButton->setVisible(false);
    resetInputFields();
}

