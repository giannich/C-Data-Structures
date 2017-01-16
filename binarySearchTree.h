// BST taking the format key->char *, definition->char *

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
	char *key;
	char *definition;
	struct Node *left, *right;
} Node;

void testUnit(Node *tree);

Node *createTree();
void freeTree(Node *tree);

Node *createNode();
void freeNode(Node *node);

Node *findNode(Node *tree, char *key);
int insertNode(Node *tree, char *newKey, char *definition);
char *deleteNode(Node *tree, char* deleteKey);

void printNode(Node *node);
void printTree(Node *node);

void testUnit(Node *tree)
{
	// Tests an empty tree
	findNode(tree, "Gorilla");
	deleteNode(tree, "Giraffe");

	// Test insertion and duplicate insertion
	insertNode(tree, "Tuna", "Fish");
	insertNode(tree, "Dog", "Mammal");
	insertNode(tree, "Cat", "Mammal");
	insertNode(tree, "Turtle", "Reptile");
	insertNode(tree, "Dog", "Mammal");

	// Test finding
	findNode(tree, "Cat");
	findNode(tree, "Hippo");

	// Test deletion and wrong deletion
	deleteNode(tree, "Turtle");
	deleteNode(tree, "Gorilla");

	// Prints the tree and lastly frees it
	printTree(tree);
	freeTree(tree);

	printf("Successfully freed dictionary.\n");
	printf("Successfully completed test.\n");
}

// Creates the tree
Node *createTree()
{
	Node *node;
	node = createNode();
	node->left = NULL;
	printf("Dictionary successfully created.\n");
	return node;
}

// Traverses the tree and frees the pointers but in POST-ORDER
void freeTree(Node *node)
{
	// Returns if the node is Null as there is nothing to free
   	if (node == NULL) 
   		return;

   	// Does the recursion on itself if and only if the node is not a leaf itself
   	if (node->right != NULL)
   	{
   		freeTree(node->left);
   		freeTree(node->right);
   	}

   	// Lastly, the node itself is freed
   	freeNode(node);
}

// Declares, mallocs, and returns a node pointer
Node *createNode()
{
	Node *node = (Node *) malloc(sizeof(Node));
	return node;
}

// Frees the node pointer
void freeNode(Node *node)
{
	free(node);
}

// Finds a word and returns the pointer
// I used the code in the slides
Node *findNode(Node *tree, char *key)
{
	// Create a temp Node
	Node *tempNode;

  	// Returns null if tree is empty
  	if( tree->left == NULL )
  	{
  		printf("Error: the word %s cannot be found since the dictionary is empty.\n", key);
    	return(NULL);
    }

  	else
  	{
  		// Start from the root
  		tempNode = tree;

  		// As long as the current node is not a leaf
  		while( tempNode->right != NULL )
    	{
    		// If the key is smaller than the current node's key, go to the left, otherwise go right
      		if( strcmp(key, tempNode->key) < 0 )
        		tempNode = tempNode->left;
      		else
        		tempNode = tempNode->right;
    	}

    	// Once a leaf node is reached, if the node's key and the key are the same, return the result, otherwise return null
    	if( strcmp(tempNode->key, key) == 0 )
    	{
    		printf("The definition for the word %s is as follows: %s.\n", key, (char *) tempNode->left);
      		return(tempNode);
      	}
    	else
    	{	
    		printf("Error: the word %s is currently not defined in the dictionary.\n", key);
      		return (NULL);
      	}
    }
}

// Inserts the node into the tree
// I used the code in the slides
int insertNode(Node *tree, char *newKey, char *definition)
{

	Node *tempNode;

	// If the tree is empty, put the newkey there, the definition on its left child and null on its right child
	if( tree->left == NULL )
	{
    	tree->left = (Node *) definition;
    	tree->key  = newKey;
    	tree->right = NULL;
    	printf("Successfully defined the word %s as follows: %s.\n", newKey, definition);
    	return( 0 );
  	}

  	// Otherwise look into the tree
  	else
  	{
  		// Start from the root
    	tempNode = tree;

  		// As long as the current node is not a leaf
    	while( tempNode->right != NULL )
    	{
    		// Go to left if key is smaller than the node's, otherwise go right
      		if( strcmp(newKey, tempNode->key) < 0 )
        		tempNode = tempNode->left;
      		else
        		tempNode = tempNode->right;
    	}
    }

    // If the key already exists, exit with an error
	if ( !strcmp(tempNode->key, newKey) )
	{
		printf("Error: the word %s already exists in the dictionary.\n", newKey);
    	return( -1 );
    }

    // Declare the new children leaves
    Node *oldLeaf, *newLeaf;
        
    // The old leaf just becomes a copy of the original tempNode
    oldLeaf = createNode();
    oldLeaf->left = tempNode->left;
    oldLeaf->key = tempNode->key;
    oldLeaf->right = NULL;

	// The new leaf gets its own leaf node with its left child pointing to its data and right child to Null
    newLeaf = createNode();
    newLeaf->left = (Node *) definition;
    newLeaf->key = newKey;
    newLeaf->right  = NULL;

    // Now, if the new key is bigger than the tempNode's key, then the temp node's key becomes the new key
    // The temp node will now point to the two newly created nodes
    if( strcmp(tempNode->key, newKey) < 0 )
    {   
    	tempNode->left  = oldLeaf;
        tempNode->right = newLeaf;
        tempNode->key = newKey;
    }
    else
    {   
        tempNode->left  = newLeaf;
        tempNode->right = oldLeaf;
    }

    printf("Successfully defined the word %s as follows: %s.\n", newKey, definition);
    return( 0 );
}

// Deletes a word given the key, and returns the pointer to the deleted Node
// I used the code in the slides
char *deleteNode(Node *tree, char* deleteKey)
{
	Node *tempNode, *parentNode, *siblingNode;
 	char *definition;

 	// If the tree is empty, then there is nothing to delete
	if( tree->left == NULL )
	{
		printf("Error: the dictionary is empty, so there is nothing to delete.\n");
    	return( NULL );
    }

    // If the root is also a leaf AND its key matches the delete key,
    // then it sets its left child as null, therefore making it an empty tree and returning its definition
    // Otherwise, if it is a leaf AND its key does not match the delete key, an error is printed and the function returns NULL
	else if( tree->right == NULL )
  	{
    	if ( strcmp(tree->key, deleteKey) == 0 )
    	{
      		definition = (char *) tree->left;
      		tree->left = NULL;
      		printf("Successfully deleted the word %s from the dictionary.\n", deleteKey);
      		return( definition );
    	}
    	else
    	{
    		printf("Error: the word %s does not currently exist in the dictionary.\n", deleteKey);
      		return( NULL );
      	}
    }

    // Else it looks deeper into the tree
    else
    {
    	// Start from the root
    	tempNode = tree;

    	// While not in a leaf
    	while( tempNode->right != NULL )
    	{
    		// The parent node is now the current node
      		parentNode = tempNode;

      		// If the delete key is smaller than the tempNode key
      		// Then the left child is the new tempNode and the right child is the sibling node
      		if( strcmp(deleteKey, tempNode->key) < 0 )
      		{
        		tempNode   = parentNode->left;
        		siblingNode = parentNode->right;
      		}
      		// Else, the right child is the new tempNode and the left child is the sibling node
      		else
      		{
        		tempNode   = parentNode->right;
        		siblingNode = parentNode->left;
      		}
    	}

    	// Once arrived at a leaf node, if the delete key does not match, then it prints an error and returns NULL
		if( strcmp(tempNode->key, deleteKey) )
		{
			printf("Error: the word %s does not currently exist in the dictionary.\n", deleteKey);
    		return( NULL );
    	}

    	// Otherwise, if the key matches, the sibling becomes the new parent node and is now a leaf node
		else
		{
			// Setting the parent node's parameters equal to the sibling node
    		parentNode->key   = siblingNode->key;
    		parentNode->left  = siblingNode->left;
    		parentNode->right = siblingNode->right;

    		// The definition is now set to the deleted key's left child
    		definition = (char *) tempNode->left;

    		// And the pointers are freed
    		freeNode( tempNode ); 
    		freeNode( siblingNode ); 

    		// Finally, the definition is returned
    		printf("Successfully deleted the word %s from the dictionary.\n", deleteKey);
    		return( definition );
    	}
    }
}

// Prints the sorted tree
void printNode(Node *node)
{
	if (node->right == NULL)
		printf("Word: [%s], Definition: %s\n", node->key, (char *) node->left);
}

// Traverses the node and prints the stuff in PRE-ORDER
void printTree(Node *node)
{
	// Returns if the node is Null as there is nothing to print
   	if (node == NULL) 
   		return;

   	// Prints the current leaf node with its left child description
   	printNode(node);

   	// Does the recursion on its children if and only if the node itself is not a leaf
   	if (node->right != NULL)
   	{
   		printTree(node->left);
   		printTree(node->right);
   	}
}
