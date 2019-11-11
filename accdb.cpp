#include "accdb.h"


accdb::accdb()
{
    /*
     *  this constructor for accessing database
    */

    this->hostname ="localhost";
    this->username="trumon";
    this->password="tappingbox123";
    this->database="Trumon";

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

void accdb::get_iklan(int mode, QString CPUID_string)
{
    if (mode==1){
        qInfo()<<"get data iklan"<<endl;
        if(!this->db.open()){
            qInfo()<<"gak bisa dapet data iklan dbnya ngaco";
        }
        else{
            qInfo()<<"CPUID_string : "<<CPUID_string;
            qInfo()<<"waktunya execute the query";
            /* simple with the man */
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

    } else if(mode==2){
        qInfo()<<"get data iklan"<<endl;
        if(!this->db.open()){
            qInfo()<<"gak bisa dapet data iklan dbnya ngaco";
        }
        else{
            qInfo()<<"waktunya execute the query";
            qInfo()<<"CPUID String : "<<CPUID_string;
            QSqlQuery query;
            QString cmd="SELECT COALESCE(MAX(CASE WHEN UUID_ENC  IS NOT NULL and CPUID=:cpuid THEN NO END), MAX(NO)) AS id FROM   NeiraIklanVer3;";
            query.prepare(cmd);
            query.bindValue(":cpuid", CPUID_string);
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
