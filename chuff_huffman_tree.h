// chuff_huffman_tree.h
// Ben Crabtree, 2021

# ifndef CHUFF_HUFFMAN_TREE_H
# define CHUFF_HUFFMAN_TREE_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>

struct HTNode
{
    struct HTNode* left;
    struct HTNode* right;
    int freq;
    int cs; // Int representation of ascii character (or sum of ascii characters)
    bool visited; // For use in dfs() to set huffman encodings
    char code; // 1 or 0 depending on if this is a left or right child of another node
};

typedef struct HTNode HTNode;

/*
Initialises a HTNode:
- Allocates memory
- Set frequency and cs using values passed in as arguments
- Sets left and right children to NULL
- Set visited to false
- Sets code to 'X'
Takes:
- An int freq
- An int cs
Returns:
- A pointer to the initialised HTNode
*/
HTNode* htnode_init(int freq, int cs);

/*
Initialise a parent HTNode:
- Allocates memory
- Sets left and right to point at HTNodes passed in as arguments
- Sets frequency to freq of left_child + freq of right_child
- Sets cs to cs of left_child + cs of right_child
- Sets visited to false
- Sets code to 'X'
Takes:
- A pointer to a HTNode left_child
- A pointer to a HTNode right_child
Returns:
- A pointer to the initialised HTNode
*/
HTNode* htparent_init(HTNode* left_child, HTNode* right_child);

/*
Prints Huffman tree as series of stumps (parent and two children)
Each node printed is of the form {freq, cs, code}
Takes:
- A pointer to a HTNode which is the root of a Huffman tree
Returns:
- Void
*/
void ht_print(HTNode* ht);

# endif