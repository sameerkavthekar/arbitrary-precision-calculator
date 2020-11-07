bc: main.o list.o stack.o functions.o infix.o
	cc main.o list.o stack.o functions.o infix.o -Wall -o bc
	rm -f *.o *~ 
main.o: infix.h main.c
	cc -c main.c -Wall
functions.o: functions.c list.h functions.h
	cc -c functions.c -Wall
stack.o: stack.c stack.h list.h
	cc -c stack.c -Wall
list.o: list.c list.h
	cc -c list.c -Wall
infix.o: infix.c functions.h list.h stack.h
	cc -c infix.c -Wall