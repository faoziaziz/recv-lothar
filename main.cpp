#include <QCoreApplication>
#include "server.h"
#include "accdb.h"

int main(int argc, char *argv[])
{

    /*
        username : trumonuser
        password : trumonpassword
    */

    QCoreApplication a(argc, argv);
    QString hostname;
    QString username;
    QString password;
    QString database;
    Server server;

    //server.start(2020);
    if (argc == 5){
        hostname = argv[1];
        username = argv[2];
        password = argv[3];
        database = argv[4];
        accdb aksesD(hostname, username, password, database);
        
        server.start(6969);
    }
    else{
        qInfo()<<"Cara menggunakannya ";
        qInfo()<<"./NeiraRecvServ [hosname] [username] [password] [database]";
    }



    return a.exec();
}
