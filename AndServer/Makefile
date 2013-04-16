CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -lrt -pthread -lssl -lcrypto
PROJECT = AndServer

OBJDIR  = Bin
SRCDIR  = Src
INCLDIR = Include

OPTS = $(CXXFLAGS) -I$(INCLDIR)/

SRCS    = $(shell find $(SRCDIR) -name '*.cpp')
SRCDIRS = $(shell find $(SRCDIR) -type d | sed 's/$(SRCDIR)/./g' )
INCLS   = $(shell find $(INCLDIR) -name '*.h')
OBJS    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

all: $(PROJECT)

$(PROJECT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCLDIR)/%.h
	$(CXX) $(OPTS) -c $< -o $@

certificate:
	openssl genrsa -out Cert/server.key 4096
	openssl req -new -key Cert/server.key -out Cert/server.csr
	openssl x509 -req -days 1095 -in Cert/server.csr -signkey Cert/server.key -out Cert/server.crt
	cat Cert/server.key > Cert/server.pem
	cat Cert/server.crt >> Cert/server.pem

	rm -rf Cert/server.key Cert/server.csr Cert/server.crt

clientcert:
	openssl genrsa -out Cert/client.key 4096
	openssl req -new -key Cert/client.key -out Cert/client.csr
	openssl x509 -req -days 1095 -in Cert/client.csr -signkey Cert/client.key -out Cert/client.crt
	cat Cert/client.key > Cert/client.pem
	cat Cert/client.crt >> Cert/client.pem
	openssl pkcs12 -in Cert/client.pem -export -out Cert/client.pkcs12 -passout pass:123456

	rm -rf Cert/client.key Cert/client.csr Cert/client.crt

androidbks:
	chmod +x mkStore.sh
	./mkStore.sh

clean:
	rm $(PROJECT) $(OBJDIR)/*.o *.pem -f

certclean:
	rm -f Cert/server.pem Cert/client.pem Cert/client.pkcs12 Cert/andserver.bks

install:
	cp AndServer /usr/bin
	cp andserver.conf.example /etc/andserver.conf

certinstall:
	mkdir -p /etc/andserver
	cp Cert/server.pem Cert/client.pem /etc/andserver

distclean:
	rm -f /usr/bin/AndServer /etc/andserver.conf
	rm -f /etc/andserver/server.pem /etc/andserver/client.pem
	rm -df /etc/andserver

gentoo-install:
	cp Initscripts/andserver-service-gentoo /etc/init.d/
	chmod +x /etc/init.d/andserver-service-gentoo
	rc-update add andserver-service-gentoo default

gentoo-uninstall:
	rc-update del andserver-service-gentoo
	rm -f /etc/init.d/andserver-service-gentoo

ubuntu-install:
	cp Initscripts/andserver-service-ubuntu /etc/init/andserver-service.conf
	initctl start andserver-service

ubuntu-uninstall:
	rm -f /etc/init/andserver-service.conf

