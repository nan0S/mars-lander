TARGET = main
EXENAME = mars-lander
OBJS = main.o

CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wreorder -O3 -Ofast
DFLAGS = -fsanitize=address -fsanitize=undefined
RFLAGS = -DNDEBUG

all: $(TARGET)

install: $(TARGET) clean

release: CXXFLAGS += $(RFLAGS)
release: $(TARGET)

debug: CXXFLAGS += $(DFLAGS)
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(EXENAME) $^

main.o: main.cpp
	$(CC) $(CXXFLAGS) -DLOCAL -c -o $@ $<

%.o: %.cpp %.hpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf *.o
distclean: clean
	rm -f $(EXENAME)

.PHONY: clean
