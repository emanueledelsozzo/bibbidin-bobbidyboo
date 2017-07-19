
# Authors
# Emanuele Del Sozzo, Lorenzo Di Tucci, Marco Rabozzi, Marco Nanni

app := N-Body

sources := $(wildcard *.c)
headers := $(wildcard *.h)
src := $(sources) $(headers)
OBJECTS = $(sources:.c=.o)
OFLAGS = -O3
STD = -std=c99

all: $(app)

CFLAGS := -c

$(app).o: $(src)
	$(CC) $(sources) $(STD) $(CFLAGS)

$(app): $(app).o
	$(CC) $(OBJECTS) $(STD) -o $@ -lm

.PHONY: clean

o3:
	$(CC) $(sources) $(STD) $(OFLAGS) -lm -o $(app) 

clean:
	@rm *.o $(app) 2> /dev/null || true
