BUILD_DIR = build
DPT_DIR = build/.dpt
BINARY_DIR = bin
SOURCE_DIR = src

CPP_SRC_FILES = $(shell find $(SOURCE_DIR) -name "*.cpp")
CPP_OBJ_FILES = $(patsubst $(SOURCE_DIR)%, $(BUILD_DIR)%, $(patsubst %.cpp, %.o, $(CPP_SRC_FILES)))
CPP_DPT_FILES = $(patsubst $(SOURCE_DIR)%, $(DPT_DIR)%, $(patsubst %.cpp, %.d, $(CPP_SRC_FILES)))

# put targets at the root of folder src!
TARGET = client server
TARGET_OBJ_FILES = $(patsubst %, $(BUILD_DIR)/%.o, $(TARGET))
SUPPORT_OBJ_FILES = $(filter-out $(TARGET_OBJ_FILES), $(CPP_OBJ_FILES))

CC = clang++
CC_FLAGS = -std=c++17
LDFLAGS =


all: $(TARGET) 

$(TARGET): %:$(filter %, $(TARGET_OBJ_FILES)) $(SUPPORT_OBJ_FILES)
	mkdir -p $(BINARY_DIR) 
	$(CC) $(SUPPORT_OBJ_FILES) $< -o $(BINARY_DIR)/$@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(DPT_DIR)/%.d: $(SOURCE_DIR)/%.cpp 
	mkdir -p $(dir $@); \
	set -e; \
	rm -f $@; \
	$(CC) -MM $(CC_FLAGS) $< > $@.$$$$.dtmp; \
	sed 's,\($(notdir $*)\)\.o[ :]*,$(patsubst $(DPT_DIR)%.d,$(BUILD_DIR)%.o,$@) $@ : ,g' < $@.$$$$.dtmp > $@; \
	rm -f $@.$$$$.dtmp

-include $(CPP_DPT_FILES)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BINARY_DIR)