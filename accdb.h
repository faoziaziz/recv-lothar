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
    /*
        get iklan provide mode, if 1 then earning
        if 2 then redemn
    */
    void get_iklan(int mode, QString CPUID_String);
    /* destructor */
    ~accdb();

    /* crot */
    QString iklan_id;
    QString data_iklan;

    /* variable */
    QString hostname;
    QString username;
    QString password;
    QString database;
    //QByteArray data_iklan;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");


};

#endif // ACCDB_H
