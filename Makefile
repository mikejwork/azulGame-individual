.PHONY: all

all: azul ;

CXX=g++
CFLAGS=-Wall -Werror -g -std=c++14

CLASSES= \
	Factory \
	GameManager \
	GameIO \
	Menu \
	Mosaic \
	Player \
	Row \
	Tile \
	TileBag \
	Turn \
	Settings \
	BoxLid \
	BoxLidNode \
	AiBrain

OTHER_FILES= \
	main.o

main.o: main.cpp
	$(CXX) $(CFLAGS) -c $<

.PHONY: class_%

class_%: %.cpp %.h
	$(CXX) $(CFLAGS) -c $<

.PHONY: classes

classes: $(foreach class, $(CLASSES), class_$(class)) ;

azul: classes $(OTHER_FILES)
	$(CXX) $(CFLAGS) -o $@ *.o

.PHONY: clean

clean:
	rm *.o
