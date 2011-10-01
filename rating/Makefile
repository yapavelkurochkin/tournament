
TARGET=rating

LDFLAGS += -lm

$(TARGET): polycoeff.o polynom.o

polycoeff.c: 
	./polycalc.octave > $@

clean:
	rm -f *.o $(TARGET) polycoeff.c 

