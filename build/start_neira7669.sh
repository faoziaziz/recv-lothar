
#!/bin/bash

#script untuk restart aplikasi

date

var=$(ps axu | grep -v grep | grep "neira7669 127.0.0.1")
        if [ ! -z "$var" ];
                then echo "found";
        else
                ./neira7669 127.0.0.1 trumon  pos_almar @Alm4r2020
fi


