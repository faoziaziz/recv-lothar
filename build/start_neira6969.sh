#!/bin/bash

#script untuk restart aplikasi

date

var=$(ps axu | grep -v grep | grep "neira6969 127.0.0.1")
        if [ ! -z "$var" ];
                then echo "found";
        else
                 ./neira6969 127.0.0.1 trumon tappingbox123 trumon
fi


