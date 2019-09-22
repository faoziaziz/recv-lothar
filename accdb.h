#ifndef ACCDB_H
#define ACCDB_H

#include<QDebug>
#include<QtSql>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QPluginLoader>
#include<QVariant>

class accdb
{
public:

    /* sett constructor */
    accdb();
    accdb(QString h_name, QString u_name, QString u_password, QString dbase);
    void test_db();

    void write_db(QByteArray data);
    /* destructor */
    ~accdb();
    QString hostname;
    QString username;
    QString password;
    QString database;

};

#endif // ACCDB_H
