#!/bin/bash 

(cd src/main/clib ; make)
sudo rm -rf /tmp/jni/
mvn clean compile package 
sudo java -cp target/jsysbox-0.2.jar ir.moke.jsysbox.MainClass
