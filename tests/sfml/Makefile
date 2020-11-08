CXX = g++
CXXFLAGS = -std=c++17 -Ofast -O3 -flto -march=native -s -Wall -Wextra -fdiagnostics-color=always
LIBS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

TARGET = main
OBJS = $(TARGET).o

all: $(TARGET)

install: $(TARGET) clean

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf *.o
distclean: clean
	rm -f $(TARGET)

.PHONY: clean
