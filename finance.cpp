#include "finance.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>

finance::finance(QString id, QString dt, int mt,QString tp,QString cat,QString mode)

{
    this->id = id;
    this->dt = dt;
    this->mt = mt;
    this->tp = tp;
    this->cat = cat;
    this->mode = mode;
}

bool finance::ajouter()
{
    QSqlQuery query;

    // Préparation de la requête SQL
    query.prepare("INSERT INTO FINANCE (ID_T, DATE_TRANSACTION, MONTANT,TYPE,CATEGORIE,MODE_PAIEMENT) "
                  "VALUES (:id, :tr, :mt, :tp, :cat, :mode)");

    // Création des variables liées
    query.bindValue(":id", id);                           // Liaison de l'identifiant (id)
    query.bindValue(":tr", dt);                           // Liaison de la date de transaction (dt)
    query.bindValue(":mt", QString::number(mt));          // Conversion de mt en QString pour la liaison
    query.bindValue(":tp", tp);
    query.bindValue(":cat", cat);
    query.bindValue(":mode", mode);


    // Exécution de la requête
    return query.exec();
}


QSqlQueryModel * finance::afficher()
{
    auto * model = new QSqlQueryModel();
    QSqlQuery query;

    // Execute the query and check for errors
    if (!query.exec("SELECT * FROM FINANCE")) {
        // Log the error message
        qDebug() << "Error executing query:" << query.lastError().text();

        delete model; // Clean up in case of error
        return nullptr; // Indicate failure
    }

    // Use std::move to pass the QSqlQuery by move ID_T, DATE_TRANSACTION, MONTANT,TYPE,CATEGORIE,MODE_PAIEMENT
    model->setQuery(std::move(query));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_TRANSACTION"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("DATE_TRANSACTION"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("MONTANT"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("CATEGORIE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("MODE_PAIEMENT"));

    return model;
}

 bool finance::supprimer(QString ID_T){
     QSqlQuery query;


         // Prepare the DELETE query
         query.prepare("DELETE FROM FINANCE WHERE ID_T = :id");

         // Bind the QString ID_T directly to the query
         query.bindValue(":id", ID_T);

         // Execute the query and return the result
         return query.exec();
     }
 bool finance::existe(const QString &ID_T) {
     QSqlQuery query;
     query.prepare("SELECT * FROM FINANCE WHERE ID_T = :id");
     query.bindValue(":id", ID_T);

     if (!query.exec()) {
         qDebug() << "Erreur : impossible d'exécuter la requête SELECT.";
         return false;
     }

     return query.next();  // Si l'ID existe, on retourne true
 }

 bool finance::modifier(const QString &ID_T, const QString &attribut, const QString &nouvelleValeur) {
     QSqlQuery query;

     // Prépare la requête UPDATE en fonction de l'attribut sélectionné
     query.prepare(QString("UPDATE FINANCE SET %1 = :nouvelleValeur WHERE ID_T = :id").arg(attribut));
     query.bindValue(":nouvelleValeur", nouvelleValeur);
     query.bindValue(":id", ID_T);

     if (!query.exec()) {
         qDebug() << "Erreur : impossible d'exécuter la requête UPDATE.";
         return false;
     }

     return true;  // Modification réussie
 }
 bool finance::modifierAttribut(const QString &ID_T, const QString &attribut, const QString &nouvelleValeur) {
     QSqlQuery query;
     QString queryStr = QString("UPDATE FINANCE SET %1 = :nouvelleValeur WHERE ID_T = :id").arg(attribut);
     query.prepare(queryStr);
     query.bindValue(":nouvelleValeur", nouvelleValeur);
     query.bindValue(":id", ID_T);

     if (query.exec()) {
         qDebug() << "Modification dans la base de données réussie.";
         return true;
     } else {
         qDebug() << "Erreur de requête SQL :" << query.lastError();
         return false;
     }
 }

