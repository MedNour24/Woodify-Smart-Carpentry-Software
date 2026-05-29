#include "connsql.h"

// Ctor
// Initialise l'attribut `db` avec le driver ODBC.
// On le fait dans le constructeur pour que `db` soit stocké dans le Singleton.
// Cela signifie que la même instance de `QSqlDatabase` sera utilisée partout dans l'application
// via le Singleton, au lieu de créer une nouvelle connexion à chaque fois.
ConnSQL::ConnSQL() : status(false) {
    db = QSqlDatabase::addDatabase("QODBC");
}

// Dtor
// Ferme la connexion si elle est encore ouverte lorsque l'objet est détruit
ConnSQL::~ConnSQL() {
    if (db.isOpen())
        db.close(); // Libère proprement les ressources
}

// --- Accès à l'instance unique du Singleton ---
// Utilise la technique "Meyers Singleton" : instance statique locale
// Elle est créée la première fois qu'on appelle createInstance()
// et reste vivante pendant toute la durée du programme (thread-safe en C++11+)
ConnSQL& ConnSQL::createInstance() {
    static ConnSQL instance; // Instance unique du Singleton
    return instance;         // Retourne une référence à cette instance
}

// Connexion sans paramètres (valeurs par défaut à remplir)
bool ConnSQL::createConnection() {
    db.setDatabaseName("2A7"); // Saisissez le nom de votre source de données ici
    db.setUserName("carp");     // Saisissez votre nom d'utilisateur ici
    db.setPassword("123");     // Saisissez votre mot de passe utilisateur ici

    if (db.open()) {
        status = true;
        qDebug() << "Connexion établie";
        return true;
    }

    status = false;
    qDebug() << "Échec de la connexion :" << db.lastError().text();
    QMessageBox::critical(nullptr, "Erreur de connexion", db.lastError().text());
    return false;
}

// Surcharge : connexion avec paramètres dynamiques
bool ConnSQL::createConnection(QString dbName, QString user, QString password) {
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);

    if (db.open()) {
        status = true;
        qDebug() << "Connexion établie";
        return true;
    }

    status = false;
    qDebug() << "Échec de la connexion :" << db.lastError().text();
    QMessageBox::critical(nullptr, "Erreur de connexion", db.lastError().text());
    return false;
}

// Fermeture manuelle de la connexion
void ConnSQL::close() {
    if (db.isOpen()) {
        db.close();
        status = false;
        qDebug() << "Connexion fermée";
    }
}

// Retourne un pointeur vers l'objet QSqlDatabase
QSqlDatabase* ConnSQL::getDB() {
    return &db;
}
