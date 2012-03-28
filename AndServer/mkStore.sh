#!/bin/bash

export CLASSPATH="bcprov-jdk.jar"
CERTSTORE=Cert/andserver.bks
if [ -a $CERTSTORE ]; then
    rm $CERTSTORE || exit 1
fi
echo CLASSPATH
keytool \
      -import \
      -v \
      -trustcacerts \
      -alias 0 \
      -file <(openssl x509 -in Cert/server.pem) \
      -keystore $CERTSTORE \
      -storetype BKS \
      -noprompt \
      -provider org.bouncycastle.jce.provider.BouncyCastleProvider \
      -providerpath $CLASSPATH \
      -storepass 123456

