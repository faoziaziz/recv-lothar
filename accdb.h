#ifndef ACCDB_H
#define ACCDB_H
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include<QDebug>
#include<QtSql>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QPluginLoader>
#include<QVariant>
/* untuk akses api client*/
#include<QtNetwork>
/* add some header to execute regex */
#include<QRegularExpression>
#include<QDateTime>


class accdb
{
public:

    /* sett constructor */
    accdb();
    accdb(QString h_name, QString u_name, QString u_password, QString dbase);
    void test_db();

    void write_db(QByteArray data);
    void parser_db(QString SerialNumber, int ParserMode, QByteArray data );
    void lets_parse(QByteArray DataTrans);
    void get_iklan();
	void save_trans(QString SerialNumber, QString total, QString data_nomer);
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
