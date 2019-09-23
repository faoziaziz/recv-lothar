#include "accdb.h"


accdb::accdb()
{
    this->hostname ="localhost";
    this->username="trumon";
    this->password="tappingbox123";
    this->database="Trumon";

}

accdb::accdb(QString h_name, QString u_name, QString u_password, QString u_dbase)
{
    qInfo()<<"Inititalization Database";
    this->hostname=h_name;
    this->username=u_name;
    this->password=u_password;
    this->database=u_dbase;
}




void accdb::write_db(QByteArray data)
{
    qInfo()<<"hostname : "<<this->hostname;
    qInfo()<<"username : "<<this->username;
    qInfo()<<"password : "<<this->password;
    qInfo()<<"database : "<<this->database;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(this->hostname);
    db.setUserName(this->username);
    db.setPassword(this->password);
    db.setDatabaseName(this->database);


    if(!db.open())
    {
        qInfo() << "Could not open connection!";
        qInfo() << db.lastError().text();
    }
    else
    {
        qInfo() << "Connected, lets make a query...";
        QSqlQuery query;
        query.prepare( "INSERT INTO NeiraRecv (data) VALUES (:captureData);" );
        query.bindValue( ":captureData", data );
        
        if(!query.exec())
        {
            /* Query not executed */
            qInfo() << db.lastError().text();
            qInfo() << query.lastError().text();
        }
        else
        {   
            //
            qInfo() << "Sukses write, Tan . . ! ! " ;
        }

        db.close();
        qInfo() << "Disconnected";

    }
    
}

void accdb::test_db(){
    qInfo()<<"Selesai ";
}

accdb::~accdb()
{
    qInfo()<<"\nend of database\n";
}
