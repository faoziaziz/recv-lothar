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
    AksesDB.write_db(request);


    /* Daemon */
    QByteArray data("kalistaumari");
    QByteArray response;


    response.append(data);
    socket->write(response);


    socket->close();
    socket->deleteLater();

    qInfo() << this << " done " << QThread::currentThread();


}
