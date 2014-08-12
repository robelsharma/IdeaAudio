
CC = g++
CXXFLAGS += -fPIC
CXXFLAGS += -O3
FILES = AudioAlsa.cpp \
	aud_device_alsa.cpp \
	AudioEngine.cpp 

TARGET = libIdeaAudioEngine.so
LIB = -lasound -lpthread

.PHONY: all build

build: $(FILES)
	$(CC) $(LIB) $(CXXFLAGS) -shared $^ -o $(TARGET)
	

clean:
	rm -f *.o core
	rm -f $(OUT_EXE)

all: clean build
