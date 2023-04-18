CC = g++

TARGET = client server

all: $(TARGET)

$(TARGET): %: %.cpp
	$(CC) -o $@ $@.cpp

.PHONY: clean

clean: $(RM) $(TARGET)