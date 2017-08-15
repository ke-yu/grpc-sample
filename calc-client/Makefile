CC = g++
CFLAGS = -std=c++11
CFLAGS += -c -Wall
CFLAGS += -I./src/protos -I/usr/local/include -pthread
CFLAGS_DEBUG = -g -DDEBUG
CFLAGS_RELEASE = -O3 -DNDEBUG

LDFLAGS += -L/usr/local/lib 
LDFLAGS += -lgrpc++ -lgrpc++_reflection -lprotobuf -lpthread

HEADERS += $(wildcard src/*.h)
HEADERS += $(wildcard src/**/*.h)
SOURCES = $(shell find src/* -iname *.cc)

PROTOS = $(wildcard protos/*.proto)
PROTOSTUBS = $(foreach proto, $(PROTOS), $(addprefix src/, $(addsuffix .pb.cc, $(subst .proto, , $(proto)))))
PROTOSTUBS += $(foreach proto, $(PROTOS), $(addprefix src/, $(addsuffix .grpc.pb.cc, $(subst .proto, , $(proto)))))

PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

OBJECTS_DEBUG = $(addprefix bin/debug/, $(patsubst src/%, %, $(SOURCES:.cc=.o)))
OBJECTS_RELEASE = $(addprefix bin/release/, $(patsubst src/%, %, $(SOURCES:.cc=.o)))

SERVICE_DEBUG=bin/debug/calc-client
SERVICE_RELEASE=bin/release/calc-client

debug: 
	$(MAKE) protos
	$(MAKE) $(SERVICE_DEBUG)

release: 
	$(MAKE) protos
	$(MAKE) $(SERVICE_RELEASE)

clean:
	rm -rf ./src/protos
	rm -rf ./bin
	rm -rf ./calculator.desc

protos: $(PROTOSTUBS)
	touch protos

$(SERVICE_DEBUG): $(SOURCES) $(OBJECTS_DEBUG)
	mkdir -p bin/debug
	$(CC) $(OBJECTS_DEBUG) -o $@ $(LDFLAGS) 

$(SERVICE_RELEASE): $(SOURCES) $(OBJECTS_RELEASE)
	mkdir -p bin/debug
	$(CC) $(OBJECTS_RELEASE) -o $@ $(LDFLAGS) 

bin/debug/%.o: src/%.cc $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -o $@

bin/debug/protos/%.o: src/protos/%.cc 
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) $< -o $@

bin/release/%.o: src/%.cc $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) $< -o $@

bin/release/protos/%.o: src/protos/%.cc 
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) $< -o $@

src/protos/%.pb.cc: protos/%.proto
	$(warning $(GRPC_CPP_PLUGIN_PATH))
	mkdir -p src/protos
	$(PROTOC) -I ./protos --cpp_out=./src/protos $<
	$(PROTOC) -I ./protos --grpc_out=./src/protos --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<
