// Singly Linked List taking the format indexTo->weight, weight->int
// This example does not automatically sort itself though

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 6301
#define MAX_CHAR_SIZE 50

typedef struct Element{
	struct Element *next;
	int indexTo;
	int weight;
} Element;

Element *createList();
void freeList(Element *list);
void insertElement(Element *list, int newIndexTo, int newWeight);

Element *createList()
{
	Element *list = malloc(sizeof(Element) * SIZE);
	list->indexTo = -1;
	list->weight = -1;
	list->next = NULL;
	return list;
}

void freeList(Element *list)
{
	Element *currentElement = list;

	while(currentElement->next != NULL)
	{
		free(currentElement);
		currentElement = currentElement->next;
	}

	free(list);
}

void insertElement(Element *list, int newIndexTo, int newWeight)
{
	Element *currentElement = list;

	// If the current list is empty, just fill the first element
	if (currentElement->indexTo == -1)
	{
		currentElement->indexTo = newIndexTo;
		currentElement->weight = newWeight;
		return;
	}

	// Else proceed as usual and create a new element
	Element *newEdge;
	newEdge = (Element *) malloc(sizeof(Element));
	newEdge->indexTo = newIndexTo;
	newEdge->weight = newWeight;
	newEdge->next = NULL;

	// Loop until you get to the end
	while (currentElement->next != NULL)
		currentElement = currentElement->next;

	// Append the new element there
	currentElement->next = newEdge;
	return;
}