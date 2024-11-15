#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finance.h"
#include <QMessageBox>
#include <QSqlError>
#include <QRegularExpressionValidator>
#include <QValidator>
#include <QDate>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableViewtr->setModel(f.afficher());

    // Connect buttons to respective slots
    connect(ui->projet, &QPushButton::clicked, this, &MainWindow::on_projet_clicked);
    connect(ui->employer, &QPushButton::clicked, this, &MainWindow::on_employer_clicked);
    connect(ui->materielle, &QPushButton::clicked, this, &MainWindow::on_materielle_clicked);
    connect(ui->fournisseur, &QPushButton::clicked, this, &MainWindow::on_fournisseur_clicked);
    connect(ui->facture, &QPushButton::clicked, this, &MainWindow::on_facture_clicked);
    connect(ui->ajouter_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked);
    connect(ui->supprimer_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_supprimer_clicked);
    connect(ui->modifier_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_modifierer_clicked);
    connect(ui->actu, &QPushButton::clicked, this, &MainWindow::on_ACTUALISER_clicked);
    connect(ui->comboBox_trii, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_trii_changed);
    connect(ui->modifier_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_9_clicked);



}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot to switch to "projet_3" widget
void MainWindow::on_projet_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);  // Assuming "projet_3" widget is at index 0
}

// Slot to switch to "employer_3" widget
void MainWindow::on_employer_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);  // Assuming "employer_3" widget is at index 1
}

// Slot to switch to "materielle_3" widget
void MainWindow::on_materielle_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);  // Assuming "materielle_3" widget is at index 2
}

// Slot to switch to "fournisseur_3" widget
void MainWindow::on_fournisseur_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);  // Assuming "fournisseur_3" widget is at index 3
}

// Slot to switch to "facture_3" widget
void MainWindow::on_facture_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);  // Assuming "facture_3" widget is at index 4
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    // Récupérer les informations saisies dans les champs
    QString ID_T = ui->lineEdit_id->text();
    QString DATE_TRANSACTION = ui->lineEdit_date->text();
    int MONTANT = ui->lineEdit_montant->text().toInt();
    QString TYPE = ui->lineEdit_tp->text();
    QString CATEGORIE = ui->lineEdit_cat->text();
    QString MODE_PAIEMENT = ui->lineEdit_mode->text();

    // Validation de l'ID_T (Doit commencer par T et suivi de 1 à 3 chiffres)
    QRegularExpression idRegex("^T\\d{1,3}$");
    QRegularExpressionValidator idValidator(idRegex);
    int pos = 0;

    if (idValidator.validate(ID_T, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Erreur", "L'ID doit être sous la forme T1, T12, ou T123.");
        return;
    }

    // Validation de la Date (Format dd/MM/yyyy)
    if (!QDate::fromString(DATE_TRANSACTION, "dd/MM/yyyy").isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une date valide au format dd/MM/yyyy.");
        return;
    }

    // Validation du Montant (entre 1 et 999)
    if (MONTANT < 1 || MONTANT > 999) {
        QMessageBox::warning(this, "Erreur", "Le montant doit être un nombre entre 1 et 999.");
        return;
    }

    // Validation du Type (Doit être Revenu ou Dépense)
    if (TYPE != "Revenu" && TYPE != "Dépense") {
        QMessageBox::warning(this, "Erreur", "Le type doit être 'Revenu' ou 'Dépense'.");
        return;
    }

    // Validation de la Categorie (Doit être Achat ou Ventes)
    if (CATEGORIE != "Achat" && CATEGORIE != "Ventes") {
        QMessageBox::warning(this, "Erreur", "La catégorie doit être 'Achat' ou 'Ventes'.");
        return;
    }

    // Validation du Mode de Paiement (Doit être Espèce ou Chèque)
    if (MODE_PAIEMENT != "Espèce" && MODE_PAIEMENT != "Chèque") {
        QMessageBox::warning(this, "Erreur", "Le mode de paiement doit être 'Espèce' ou 'Chèque'.");
        return;
    }

    // Vérifier la relation entre la catégorie et le type
    if ((CATEGORIE == "Achat" && TYPE != "Dépense") || (CATEGORIE == "Ventes" && TYPE != "Revenu")) {
        QMessageBox::warning(this, "Erreur", "Si la catégorie est 'Achat', le type doit être 'Dépense'. Si la catégorie est 'Ventes', le type doit être 'Revenu'.");
        return;
    }

    // Instancier l'objet de la classe finance
    finance f(ID_T, DATE_TRANSACTION, MONTANT, TYPE, CATEGORIE, MODE_PAIEMENT);

    // Tenter d'ajouter l'élément dans la base de données
    bool test = f.ajouter();

    // Vérifier si l'ajout a réussi
    if (test) {
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Ajout effectué avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout non effectué.");
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    QString ID_T = ui->supp->text();

    // Vérification : si l'ID est vide, afficher un message d'erreur
    if (ID_T.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
        return;
    }

    // Préparer la requête SQL pour vérifier si l'ID existe dans la base de données
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM FINANCE WHERE ID_T = :id");
    query.bindValue(":id", ID_T);
    if (!query.exec() || !query.next() || query.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Erreur", "ID non trouvé dans la base de données.");
        return;
    }

    bool test = f.supprimer(ID_T);
    if (test) {
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Suppression effectuée avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Suppression non effectuée.");
    }
}

void MainWindow::on_pushButton_modifierer_clicked()
{
    QString ID_T = ui->lineEdit_2->text();

    // Vérification si l'ID est vide
    if (ID_T.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
        return;
    }

    QString attribut = ui->comboBox_MOD->currentText();
    QString nouvelleValeur = ui->lineEdit_3->text();

    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une nouvelle valeur.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM FINANCE WHERE ID_T = :id");
    query.bindValue(":id", ID_T);
    if (!query.exec() || !query.next() || query.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Erreur", "L'ID spécifié n'existe pas.");
        return;
    }

    QString requete = QString("UPDATE FINANCE SET %1 = :valeur WHERE ID_T = :id").arg(attribut);
    query.prepare(requete);
    query.bindValue(":valeur", nouvelleValeur);
    query.bindValue(":id", ID_T);

    if (query.exec()) {
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Modification réussie !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification : " + query.lastError().text());
    }
}
void MainWindow::on_pushButton_modifier_f_clicked() {
    // Récupérer l'ID depuis le champ de texte
    QString ID_T = ui->lineEdit_2->text();

    // Vérifier si l'ID est vide
    if (ID_T.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
        return;
    }

    // Récupérer l'attribut à modifier et la nouvelle valeur
    QString attribut = ui->comboBox_MOD->currentText();  // Attribut à modifier
    QString nouvelleValeur = ui->lineEdit_3->text();  // Nouvelle valeur

    // Vérifier si la nouvelle valeur est vide
    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une nouvelle valeur.");
        return;
    }

    // Appeler la fonction modifier
    bool result = f.modifier(ID_T, attribut, nouvelleValeur);

    if (result) {
        //  modification est réussie actualiser la table
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Modification effectuée avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué.");
    }
}

void MainWindow::on_ACTUALISER_clicked()
{
     ui->tableViewtr->setModel(f.afficher());
}


void MainWindow::on_comboBox_trii_changed(int index)
{
    QString selectedFilter = ui->comboBox_trii->currentText();

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    if (selectedFilter == "Montant") {
        // Tri par Montant en ordre croissant
        query.prepare("SELECT  MONTANT, TYPE, CATEGORIE, MODE_PAIEMENT FROM FINANCE ORDER BY MONTANT ASC");
    } else if (selectedFilter == "Type") {
        // Tri par Type
        query.prepare("SELECT ID_T, DATE_TRANSACTION, MONTANT, TYPE, CATEGORIE, MODE_PAIEMENT FROM FINANCE ORDER BY TYPE");
    }

    if (query.exec()) {
        model->setQuery(query);

        // Définir les en-têtes des colonnes
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Transaction"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Montant"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Catégorie"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mode Paiement"));

        ui->tableViewtr->setModel(model);
        ui->tableViewtr->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'application du filtre : " + query.lastError().text());
    }
}

void MainWindow::on_lineEdit_15_cursorPositionChanged(const QString& text)
{
    // Here you can adjust the search criteria. Let's assume we are searching by ID_T (which is VARCHAR)
    QString referenceRecherche = text;

    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;

    // Modify this query to search the FINANCE table
    // If you want to search by ID_T (referance in your case), you can use this
    query.prepare("SELECT ID_T, DATE_TRANSACTION, MONTANT, TYPE, CATEGORIE, MODE_PAIEMENT FROM FINANCE WHERE ID_T = :id");
    query.bindValue(":id", referenceRecherche);

    // Execute the query and update the table model
    if (query.exec())
    {
        model->setQuery(query);

        // Set headers based on your FINANCE table's structure
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_T"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Transaction"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Montant"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Catégorie"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mode Paiement"));

        // Display the results in your tableViewtr
        ui->tableViewtr->setModel(model);
        ui->tableViewtr->resizeColumnsToContents();
    }
    else
    {
        // Show an error message if the query fails
        QMessageBox::critical(this, "Erreur", "Échec de la recherche : " + query.lastError().text());
    }
}
void MainWindow::on_pushButton_9_clicked()
{
    // Récupérer l'ID entré dans le champ lineEdit_15
    QString referenceRecherche = ui->lineEdit_15->text();

    // Vérifier si l'ID est vide
    if (referenceRecherche.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
        return;
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Préparer la requête SQL pour rechercher par ID_T
    query.prepare("SELECT ID_T, DATE_TRANSACTION, MONTANT, TYPE, CATEGORIE, MODE_PAIEMENT FROM FINANCE WHERE ID_T = :id");
    query.bindValue(":id", referenceRecherche);

    // Exécuter la requête
    if (query.exec()) {
        // Si la requête est exécutée avec succès, vérifier si des données sont retournées
        if (query.next()) {
            model->setQuery(query);

            // Définir les en-têtes des colonnes
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_T"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Transaction"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("Montant"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("Catégorie"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mode Paiement"));

            // Mettre à jour le modèle dans la tableViewtr
            ui->tableViewtr->setModel(model);
            ui->tableViewtr->resizeColumnsToContents();
        } else {
            // Si aucun résultat trouvé pour l'ID
            QMessageBox::information(this, "Aucun résultat", "Aucune donnée trouvée pour cet ID.");
        }
    } else {
        // Afficher un message d'erreur si la requête échoue
        QMessageBox::critical(this, "Erreur", "Échec de la recherche : " + query.lastError().text());
    }
}
