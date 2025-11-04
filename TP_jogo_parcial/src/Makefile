snake: listacobra.o snake.o
	gcc listacobra.o snake.o -o snake -lraylib -lm
snake.o: snake.c listacobra.h
	gcc -g -c snake.c
listacobra.o: listacobra.h listacobra.c
	gcc -g -c listacobra.c
clean:
	rm -f *.o
