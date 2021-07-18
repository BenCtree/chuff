// chuff_linked_list.h
// Ben Crabtree, 2021

# ifndef CHUFF_LINKED_LIST_H
# define CHUFF_LINKED_LIST_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>

# include "chuff_shared.h"
# include "chuff_huffman_tree.h"

struct LLNode
{
    HTNode* htnode;
    struct LLNode* next;
    bool is_head;
};

typedef struct LLNode LLNode;

/*
Initialise an LLNode:
- Allocates memory
- Sets htnode to point to a HTNode passed in as an argument
- Sets next to NULL
- Sets is_head to false
Takes:
- A pointer to a HTNode
Returns:
- A pointer to the initialised LLNode
*/
LLNode* llnode_init(HTNode* htnode);

/*
Inserts an LLNode into a linked list so nodes are sorted in terms of freq from smallest to largest
Takes:
- A pointer to an LLNode which is the head of a linked list
- A pointer to an LLNode to insert into the list
Returns:
- A pointer to the LLNode which is the head of the modified linked list
*/
LLNode* ll_insert(LLNode* head, LLNode* node);

/*
Gets the element of a linked list with the smallest frequency
Takes:
- A pointer to an LLNode which is the head of a linked list
Returns:
- A pointer to the LLNode which is the head of the list (assuming the list has been built with ll_build)
*/
LLNode* ll_get_min(LLNode* head);

/*
Deletes the node in a linked list with the smallest frequency (the head)
Takes:
- A pointer to an LLNode which is the head of a linked list
Returns:
- A pointer to the head of the modified list.
OR
- If the head is the only element in the list, returns a single LLNode
  with a pointer to an HTNode with freq set to -1 and cs set to -1
*/
LLNode* ll_delete_min(LLNode* head);

/*
Prints out linked list with nodes of form {freq, cs}
Takes:
- A pointer to an LLNode which is the head of a linked list
Returns:
- Void
*/
void ll_print(LLNode* head);

/*
For each character with non-zero frequency in ascii_counts, create an LLNode
Insert into a linked list in order sorted by frequency from smallest to lartest
Takes:
- A pointer to an LLNode which is the head of a linked list
Returns:
- A pointer to an LLNode which is the head of the modified linked list
*/
LLNode* ll_build(LLNode* head, int ascii_counts[]);

/*
For use with dfs()
Inserts an LLNode at the end a linked list
Takes:
- A pointer to an LLNode which is the head of a visited nodes list
- A pointer to an LLNode which is a node to insert into the visited list
Returns:
- A pointer to an LLNode which is the head of the modified linked list
*/
LLNode* ll_insert_end(LLNode* visited, LLNode* node);

/*
For use with dfs()
Inserts a node at the start of a linked list
Takes:
- A pointer to an LLNode which is the head of a to_visit list
- A pointer to an LLNode which is a node to insert into the to_visit list
Returns:
- A pointer to an LLNode which is the head of the modified linked list
*/
LLNode* ll_insert_start(LLNode* to_visit, LLNode* node);

/*
For use with dfs()
Deletes an element from the end of a linked list, freeing the memory for that node
Takes:
- A pointer to an LLNode which is the head of a visited list
Returns:
- A pointer to an LLNode which is the head of the modified visited list
*/
LLNode* ll_pop_end(LLNode* visited);

/*
For use with dfs()
Gets the last element of a linked list
Takes:
- A pointer to an LLNode which is the head of a visited list
Returns:
- A pointer to an LLNode which is last node in the list
*/
LLNode* ll_get_end(LLNode* visited);

/*
Separates the first element from a linked list but don't delete it and free its memory as in ll_delete_min()
Takes:
- A pointer to an LLNode which is the head of a to_visit list
Returns:
- A pointer to an LLNode which is the new head of the modified linked list.
(Use ll_get_end() to first get a reference to that last element which ll_pop_start() will separate from the list)
*/
LLNode* ll_pop_start(LLNode* to_visit);

/*
Builds a Huffman tree
Uses a lot of linked list functions though, so it goes in here
Takes:
- Pointer to an LLNode which is the head of a sorted linked list
Returns:
- A pointer to an LLNode containing a pointer to root of the Huffman tree
*/
LLNode* ht_build(LLNode* head);

# endif