COMPILER = g++
CFLAGS    = -g -Wall -Wextra -Winit-self -Wno-unused-parameter -Wfloat-equal -std=c++11
INSTALL_DIR=~/bin
TARGET = hit

$(TARGET): main.cc
	$(COMPILER) -O -O1 -O2 -o $(TARGET) main.cc


all: $(TARGET)

install: $(TARGET)
	install -c $(TARGET)  $(INSTALL_DIR)

clean:
	rm -f $(TARGET) *~ \#*

