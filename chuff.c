// chuff.c
// Ben Crabtree, 2021

/*
Huffman Coding Algorithm
- Input text file
- Count frequency of each ascii character in that text file - store in ascii_counts
  The indices of ascii_counts correspond to the ascii value of a given character
- Scan through ascii_counts, creating a Node for each char with non-zero frequency.
  Nodes have form: node = (Node* left, Node* right, int freq, int code, char* s)
  Insert pointer to each Node into a min heap, where root is pointer to node with min freq.
- Create Huffman binary tree from bottom up.
  Pop first two nodes node1, node2 from min heap (these will have the two least frequent chars)
  So node1 = (NULL, NULL, freq1, code1, char1), node2 = (NULL, NULL, freq2, code2, char2)
- Form a parent Node for them, Parent = (node1, node2, freq1+freq2, code3, char1char2)
  by concatenating the chars and adding their frequencies.
  This Parent node becomes the root of the Huffman Tree.
  Set: node1->code1 = 0 as node1 is left child, node2->code2 = 1 as node2 is right child.
- Insert pointer to Parent into min heap
  That way this new node will be included when continuing to build the Huffman tree.
- Keep popping two Node pointers from min heap, creating a parent for them
  and inserting them into Huffman Tree until min heap is empty
  Then our Huffman Tree will be complete.
*/

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>

# define NUM_ASCII 200
//# define true 1
//# define false 0

// Initialise list of ascii character counts to all 0
int ascii_counts[NUM_ASCII] = {};
char* huffman_encodings[NUM_ASCII];

int set_ascii_counts(FILE* fp)
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
    // Print ascii_counts
    for (int i = 0; i < NUM_ASCII; i++)
    {
        printf("%d ", ascii_counts[i]);
    }
    printf("\n");
}

// A count of the ascii characters in the file with non-0 frequency
//int count_chars()
//{
//    int count = 0;
//    for (int i = 0; i < NUM_ASCII; i++)
//    {
//        if (ascii_counts[i] > 0)
//        {
//            count++;
//        }
//    }
//    return count;
//}

struct HTNode
{
    //struct HTNode* parent;
    struct HTNode* left;
    struct HTNode* right;
    int freq;
    int cs;
    bool visited;
    char code; // 1 or 0 depending on if this is a left or right child of another node
    //char* cs; // Use int to represent concatenated chars - just add ascii values of chars
    //int code;
};

typedef struct HTNode HTNode;

//HTNode* htnode_init(int freq, char* cs)
HTNode* htnode_init(int freq, int cs)
{
    HTNode* node = malloc( sizeof(HTNode) );
    //node->parent = NULL;
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
    // Use calloc to make sure visited attribute is zeroed out 
    HTNode* node = malloc( sizeof(HTNode) );
    node->left = left_child;
    node->right = right_child;
    node->freq = left_child->freq + right_child->freq;
    //node->code = -1;
    node->cs = left_child->cs + right_child->cs;
    node->visited = false;
    node->code = 'X';
    //left_child->parent = node;
    //right_child->parent = node;
    //char string[30] = " ";
    //char* new_cs;
    //new_cs = strcat(string, left_child->cs);
    //new_cs = strcat(string, right_child->cs);
    //printf("htparent_init new_cs: %s", new_cs);
    //node->cs = new_cs;
    return node;
}

struct LLNode
{
    HTNode* htnode;
    struct LLNode* next;
    bool is_head;
};

typedef struct LLNode LLNode;

LLNode* llnode_init(HTNode* htnode)
{
    LLNode* llnode = malloc(sizeof(LLNode));
    llnode->htnode = htnode;
    llnode->next = NULL;
    llnode->is_head = false;
    return llnode;
}

// Inserts an LLNode into LinkedList so LLNodes are sorted in terms of freq from smallest to largest
//LLNode* ll_insert(struct LinkedList* ll, LLNode* node)
LLNode* ll_insert(LLNode* head, LLNode* node)
{
    LLNode* curr = head;
    LLNode* prev;
    // Iterate through linked list
    while (curr != NULL)
    {
        // If node freq > current freq
        if (node->htnode->freq > curr->htnode->freq)
        {
            // If there is a nod after curr, increment curr
            if (curr->next != NULL)
            {
                prev = curr;
                curr = curr->next;
            }
            // Otherwise, insert node after curr
            else
            {
                //printf("INSERT NODE %c after curr %c (end of list)", (char)node->htnode->cs, (char)curr->htnode->cs);
                curr->next = node;
                return head;
            }
        }
        else // if node freq <= current freq
        {
            // if curr is root, insert node before curr and set node to root
            if (curr->is_head == true)
            {
                //printf("INSERT NODE %c before curr %c and make it head \n", (char)node->htnode->cs, (char)curr->htnode->cs);
                node->next = curr;
                curr->is_head = false;
                node->is_head = true;
                return node;
            }
            else // if curr is not head, insert node before curr
            {
                //printf("INSERT NODE %c before curr %c \n", (char)node->htnode->cs, (char)curr->htnode->cs);
                node->next = curr;
                prev->next = node;
                return head;
            }
        }
    }
}

LLNode* ll_get_min(LLNode* head)
{
    LLNode* min = head;
    return min;
}

LLNode* ll_delete_min(LLNode* head)
{
    // If head is last node in ll, set its attributes to null values and return it
    if (head->next == NULL)
    {
        //HTNode* htnull = htnode_init(-1, " ");
        HTNode* htnull = htnode_init(-1, -1);
        LLNode* llnull = llnode_init(htnull);
        LLNode* temp = head;
        head = llnull;
        llnull->is_head = true;
        free(temp);
        return head;
    }
    LLNode* temp = head;
    head = head->next;
    head->is_head = true;
    free(temp);
    return head;
}

void ll_print(LLNode* head)
{
    LLNode* curr = head;
    while (curr != NULL)
    {
        //printf("{%d, %c} ", curr->htnode->freq, curr->htnode->cs);
        printf("{%d, %d} ", curr->htnode->freq, curr->htnode->cs);
        //printf("{%d, %s} ", curr->htnode->freq, curr->htnode->cs);
        if (curr->next != NULL)
        {
            printf("-> ");
        }
        curr = curr->next;
    }
    printf("\n");
}

// Search for 
//bool ll_node_in_list(HTNode* ht_node)

// Returns a linked list containing indices of ascii_counts elems
// sorted in order of elem size (char frequency) from smallest to largest
//void ll_build(struct LinkedList* ll, LLNode* node_array)
LLNode* ll_build(LLNode* head)
{
    for (int i = 0; i < NUM_ASCII; i++)
    {
        // We're only interested in ascii characters with frequency > 0
        if (ascii_counts[i] > 0)
        {
            //char c = (char)i;
            //printf("ll_build print c: %c\n", c);
            //char* cs = &c;
            //cs[0] = c;
            //printf("ll_build print cs: %s\n", cs);
            int freq = ascii_counts[i];
            // If head node has not yet been initialised to proper values, do it.
            //if (head->htnode->freq == -1 && head->htnode->cs == " ")
            if (head->htnode->freq == -1 && head->htnode->cs == -1)
            {
                head->htnode->freq = freq;
                //head->htnode->cs = cs;
                head->htnode->cs = i;
            }
            // If head has proper values, make a new node and insert it into the linked list.
            else
            {
                //HTNode* htnode = htnode_init(freq, cs);
                HTNode* htnode = htnode_init(freq, i);
                LLNode* llnode = llnode_init(htnode);
                head = ll_insert(head, llnode);
                //printf("head->htnode->cs: %s\n", head->htnode->cs);
            }
        }
    }
    return head;
}

LLNode* ht_build(LLNode* head)
{
    // Return root of Huffman tree
    while (head->next != NULL) // Loop until there is one node left in ll
    {
        // Make left htnode
        LLNode* llnode = ll_get_min(head);
        HTNode* htnode_left = llnode->htnode;
        htnode_left->code = '0';
        //htnode_left->code = 0;
        head = ll_delete_min(head);
        // Make right htnode
        llnode = ll_get_min(head);
        HTNode* htnode_right = llnode->htnode;
        htnode_right->code = '1';
        //htnode_right->code = 1;
        head = ll_delete_min(head);
        // Generate parent htnode
        HTNode* htparent = htparent_init(htnode_left, htnode_right);
        // Check if head was last node in ll and a head with null values has been returned by ll_delete_min()
        //if (head->htnode->freq == -1 && head->htnode->cs == " ")
        if (head->htnode->freq == -1 && head->htnode->cs == -1)
        {
            head->htnode = htparent;
        }
        else
        {
            LLNode* new_llnode = llnode_init(htparent);
            head = ll_insert(head, new_llnode);
        }
        //printf("New ll... \n");
        //ll_print(head);
    }
    return head; // Head is only node left in ll and contains pointer to root of Huffman tree
}

void ht_print(HTNode* ht)
{
    HTNode* curr = ht;
    if (curr->left != NULL && curr->right != NULL)
    {
        //printf("Parent: {%d, %c}, Left: {%d, %c}, Right: {%d, %c}\n",
        printf("       {%d, %d, %c}\n", curr->freq, curr->cs, curr->code);
        printf("          /   \\ \n");
        printf("{%d, %d, %c}", curr->left->freq, curr->left->cs, curr->left->code);
        printf("    {%d, %d, %c}\n", curr->right->freq, curr->right->cs, curr->right->code);
        printf("\n");
        ht_print(curr->left);
        ht_print(curr->right);
    }
}

char* set_binary_string(LLNode* visited)
{
    //printf("Entered set_binary_string\n");
    LLNode* curr = visited;
    int len_binary_string = 0;
    while (curr != NULL)
    {
        len_binary_string++;
        curr = curr->next;
    }
    //printf("len_binary_string: %d\n", len_binary_string);
    //printf("Allocating memory for binarystring\n");

    // We have an X at the head of each list which we don't want to include
    // So only need room for the rest of the binary chars
    char* binary_string = malloc( (len_binary_string) * sizeof(char) );
    //printf("Set null byte of binary string\n");
    //printf("length of binary_string: %d\n", sizeof(binary_string)/sizeof(char));
    binary_string[len_binary_string-1] = '\0';
    int i = 0;
    //printf("Filling binary string with binary encodings\n");
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
    //printf("binary string: %s\n", binary_string);
    //printf("Returning binary string, exiting set_binary_string\n");
    return binary_string;
}

LLNode* ll_insert_end(LLNode* visited, LLNode* node)
{
    LLNode* curr = visited;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = node;
    return visited;
}

LLNode* ll_insert_start(LLNode* to_visit, LLNode* node)
{
    node->next = to_visit;
    node->is_head = true;
    to_visit->is_head = false;
    return node;
}

// Pop element off end of linked list
LLNode* ll_pop_end(LLNode* visited)
{
    LLNode* prev;
    LLNode* curr = visited;
    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    LLNode* temp = curr;
    prev->next = NULL;
    free(temp);
    return visited;
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

//LLNode* check_reached_leaf(bool reached_leaf, LLNode* visited)
LLNode* check_reached_leaf(LLNode* visited, HTNode* node)
{
    //printf("Entered check_reached_leaf\n");
    if (node->left == NULL && node->right == NULL)
    {
        //printf("Node is leaf - set binary string\n");
        char* binary_string = set_binary_string(visited);
        int index = node->cs;
        huffman_encodings[index] = binary_string;
        //printf("Pop node off end of visited");
        visited = ll_pop_end(visited);
        //printf("Return modified visited, exit check_reached_leaf\n");
        return visited;
    }
    //printf("Return unmodified visited, exit check_reached_leaf\n");
    return visited;
}

LLNode* ll_get_end(LLNode* visited)
{
    LLNode* curr = visited;
    while(curr->next != NULL)
    {
        curr = curr->next;
    }
    return curr;
}

//bool htnodes_are_siblings(HTNode* htnode1, HTNode* htnode2)
//{
//    if (htnode1->parent == htnode2->parent)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

// Separate first element from ll but don't delete it as in ll_delete_min(). Return new head of ll.
LLNode* ll_pop_start(LLNode* to_visit)
{
    LLNode* node = to_visit;
    to_visit = to_visit->next;
    to_visit->is_head = true;
    node->next = NULL;
    node->is_head = false;
    return to_visit;
}

bool children_visited(LLNode* visited)
{
    //printf("Inside children_visited")
    LLNode* visited_end = ll_get_end(visited);
    HTNode* ht_node = visited_end->htnode;
    //printf("Left visited: %d, Right visited: %d\n", ht_node->left->visited, ht_node->right->visited);
    if (ht_node->left->visited == true && ht_node->right->visited == true)
    {
        return true;
    }
    return false;
}

void dfs(LLNode* visited, LLNode* to_visit)
{
    //printf("Inside dfs() now. \n");
    //HTNode* ht_node = visited->htnode;

    // When we add a node to visited we check to see if it has children
    // If it does, we add them to start of to_visit (right first, then left)
    //printf("Expanding nodes. \n");
    to_visit = expand_nodes(to_visit, visited->htnode);
    //printf("Finished expanding nodes. \n");
    //printf("Checking if leaf reached. \n");
    //printf("reached_leaf: %d", reached_leaf);
    visited = check_reached_leaf(visited, visited->htnode);
    //printf("Finished checking if leaf reached. \n");
    
    //printf("\n");
    //printf("Visited: \n");
    //ll_print(visited);
    //printf("To visit: \n");
    //ll_print(to_visit);
    //printf("\n");

    if (to_visit->htnode->freq == -1 && to_visit->htnode->cs == -1 && to_visit->htnode->code == 'X')
    {
        return;
    }
    //LLNode* curr = ll_get_end(to_visit);
    //while (curr != NULL)
    
    //while (true)
    LLNode* ll_node;
    while (to_visit->htnode->freq != -1 && to_visit->htnode->cs != -1 && to_visit->htnode->code != 'X')
    {
        // TO DO: ll_pop_start
        //printf("Entered while loop in dfs()\n");
        //ll_print(to_visit);
        //printf("Pop first element of to_visit\n");
        //LLNode* ll_node = ll_get_min(to_visit);
        ll_node = ll_get_min(to_visit);
        //printf("Popped first elem of to_visit: {%d, %d}\n", ll_node->htnode->freq, ll_node->htnode->cs);
        
        to_visit = ll_pop_start(to_visit);
        
        //printf("Get end node of visited\n");
        //LLNode* visited_end = ll_get_end(visited);
        //printf("End node of visited: {%d, %d}\n", visited_end->htnode->freq, visited_end->htnode->cs);
        
        while (children_visited(visited) == true)
        {
            //printf("Children visited\n");
            visited = ll_pop_end(visited);
            //printf("Visited: \n");
            //ll_print(visited);
        }
        //if (htnodes_are_siblings(ll_node->htnode, visited_end->htnode))
        //{
        //    printf("ht nodes are siblings\n");
        //    visited = ll_pop_end(visited);
        //    visited_end = ll_get_end(visited);
        //    ll_node->htnode->visited = true;
        //    visited = ll_insert_end(visited, ll_node);
        //    to_visit = expand_nodes(to_visit, ll_node->htnode);
        //    visited = check_reached_leaf(visited, ll_node->htnode);
        //    // DO check_children_visited BEFORE htnodes_are_siblings check?
        //    visited = check_children_visited(visited, visited_end);
        //}
        //else
        //{
        //    printf("htnodes are not siblings\n");
            visited = ll_insert_end(visited, ll_node);
            ll_node->htnode->visited = true;
            to_visit = expand_nodes(to_visit, ll_node->htnode);
            visited = check_reached_leaf(visited, ll_node->htnode);
            //visited = check_children_visited(visited, visited_end);        
        //}
        
        //printf("Visited: ");
        //ll_print(visited);
        //printf("To visit: ");
        //ll_print(to_visit);
        //printf("\n");
        //printf("to_visit freq: %d, to_visit cs: %d, to_visit code: %c\n", to_visit->htnode->freq, to_visit->htnode->cs, to_visit->htnode->code);
    }
    //printf("Left dfs while loop\n");
    //printf("Exiting dfs()\n");
    return;
}

void set_huffman_encodings(HTNode* ht)
{
    // Create head of linked list storing visited nodes, make root of ht the head
    //HTNode* htnull_v = htnode_init(-1, -1);
    LLNode* visited = llnode_init(ht);
    visited->is_head = true;
    // Create head of linked list storing nodes to visit
    HTNode* htnull = htnode_init(-1, -1);
    LLNode* to_visit = llnode_init(htnull);
    to_visit->is_head = true;
    //printf("Inside set_huffman_encodings() ENTERING dfs() \n");
    dfs(visited, to_visit);
    //printf("Inside set_huffman_encodings() EXITING dfs() \n");
    return;
}

void print_huffman_encodings(char* huffman_encodings[], int ascii_counts[])
{
    //printf("Entered print_huffman_encodings\n");
    for (int i = 0; i < NUM_ASCII; i++)
    {
        if (huffman_encodings[i] != NULL)
        {
            printf("Char: %c, Frequency: %d, Encoding: %s\n", i, ascii_counts[i], huffman_encodings[i]);
        }
    }
}

int get_max_len_binary_string(char* huffman_encodings[])
{
    //printf("Entered get_max_len_binary_string()\n");
    //printf("Entered print_huffman_encodings\n");
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
    //printf("Max len binary string: %d\n", length);
    return length;
}

char* encode(FILE* fp, char* huffman_encodings[], int num_chars, int max_len_binary_string)
{
    //printf("Entered encode()\n");
    // Get chars in text file one at a time and concat binary encodings of each to encoded_text string buffer
    //int num_chars = count_chars();
    //printf("Num chars: %d\n", num_chars);
    char* encoded_text = malloc( num_chars * max_len_binary_string * sizeof(char) );
    // Set null byte of encoded_text string buffer
    //encoded_text[(num_chars * max_len_binary_string) - 1] = '\0';
    // Set first position of encoded_text string buffer to null byte
    // as strncat will overwrite this and set new null byte
    //encoded_text[0] = 'X';
    encoded_text[0] = '\0';
    //printf("Initial encoded_text: %s\n", encoded_text);
    //encoded_text[0] = ' ';
    // Pointer to start of string buffer
    //char* encoded_text;
    char c;
    while( (c = fgetc(fp)) != EOF )
    {
        //printf("Entered encode while loop.\n");
        int index = (int)c;
        char* binary_string = huffman_encodings[index];
        //printf("Binary string for %c: %s\n", c, binary_string);
        encoded_text = strncat(encoded_text, binary_string, strlen(binary_string));
        //printf("encoded_text so far: %s\n", encoded_text);
    }
    return encoded_text;
}

//char* set_null_string(char* string)
//{
//    for (int i = 0; i < strlen(string); i++)
//    {
//        string[i] = '\0';
//    }
//    return string;
//}

char* decode(char* encoded_text, char* huffman_encodings[], int max_len_binary_string)
{
    char* decoded_text = malloc( strlen(encoded_text) * sizeof(char) );
    decoded_text[0] = '\0';
    char* binary_string = calloc( (max_len_binary_string + 1), sizeof(char) );
    binary_string[0] = '\0';
    for (int i = 0; i < strlen(encoded_text); i++)
    //for (int i = 0; i < 10; i++)
    {
        char bin = encoded_text[i];
        binary_string = strncat(binary_string, &bin, 1);
        //printf("binary_string so far: %s\n", binary_string);
        // MORE EFFICIENT WITH huffman_encodings AS A HASH TABLE? NO NESTED FOR LOOP NECESSARY
        for (int j = 0; j < NUM_ASCII; j++)
        {
            bool char_match = false;
            //CHECK huffman_encodings[j] is actually a string before using strncmp
            if (huffman_encodings[j] != NULL)
            {
                //printf("binary_string so far: %s\n", binary_string);
                //printf("huffman_encoding %d: %s\n", j, huffman_encodings[j]);
                //printf("Match: %d\n", strncmp(binary_string, huffman_encodings[j], strlen(huffman_encodings[j])));
                //printf("\n");
                if (strncmp(binary_string, huffman_encodings[j], strlen(huffman_encodings[j])) == 0)
                {
                    char c = (char)j;
                    decoded_text = strncat(decoded_text, &c, 1);
                    char* temp = binary_string;
                    binary_string = calloc( (max_len_binary_string + 1), sizeof(char) );
                    free(temp);
                    //binary_string = set_null_string(binary_string);
                    //free(binary_string);
                    //char* binary_string = malloc( (max_len_binary_string + 1) * sizeof(char) );
                    char_match = true;
                    if (char_match) break;
                }
                if (char_match) break;
            }
            if (char_match) break;
        }
        //printf("decoded_text so far: %s\n", decoded_text);
    }
    return decoded_text;
}

int main(int argc, char* argv[])
{

    if (argc < 2)
    {
        printf("Please enter a text file name. \n");
        return 1;
    }

    // Get file name and open file
    char* filename = argv[1];
    FILE* fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Could no open file. \n");
        return 1;
    }

    int num_chars = set_ascii_counts(fp);
    print_ascii_counts();
    printf("\n");

    //int num_chars = count_chars();
    //MinHeap* mh = mh_init(num_chars);
    
    // Allocate memory for min_heap - an array of pointers to Nodes
    //Node** min_heap = malloc( num_chars * sizeof(Node*) );


    // Initialise the head of the linked list
    HTNode* htnode = htnode_init(-1, -1);
    //HTNode* htnode = htnode_init(-1, " ");
    LLNode* head = llnode_init(htnode);
    head->is_head = true;
    // Build linked list storing HTNode pointers sorted by character frequency, smallest to largest.
    head = ll_build(head);
    ll_print(head);
    printf("\n");
    
    //LLNode* ll_node = ll_get_min(head);
    //HTNode* ht_node = ll_node->htnode;
    //head = ll_delete_min(head);
    //ll_print(head);
    //printf("{%d, %d, %c} \n", ht_node->freq, ht_node->code, ht_node->cs);

    head = ht_build(head);
    // Get pointer to root of Huffman tree
    HTNode* ht = head->htnode;
    ht_print(ht);
    printf("\n");

    set_huffman_encodings(ht);
    print_huffman_encodings(huffman_encodings, ascii_counts);

    // Set file pointer back to start of file
    //rewind(fp);
    fseek(fp, 0, SEEK_SET);
    int max_len_binary_string = get_max_len_binary_string(huffman_encodings);
    //printf("Max len binary string: %d\n", max_len_binary_string);
    char* encoded_text = encode(fp, huffman_encodings, num_chars, max_len_binary_string);
    printf("\nEncoded text: \n%s\n", encoded_text);
    //printf("Strlen encoded text: %d\n", strlen(encoded_text));
    //printf("Capacity of encoded_text (max num chars): %d\n", sizeof(*encoded_text));
    // Realloc encoded_text to get rid of extra space?

    char* decoded_text = decode(encoded_text, huffman_encodings, max_len_binary_string);
    printf("\nDecoded text: \n%s\n", decoded_text);
    
    // Close file
    fseek(fp, 0, SEEK_SET);
    fclose(fp);
    return 0;

}


//# define ALPHABET_LEN 26
//int char_counts[ALPHABET_LEN];
//char* alphabet = ""
//int num_unique_chars;
//char all_chars[] = "`~1!2@3#4$5%6^7&8*9(0)-_=+qQwWeErRtTyYuUiIoOpP[{]}\\|aAsSdDfFgGhHjJkKlL;:'\"zZxXcCvVbBnNmM,<.>/? ";
//int char_counts[95];
//void get_char_frequencies(char all_chars[], int char_counts[], char c)
//{
//}
//int count_unique_chars(char c)

//struct char_count* hash_table[100];


//void ll_print(struct LinkedList* ll)
//{
//    struct LLNode* curr = ll->root;
//    for(curr; curr->next != NULL; curr = curr->next)
//    {
//        printf("{ freq: %d, char: %c } -> ", curr->freq, (char)curr->index);
//    }
//    printf("\n");
//}


//struct HuffmanTree
//{
//    Node* root;
//    //struct Node* left;
//    //struct Node* right;
//};




//int make_nodes()
//{
//    //int min_count;
//    int count = 0;
//    for (int i = 0; i < NUM_ASCII; i++)
//    {
//        if (ascii_counts[i] != 0)
//        {
//            struct Node node = {.parent = NULL, 
//                                .left = NULL, 
//                                .right = NULL, 
//                                .count = ascii_counts[i], 
//                                .code = -1, 
//                                .c = (char) i};
//            
//        }
//    }
//    return count;
//}

//int num_chars = count_chars();
// int sorted_indices[];
//struct Node* node_array[num_chars];


    //int num_chars = count_chars();
    //printf("num_chars: %d \n", num_chars);
    // Create empty linked list
    //struct LinkedList sorted_indices_ll = {.root = NULL};
    // Allocate memory for array of node structs
    //struct LLNode* node_array = malloc(num_chars * sizeof(struct LLNode));
    //ll_build();

    // WRITE FUNCTION TO FREE MEMORY FOR EACH NODE IN LL
    //free_ll(head);

    //free(node_array);

        //printf("c: ");
        //putchar(c);
        //printf("\n");
        //printf("c as int: %d", (int)c);
        //printf("\n");
        //count_chars();

    //printf("\n");
    //printf("%s", all_chars);
    //printf("ascii test: %c\n", 'b'-1);

    /*LLNode* curr = visited;
    while (curr != NULL)
    {
        //curr = visited;
        // If current node has no children, it is a leaf and we extract byte string from visited list
        if (curr->htnode->left == NULL && curr->htnode->right == NULL)
        {
            char* byte_string = set_byte_string(visited);
            int index = curr->htnode->cs;
            // store pointer to byte_string in huffman_encodings array
            huffman_encodings[index] = byte_string;
            // pop curr off end of list
            visited = ll_pop(visited);
        }
        if (ll_node_in_list(curr->htnode->left) == false)
        {
            LLNode* node = llnode_init(curr->htnode->left);
            visited->next = node;
            //dfs(curr->htnode->left, visited);
        }
        if (ll_node_in_list(curr->htnode->right) == false)
        {
            LLNode* node = llnode_init(curr->htnode->right);
            visited->next = node;
            //dfs(curr->htnode->right, visited);
        }
        curr = curr->next;
    }
}*/
