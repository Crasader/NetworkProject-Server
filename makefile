CXX_STD_FLAGS := -std=c++17

G_BIN_DIR := $(shell realpath ./bin)
G_OBJ_DIR := $(shell realpath ./obj)

export CXX_STD_FLAGS
export G_BIN_DIR
export G_OBJ_DIR

CXX_DEBUG_FLAGS := $(if $(DEBUG), -g,)
export CXX_DEBUG_FLAGS

all: build_protobuf build_gamelogic build_server build_client 
	

build_protobuf:
	$(MAKE) -C protobuf

build_server:
	$(MAKE) -C server

build_client:
	$(MAKE) -C client

build_gamelogic:
	$(MAKE) -C game_logic

.PHONY: clean

clean:
	$(MAKE) -C protobuf clean
	$(MAKE) -C server clean
	$(MAKE) -C client clean
	$(MAKE) -C game_logic clean

	rm -rf $(G_OBJ_DIR)