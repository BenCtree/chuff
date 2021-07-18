// chuff_huffman_tree.c
// Ben Crabtree, 2021

# include "chuff_huffman_tree.h"

HTNode* htnode_init(int freq, int cs)
{
    HTNode* node = malloc( sizeof(HTNode) );
    node->left = NULL;
    node->right = NULL;
    node->freq = freq;
    node->cs = cs;
    node->visited = false;
    node->code = 'X';
    return node;
}

HTNode* htparent_init(HTNode* left_child, HTNode* right_child)
{
    HTNode* node = malloc( sizeof(HTNode) );
    node->left = left_child;
    node->right = right_child;
    node->freq = left_child->freq + right_child->freq;
    node->cs = left_child->cs + right_child->cs;
    node->visited = false;
    node->code = 'X';
    return node;
}

void ht_print(HTNode* ht)
{
    HTNode* curr = ht;
    if (curr->left != NULL && curr->right != NULL)
    {
        printf("       {%d, %d, %c}\n", curr->freq, curr->cs, curr->code);
        printf("          /   \\ \n");
        printf("{%d, %d, %c}", curr->left->freq, curr->left->cs, curr->left->code);
        printf("    {%d, %d, %c}\n", curr->right->freq, curr->right->cs, curr->right->code);
        printf("\n");
        ht_print(curr->left);
        ht_print(curr->right);
    }
}
