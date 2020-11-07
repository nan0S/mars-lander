TARGETS = mars-lander random-simulations
OBJS = Map.o \
	Common.o \
	Agent.o \
	Drawer.o \
	Evolution.o \
	Options.o

POPULATION_LENGTH = 50
CHROMOSOME_LENGTH = 50

CXX = g++
CXXFLAGS = -std=c++17 -DLOCAL -Ofast -O3 -flto -march=native -s -Wall -Wextra -Wreorder
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

DFLAGS = -fsanitize=address -fsanitize=undefined
RFLAGS = -DNDEBUG
OPTS = -DPOPL=$(POPULATION_LENGTH) -DCHL=$(CHROMOSOME_LENGTH)

all: $(TARGETS)

install: $(TARGETS) clean

visual: CXXFLAGS += -DVISUAL
visual: remove-visual
visual: $(TARGETS)

novisual: remove-visual
novisual: $(TARGETS)

remove-visual:
	rm -f Evolution.o simulator.o main.o

reconf: remove-reconf $(TARGETS)
remove-reconf:
	rm -f Evolution.o

release: CXXFLAGS += $(RFLAGS)
release: $(TARGETS)

debug: CXXFLAGS += $(DFLAGS)
debug: $(TARGETS)

mars-lander: $(OBJS) main.o
	$(CXX) $(OPTS) $(CXXFLAGS) $(LIBS) -o $@ $^

random-simulations: $(OBJS) simulator.o
	$(CXX) $(OPTS) $(CXXFLAGS) $(LIBS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) $(OPTS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf *.o
distclean: clean
	rm -f mars-lander random-simulations

.PHONY: clean
