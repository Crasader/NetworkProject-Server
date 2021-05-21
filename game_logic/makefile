PROJ_NAME := gamelogic

CXX := g++
CXXFLAGS := -pthread -I/opt/homebrew/Cellar/protobuf/3.15.8/include $(CXX_STD_FLAGS) $(CXX_DEBUG_FLAGS)
LDFLAGS := -L/opt/homebrew/Cellar/protobuf/3.15.8/lib -lprotobuf -lpthread

SRC_DIR := ./src
OBJ_DIR := $(if $(G_OBJ_DIR),$(G_OBJ_DIR)/$(PROJ_NAME),./obj)
LIB_DIR := $(if $(G_BIN_DIR),$(G_BIN_DIR),./lib)

SRC_FILES := $(shell find $(SRC_DIR) -name *.cpp -or -name *.cc)
OBJ_FILES := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.o,$(SRC_FILES))

DEP_OBJ_FILES := $(G_OBJ_DIR)/protobuf/game.pb.cc.o

build_archive: $(LIB_DIR)/lib$(PROJ_NAME).a


# server -> build_obj
$(LIB_DIR)/lib$(PROJ_NAME).a: build_obj | lib_dir_check
	ar rcs $@ $(OBJ_FILES)

build_obj: obj_dir_check $(OBJ_FILES)


# *.cpp.o -> *.cpp
$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

# *.cc.o -> *.cc
$(OBJ_DIR)/%.cc.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

lib_dir_check:
	@mkdir -p $(LIB_DIR)

obj_dir_check:
	@mkdir -p $(dir $(OBJ_FILES))

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)