#include "client.h"
#include "accdb.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

Client::Client(QObject *parent, qintptr handle) : QObject(parent), QRunnable ()
{
    this->handle = handle;
}

void Client::run()
{

    /* Regular expression for earning and redeem */
    QRegularExpression reERN, reRDM;
    QRegularExpressionMatch matchERN, matchRDM;
    QByteArray  string_MCU= "123124123 REQUEST_RDT";
    reERN.setPattern("REQUEST_RDT");
    reRDM.setPattern("REQUEST_ERP");





    qInfo() << this << " run " << QThread::currentThread();

    QTcpSocket* socket = new QTcpSocket(nullptr);
    if(!socket->setSocketDescriptor(handle))
    {
        qCritical() << socket->errorString();
        delete socket;
        return;
    }



    /* waitForReadyRead */
    socket->waitForReadyRead();
    QByteArray request = socket->readAll();

    /* Show information for data recive*/
    qInfo() << "Request Length: " << request.length();
    qInfo()<<"Data Dari MCU"<<endl;
    qInfo() <<request;

    /* For executing */
    matchERN = reERN.match(request);
    matchRDM =reRDM.match(request);

    if(matchERN.hasMatch()){
        qInfo()<<"REQUEST_REDT Executed";
        qInfo()<<request;
        qInfo()<<"ada yang cocok";
        qInfo()<<matchERN.captured(0);
        accdb AksesDB;

        /* Daemon data reply to give feedback to mcu,
         * this called adv response */

        QByteArray data("HEAD");
        QByteArray response;
        AksesDB.get_iklan(1);


        /*
         *  response append data will get the socket to write
         *  Store data earning and redemtion in varible request
         *  those string are REQUEST_RDT and REQUEST_ERP
        */


        /* Using regex to define earning or redemtion */

        response.append(data);
        response.append(AksesDB.iklan_id);

        qDebug()<<"Iklan Id :"<<AksesDB.iklan_id;
        response.append("K");
        response.append("02");
        response.append(AksesDB.data_iklan);
        response.append("TAIL");
        /* kalista*/

        AksesDB.write_db(request);
        qInfo()<<"CROT pesan terikirim : "<<response;
        socket->write(response);
        socket->waitForBytesWritten();

    }
    else if(matchRDM.hasMatch()) {
        qInfo()<<"REQUEST_REDT Executed";
        qInfo()<<request;
        qInfo()<<"ada yang cocok";
        qInfo()<<matchERN.captured(0);
        accdb AksesDB;

        /* Daemon data reply to give feedback to mcu,
         * this called adv response */

        QByteArray data("HEAD");
        QByteArray response;
        AksesDB.get_iklan(2);


        /*
         *  response append data will get the socket to write
         *  Store data earning and redemtion in varible request
         *  those string are REQUEST_RDT and REQUEST_ERP
        */


        /* Using regex to define earning or redemtion */

        response.append(data);
        response.append(AksesDB.iklan_id);

        qDebug()<<"Iklan Id :"<<AksesDB.iklan_id;
        response.append("K");
        response.append("02");
        response.append(AksesDB.data_iklan);
        response.append("TAIL");
        /* kalista*/

        AksesDB.write_db(request);
        qInfo()<<"CROT pesan terikirim : "<<response;
        socket->write(response);
        socket->waitForBytesWritten();
    }
    else{
        qInfo()<<"Gak ada yagn cocok";
        socket->write("NOK");
        socket->waitForBytesWritten();
    }


    /*socket close */
    socket->close();
    socket->deleteLater();

    /* show the threading because i use multi threading */
    qInfo() << this << " done " << QThread::currentThread();


}
