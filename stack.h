// Stack taking the format data->int

#include <stdlib.h>
#include <stdio.h>

typedef struct Stack_node
{
	struct Stack_node *next, *prev;
	int data;
} Stack_node;

Stack_node *initStack();
Stack_node *createNode();

void push(Stack_node *stackPtr, int newData);
int pop(Stack_node *stackPtr);
int peek(Stack_node *stackPtr);
int isEmpty(Stack_node *stackPtr);

Stack_node *initStack()
{
	Stack_node *newStack;
	newStack = createNode();
	newStack->next = NULL;
	newStack->prev = NULL;
	newStack->data = -1;
	return newStack;
}

Stack_node *createNode()
{
	Stack_node *node = (Stack_node *) malloc(sizeof(Stack_node));
	return node;
}

void push(Stack_node *stackPtr, int newData)
{
	// If the stack is empty, create a new stack
	if (isEmpty(stackPtr))
	{
		stackPtr->data = newData;
		return;
	}
	else
	{
		Stack_node *oldNode = initStack();

		oldNode->data = stackPtr->data;
		oldNode->prev = stackPtr->prev;
		oldNode->next = stackPtr;

		stackPtr->data = newData;
		stackPtr->prev = oldNode;
		return;
	}
}

int pop(Stack_node *stackPtr)
{
	if (!isEmpty(stackPtr))
	{
		int popData = stackPtr->data;

		if (stackPtr->prev != NULL)
		{
			stackPtr->data = stackPtr->prev->data;
			stackPtr->prev = stackPtr->prev->prev;
		}
		else
		{
			stackPtr->data = -1;
			stackPtr->prev = NULL;
		}
		return popData;
	}
	else
		return -1;
}

int peek(Stack_node *stackPtr)
{
	if (!isEmpty(stackPtr))
		return stackPtr->data;
	else
		return -1;
}

int isEmpty(Stack_node *stackPtr)
{
	if (stackPtr->data == -1)
		return 1;
	else
		return 0;
}




