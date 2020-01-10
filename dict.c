#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio_ext.h>

struct kontakt  // data for doubly linked list
{
    char* imie;
    char* nazwisko;
    char* numer;
    char* nazwaGrupy;
};

struct list    // doubly linked list
{
    struct kontakt k;
    struct list* next;
    struct list* before;
};

typedef struct kontakt kontakt;
typedef struct list list;


list* read_csv(list** top, list** bottom);
void zapis_csv(list* top, list* bottom, char* fileName);
char* strMerge(char* s1, char* s2, char* s3, char* s4);
list* addlist(list** top, list** bottom, char* buffer);
void listClean(list** top);
void listState(list* bottom);
void listDispaly(list* l);
void listDelete(list* top, list* bottom, list** found);
list* to_delete(list* top, int index);
list* listSearch(list* top);
void strlower(char* str);
void listAfiliation(list* top, char* key);
void textDisplay(char* line);

// insetionSort
void sortedInsert(struct list** head_ref, struct list* newNode); 
void insertionSort(struct list** head_ref);


int main()
{
    char fileName[100];
    char errorBuff[200];
    char grupa [50];    // group of afiliation
    list* top = NULL;
    list* bottom = NULL;
    list* found;    // for search
    int index = -1;
    char menuControl = 50;
    while(menuControl)
    {
        textDisplay("Menu");
        printf("[1] read data file\n");
        printf("[2] display data base\n");
        printf("[3] write data into file\n");
        printf("[4] sort by key\n");
        printf("[5] Afiliation grup search\n");
        printf("[6] Search list element\n");
        printf("[7] Delete list element\n");
        printf("[8] Exit\n");
        __fpurge(stdin);
        menuControl = getchar();
        system("clear");
        switch(menuControl)
        {
        case '1':   // read
            top = read_csv(&top, &bottom);
            break;
        case '2':
            listState(bottom);
            break;
        case '3':   // save
            zapis_csv(top, bottom, "listContent.csv");
            break;
        case '4':   // sort
            insertionSort(&bottom);
            zapis_csv(top, bottom, "listSorted.csv");
            break;
        case '5':   // membership of grup  // example znajomi
            scanf("%s", grupa);
            listAfiliation(top, grupa);
            break;
        case '6':   //search
            printf("Enter partt of first of last name");
            listSearch(top);
            break;
        case '7':   //delete
            printf("search for delete\n");
            printf("Enter parto of first of last name");
            printf("delete means you lost that data forever");

            listSearch(top);
            
            printf("enter index of element to delete\n");
            scanf("%d", &index);
            found = to_delete(top, index);
            printf("%p", found);
            if (found != NULL) {    
                listDispaly(found);
                listDelete(top, bottom, &found);
            }
            else {
                printf("element to delete not found\n");    
            }
            break;
        case '8':   // exit || end of program
            menuControl = 0;
            break;
        default:
            printf("wrong menu option\n");
            break;
        }
    }    
    listClean(&top); // cleaning list   // guitar
    while( (fgets(errorBuff, 200, stderr)) != NULL) // error stream
        puts(errorBuff);
    textDisplay("Koniec Programu");
    return 0;
}

// git
list* read_csv(list** top, list** bottom)
{
    FILE* fp = fopen("kontakty.csv", "r");
    char buffer [200];
	if(fp != NULL)
    {
        // fscanf(fp, %[^;], buffer); // fajny bajer czyta do ";"
        while( (fgets(buffer, 200, fp)) != NULL)
            *top = addlist(top, bottom, buffer); // top now points to newElement
    }
    else{
        printf("problem with opening file\n");
        perror("error 1 - read - opening file error");
    }
    fclose(fp); // zamykanie pliku

    return *top;
}

void zapis_csv(list *top, list* bottom, char* fileName)
{
    char* buffer;
    FILE* fp = fopen(fileName, "w");
    if(fp != NULL)
    {
        while(bottom != NULL)
        {
            buffer = strMerge(bottom->k.imie, bottom->k.nazwisko, bottom->k.numer, bottom->k.nazwaGrupy);
            fprintf(fp, "%s", buffer);
            free(buffer);
            bottom = bottom->next;
        }
    }
    else{
        printf("problem with opening file\n");
        perror("error 1 - read - opening file error");
    }
    fclose(fp);
}

// my version of sscanf() 
char* strMerge(char* s1, char* s2, char* s3, char* s4)
{
    char* buffer = (char*) calloc(200, sizeof(char));
    int i = 0;
    while(*s1)
        *(buffer + i++) = *s1++;
    *(buffer + i++) = ';';
    while(*s2)
        *(buffer + i++) = *s2++;
    *(buffer + i++) = ';';
    while(*s3)
        *(buffer + i++) = *s3++;
    *(buffer + i++) = ';';
    while(*s4)
        *(buffer + i++) = *s4++;
    *(buffer + i++) = '\n';
    // '\n' at the end in cause of puts
    buffer = (char*) realloc(buffer, i* sizeof(char));
    printf("%s", buffer);
    return buffer;
}

// git
list * addlist(list** top, list** bottom, char* buffer)
{
    list* newElement = malloc(sizeof(list));
    if(newElement == NULL){  // unable to alloc memory error
    perror("error 2 - unable to alloc memory error");
    printf("error 2 - unable to alloc memory error");
    }
    if (*top != NULL)
    {
        newElement->before = *top;
        newElement->next = NULL;
        (*top)->next = newElement;
    }
    else if (*top == NULL) // 1 list is empty
    {
        *bottom = newElement;
        newElement->before = NULL;
        newElement->next = NULL;
    }
    newElement->k.imie = strdup(strtok(buffer, ";"));
    newElement->k.nazwisko = strdup(strtok(NULL, ";"));
    newElement->k.numer = strdup(strtok(NULL, ";")); 
    newElement->k.nazwaGrupy = strdup(strtok(NULL, "\n;"));
    return newElement;
}

void listDelete(list* top, list* bottom, list** found)
{
    if( (*found)->next != NULL) // check if is it last element
        (*found)->next->before = (*found)->before;
    else  // that is last element
        top = (*found)->before;
    if( (*found)->before != NULL)   // for first element do not
        (*found)->before->next = (*found)->next;
    else
        bottom = (*found)->next;
    free(*found);
    printf("element deleted succesfully\n");
}

// Sort
void sortedInsert_Fname(struct list** head_ref, struct list* newNode) 
{ 
    struct list* current; 

    // 1 if list is empty 
    if (*head_ref == NULL) 
        *head_ref = newNode; 
  
    // 2 insert on begining
    else if (strcmp((*head_ref)->k.imie, newNode->k.imie) >= 0) { 
        newNode->next = *head_ref; 
        newNode->next->before = newNode; 
        *head_ref = newNode; 
    } 
  
    else { 
        current = *head_ref; 
  
        // scroll the list to found where to insert
        while (current->next != NULL &&  strcmp(current->next->k.imie, newNode->k.imie) < 0) 
            current = current->next; 
  
        // links
        newNode->next = current->next; 
  
        // if it's not on the end of the list
        if (current->next != NULL) 
            newNode->next->before = newNode; 
  
        current->next = newNode; 
        newNode->before = current; 
    } 
} 

void sortedInsert_Sname(struct list** head_ref, struct list* newNode) 
{ 
    struct list* current; 

    // 1 if list is empty 
    if (*head_ref == NULL) 
        *head_ref = newNode; 
  
    // 2 insert on begining
    else if (strcmp((*head_ref)->k.nazwisko, newNode->k.nazwaGrupy) >= 0) { 
        newNode->next = *head_ref; 
        newNode->next->before = newNode; 
        *head_ref = newNode; 
    } 
  
    else { 
        current = *head_ref; 
  
        // scroll the list to found where to insert
        while (current->next != NULL &&  strcmp(current->next->k.nazwisko, newNode->k.nazwisko) < 0) 
            current = current->next; 
  
        // links
        newNode->next = current->next; 
  
        // if it's not on the end of the list
        if (current->next != NULL) 
            newNode->next->before = newNode; 
  
        current->next = newNode; 
        newNode->before = current; 
    } 
} 

void sortedInsert_group(struct list** head_ref, struct list* newNode) 
{ 
    struct list* current; 

    // 1 if list is empty 
    if (*head_ref == NULL) 
        *head_ref = newNode; 
  
    // 2 insert on begining
    else if (strcmp((*head_ref)->k.nazwaGrupy, newNode->k.nazwaGrupy) >= 0) { 
        newNode->next = *head_ref; 
        newNode->next->before = newNode; 
        *head_ref = newNode; 
    } 
  
    else { 
        current = *head_ref; 
  
        // scroll the list to found where to insert
        while (current->next != NULL &&  strcmp(current->next->k.nazwaGrupy, newNode->k.nazwaGrupy) < 0) 
            current = current->next; 
  
        // links
        newNode->next = current->next; 
  
        // if it's not on the end of the list
        if (current->next != NULL) 
            newNode->next->before = newNode; 
  
        current->next = newNode; 
        newNode->before = current; 
    } 
} 

void insertionSort(struct list** head_ref) 
{   struct list* sorted = NULL; 
    struct list* current = *head_ref; 
    
    // menu
    char key = 5;
    printf("[1] imie\n");
    printf("[2] nazwisko\n");
    printf("[3] grupa\n");
    __fpurge(stdin);
    key = getchar();
    switch (key)
    {
        case '1':
            while (current != NULL) { 
                struct list* next = current->next; 
                current->before = current->next = NULL; 
                sortedInsert_Fname(&sorted, current); 
                current = next; 
            } 
            // upgread the head
            *head_ref = sorted; 
            break;
        case '2':
            while (current != NULL) { 
                struct list* next = current->next; 
                current->before = current->next = NULL; 
                sortedInsert_Sname(&sorted, current); 
                current = next; 
            } 
            // upgread the head
            *head_ref = sorted; 
            break;
        case '3':
            while (current != NULL) { 
                struct list* next = current->next; 
                current->before = current->next = NULL; 
                sortedInsert_group(&sorted, current); 
                current = next; 
            } 
            // upgread the head
            *head_ref = sorted; 
            break;
        default:
            printf("Wrong key\n");
    }


} 

// mergeSort
/*
list * mergeSortedLists(list* head1, list* head2)
{
   list* result = NULL;
   if (head1 == NULL) {
      return head2;
   }
   if (head2 == NULL) {
      return head1;
   }
   if (head1->k.imie < head2->k.imie) { // decide key   // dla imienia
      head1->next = mergeSortedLists(head1->next, head2);
      head1->next->before = head1;
      head1->before = NULL;
      return head1;
   } else {
      head2->next = mergeSortedLists(head1, head2->next);
      head2->next->before = head2;
      head2->before= NULL;
      return head2;
   }
}

void splitList(list *src, list **fRef, list **bRef)
{
   list *fast;
   list *slow;
   slow = src;
   fast = src->next;
   while (fast != NULL) {
      fast = fast->next;
      if (fast != NULL) {
         slow = slow->next;
         fast = fast->next;
      }
   }
   *fRef = src;
   *bRef = slow->next;
   slow->next = NULL;
}

void mergeSort(list **head){
   list *p = *head;
   list *a = NULL;
   list *b = NULL;
   if (p == NULL || p->next == NULL) {
      return;
   }
   splitList(p, &a, &b);
   mergeSort(&a);
   mergeSort(&b);
   *head = mergeSortedLists(a, b);
}
*/


// git afilation
void listAfiliation(list* top, char* key)   // checks membership to priorityGroup
{
    while(top != NULL)
    {
        if(strstr(top->k.nazwaGrupy, key) != NULL)
            listDispaly(top);
        // operation
        top = top->before;
    }    
}
// git
/*
list* listSearch(list* top)  // zwraca adres elementu listy z poszukiwanym keyem
{
    char key[50];
    printf("Key to search for:\n");
    scanf("%s", key);
    strlower(key);
    while (top != NULL)
    {
        strlower(top->k.imie);
        if(strstr(top->k.imie, key))    // founded
            break;
        strlower(top->k.nazwisko);
        if(strstr(top->k.nazwisko, key))    // founded
            break;
        top = top->before;    
    }
    if(top == NULL)
        return NULL;
    else
        return top;
}
*/

list* listSearch(list* top)  // zwraca adres elementu listy z poszukiwanym keyem
{
    int i = 0;
    char key[50];
    int isFound = 0;
    printf("Key to search for:\n");
    scanf("%s", key);
    strlower(key);
    while (top != NULL)
    {
        strlower(top->k.imie);
        if (strstr(top->k.imie, key)) {    // founded
            printf(" [%d] ||", i);
            listDispaly(top);
            isFound = 1;
        }
        strlower(top->k.nazwisko);
        if (strstr(top->k.nazwisko, key)) {   // founded
            printf(" [%d] ||", i);
            listDispaly(top);
            isFound = 1;
        }
        top = top->before;
        i++;    
    }
    if (isFound == 0){
        printf("not found\n");
    }
}

list* to_delete(list* top, int index)
{
    int i = 0;
    while (top != NULL)
    {
        if(index == i)
            break;
        top = top->before;
        i++;
    }
    return top;
}

// git
void strlower(char* str)
{
    while(*str != '\0'){
        *str = tolower(*str);
        str++;
    }
}

// git
void listClean(list** top)
{
    list* newTop;
    while(*top != NULL){
        newTop = (*top)->before;
        free(*top);
        *top = newTop;
    }
}

// git
void listState(list* bottom)   // traverse
{
    while(bottom != NULL)
    {
        listDispaly(bottom);
        bottom = bottom->next;
    }    
}

// git
void listDispaly(list* l)  // display 1 element of list
{
    printf("%s||", l->k.imie);
    printf("%s||", l->k.nazwisko);
    printf("%s||", l->k.numer);
    printf("%s\n", l->k.nazwaGrupy);
}

// funny text display
void textDisplay(char* line)
{
    int i = 0;
    printf("||");
    for(i = 0; i < (strlen(line) + 2); i++)
        printf("-");
    printf("||\n");
    printf("|| %s ||\n||", line);
    for(i = 0; i < (strlen(line) + 2); i++)
        printf("-");
    printf("||\n");
}
