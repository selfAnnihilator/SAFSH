CC = gcc
CFLAGS = -o
LIBS = -lreadline

safsh: safsh.c
	$(CC) $(CFLAGS) safsh.c -o safsh $(LIBS)

clean:
	rm -f safsh
