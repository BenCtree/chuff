// chuff.c
// Ben Crabtree, 2021

/*
Huffman Coding Algorithm
- Input text file
- Count frequency of each ascii character in that text file - store in ascii_counts array.
  The indices of ascii_counts correspond to the ascii value of a given character.
- Scan through ascii_counts, creating a node struct for each char with non-zero frequency.
  Insert into a linked list in sorted order.
- Create Huffman binary tree from bottom up.
  Take first two nodes LLNode1 & LLNode2 from linked list (these will have the two least frequent chars)
  and make two HTNodes of form (left_child, right_child, freq, code, char)
  Then delete LLNode1 & LLNode2 from start of linked list.
  So HTNode1 = (NULL, NULL, freq1, code1, char1), HTNode2 = (NULL, NULL, freq2, code2, char2)
  We assign: code1 = 0 as HTNode1 is left child, code2 = 1 as HTNode2 is right child.
- Form a parent HTNode for them, HTNode3 = (HTNode1, HTNode2, freq1+freq2, code3, char1char2)
  by concatenating the chars and adding their frequencies.
- Make LLNode for HTNode3 and insert into linked list (in order of frequency).
  That way this new node will be included when building the Huffman tree.
- Keep taking two LLNodes from start of linked list and making them into HTNodes
  and inserting them into Huffman Tree, then deleting them from linked list until linked list is empty.
  Then our Huffman Tree will be complete.
*/

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>

# include "chuff_shared.h"
# include "chuff_linked_list.h"
# include "chuff_huffman_tree.h"


//# define NUM_ASCII 200

// Initialise list of ascii character counts to all 0
int ascii_counts[NUM_ASCII] = {};
char* huffman_encodings[NUM_ASCII];

int set_ascii_counts(FILE* fp, int ascii_counts[])
{
    int num_chars = 0;
    // Get chars in text file one at a time and updates count in ascii_counts
    char c;
    while( (c = fgetc(fp)) != EOF )
    {
        int index = (int)c;
        ascii_counts[index] += 1;
        num_chars++;
    }
    return num_chars;
}

void print_ascii_counts()
{
    for (int i = 0; i < NUM_ASCII; i++)
    {
        printf("%d ", ascii_counts[i]);
    }
    printf("\n");
}

char* set_binary_string(LLNode* visited)
{
    LLNode* curr = visited;
    int len_binary_string = 0;
    while (curr != NULL)
    {
        len_binary_string++;
        curr = curr->next;
    }
    // We have an X at the head of each list which we don't want to include
    // So only need room for the rest of the binary chars
    char* binary_string = malloc( (len_binary_string) * sizeof(char) );
    binary_string[len_binary_string-1] = '\0';
    int i = 0;
    curr = visited;
    while (curr != NULL)
    {
        if (curr->htnode->code != 'X')
        {
            binary_string[i] = curr->htnode->code;
            i++;
        }
        curr = curr->next;
    }
    return binary_string;
}

LLNode* expand_nodes(LLNode* to_visit, HTNode* node)
{
    // if node has children, insert them to at head of to_visit (right, then left)
    if (node->left != NULL && node->right != NULL)
    {
        LLNode* llnode_right = llnode_init(node->right);
        LLNode* llnode_left = llnode_init(node->left);
        to_visit = ll_insert_start(to_visit, llnode_right);
        to_visit = ll_insert_start(to_visit, llnode_left);
        return to_visit;
    }
    else
    {
        return to_visit;
    }
}

LLNode* check_reached_leaf(LLNode* visited, HTNode* node)
{
    if (node->left == NULL && node->right == NULL)
    {
        char* binary_string = set_binary_string(visited);
        int index = node->cs;
        huffman_encodings[index] = binary_string;
        visited = ll_pop_end(visited);
        return visited;
    }
    return visited;
}

bool children_visited(LLNode* visited)
{
    LLNode* visited_end = ll_get_end(visited);
    HTNode* ht_node = visited_end->htnode;
    if (ht_node->left->visited == true && ht_node->right->visited == true)
    {
        return true;
    }
    return false;
}

void dfs(LLNode* visited, LLNode* to_visit)
{
    // When we add a node to visited we check to see if it has children
    // If it does, we add them to start of to_visit (right first, then left)
    to_visit = expand_nodes(to_visit, visited->htnode);
    visited = check_reached_leaf(visited, visited->htnode);
    if (to_visit->htnode->freq == -1 && to_visit->htnode->cs == -1 && to_visit->htnode->code == 'X')
    {
        return;
    }
    LLNode* ll_node;
    while (to_visit->htnode->freq != -1 && to_visit->htnode->cs != -1 && to_visit->htnode->code != 'X')
    {
        ll_node = ll_get_min(to_visit);
        to_visit = ll_pop_start(to_visit);
        while (children_visited(visited) == true)
        {
            visited = ll_pop_end(visited);
        }
        visited = ll_insert_end(visited, ll_node);
        ll_node->htnode->visited = true;
        to_visit = expand_nodes(to_visit, ll_node->htnode);
        visited = check_reached_leaf(visited, ll_node->htnode);
    }
    return;
}

void set_huffman_encodings(HTNode* ht)
{
    // Create head of linked list storing visited nodes, make root of ht the head
    LLNode* visited = llnode_init(ht);
    visited->is_head = true;
    // Create head of linked list storing nodes to visit
    HTNode* htnull = htnode_init(-1, -1);
    LLNode* to_visit = llnode_init(htnull);
    to_visit->is_head = true;
    dfs(visited, to_visit);
    return;
}

void print_huffman_encodings(char* huffman_encodings[], int ascii_counts[])
{
    for (int i = 0; i < NUM_ASCII; i++)
    {
        if (huffman_encodings[i] != NULL)
        {
            if (i == 9 || i == 11)
                printf("Char: TAB      Frequency: %d      Encoding: %s\n", ascii_counts[i], huffman_encodings[i]);
            else if (i == 10 || i == 13)
                printf("Char: NEW LINE Frequency: %d      Encoding: %s\n", ascii_counts[i], huffman_encodings[i]);
            else if (i == 32)
                printf("Char: SPACE    Frequency: %d      Encoding: %s\n", ascii_counts[i], huffman_encodings[i]);
            else
                printf("Char: %c        Frequency: %d      Encoding: %s\n", i, ascii_counts[i], huffman_encodings[i]);
        }
    }
}

int get_max_len_binary_string(char* huffman_encodings[])
{
    int length = 0;
    for (int i = 0; i < NUM_ASCII; i++)
    {
        if (huffman_encodings[i] != NULL)
        {
            if (strlen(huffman_encodings[i]) > length)
            {
                length = strlen(huffman_encodings[i]);
            }
        }
    }
    return length;
}

char* encode(FILE* fp, char* huffman_encodings[], int num_chars, int max_len_binary_string)
{
    // Get chars in text file one at a time and concat binary encodings of each to encoded_text string buffer
    char* encoded_text = malloc( num_chars * max_len_binary_string * sizeof(char) );
    // Set first position of encoded_text string buffer to null byte
    // as strncat will overwrite this and set new null byte
    encoded_text[0] = '\0';
    char c;
    while( (c = fgetc(fp)) != EOF )
    {
        int index = (int)c;
        char* binary_string = huffman_encodings[index];
        encoded_text = strncat(encoded_text, binary_string, strlen(binary_string));
    }
    return encoded_text;
}

char* decode(char* encoded_text, char* huffman_encodings[], int max_len_binary_string)
{
    char* decoded_text = malloc( strlen(encoded_text) * sizeof(char) );
    decoded_text[0] = '\0';
    char* binary_string = calloc( (max_len_binary_string + 1), sizeof(char) );
    binary_string[0] = '\0';
    int len_encoded = strlen(encoded_text);
    for (int i = 0; i < len_encoded; i++)
    {
        char bin = encoded_text[i];
        binary_string = strncat(binary_string, &bin, 1);
        // MORE EFFICIENT WITH huffman_encodings AS A HASH TABLE? NO NESTED FOR LOOP NECESSARY
        for (int j = 0; j < NUM_ASCII; j++)
        {
            bool char_match = false;
            //CHECK huffman_encodings[j] is actually a string before using strncmp
            if (huffman_encodings[j] != NULL)
            {
                if (strncmp(binary_string, huffman_encodings[j], strlen(huffman_encodings[j])) == 0)
                {
                    char c = (char)j;
                    decoded_text = strncat(decoded_text, &c, 1);
                    char* temp = binary_string;
                    binary_string = calloc( (max_len_binary_string + 1), sizeof(char) );
                    free(temp);
                    char_match = true;
                    if (char_match) break;
                }
            }
        }
    }
    return decoded_text;
}

int main(int argc, char* argv[])
{

    // Check if text file name provided
    if (argc < 2)
    {
        printf("Please enter a text file name.\n");
        return 1;
    }

    // Check if too many arguments provided
    if (argc > 2)
    {
        printf("Too many arguments.\n");
        return 1;
    }

    // Get file name and open file
    char* filename = argv[1];
    FILE* fp = fopen(filename, "r");

    // Check if file exists
    if (fp == NULL)
    {
        printf("Could no open file.\n");
        return 1;
    }

    // Check if file is empty
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0)
        {
            printf("File is empty.\n");
            return 1;
        }
    }

    // Get the frequency counts of each character in text file
    fseek(fp, 0, SEEK_SET);
    int num_chars = set_ascii_counts(fp, ascii_counts);
    //print_ascii_counts();
    //printf("\n");

    // Initialise the head of the linked list
    HTNode* htnode = htnode_init(-1, -1);
    LLNode* head = llnode_init(htnode);
    head->is_head = true;
    
    // Build linked list storing HTNode pointers sorted by character frequency, smallest to largest.
    head = ll_build(head, ascii_counts);
    //ll_print(head);
    //printf("\n");
    
    // Build Huffman Tree
    head = ht_build(head);
    // Get pointer to root of Huffman tree
    HTNode* ht = head->htnode;
    //ht_print(ht);
    //printf("\n");

    // Generate the binary string encodings for each character in text file
    set_huffman_encodings(ht);
    printf("Huffman encodings:\n");
    print_huffman_encodings(huffman_encodings, ascii_counts);

    // Set file pointer back to start of file
    fseek(fp, 0, SEEK_SET);
    int max_len_binary_string = get_max_len_binary_string(huffman_encodings);
    
    // Encode text file
    char* encoded_text = encode(fp, huffman_encodings, num_chars, max_len_binary_string);
    printf("\nEncoded text: \n%s\n", encoded_text);

    // Decode text file
    char* decoded_text = decode(encoded_text, huffman_encodings, max_len_binary_string);
    printf("\nDecoded text: \n%s\n", decoded_text);
    
    // Close file
    fseek(fp, 0, SEEK_SET);
    fclose(fp);
    return 0;
}
