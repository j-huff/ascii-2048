CC=g++ 
CFLAGS=-c -g 
LDFLAGS= -std=c++11
SOURCES=main.cc game.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=ascii2048

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -std=c++11 -g -o $@ -lcurses

.cc.o:
	$(CC) $(CFLAGS)  $< -std=c++11 -o $@

clean:
	rm *.o