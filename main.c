#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Autor: Senger, Jonas; Matrikelnummer: 221200070; Datum: 08.01.2022
 */

struct Node {
    char *data;
    struct Node *next;
};

void insertLine(struct Node **head_node_ref, char *line);

void unsortedInsertLine(struct Node **head_node_ref, char *line);

void merge(struct Node **head_node_ref, struct Node **node_ref);

/**
 * Duplicates string.
 * @param s
 * @return
 */
char *strdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p) {
        memcpy(p, s, size);
    }
    return p;
}

/**
 * Seperates string on delimiter.
 * @param stringp
 * @param delim
 * @return
 */
char *strsep(char **stringp, const char *delim) {
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0;
    }
    return rv;
}

/**
 * Splits input into multiple string along found delimiters and inputs lines into list.
 * @param head_node_ref
 * @param delim
 * @param input
 * @param bound the upper bound for lines split and put into the given list
 */
void read_and_split(struct Node **head_node_ref, const char *delim, char *input, int bound) {
    char *string, *found;
    string = (char *) strdup(input);
    if (bound < 0) {
        while ((found = strsep(&string, delim)) != NULL)
            insertLine(head_node_ref, found);
    } else {
        for (int i = 0; i < bound && ((found = strsep(&string, delim)) != NULL); i++)
            insertLine(head_node_ref, found);
    }
    // free(string);
    // free(found);
}

/**
 * Splits input into multiple string along found delimiters and inputs lines into list.
 * @param head_node_ref
 * @param delim
 * @param input
 * @param bound the upper bound for lines split and put into the given list
 */
void unsorted_read_and_split(struct Node **head_node_ref, const char *delim, char *input, int bound) {
    char *string, *found;
    string = (char *) strdup(input);

    if (bound < 0) {
        while ((found = strsep(&string, delim)) != NULL)
            unsortedInsertLine(head_node_ref, found);
    } else {
        for (int i = 0; i < bound && ((found = strsep(&string, delim)) != NULL); i++)
            unsortedInsertLine(head_node_ref, found);
    }
    //free(string);
    // free(found);
}


/**
 * Non-recursive unsorted insert.
 * @param head_node_ref
 * @param new_node
 */
void insert(struct Node **head_node_ref,
            struct Node *new_node) {
    struct Node *last = *head_node_ref;
    new_node->next = NULL;
    if (*head_node_ref == NULL) {
        *head_node_ref = new_node;
        return;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
}

/**
 * A04.5 Sorts list by comparing while inserting.
 * @param head_node_ref
 * @param new_node
 */
void sortedInsert(struct Node **head_node_ref,
                  struct Node *new_node) {
    struct Node *current;
    if (*head_node_ref == NULL
        || strcmp((*head_node_ref)->data, new_node->data)
           >= 0) {
        new_node->next = *head_node_ref;
        *head_node_ref = new_node;
    } else {
        current = *head_node_ref;
        while (current->next != NULL
               && strcmp(current->next->data, new_node->data) < 0) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

/**
 * creates new node with given data.
 * @param new_data
 * @return
 */
struct Node *createNewNode(char *new_data) {
    /* allocate Node */
    struct Node *new_node
            = (struct Node *) malloc(
                    sizeof(struct Node));

    /* put in the data  */
    new_node->data = new_data;
    new_node->next = NULL;

    return new_node;
}

/**
 * prints list
 * @param head
 */
void printList(struct Node *head) {
    struct Node *temp = head;
    while (temp != NULL) {
        printf("<%s>", temp->data);
        temp = temp->next;
        printf("\n");
    }
}

/**
 * A04.2
 * @param head_node_ref
 */
void reverse(struct Node **head_node_ref) {
    struct Node *prev = NULL;
    struct Node *current = *head_node_ref;
    struct Node *next;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    *head_node_ref = prev;
}


/**
 * pushes node in list
 * @param head
 * @param data
 */
__attribute__((unused)) void push(struct Node **head, char *data) {
    struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
}

/**
 * A04.4
 * Splits list into two (non-sorted) parts.
 * Works by using Floyd's fast/slow pointer strategy.
 * Kommentar zur Aufgabe A04.4:
 * Wenn die Liste(@param input) eine ungerade Anzahl an Elementen hat, dann wird die erste Liste(@param front_node_ref)
 * ein Element mehr als die zweite Liste(@param back_node_ref) haben.
 * @param input
 * @param front_node_ref in this head node the front part will be stored
 * @param back_node_ref in this head node the back part will be stored
 */
void split(struct Node *input, struct Node **front_node_ref,
           struct Node **back_node_ref) {
    // if the length is less than 2, handle it separately
    if (input == NULL || input->next == NULL) {
        *front_node_ref = input;
        *back_node_ref = NULL;
        return;
    }

    struct Node *slow = input;
    struct Node *fast = input->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // `slow` is now before the midpoint in the list, so split it in two
    // at that point.
    *front_node_ref = input;
    *back_node_ref = slow->next;
    slow->next = NULL;
}

/**
 * A04.3
 * Recursive (sorted) merge. End result is in head_node_ref
 * @param head_node_ref head node ref of list to be inserted in
 * @param node_ref node ref that is to be inserted in head_node_ref
 */
void merge(struct Node **head_node_ref, struct Node **node_ref) {
    struct Node *current = *node_ref;
    if (current->next == NULL) {
        sortedInsert(head_node_ref, current);
    } else {
        merge(head_node_ref, &(current->next));
        sortedInsert(head_node_ref, current);
    }
}

void insertLine(struct Node **head_node_ref, char *line) {
    sortedInsert(head_node_ref, createNewNode(line));
}

void unsortedInsertLine(struct Node **head_node_ref, char *line) {
    insert(head_node_ref, createNewNode(line));
}

/**
 * Ich arbeite in CLion (JetBrains C IDE) und der code funktioniert in meiner IDE meiner letzte Hausaufgabe, weswegen
 * es mich gewundert hat in der letzten Hausaufgabe (und der davor) zu lesen dass es nicht kompiliert hat. Ich pushe
 * den code immer auf github damit ich auf mehreren Geräten arbeiten kann
 * und wenn alles fertig ist push ich es auf svn. Ich schätze meine IDE verwendet einen anderen Compiler.
 * Diesmal funktioniert aber "gcc main.c -pedantic -Wall -Werror -std=c99".
 *
 * @return exit code
 */
int main() {
    struct Node *head0 = NULL;
    struct Node *head1 = NULL;
    struct Node *head2 = NULL;
    struct Node *file_nhead = NULL;
    //debug 0 read and split input0 string
    char *input0 = "random \n input \n to \n test";
    char *input1 = "the \n merge \n function \n file is too \n big";
    char *input2 = "another \n arrangement \n of \n words \n to \n test \n the \n split \n function";
    char *delim = (char *) strdup("\n");
    char *file_name = "moby-dick.txt";

    read_and_split(&head0, delim, input0, 1000);
    read_and_split(&head1, delim, input1, 1000);
    read_and_split(&head2, delim, input2, 1000);

    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen(file_name, "rb");
    if (!fp) perror("blah.txt"), exit(1);

    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    buffer = calloc(1, lSize + 1);
    if (!buffer) fclose(fp), fputs("calloc fails?", stderr), exit(1);

    if (1 != fread(buffer, lSize, 1, fp))
        fclose(fp), free(buffer), fputs("read fails? possible reason: file name is not valid.", stderr), exit(1);

    // printf("\nCreated char* buffer, reading input0...\n");
    unsorted_read_and_split(&file_nhead, delim, buffer, 1000);
    fclose(fp);
    free(buffer);
    fclose(fp);


    printf("A04.1; Ausgabe ohne Reverse: (1000 Zeilen max.)");
    printf("\n--------------------------------------------\n");
    sleep(4);
    printList(file_nhead);
    printf("\n--------------------------------------------\n");
    printf("A04.2; Ausgabe mit Reverse: (1000 Zeilen max.)");
    printf("\n--------------------------------------------\n");
    sleep(5);
    reverse(&file_nhead);
    printList(file_nhead);
    printf("\n--------------------------------------------\n");
    printf("\nA04.3: Ausgabe von zwei vereinigten Listen:");
    sleep(5);
    //debug 2 merge list0 and list1
    merge(&head1, &head0);
    printf("\nResult:");
    printf("\n--------------------------------------------\n");
    printList(head1);
    printf("\n--------------------------------------------\n");
    sleep(5);
    //debug 3 split list0 into multiple lists.
    struct Node *a1 = NULL;
    struct Node *a2 = NULL;
    split(head2, &a1, &a2);

    printf("\nA04.4: Ausgabe von zwei gespaltenen Listen:");
    printf("\nErste Liste:");
    sleep(4);
    printf("\n--------------------------------------------\n");
    printList(a1);
    printf("\n--------------------------------------------\n");
    printf("\nZweite Liste:");
    sleep(4);
    printf("\n--------------------------------------------\n");
    printList(a2);
    printf("\n--------------------------------------------\n");
    sleep(5);
    fp = fopen(file_name, "rb");
    if (!fp) perror("blah.txt"), exit(1);

    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    buffer = calloc(1, lSize + 1);
    if (!buffer) fclose(fp), fputs("calloc fails?", stderr), exit(1);

    if (1 != fread(buffer, lSize, 1, fp))
        fclose(fp), free(buffer), fputs("read fails? possible reason: file name is not valid.", stderr), exit(1);

    // printf("\nCreated char* buffer, reading input0...\n");
    read_and_split(&file_nhead, delim, buffer, 1000);
    fclose(fp);
    free(buffer);
    fclose(fp);

    printf("A04.5; Sortierte Ausgabe mit Reverse: (1000 Zeilen max.)");
    printf("\n--------------------------------------------\n");
    sleep(5);
    reverse(&file_nhead);
    printList(file_nhead);
    printf("\n--------------------------------------------\n");

}