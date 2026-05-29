#include "smartcarpentry.h"
#include "login.h"
#include "connsql.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize database connection via Singleton
    ConnSQL &db = ConnSQL::createInstance();
    // If the DB is unavailable, keep the UI accessible.
    // createConnection() already shows the error message.
    db.createConnection();

    // Start with the login window instead of going directly to main window
    login loginWindow;
    loginWindow.showMaximized();
    return a.exec();
}
