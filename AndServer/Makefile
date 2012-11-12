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
	openssl genrsa -des3 -out Cert/server.key 4096
	openssl req -new -key Cert/server.key -out Cert/server.csr
	openssl x509 -req -days 1095 -in Cert/server.csr -signkey Cert/server.key -out Cert/server.crt
	openssl rsa -in Cert/server.key -out Cert/server.key.insecure
	cat Cert/server.key.insecure > Cert/server.pem
	cat Cert/server.crt >> Cert/server.pem

	rm -rf Cert/server.key Cert/server.csr Cert/server.key.insecure Cert/server.crt

clientcert:
	openssl genrsa -des3 -out Cert/client.key 4096
	openssl req -new -key Cert/client.key -out Cert/client.csr
	openssl x509 -req -days 1095 -in Cert/client.csr -signkey Cert/client.key -out Cert/client.crt
	openssl rsa -in Cert/client.key -out Cert/client.key.insecure
	cat Cert/client.key.insecure > Cert/client.pem
	cat Cert/client.crt >> Cert/client.pem
	openssl pkcs12 -in Cert/client.pem -export -out Cert/client.pkcs12 -passout pass:123456

	rm -rf Cert/client.key Cert/client.pem Cert/client.key.insecure Cert/client.csr

androidbks:
	chmod +x mkStore.sh
	./mkStore.sh

clean:
	rm $(PROJECT) $(OBJDIR)/*.o *.pem -f

certclean:
	rm -f Cert/*.pem Cert/*.bks

install:
	cp AndServer /usr/bin
	cp andserver.conf.example /etc/andserver.conf

distclean:
	rm /usr/bin/AndServer /etc/andserver.conf

gentoo-install:
	cp Initscripts/andserver-service-gentoo /etc/init.d/
	chmod +x /etc/init.d/andserver-service-gentoo
	rc-update add andserver-service-gentoo default

gentoo-uninstall:
	rc-update del andserver-service-gentoo
	rm /etc/init.d/andserver-service-gentoo

ubuntu-install:
	cp Initscripts/andserver-service-ubuntu /etc/init/andserver-service.conf
	initctl start andserver-service

ubuntu-uninstall:
	rm /etc/init/andserver-service.conf

