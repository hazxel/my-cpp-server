BLD_DIR = build
DPT_DIR = build/.dpt
BIN_DIR = bin
DBG_DIR = dbg
SRC_DIR = my-cpp-server
INCLUDE_DIR = include

CPP_SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")
CPP_OBJ_FILES = $(patsubst $(SRC_DIR)%, $(BLD_DIR)%, $(patsubst %.cpp, %.o, $(CPP_SRC_FILES)))
CPP_DPT_FILES = $(patsubst $(SRC_DIR)%, $(DPT_DIR)%, $(patsubst %.cpp, %.d, $(CPP_SRC_FILES)))

# put targets at the root of folder src!
TARGETS = client_run server_run test
TARGET_BIN_FILES = $(addprefix $(BIN_DIR)/, $(TARGETS))
TARGET_DBG_FILES = $(addprefix $(DBG_DIR)/, $(TARGETS))
TARGET_OBJ_FILES = $(patsubst %, $(BLD_DIR)/%.o, $(TARGETS))
SUPPORT_OBJ_FILES = $(filter-out $(TARGET_OBJ_FILES), $(CPP_OBJ_FILES))

ARCH = $(shell uname -s)
DARWIN = "Darwin"
LINUX = "Linux"
DEFS = 
ifeq ($(ARCH), Darwin)
	DEFS += -DMY_CPP_SERVER_PLATFORM_DARWIN
endif
ifeq ($(ARCH), Linux)
	DEFS += -DMY_CPP_SERVER_PLATFORM_LINUX
endif

CC = g++
CC_FLAGS = -std=c++17 -I$(INCLUDE_DIR) $(DEFS)
LDFLAGS =


all: $(TARGET_BIN_FILES)

debug: CC_FLAGS += -glldb
debug: $(TARGET_DBG_FILES)

$(BIN_DIR)/%: $(BLD_DIR)/%.o $(SUPPORT_OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(DBG_DIR)/%: $(BLD_DIR)/%.o $(SUPPORT_OBJ_FILES)
	mkdir -p $(DBG_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BLD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(DPT_DIR)/%.d: $(SRC_DIR)/%.cpp 
	mkdir -p $(dir $@); \
	set -e; \
	rm -f $@; \
	$(CC) -MM $(CC_FLAGS) $< > $@.$$$$.dtmp; \
	sed 's,\($(notdir $*)\)\.o[ :]*,$(patsubst $(DPT_DIR)%.d,$(BLD_DIR)%.o,$@) $@ : ,g' < $@.$$$$.dtmp > $@; \
	rm -f $@.$$$$.dtmp

-include $(CPP_DPT_FILES)

.PHONY: clean
clean:
	rm -rf $(BLD_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(DBG_DIR)