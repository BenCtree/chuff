chuff: chuff.o chuff_linked_list.o chuff_huffman_tree.o chuff_shared.h
	gcc chuff.o chuff_linked_list.o chuff_huffman_tree.o -o chuff

chuff.o: chuff.c chuff_shared.h
	gcc -c chuff.c

chuff_linked_list.o: chuff_linked_list.h chuff_linked_list.c chuff_shared.h
	gcc -c chuff_linked_list.c

chuff_huffman_tree.o: chuff_huffman_tree.h chuff_huffman_tree.c
	gcc -c chuff_huffman_tree.c

clean:
	rm *.o chuff
