TARGET = main
EXENAME = mars-lander
OBJS = main.o \
	Map.o \
	Common.o

CC = g++
CXXFLAGS = -std=c++17 -DLOCAL -Ofast -O3 -flto -march=native -s -Wall -Wextra -Wreorder
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

%.o: %.cpp %.hpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf *.o
distclean: clean
	rm -f $(EXENAME)

.PHONY: clean
