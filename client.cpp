#include "client.h"
#include "accdb.h"
Client::Client(QObject *parent, qintptr handle) : QObject(parent), QRunnable ()
{
    this->handle = handle;
}

void Client::run()
{

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
    qInfo() << "Request Length: " << request.length();
    qInfo() <<request;
    accdb AksesDB;




    /* Daemon data reply to give feedback to mcu,
     * this called adv response */

    QByteArray data("HEAD");
    QByteArray response;
    AksesDB.get_iklan();


    /* response append data will get the socket to write*/
    response.append(data);
    response.append(AksesDB.iklan_id);
    qDebug()<<"Iklan Id :"<<AksesDB.iklan_id;
    response.append("K");
    response.append("02");
    response.append(AksesDB.data_iklan);
    response.append("TAIL");
    /* kalista*/

    AksesDB.write_db(request);
    qInfo()<<"CCROT pesan terikirim : "<<response;
    socket->write(response);
    socket->waitForBytesWritten();


    /*socket close */
    socket->close();
    socket->deleteLater();

    /* show the threading because i use multi threading */
    qInfo() << this << " done " << QThread::currentThread();


}
