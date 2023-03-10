BUILD_DIR = ./build
BINARY_DIR = ./bin
SOURCE_DIR = ./src

CPP_SRC_FILES = $(shell find $(SOURCE_DIR) -name "*.cpp")
CPP_OBJ_FILES = $(patsubst %.cpp, %.o, $(shell basename $(CPP_SRC_FILES)))
CPP_DPT_FILES = $(patsubst %.cpp, %.d, $(shell basename $(CPP_SRC_FILES)))

TARGET = client server
TARGET_SRC_FILES = $(patsubst %, $(SOURCE_DIR)/%.cpp, $(TARGET))
TARGET_OBJ_FILES = $(patsubst %, %.o, $(TARGET))

SUPPORT_SRC_FILES = $(filter-out $(TARGET_SRC_FILES), $(CPP_SRC_FILES))
SUPPORT_OBJ_FILES = $(filter-out $(TARGET_OBJ_FILES), $(CPP_OBJ_FILES))

CC = g++
CC_FLAGS = -std=c++17
LDFLAGS =


all: $(TARGET) 

$(TARGET): %: %.o $(SUPPORT_OBJ_FILES)
	$(CC) $(addprefix $(BUILD_DIR)/, $(SUPPORT_OBJ_FILES)) $(BUILD_DIR)/$< -o $(BINARY_DIR)/$@ $(LDFLAGS)

%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $(BUILD_DIR)/$@

%.d: %.cpp
	@set -e; \
	rm -f $@; \
	$(CC) -MM $(CC_FLAGS) $< > $@.$$$$.dtmp; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$.dtmp > $@; \
	rm -f $@.$$$$.dtmp

-include $(CPP_DPT_FILES)

.PHONY: clean
clean:
	rm -f $(addprefix $(BUILD_DIR)/, $(CPP_OBJ_FILES))
	rm -f $(addprefix $(BUILD_DIR)/, $(CPP_DPT_FILES))
	rm -f $(addprefix $(BINARY_DIR)/, $(TARGET))
