TARGET = mars-lander

OBJS = Options.o \
	Map.o \
	Evolution.o \
	Common.o \
	Agent.o \
	Drawer.o

POPULATION_SIZE = 200
OFFSPRING_SIZE = $(POPULATION_SIZE)
CHROMOSOME_LENGTH = 100

CXX = g++
CXXFLAGS = -std=c++17 -DLOCAL -Wall -Wextra -Wreorder -Ofast -O3 -flto -march=native -s
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

DFLAGS = -g -fsanitize=address -fsanitize=undefined
RFLAGS = -DNDEBUG
OPTS = -DPOPL=$(POPULATION_SIZE) -DCHL=$(CHROMOSOME_LENGTH)

all: $(TARGET)

release: CXXFLAGS += $(RFLAGS)
release: $(TARGET)

debug: CXXFLAGS += $(DFLAGS)
debug: $(TARGET)

$(TARGET): $(OBJS) main.o
	$(CXX) $(CXXFLAGS) $(LIBS) $(OPTS) -o $@ $^

benchmark: $(OBJS) simulator.o
	$(CXX) $(CXXFLAGS) $(LIBS) $(OPTS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(OPTS) -c -o $@ $<

clean:
	rm -f *.o
distclean: clean
	rm -f mars-lander