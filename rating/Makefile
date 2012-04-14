
TARGET=rating

LDFLAGS += -lm

$(TARGET): main.o polycoeff.o polynom.o

#polycoeff.c: 
#	./polycalc.octave > $@

clean:
	rm -f *.o $(TARGET) polycoeff.c 

