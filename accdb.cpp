#include "accdb.h"


accdb::accdb()
{
  /*
   *  this constructor for accessing database
   */
  this->hostname ="localhost";
  this->username="pos_almar";
  this->password="@Alm4r2020";
  this->database="trumon";

  this->db.setHostName(this->hostname);
  this->db.setUserName(this->username);
  this->db.setPassword(this->password);
  this->db.setDatabaseName(this->database);

}

accdb::accdb(QString h_name, QString u_name, QString u_password, QString u_dbase)
{
  qInfo()<<"Inititalization Database";
  this->hostname=h_name;
  this->username=u_name;
  this->password=u_password;
  this->database=u_dbase;

  this->db.setHostName(this->hostname);
  this->db.setUserName(this->username);
  this->db.setPassword(this->password);
  this->db.setDatabaseName(this->database);
}




void accdb::write_db(QByteArray data)
{
  qInfo()<<"hostname : "<<this->hostname;
  qInfo()<<"username : "<<this->username;
  qInfo()<<"password : "<<this->password;
  qInfo()<<"database : "<<this->database;


  if(!this->db.open())
    {
      qInfo() << "Could not open connection!";
      qInfo() << this->db.lastError().text();
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
	  /* for notification success write */
	  qInfo() << "Sukses write, Tan . . ! ! " ;
        }

      db.close();
      qInfo() << "Disconnected";

    }
    
}

void accdb::get_iklan()
{
  qInfo()<<"get data iklan"<<endl;
  if(!this->db.open()){
    qInfo()<<"gak bisa dapet data iklan dbnya ngaco";
  }
  else{
    qInfo()<<"waktunya execute the query";
    QSqlQuery query;
    QString cmd="select * from NeiraIklanVer2 Where (NO = (select min(NO) from NeiraIklanVer2) and Flag=0);";
    if(!query.exec(cmd)){
      qInfo()<<"Query iklan error, croot";
    }
    else{
      qInfo()<<"Nyambung dengan query";
      while(query.next()){
	QString idiklan=query.value(1).toString();
	QString dataiklan=query.value(3).toString();
	qDebug()<<"database Iklan id : "<<idiklan;
	this->iklan_id = idiklan;
	this->data_iklan = dataiklan;


      }
    }
  }
}


/* ini fungsi untuk parser */
void accdb::parser_db(QString SerialNumber, int ModeParser, QByteArray data){

  /* parserdb */
  qInfo()<<"entering parserdb"<<endl;
  /* get pattern form table */
  if(!this->db.open()){
    /* jika terjadi gagal connecsi*/
    qInfo()<<"Sending error message from db parser"<<endl;
  } else {
    /* jika conneksi telah selesai */
    qInfo()<<"entering to get pattern"<<endl;
    QSqlQuery query;
    QString cmd="SELECT * from DeviceTable where (SerialNumber=:ser_num and ParserMode=:mode_parser)";
    
    query.prepare(cmd);
    query.bindValue(":ser_num", SerialNumber);
    query.bindValue(":mode_parser", ModeParser);
    if(!query.exec()){
      qInfo()<<"Query untuk parser error"<<endl;
      qInfo()<<db.lastError().text();
      qInfo()<<query.lastError().text();
    } else {


      qInfo()<<"Data yang harus di parser"<<endl;
      qInfo()<<"============================="<<endl;
      qInfo()<<"connected to query parser "<<endl;
      qInfo()<<data;
      while(query.next()){
	/* to get query every pattern need */
	QString patternTotal = query.value(16).toString();
	/* pattern qr nomer 20 alias PatternCustomField */
	QString patternQR = query.value(19).toString();
	/* pattern nomer 14 */
	QString patternNomer = query.value(13).toString();
	


	qInfo()<<"====================================="<<endl;
	qInfo()<<"Pattern Total : "<<patternTotal<<endl;
	qInfo()<<"Pattern QR : "<<patternQR<<endl;
	qInfo()<<"Pattern No : "<<patternNomer<<endl;
	qInfo()<<"======================================="<<endl;

	/*lets enter the parse */
	/* first we need to null checker so it will normalize the data transaction */
	for(int iter=0; iter<data.size();iter++){
	  if (data.at(iter)==0){
	    data[iter]=' ';
	  }
	}
	/* next i should get combine the regex */
	qInfo()<<"process the parser"<<endl;

	/* Declare the regular expression for every unit */
	QRegularExpression REGEX_TOTAL;
	QRegularExpression REGEX_QR;
	QRegularExpression REGEX_NOMER;

	/* execute the regex */
	REGEX_TOTAL.setPattern(patternTotal);
	REGEX_QR.setPattern(patternQR);
	REGEX_NOMER.setPattern(patternNomer);

	/* combine the regex with the string data */
        QString data_total = REGEX_TOTAL.match(data).captured(REGEX_TOTAL.match(data).lastCapturedIndex());
	QString data_qr = REGEX_QR.match(data).captured(REGEX_QR.match(data).lastCapturedIndex());
	QString data_nomer =REGEX_NOMER.match(data).captured(REGEX_NOMER.match(data).lastCapturedIndex());
	qInfo()<<"data_total = "<<data_total;
	qInfo()<<"data_nomer = "<<data_nomer;

	/* an then post to api */

	QString total=data_total;
	QString serialnum=SerialNumber;
	save_trans(SerialNumber, total, data_nomer);
//	QString qr="mantapqr";
//	QJsonObject jObj;
//	jObj.insert("total", QJsonValue::fromVariant(total));
//	jObj.insert("serialnum", QJsonValue::fromVariant(serialnum));
//	jObj.insert("qr", QJsonValue::fromVariant(data_qr));

	/* JOBj */
	//QJsonDocument doc(jObj);
	//qDebug()<<doc.toJson();

	//QNetworkRequest req(QUrl("https://localhost:8087/neira/v1/202005/parser"));
	//req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	//QNetworkAccessManager man;

	//QNetworkReply *reply = man.post(req, doc.toJson());

	//while(!reply->isFinished()){
	//  qApp->processEvents();
	//}
	

	qInfo()<<"harusnya selesai"<<endl;
	/* [total, serialnum, qr]*/
//	QByteArray responseBytes = reply->readAll();

	
	
	

	
      }

    }
    
    
  }
  
}

/* lets parse data transaksi */
void accdb::lets_parse(QByteArray DataTrans){
  /* 
     lets do it parse 
     thats will be the nice
  */
  /* checking null */
  for (int iter=0; iter<DataTrans.size();iter++){
    if(DataTrans.at(iter)==0) {
      DataTrans[iter]=' ';
    }

  }
  

}

void accdb::test_db(){

  /* testing for database */

  qInfo()<<"Selesai ";

  if(!this->db.open()){
    qInfo()<<"Sending error message, db connection cannot be established";
  }
  else{
    /* this query executed while test db successfuly connected */
    qInfo()<<"Connected starting to query";
    QSqlQuery query;
    QString cmd="SELECT data_iklan_neira FROM NeiraIklanData WHERE flag_avalaible=1";
    if(!query.exec(cmd)){
      qInfo()<<"execute failure";
    }else {
      /* if sucses*/
      while(query.next()){
	/* get data iklan neira convert to the string */
	QString data_iklan_neira = query.value(1).toString();
	qInfo()<<"Data iklan Neira "<<data_iklan_neira;
	/*aku pengin sholat */
      }

    }



    /* get query */
    this->db.close();

 } 
}

void accdb::save_trans(QString SerialNumber, QString total, QString data_nomer){
	
	QString DeviceID;
	qInfo()<<"Save transaction !! "<<SerialNumber<<endl;
	if(!this->db.open()){
		qInfo()<<"Database tidak bisa dibuka"<<endl;

	} else {
		
		qInfo()<<"entering Transacttion \n";
		QSqlQuery query;
		QString cmd="SELECT * from DeviceTable where (SerialNumber=:ser_num);";
		query.prepare(cmd);
		query.bindValue(":ser_num", SerialNumber);
		if(!query.exec()){
			qInfo()<<"query transaction error"<<endl;
			
		} else {
			while(query.next()){
			qInfo()<<"nilai nya "<<query.value(2).toString()<<endl;
				DeviceID = query.value(1).toString();
				qInfo()<<"Device ID : "<<DeviceID;

				QSqlQuery query2;
				query2.prepare("INSERT INTO Transaksi (DeviceID, Nilai, Nomor) VALUES (:DeviceId, :Nilai, :Nomor)");
				query2.bindValue(":DeviceId", DeviceID);
				query2.bindValue(":Nilai", total);
				query2.bindValue(":Nomor", data_nomer);
				if(!query2.exec()){
					qInfo()<<"end of 2"<<endl;
				} else {
					qInfo()<<"Suksess"<<endl;
				}
			
			}
			
		}
		}
	
//	qInfo()<<"Deviceid : "<<DeviceID;
	this->db.close();


}

accdb::~accdb()
{
  /* this is destructor for database aplication*/
  qInfo()<<"\nend of database\n";


  /* some destruction of database */

  //this->db.close();
}
