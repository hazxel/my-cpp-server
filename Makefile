CC = g++
CC_FLAGS = -std=c++17

CPP_SRC_FILES = $(shell find ./ -name "*.cpp")
CPP_OBJ_FILES = $(patsubst %.cpp, %.o, $(CPP_SRC_FILES))
CPP_DPT_FILES = $(patsubst %.cpp, %.d, $(CPP_SRC_FILES))

TARGET = client server
TARGET_SRC_FILES = $(patsubst %, .//%.cpp, $(TARGET))
TARGET_OBJ_FILES = $(patsubst %, .//%.o, $(TARGET))

SUPPORT_SRC_FILES = $(filter-out $(TARGET_SRC_FILES), $(CPP_SRC_FILES))
SUPPORT_OBJ_FILES = $(filter-out $(TARGET_OBJ_FILES), $(CPP_OBJ_FILES))


all: $(TARGET)

$(TARGET): %: %.o $(SUPPORT_OBJ_FILES)
	$(CC) $(SUPPORT_OBJ_FILES) $< -o $@ 

%.o: %.cpp
	$(CC) $(CC_FLAGS) -c $< -o $@

%.d: %.cpp
	@set -e; \
	rm -f $@; \
	$(CC) -MM $(CC_FLAGS) $< > $@.$$$$.dtmp; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$.dtmp > $@; \
	rm -f $@.$$$$.dtmp

-include $(CPP_DPT_FILES)

.PHONY: clean
clean:
	rm -f $(DST)
	rm -f $(CPP_OBJ_FILES)
	rm -f $(CPP_DPT_FILES)
	rm -f $(shell find ./ -name "*.dtmp")

run:
	make all