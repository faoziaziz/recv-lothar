# recv-lothar

```sql
CREATE TABLE `NeiraRecv` (
	`ID` INT(11) NOT NULL AUTO_INCREMENT,
	`data` MEDIUMBLOB NULL DEFAULT NULL,
	`WaktuTerima` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	PRIMARY KEY (`ID`) USING BTREE
)
COMMENT='Neira Recv'
COLLATE='latin1_swedish_ci'
ENGINE=InnoDB
;

```
## How to Build
```sh
mkdir build
cd build
qmake ..
make 
./NeiraRecvServ
```
