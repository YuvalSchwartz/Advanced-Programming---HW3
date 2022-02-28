PokemonsBattles: LinkedList.o BattleByCategory.o MaxHeap.o MainSecond.o Pokemon.o
	gcc LinkedList.o BattleByCategory.o MaxHeap.o MainSecond.o Pokemon.o -o PokemonsBattles

LinkedList.o: LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c
	
BattleByCategory.o: BattleByCategory.c BattleByCategory.h LinkedList.h MaxHeap.h Defs.h
	gcc -c BattleByCategory.c
	
MaxHeap.o: MaxHeap.c MaxHeap.h Defs.h
	gcc -c MaxHeap.c
	
MainSecond.o: MainSecond.c BattleByCategory.h Pokemon.h Defs.h
	gcc -c MainSecond.c
	
Pokemon.o: Pokemon.c Pokemon.h Defs.h
	gcc -c Pokemon.c
clean:
	rm -f *.o
