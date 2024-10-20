#include <stdio.h>

struct list_element {
    struct list_element *next;
    int data;
};

// Adding a new element to the head of the list
void list_add(struct list_element *list_head, struct list_element *new_element) {
    new_element->next = list_head->next;
    list_head->next = new_element;
}

// Removing an element from the list
void list_remove(struct list_element *list_head, struct list_element *element) {
    struct list_element *prev = list_head;
    struct list_element *current = list_head->next;

    // Traverse the list to find the element to remove
    while (current != NULL) {
        if (current == element) {
            prev->next = current->next;  // Unlink the element
            return;
        }

        prev = current;
        current = current->next;
    }
}

int main() {
    struct list_element c = { NULL, 0 };
    struct list_element b = { &c, 0 };
    //struct list_element a = { &b, 0 };
    //struct list_element *head = &a;

    return 0;
}
