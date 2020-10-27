output: main.o command.o
	gcc -g --std=gnu99 main.o command.o -o smallsh

main.o: main.c
	gcc -g --std=gnu99 -c main.c

command.o: command.c
	gcc -g --std=gnu99 -c command.c

clean:
	rm *.o smallsh