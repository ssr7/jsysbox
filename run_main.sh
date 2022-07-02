#!/bin/bash 

sudo rm -rf /tmp/jni/
mvn clean compile package 
sudo java -cp target/jsysbox-0.1.jar ir.moke.jsysbox.MainClass
