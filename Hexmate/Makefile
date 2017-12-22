all:	main.o hexfile.o utils.o
	gcc -o hexmate main.o hexfile.o utils.o

main.o:	main.c 
	gcc -c main.c 

hexfile.o:
	gcc -c hexfile.c

utils.o:
	gcc -c utils.c

clean:
	rm *.o hexmate