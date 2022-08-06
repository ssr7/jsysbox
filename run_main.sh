#!/bin/bash 

(cd src/main/clib ; make)
sudo rm -rf /tmp/jni/
mvn clean compile package 
PACKAGE_VERSION=$(cat pom.xml  | grep version | head -n1  | sed 's/<version>//' | sed 's/<\/version>//' | xargs) 
sudo java -cp target/jsysbox-${PACKAGE_VERSION}.jar ir.moke.jsysbox.MainClass
