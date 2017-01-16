// Red and Black Tree taking the format key->char *, definition->char *

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
	char *key;
	char *definition;
	// 1 is for Black, 0 is for Red
	int color;
	struct Node *left, *right, *parent;
} Node;

void testUnit(Node *tree);

Node *createNode();
void freeNode(Node *node);
void freeTree(Node *node);
void printNode(Node *node);
void printTree(Node *node);

Node *findNode(Node *tree, char *key);
int insertNode(Node *tree, char *newKey, char *definition);
int deleteNode(Node *tree, char *deleteKey);

Node *grandparent(Node *n);
Node *uncle(Node *n);
Node *successor(Node *n);
Node *sibling(Node *n);
Node *redChild(Node *n);

void recolor(Node *n);
void null_recolor(Node *s);
void rotate_left(Node *n);
void rotate_right(Node *n);

void insert_case1(Node *n);
void insert_case2(Node *n);
void insert_case3(Node *n);
void insert_case4(Node *n);
void insert_case5(Node *n);


int main(int argc, char **argv)
{
  Node *tree = createNode();
  tree->color = 1;
  mainLoop(tree);
  return 0;
}

void testUnit()
{
  Node *tree = createNode();
  tree->color = 1;

  // Insertion Cases Work
  insertNode(tree, "30", "thirty");
  insertNode(tree, "20", "twenty");
  insertNode(tree, "50", "fifty");
  insertNode(tree, "40", "fourty");
  insertNode(tree, "60", "sixty");
  insertNode(tree, "10", "ten");
  insertNode(tree, "70", "seventy");

  // Deletion Cases Work
  deleteNode(tree, "20");
  deleteNode(tree, "60");

  printTree(tree);
  freeTree(tree);
}

Node *createNode()
{
	Node *node = (Node *) malloc(sizeof(Node));
	node->key = NULL;
	node->definition = NULL;
	node->color = 0; // Starts as a red
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	return node;
}

void freeNode(Node *node)
{
	free(node);
}

void freeTree(Node *node)
{
	// Returns if the node is Null as there is nothing to free
   	if (node == NULL) 
   		return;

   	// Frees the nodes
   	freeTree(node->left);
   	freeTree(node->right);
   	freeNode(node);
}

// Prints the sorted tree
void printNode(Node *node)
{
	printf("Word: [%s], Color: [%d], Definition: %s\n", node->key, node->color, node->definition);
}

// Traverses the node and prints the stuff in PRE-ORDER
void printTree(Node *node)
{
	// Returns if the node is Null as there is nothing to printf
   	if (node == NULL) 
   		return;

   	// Prints the current leaf node with its description
   	printTree(node->left);
   	printNode(node);
   	printTree(node->right);
}

// Finds a word and returns the pointer
// I used the code in the slides
Node *findNode(Node *tree, char *key)
{
	// Create a temp Node
	Node *tempNode;

  	// Returns null if tree is empty
  	if( tree == NULL )
  	{
  		printf("Error: the word %s cannot be found since the dictionary is empty.\n", key);
    	return(NULL);
    }

  	else
  	{
  		// Start from the root
  		tempNode = tree;

  		// As long as the current node is not a leaf
  		while( tempNode != NULL )
    	{
    		// In case the key is found
    		if( strcmp(tempNode->key, key) == 0 )
    		{
    			printf("The definition for the word %s is as follows: %s.\n", key, (char *) tempNode->definition);
      			return(tempNode);
      		}
    		// If the key is smaller than the current node's key, go to the left, otherwise go right
      		else if( strcmp(key, tempNode->key) < 0 )
        		tempNode = tempNode->left;
      		else
        		tempNode = tempNode->right;
    	}

    // Returns an error if the current node is NULL
    printf("Error: the word %s is currently not defined in the dictionary.\n", key);
    return (NULL);
    }
}

// Inserts the node into the tree
// I used the code in the slides
int insertNode(Node *tree, char *newKey, char *definition)
{

	Node *tempNode;
	Node *parentNode;
	int direction; // determines whether the cursor moved left(0) or right(1)

	// If the tree is empty, put the newkey there
	if( tree->key == NULL )
	{
    	tree->key = newKey;
    	tree->definition = definition;

    	printf("Successfully defined the word %s as follows: %s.\n", newKey, definition);
    	return( 0 );
  }

  // Otherwise look into the tree
  else
  {
  	// Start from the root
   	tempNode = tree;

  	// As long as the current node is not a leaf
    while( tempNode != NULL )
    {

    	// If the key is the same as the current node
    	if( strcmp(newKey, tempNode->key) == 0 )
    	{
    		printf("Error: the word %s already exists in the dictionary.\n", newKey);
    		return( -1 );
    	}
    	// Go to left if key is smaller than the node's, otherwise go right
     	else if( strcmp(newKey, tempNode->key) < 0 )
     	{
     		direction = 0;
     		parentNode = tempNode;
      	tempNode = tempNode->left;
      }
     	else
     	{
     		direction = 1;
     		parentNode = tempNode;
      	tempNode = tempNode->right;
      }
    }
  }

  Node *newNode = createNode();
  newNode->parent = parentNode;
  newNode->key = newKey;
  newNode->definition = definition;

  if(direction)
  	parentNode->right = newNode;
  else
  	parentNode->left = newNode;

  insert_case1(newNode);

  printf("Successfully defined the word %s as follows: %s.\n", newKey, definition);
  return( 0 );
}

int deleteNode(Node *tree, char *deleteKey)
{
	Node *tempNode;
	int direction; // 1 is for right, 0 is for left

 	// If the tree is empty, then there is nothing to delete
	if( tree == NULL )
	{
		printf("Error: the dictionary is empty, so there is nothing to delete.\n");
    	return( -1 );
  }
  else
    {
    	// Start from root
    	tempNode = tree;

      /****************
      * ROOT DELETION *
      ****************/

    	// If the root is the delete key
    	if(strcmp(tempNode->key, deleteKey) == 0)
    	{

        /***************
        * CASE 1: LEAF * 
        ****************/

    		// If no children, just free the root
    		if (tempNode->left == NULL && tempNode->right == NULL)
    		{
          printf("Successfully deleted the word %s.\n", deleteKey);
    			freeNode(tempNode);
    			return 0;
			  }

        /********************
        * CASE 2: ONE CHILD * 
        ********************/

    		// If the left child is null
    		else if (tempNode->left == NULL)
    		{
          Node *tmp_node = tempNode->right;

          // Set everything equal to tempNode's right
          tempNode->key = tempNode->right->key;
          tempNode->definition = tempNode->right->definition;
          tempNode->right = NULL;

    			// Free the parent
          printf("Successfully deleted the word %s.\n", deleteKey);
          freeNode(tmp_node);
          return 0;
    		}
    		// If the right child is null
    		else if (tempNode->right == NULL)
    		{
          Node *tmp_node = tempNode->left;

          // Set everything equal to tempNode's left
          tempNode->key = tempNode->left->key;
          tempNode->definition = tempNode->left->definition;
          tempNode->left = NULL;

    			// Free the parent
          printf("Successfully deleted the word %s.\n", deleteKey);
    			freeNode(tmp_node);
    			return 0;
    		}

        /***********************
        * CASE 3: TWO CHILDREN *
        ***********************/

    		// If root has both not-null children
    		else
    		{
          // Get the successor
          Node *sucNode = successor(tempNode);

    			// Now copy the data from the sucNode into the root
    			tempNode->key = sucNode->key;
    			tempNode->definition = sucNode->definition;

          // Set the sucNode's parent to NULL if it is not to the left of the tempNode
          if (tempNode->left != sucNode)
            sucNode->parent->right = NULL;
          else
          {
            rotate_left(tempNode);
            tempNode->left->left = sucNode->left;
            tempNode->right->color = 1;
          }

          // No need to check for double black since the root is always black

    			// Finally, free the sucNode
          printf("Successfully deleted the word %s.\n", deleteKey);
    			freeNode(sucNode);
    			return 0;
    		}

    	}

      /******************
      * GO THROUGH TREE *
      ******************/

    	while( tempNode != NULL )
    	{
    		// Check if the current key corresponds to the delete key
    		if ( strcmp(tempNode->key, deleteKey) == 0)
    		{
          // Initialize delNode and set it as tempNode
          // I'm doing this in order to be consistent
          Node *delNode = tempNode;
          Node *parNode = tempNode->parent;
          Node *subNode = NULL;

          /***************
          * CASE 1: LEAF *
          ***************/

    			// If leaf node just kill it off
    			if (delNode->left == NULL && delNode->right == NULL)
    			{
            Node *sibNode;
    				// It needs to dereference the parent first
    				if ( parNode->left == delNode )
            {
    					parNode->left = NULL;
              sibNode = parNode->right;
            }
    				else if( parNode->right == delNode )
            {
    					parNode->right = NULL;
              sibNode = parNode->left;
            }

            /*********************
            * DOUBLE BLACK CHECK *
            *********************/

            // In this case, if either of subNode or delNode is red, just color the new node black
            if (delNode->color == 1)
            {
              // Case 1 and Case 2: sibNode color is black
              if ( sibNode->color == 1 )
              {
                // Gets the redNode of sibNode, returns NULL if both of them are black
                Node *redNode = redChild(sibNode);

                // Case 1: at least a sibling child is red (DONE)
                if ( redNode != NULL )
                {
                  // Perform LL, LR, RR or RL rotation
                  insert_case4(redNode);
                  redNode->color = 1;
                }

                // Case 2: both sibling children are black, just perform recoloring (DONE)
                else
                  null_recolor(sibNode);
                }

              // Case 3: sibNode color is red, perform LL or RR rotation and then recolor (DONE)
              else if ( sibNode->color == 0 )
              {
                // Perform rotation depending on sibling position
                if ( parNode->right == sibNode )
                  rotate_left(sibNode);
                else if ( parNode->left == sibNode )
                  rotate_right(sibNode);

                // Recolor the subNode
                null_recolor(sibNode);
              }
            }

            free(delNode);
            printf("Successfully deleted the word %s.\n", deleteKey);
            return 0;
    			}

          /********************
          * CASE 2: ONE CHILD *
          ********************/
    			
    			// If node has only one child, just connect the grandparent with the grandchild
    			else if (delNode->left == NULL || delNode->right == NULL)
    			{
    				// If the left child is null, that means that it has a right child
    				if (delNode->left == NULL)
    				{
              // Set subNode first
              subNode = delNode->right;

              /****************************
              * RIGHT CHILD PARENT'S LEFT *
              ****************************/

    					// If the node is the left child of the parent
    					if ( parNode->left == delNode )
    					{
							  // Has Right child, Is Parent's Left
    						parNode->left = subNode;
    						subNode->parent = parNode;
    					}

              /*****************************
              * RIGHT CHILD PARENT'S RIGHT *
              *****************************/

    					// Else if the node is the right child of the parent
    					else if ( parNode->right == delNode )
    					{
    						// Has Right child, Is Parent's Right
    						parNode->right = subNode;
    						subNode->parent = parNode;
    					}
    				}
    				// Else if the right child is null, that means it has a left child
    				else if (delNode->right == NULL)
    				{
              // Set subNode first
              subNode = delNode->left;

              /***************************
              * LEFT CHILD PARENT'S LEFT *
              ***************************/

    					// If the node is the left child of the parent
    					if ( parNode->left == delNode )
    					{
							  // Has Left child, Is Parent's Left
    						parNode->left = subNode;
    						subNode->parent = parNode;
    					}

              /****************************
              * LEFT CHILD PARENT'S RIGHT *
              ****************************/

    					// Else if the node is the right child of the parent
    					else if ( parNode->right == delNode )
    					{
    						// Has Left child, Is Parent's Right
    						parNode->right = subNode;
    						subNode->parent = parNode;
    					}
    				}
    			}

          /***********************
          * CASE 3: TWO CHILDREN *
          ***********************/

    			// Normal situation with both left and right children
    			else
    			{	
            // Get the sucNode
            Node *sucNode = successor(delNode);

            /********************************
            * SUCNODE IS NOT DELNODE'S LEFT *
            ********************************/

            if (delNode->left != sucNode)
            {
    				  // Now copy the data from the sucNode into the root
              delNode->key = sucNode->key;
              delNode->definition = sucNode->definition;
              // Set the sucNode's parent to NULL if it is not to the left of the delNode
              sucNode->parent->right = NULL;
            }

            /****************************
            * SUCNODE IS DELNODE'S LEFT *
            ****************************/

            else
            {
              // Connects delNode's right child to sucNode's right and vice versa
              delNode->right->parent = sucNode;
              sucNode->right = delNode->right;
              // Connects sucNode's parent to delNode's parent
              sucNode->parent = delNode->parent;

              // delNode is it's parent's right child
              if (direction == 1)
                parNode->right = sucNode;

              // delNode is it's parent's left child
              else
                parNode->left = sucNode;
            }

            // Finally sets subNode as the sucNode node fpr the double black check process
            subNode = sucNode;
          }

            /*********************
            * DOUBLE BLACK CHECK *
            *********************/

            // In this case, if either of subNode or delNode is red, just color the new node black
            if (delNode->color == 0 || subNode->color == 0)
              subNode->color = 1;
            // Otherwise, mark the new node double black
            else
            {
              Node *sibNode = sibling(subNode);

              // Case 1 and Case 2: sibNode color is black
              if ( sibNode->color == 1 )
              {
                // Gets the redNode of sibNode, returns NULL if both of them are black
                Node *redNode = redChild(sibNode);

                // Case 1: at least a sibling child is red
                if ( redNode != NULL )
                {
                  // Perform LL, LR, RR or RL rotation
                  insert_case4(redNode);
                  // Make sibNode black
                  redNode->color = 1;
                }

                // Case 2: both sibling children are black, just perform recoloring
                else
                {
                  recolor(subNode);
                }
              }

              // Case 3: sibNode color is red, perform LL or RR rotation and then recolor
              else if ( sibNode->color == 0 )
              {
                // Perform rotation depending on sibling position
                if ( parNode->right == sibNode )
                  rotate_left(sibNode);
                else if ( parNode->left == sibNode )
                  rotate_right(sibNode);

                // Recolor the subNode
                recolor(subNode);
              }

            }
            printf("Successfully deleted the word %s.\n", deleteKey);
            free(delNode);
            return 0;
    			}

      	// If the delete key is smaller than the tempNode key
      	// Then the left child is the new tempNode and the right child is the sibling node
      	else if( strcmp(deleteKey, tempNode->key) < 0 )
      	{
        	tempNode = tempNode->left;
          direction = 0;
      	}
      	// Else, the right child is the new tempNode and the left child is the sibling node
      	else
      	{
        	tempNode = tempNode->right;
          direction = 1;
      	}
    	}

    	printf("Error: the word %s does not currently exist in the dictionary.\n", deleteKey);
    	return ( -1 );
    }

}

// Returns the sibling of the parent
Node *uncle(Node *n)
{
	if (n->parent->parent->left == NULL)
	{
		return NULL;
	}
	else if (!strcmp(n->parent->key, n->parent->parent->left->key))
		return n->parent->parent->right;
	else
		return n->parent->parent->left;
}

// Returns the parent of the parent
Node *grandparent(Node *n)
{
	if (n->parent->parent != NULL)
	{	
		return n->parent->parent;
	}
	else
	{
		return NULL;
	}
}

// Returns the successor node, or predecessor in this case
Node *successor(Node *n)
{
  // Go once to the left, and always to the right after that
  if (n->left != NULL)
  {

    Node *successor = n->left;
    while ( successor->right != NULL )
    {
      successor = successor->right;
    }

    return successor;
  }
  else
    return NULL;
}

// Returns the sibling of node n
Node *sibling(Node *n)
{
  if (n->parent->left == n)
    return n->parent->right;
  else
    return n->parent->left;
}

// Returns the red child of node n, otherwise returns NULL
Node *redChild(Node *n)
{
  // Both children are black since they both point to NULL
  if (n->left == NULL && n->right == NULL)
    return NULL;
  // If one child is NULL and the other is not, check for redness of the not NULL child
  // If right is not NULL, check if it is red
  else if (n->left == NULL && n->right != NULL)
    if (n->right->color == 0)
      return n->right;
    else
      return NULL;
  // If left is not NULL, check if is red
  else if (n->left != NULL && n->right == NULL)
    if (n->left->color == 0)
      return n->left;
    else
      return NULL;
  // Othewise if both are not NULL, check for the either of the children and return the first red encountered
  else
  {
    if (n->right->color == 0)
      return n->right;
    else if (n->left->color == 0)
      return n->left;
    else
      return NULL;
  }
}

// This is the recolor function, which is called recursively onto p
void recolor(Node *n)
{
  Node *p = n->parent;
  Node *s = sibling(n);

  // Set sibling color to red
  s->color = 1;

  // If the parent is black, recolor on it
  if ( p->color == 1 )
    recolor(p);
  // Else, just color it black and return
  else
    p->color = 1;
}

// Same as the recolor function, but if the node is a NULL
void null_recolor(Node *s)
{
  Node *p = s->parent;

  // Set sibling color to red
  s->color = 1;

  // If the parent is black, recolor on it
  if ( p->color == 1 )
    null_recolor(p);
  // Else, just color it black and return
  else
    p->color = 1;
}

// The rotations and the insert cases are all taken from the slides
void rotate_left(Node *n){

  Node *tmp_node; 
  char *tmp_key;
  char *tmp_def;

  tmp_node = n->left;
  tmp_key = n->key;
  tmp_def = n->definition;

  n->left = n->right; 
  n->key = n->right->key;
  n->definition = n->right->definition;

  n->right = n->left->right;
  // Now n->right's parent is n and not n->left anymore
  if (n->right != NULL)
    n->right->parent = n;

  n->left->right = n->left->left;
  // No need to modify parent of n->left->left

  n->left->left = tmp_node; 
  // Now n->left->left's parent is n->left and not n anymore
  if (n->left->left != NULL)
    n->left->left->parent = n->left;

  n->left->key = tmp_key;
  n->left->definition = tmp_def;
}

void rotate_right(Node *n) { 
  Node *tmp_node; 
  char *tmp_key; 
  char *tmp_def;

  tmp_node = n->right; 
  tmp_key = n->key; 
  tmp_def = n->definition;

  n->right = n->left; 
  n->key = n->left->key; 
  n->definition = n->left->definition;

  n->left = n->right->left; 
  // Now n->left's parent is n and not n->right anymore
  if (n->left != NULL)
    n->left->parent = n;

  n->right->left = n->right->right; 
  // No need to modify parent of n->right->right

  n->right->right = tmp_node; 
  // Now n->right->right's parent is n->right and not n anymore
  if (n->right->right != NULL)
    n->right->right->parent = n->right;

  n->right->key = tmp_key; 
  n->right->definition = tmp_def;
}

// Those are all test cases for Node n
void insert_case1(Node *n) {

   if (n->parent == NULL)
   {
        n->color = 1;
   } 
   else 
       insert_case2(n); 
} 

void insert_case2(Node *n){

 if (n->parent->color == 1)
 {
  return; /* Tree is still valid */
 }
 else
  insert_case3(n);
}

void insert_case3(Node *n){

 Node *u = uncle(n), *g;

 if ((u != NULL) && (u->color == 0)) {
  n->parent->color = 1;
  u->color = 1;
  g = grandparent(n);
  g->color = 0;
  insert_case1(g);
 } else {
  insert_case4(n);
 }
}

void insert_case4(Node *n){

  Node *g = grandparent(n);

  // This block takes care of all the nulls
  if (n->parent->left == NULL)
  {
  	if (g->left == NULL)
  	{
  		//printf("RR case\n");
  		rotate_left(g);
  		return;
  	}
  	else if (g->right == NULL)
  	{
  		//printf("LR case\n");
  		rotate_left(n->parent);
  	}
  }
  else if (n->parent->right == NULL)
  {
  	if (g->left == NULL)
  	{
  	  //printf("RL case\n");
  		rotate_right(n->parent);
  	}
  	else if (g->right == NULL)
  	{
  		//printf("LL case\n");
  		rotate_right(g);
  		return;
  	}
  }

  // This is for the rest of the cases where there are no nulls
  else if ( n == n->parent->right && n->parent == g->left ) 
  {
    rotate_left(n->parent);
    //no need to do this since just the key values were changed
    //n = n->left;
  } 
  else if ( n == n->parent->left && n->parent == g->right ) 
  {
    rotate_right(n->parent);
    //no need to do this since just the key values were changed
    //n = n->left;
  }
  
  insert_case5(n);
}

void insert_case5(Node *n){
  Node *g = grandparent(n);

  //no need to do this since just the key values were changed
  //n->parent->color = 1;
  //g->color = 0;

  if ( n == n->parent->left )
  {
    rotate_right(g);
  }
  else
  {
    rotate_left(g);
  }
}