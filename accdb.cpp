#include "accdb.h"


accdb::accdb()
{
  /*
   *  this constructor for accessing database
   */

  this->hostname ="mygetplus-cluster-test.cluster-cstnh3syiokv.ap-southeast-1.rds.amazonaws.com";
  this->username="prasimax";
  this->password="AtE0T8EXAV5k8prksf5z";
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
	  qInfo() << "Sukses write! " ;
        }

      db.close();
      qInfo() << "Disconnected";

    }
    
}

void accdb::get_iklan(int mode, QString CPUID_string)
{
  if (mode==1){
      
    qInfo()<<"get data iklan"<<endl;
    if(!this->db.open()){
      qInfo()<<"gak bisa dapet data iklan dbnya ngaco";
    }
    else{
      /* update expire*/
      QSqlQuery query2;
      QString cmd2;
      
      cmd2 = "Update NeiraIklanVer3 set Flag=2 where (ExpiryDate<CURRENT_TIMESTAMP and Flag=0)";
      query2.prepare(cmd2);
      if(!query2.exec()){
	qInfo()<<"Query iklan error, croot";
	qInfo() << db.lastError().text();
	qInfo() << query2.lastError().text();
      }
      else{
	qInfo()<<"Update sukses untuk all 2";
	      
      }
      
      qInfo()<<"CPUID_string : "<<CPUID_string;
      qInfo()<<"waktunya execute the query";
      /* simple with the man */
      QSqlQuery query;
      QString cmd="SELECT * FROM NeiraIklanVer3 WHERE (CPUID=:cpuid and Flag=0 and ExpiryDate>CURRENT_TIMESTAMP)  ORDER BY `NO` ASC LIMIT 1;";

      QString FormatTanggal="yyyy-MM-dd hh:mm:ss";


      query.prepare(cmd);
      query.bindValue(":cpuid", CPUID_string);
      /*this code will execute */
      if(!query.exec()){
	qInfo()<<"Query iklan error";
	qInfo() << db.lastError().text();
	qInfo() << query.lastError().text();
      }
      else{
	qInfo()<<"getting field from database";
	while(query.next()){
	  /* to catenate the sumber */
	  /* idiklan = ID_TGL */
	  QString idiklan=query.value(1).toString();
	  /* dataiklan = Content */
	  QString dataiklan=query.value(2).toString();
	  /* dataTeks1 = MemberID*/
	  QString dataTeks1 = query.value(8).toString();
	  /* dataTeks2 = PadTeks*/
	  QString dataTeks2 = query.value(12).toString();
	  /* get data timestamp*/
	  QString timeStamp = query.value(3).toString();

	  qInfo()<<"===================="<<endl;
	  qInfo()<<"expiry checking "<<endl;
	  qInfo()<<"===================="<<endl;

	  /* jika timestamp melewati expity date */
	  qInfo()<<"time stamp : "<<timeStamp<<endl;
	  /* convert timestamp into date format */
	  /* format yyyy-mm-dd hh:mm:ss */
		   
	  QDateTime expiry_date = QDateTime::fromString(timeStamp, FormatTanggal);
		    
	  /* convert expirydate to int format */
	  int expiry_date_int = expiry_date.toTime_t();
		    
	    
	  /* get current date */
	  QDateTime currentDateTime = QDateTime::currentDateTime();
		    
	  /* convert current date to integer */
	  int currentInt = currentDateTime.toTime_t();

	  // if (currentInt<expiry_date_int){
	  qDebug()<<"+++ ===== (ini Belum Expiry) ====++"<<endl;
	  qDebug()<<"database Iklan id : "<<idiklan;
	  this->iklan_id = idiklan;
	  this->data_iklan = dataiklan;
	  /* to insert the data teks */
	  this->data_teks1 = dataTeks1;
	  this->data_teks2 = dataTeks2;
	  /* update iklan to flag*/
	  update_iklan_toflag(CPUID_string, mode, idiklan);
		   

		   
	}
      }
    }

	

  } else if(mode==2){
    qInfo()<<"get data iklan"<<endl;
    if(!this->db.open()){
      qInfo()<<"gak bisa dapet data iklan dbnya ngaco";
    }
    else{
      qInfo()<<"waktunya execute the query";
      qInfo()<<"CPUID String : "<<CPUID_string;
      QSqlQuery query;
      QString cmd="SELECT * FROM NeiraIklanVer2 WHERE CPUID=:cpuid and Flag=0 ORDER BY `NO` ASC LIMIT 1;";
      query.prepare(cmd);
      query.bindValue(":cpuid", CPUID_string);
      if(!query.exec()){
                
	qInfo() << db.lastError().text();
	qInfo() << query.lastError().text();
      }
      else{
	qInfo()<<"Nyambung dengan query";
	while(query.next()){
	  QString idiklan=query.value(1).toString();
	  QString dataiklan=query.value(4).toString();
	  /* read from database */
	  QString dataTeks1 = query.value(9).toString();
	  QString dataTeks2 = query.value(10).toString();

	  qDebug()<<"database Iklan id : "<<idiklan;
	  this->iklan_id = idiklan;
	  this->data_iklan = dataiklan;

	  /* data teks 1 adalah data member id */
	  this->data_teks1 = dataTeks1;
	  /* data teks 2 adalah padding */
	  this->data_teks2 = dataTeks2;
	  update_iklan_toflag(CPUID_string, mode, idiklan);

	}
      }
    }

  }

}


/* just test for update iklan to flag*/

void accdb::update_iklan_toflag(QString CPUID_String, int mode, QString id_iklan)
{
  /* CPUID_String used for confirmated mode for check store*/
  if(mode==1){
    /* Earing mode */
    /* Update Earing table = NeiraIklanVer3 Table */
    qInfo()<<"update iklan mode "<<mode;
    if(!this->db.open()){
      qInfo()<<"gak bisa dapet data iklan dbnya ngaco";
    }
    
    else{
      qInfo()<<"waktunya execute the query";
      qInfo()<<"CPUID String : "<<CPUID_String;
      QSqlQuery query;
      QString cmd;
      cmd = "Update NeiraIklanVer3 set Flag=1 where ID_TGL=:IdTanggal";
      query.prepare(cmd);
      query.bindValue(":IdTanggal", id_iklan);
      if(!query.exec()){
	qInfo()<<"Query iklan error, croot";
	qInfo() << db.lastError().text();
	qInfo() << query.lastError().text();
      }
      else{
	qInfo()<<"Update sukses";
	      
      }
      //eksekusi kueri yang kedua
      
      QSqlQuery query2;
      QString cmd2;
      
      cmd2 = "Update NeiraIklanVer3 set Flag=2 where (ExpiryDate<CURRENT_TIMESTAMP and Flag=0)";
      query2.prepare(cmd2);
      if(!query2.exec()){
	qInfo()<<"Query iklan error, croot";
	qInfo() << db.lastError().text();
	qInfo() << query2.lastError().text();
      }
      else{
	qInfo()<<"Update sukses untuk all 2";
	      
      }
    }

   
  }


  else if(mode==2){
    /* Reedem mode */
    /* Update to Redemn Table */
    qInfo()<<"update iklan mode "<<mode;
    qInfo()<<"untuk kasus expiry udah expire"<<endl;

    if(!this->db.open()){
      qInfo()<<"gak bisa dapet data iklan dbnya ngaco";
    }
    else{
      qInfo()<<"waktunya execute the query";
      qInfo()<<"CPUID String : "<<CPUID_String;
      QSqlQuery query;
      QString cmd;
      cmd = "Update NeiraIklanVer2 set Flag=1 where ID_TGL=:IdTanggal";
      query.prepare(cmd);
      query.bindValue(":IdTanggal",this->iklan_id);
      if(!query.exec()){
	qInfo()<<"Query iklan error, croot";
	qInfo() << db.lastError().text();
	qInfo() << query.lastError().text();
      }
      else{
	qInfo()<<"Update sukses";
      }
    }
  }
    

}

/* hak mu */
void accdb::update_iklan_toflag2(QString CPUID_String, int mode, QString idiklan)
{
  /* CPUID_String used for confirmated mode for check store*/
  if(mode==1){
    /* Earing mode */
    /* Update Earing table = NeiraIklanVer3 Table */
    qInfo()<<"update iklan mode "<<mode;
    if(!this->db.open()){
      qInfo()<<"gak bisa dapet data iklan dbnya ngaco";
    }
    else{
      qInfo()<<"waktunya execute the query";
      qInfo()<<"CPUID String : "<<CPUID_String;
      QSqlQuery query;
      QString cmd;
      cmd = "Update NeiraIklanVer3 set Flag=2 where ID_TGL=:IdTanggal";
      query.prepare(cmd);
      query.bindValue(":IdTanggal", idiklan);
      if(!query.exec()){
	qInfo()<<"Query iklan error, croot";
	qInfo() << db.lastError().text();
	qInfo() << query.lastError().text();
      }
      else{
	qInfo()<<"Update sukses untuk expire"<<idiklan;
	this->db.commit();
      }
    }
  }


  else if(mode==2){
    /* Reedem mode */
    /* Update to Redemn Table */
    qInfo()<<"update iklan mode "<<mode;

    if(!this->db.open()){
      qInfo()<<"gak bisa dapet data iklan dbnya ngaco";
    }
    else{
      qInfo()<<"waktunya execute the query";
      qInfo()<<"CPUID String : "<<CPUID_String;
      QSqlQuery query;
      QString cmd;
      cmd = "Update NeiraIklanVer2 set Flag=2 where ID_TGL=:IdTanggal";
      query.prepare(cmd);
      query.bindValue(":IdTanggal",this->iklan_id);
      if(!query.exec()){
	qInfo()<<"Query iklan error, croot";
	qInfo() << db.lastError().text();
	qInfo() << query.lastError().text();
      }
      else{
	qInfo()<<"Update sukses";
      }
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

accdb::~accdb()
{
  /* this is destructor for database aplication*/
  qInfo()<<"\nend of database\n";


  /* some destruction of database */

  //this->db.close();
}
