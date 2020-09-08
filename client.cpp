/*
    Author : Aziz Amerul Faozi
    Description :
        Digunakan hanya untuk melakukan testing di server tertentu.
*/

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
    QString request2, request3;
    QString DeviceId;


    qInfo()<<"Something happend !! ()()====>>>"<<endl;
    reERN.setPattern("REQUEST_ERP");
    reRDM.setPattern("REQUEST_RDT");

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

    /* Convert to request string siapa tahu suatu harii kepake*/
    for (int i=0; i<11;i++)
    {
        request3[i] = request[16+i];
    }

    /*get device id */
    for (int i=0;i<15;i++){
        DeviceId[i]=request[i];
    }

    qInfo()<<"Device Id "<<endl;
    /* For executing */
    request2 = request;


    matchERN = reERN.match(request.mid(16,11));
    matchRDM =reRDM.match(request.mid(16,11));

    if(matchERN.hasMatch()){
        /* This will execute if device requie Earning */
        qInfo()<<"REQUEST_EARNING Executed. ";
        qInfo()<<request;
        qInfo()<<"ada yang cocok. ";
        qInfo()<<matchERN.captured(0);
        accdb AksesDB;

        /* Daemon data reply to give feedback to mcu,
         * this called adv response */

        QByteArray data("HEAD");
        QByteArray response;

	/* earning will set to mode==1 see the code accdb.cpp 1 */
        AksesDB.get_iklan(1, DeviceId);


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
        //response.append(AksesDB.data_iklan);
        //response.append("#");
        /* for padding teks */
        response.append(AksesDB.data_teks2);
        response.append("|");
        /* for member id */
        response.append(AksesDB.data_teks1);
        response.append("#");
        response.append(AksesDB.data_iklan);
        response.append("TAIL");


        /* to send request to device */
        qInfo()<<"CROT pesan terikirim : "<<response;
        socket->write(response);
        socket->waitForBytesWritten();

    }
    else if(matchRDM.hasMatch()) {
        /*
         *  For Redempoint algorithm
        */
        qInfo()<<"REQUEST_REDT Executed";
        qInfo()<<request;
        qInfo()<<"ada yang cocok";
        qInfo()<<matchERN.captured(0);
        accdb AksesDB;

        /* Daemon data reply to give feedback to mcu,
         * this called adv response */

        QByteArray data("HEAD");
        QByteArray response;
	/* acces db set iklan 2*/
        AksesDB.get_iklan(2, DeviceId);

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
        //response.append(AksesDB.data_iklan);
        //response.append("#");
        /* for padding teks */
        response.append(AksesDB.data_teks2);
        response.append("|");

	
        /* for member id */
        response.append(AksesDB.data_teks1);
        response.append("#");
        response.append(AksesDB.data_iklan);
        response.append("TAIL");

        /* Write request to database*/

        AksesDB.write_db(request);
        qInfo()<<"CROT pesan terikirim : "<<response;
        socket->write(response);
        socket->waitForBytesWritten();
    }
    else{
        /* for Nothing Algorithm */

        /* RegularExpression for matching from data interface*/
        QRegularExpression re;
        QRegularExpressionMatch match1;
        re.setPattern("REQ");
        qInfo()<<"Gak ada yagn cocok";
        qInfo()<<"Request yang dikirim ";
        qInfo()<<request;
        match1 = re.match(request);
        if (match1.hasMatch()){
            qInfo()<<"Kedetek";
        }
	qInfo()<<"===================="<<endl;
	qInfo()<<"Seending nok "<<endl;
	qInfo()<<"===================="<<endl;
        socket->write("NOK");
        socket->waitForBytesWritten();
    }


    /*socket close */
    socket->close();
    socket->deleteLater();

    /* show the threading because i use multi threading */
    qInfo() << this << " done " << QThread::currentThread();


}
