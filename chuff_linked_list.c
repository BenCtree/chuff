// chuff_linked_list.c
// Ben Crabtree, 2021

# include "chuff_linked_list.h"

LLNode* llnode_init(HTNode* htnode)
{
    LLNode* llnode = malloc(sizeof(LLNode));
    llnode->htnode = htnode;
    llnode->next = NULL;
    llnode->is_head = false;
    return llnode;
}

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
                curr->next = node;
                return head;
            }
        }
        else // if node freq <= current freq
        {
            // if curr is root, insert node before curr and set node to root
            if (curr->is_head == true)
            {
                node->next = curr;
                curr->is_head = false;
                node->is_head = true;
                return node;
            }
            else // if curr is not head, insert node before curr
            {
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
        printf("{%d, %d} ", curr->htnode->freq, curr->htnode->cs);
        if (curr->next != NULL)
        {
            printf("-> ");
        }
        curr = curr->next;
    }
    printf("\n");
}

LLNode* ll_build(LLNode* head, int ascii_counts[])
{
    for (int i = 0; i < NUM_ASCII; i++)
    {
        // We're only interested in ascii characters with frequency > 0
        if (ascii_counts[i] > 0)
        {
            int freq = ascii_counts[i];
            // If head node has not yet been initialised to proper values, do it.
            if (head->htnode->freq == -1 && head->htnode->cs == -1)
            {
                head->htnode->freq = freq;
                head->htnode->cs = i;
            }
            // If head has proper values, make a new node and insert it into the linked list.
            else
            {
                HTNode* htnode = htnode_init(freq, i);
                LLNode* llnode = llnode_init(htnode);
                head = ll_insert(head, llnode);
            }
        }
    }
    return head;
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

LLNode* ll_get_end(LLNode* visited)
{
    LLNode* curr = visited;
    while(curr->next != NULL)
    {
        curr = curr->next;
    }
    return curr;
}

LLNode* ll_pop_start(LLNode* to_visit)
{
    LLNode* node = to_visit;
    to_visit = to_visit->next;
    to_visit->is_head = true;
    node->next = NULL;
    node->is_head = false;
    return to_visit;
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
        head = ll_delete_min(head);
        // Make right htnode
        llnode = ll_get_min(head);
        HTNode* htnode_right = llnode->htnode;
        htnode_right->code = '1';
        head = ll_delete_min(head);
        // Generate parent htnode
        HTNode* htparent = htparent_init(htnode_left, htnode_right);
        // Check if head was last node in ll and a head with null values has been returned by ll_delete_min()
        if (head->htnode->freq == -1 && head->htnode->cs == -1)
        {
            head->htnode = htparent;
        }
        else
        {
            LLNode* new_llnode = llnode_init(htparent);
            head = ll_insert(head, new_llnode);
        }
    }
    return head; // Head is only node left in ll and contains pointer to root of Huffman tree
}
