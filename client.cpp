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

    /* request adalah data yang harus di parsing */
   
    accdb AksesDB;




    /* Daemon data reply to give feedback to mcu,
     * this called adv response */

    QByteArray data("HEAD");
    QByteArray response;

    if (request.length()<64){
        /* reply NOK */
        socket->write("NOK");
        socket->waitForBytesWritten();
    } else {

      /* write to database as recv*/
      
        AksesDB.write_db(request);
	/* how get the data */
	QByteArray cpu_id_byte;

	//request.resize(15);
	for(int i=0; i<15; i++){
	  cpu_id_byte[i]=request[i];
	}

	QString SerialNumber = QString(cpu_id_byte);

	qInfo()<<"Serial number ("<<SerialNumber<<")()=>>>>"<<endl;
       

	/* akses to parser */
	AksesDB.parser_db(SerialNumber, 2, request);

	
        qInfo()<<"CCROT pesan terikirim : "<<response;
        socket->write("ACK");
        socket->waitForBytesWritten();

    }

    /*socket close */
    socket->close();
    socket->deleteLater();

    /* show the threading because i use multi threading */
    qInfo() << this << " done " << QThread::currentThread();


}
