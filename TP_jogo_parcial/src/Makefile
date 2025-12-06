snake: listacobra.o snake.o ranking.o menu.o giracobra.o
	gcc listacobra.o snake.o ranking.o menu.o giracobra.o -o snake -lraylib -lm
snake.o: snake.c listacobra.h ranking.h menu.h giracobra.h
	gcc -g -c snake.c
listacobra.o: listacobra.h listacobra.c
	gcc -g -c listacobra.c
ranking.o: ranking.c ranking.h
	gcc -g -c ranking.c
menu.o: menu.c menu.h
	gcc -g -c menu.c
giracobra.o: giracobra.c giracobra.h
	gcc -g -c giracobra.c
run: snake
	./snake
clean:
	rm -f *.o
