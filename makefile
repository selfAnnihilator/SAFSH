CC = gcc
CFLAGS = -Wall
LIBS = -lreadline

safsh: safsh.c
	$(CC) $(CFLAGS) safsh.c -o safsh $(LIBS)

clean:
	rm -f safsh
