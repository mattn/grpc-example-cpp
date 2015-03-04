GRPC_ROOT = /home/mattn/dev/grpc
CFLAGS = -Wall -I$(GRPC_ROOT)/include -I$(GRPC_ROOT)/third_party/protobuf/src -std=c++11
LDFLAGS = -L$(GRPC_ROOT)/libs/opt -lgrpc++ -lgrpc -lgpr -L$(GRPC_ROOT)/libs/opt/protobuf -lprotobuf

.SUFFIXES: .cc .o

all : client server

client : client.o customer_service.pb.o
	g++ -o client client.o customer_service.pb.o $(LDFLAGS)

server : server.o customer_service.pb.o
	g++ -o server server.o customer_service.pb.o $(LDFLAGS)

.cc.o :
	g++ -c $(CFLAGS) -I. $< -o $@

customer_service.pb.h : customer_service.proto
	$(GRPC_ROOT)/bins/opt/protobuf/protoc --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_ROOT)/bins/opt/grpc_cpp_plugin customer_service.proto

clean :
	rm -f *.o server client
