#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


void q_swap_node(struct list_head *node1, struct list_head *node2);
/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head;
    head = (struct list_head *) malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    struct list_head *now, *safe;
    list_for_each_safe (now, safe, l)
        q_release_element(list_entry(now, element_t, list));
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *insert;
    insert = (element_t *) malloc(sizeof(element_t));
    if (!insert)
        return NULL;
    INIT_LIST_HEAD(&insert->list);
    insert->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    if (!(insert->value)) {
        free(insert);
        return NULL;
    }
    strncpy(insert->value, s, strlen(s) + 1);
    list_add(&(insert->list), head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *insert;
    insert = (element_t *) malloc(sizeof(element_t));
    if (!insert)
        return NULL;
    INIT_LIST_HEAD(&insert->list);
    insert->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    if (!(insert->value)) {
        free(insert);
        return NULL;
    }
    strncpy(insert->value, s, strlen(s) + 1);
    list_add_tail(&(insert->list), head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    struct list_head *node;
    element_t *removed;
    if (!head)
        return NULL;
    if (list_empty(head))
        return NULL;
    node = head->next;
    removed = list_entry(node, element_t, list);
    list_del_init(node);
    if (sp)  // fixed bug when sp is NULL
        strncpy(sp, removed->value, bufsize);
    return removed;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    struct list_head *node;
    element_t *removed;
    if (!head)
        return NULL;
    if (list_empty(head))
        return NULL;
    node = head->prev;
    removed = list_entry(node, element_t, list);
    list_del_init(node);
    if (sp)  // fixed bug when sp is NULL
        strncpy(sp, removed->value, bufsize);
    return removed;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head)
        return false;
    if (list_empty(head))
        return false;
    if (list_is_singular(head)) {
        q_remove_head(head, NULL, 0);
        return true;
    }

    int n = q_size(head) % 2
                ? q_size(head) / 2 + 1
                : q_size(head) / 2;  // if the size of the list is odd , you
                                     // need to add 1 to fix
    struct list_head *node;
    node = head;
    for (int i = 0; i < n; i++)
        node = node->next;
    list_del_init(node);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    struct list_head *now, *safe;
    if (!head)
        return false;
    if (list_empty(head))
        return false;
    list_for_each_safe (now, safe, head) {
        if (list_entry(now, element_t, list)->value ==
            list_entry(safe, element_t, list)->value)
            list_del(safe->prev);
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    struct list_head *n1, *n2,
        *ns;          // n1,n2 are node to swap , ns is start point
    ns = head->next;  // the first start point
    for (int i = 0; i < q_size(head) / 2; i++) {
        n1 = ns;
        n2 = ns->next;
        ns = ns->next->next;  // start point next time
        // swap
        n1->next = n2->next;
        n2->next = n1;
        n2->prev = n1->prev;
        n1->prev = n2;
        n2->prev->next = n2;
        n1->next->prev = n1;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    /*struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        node->next = node->prev;
        node->prev = safe;
    }
    safe = head->next;
    head->next = head->prev;
    head->prev = safe;*/
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    struct list_head *node_start, *node_k, *node_next, *tmp;
    node_start = head->next;
    for (int i = 0; i < q_size(head) / k; i++) {
        node_k = node_start;
        for (int j = 0; j < k - 1; j++)
            node_k = node_k->next;
        node_next = node_k->next;
        for (int j = 0; j < k / 2; j++) {
            q_swap_node(node_start, node_k);
            tmp = node_start;
            node_start = node_k->next;
            node_k = tmp->prev;
        }
        node_start = node_next;
    }
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}

void q_swap_node(struct list_head *node1, struct list_head *node2)
{
    struct list_head *safe1, *safe2;
    if (!node1 || !node2)
        return;
    safe1 = node1->next;
    safe2 = node2->prev;
    if (!safe1 || !safe2 || !(node1->prev) || !(node2->next))
        return;
    bool flag = (node1->next == node2);
    // swap
    node1->next = node2->next;
    node1->next->prev = node1;
    node2->prev = node1->prev;
    node2->prev->next = node2;
    if (flag) {
        node1->prev = node2;
        node2->next = node1;
    } else {
        node1->prev = safe2;
        safe2->next = node1;
        node2->next = safe1;
        safe1->prev = node2;
    }
}