CC = g++

TARGET = client server

all: $(TARGET)

$(TARGET): %: %.cpp
	$(CC) -M -o $@ $<

.PHONY: clean

clean: $(RM) $(TARGET)