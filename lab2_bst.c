/*
 *	Operating System Lab
 *	    Lab2 (Synchronization)
 *	    Student id : 
 *	    Student name : 
 *
 *   lab2_bst.c :
 *       - thread-safe bst code.
 *       - coarse-grained, fine-grained lock code
 *
 *   Implement thread-safe bst for coarse-grained version and fine-grained version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"

int print_count;
pthread_mutex_t mutex_global = PTHREAD_MUTEX_INITIALIZER;

/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree *tree) {
	// You need to implement lab2_node_print_inorder function.
	print_count = 0;
	lab2_node_print(tree->root);
	return print_count;
}

/*
 *
 *
 *
 *
 *
 */
void lab2_node_print(lab2_node *node){
	if(node){
		lab2_node_print(node->left);
		print_count++;
		lab2_node_print(node->right);
	}
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
	// You need to implement lab2_tree_create function.
	lab2_tree* newTree = (lab2_tree*)malloc(sizeof(lab2_tree));
	assert(newTree != NULL);
	newTree->root = NULL;

	return newTree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
	// You need to implement lab2_node_create function.
	lab2_node* newNode = (lab2_node*)malloc(sizeof(lab2_node));
	assert(newNode != NULL);
	pthread_mutex_init(&newNode->mutex, NULL);
	newNode -> left = NULL;
	newNode -> right = NULL;
	newNode -> key = key;

	return newNode;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
	// You need to implement lab2_node_insert function.
	if(!tree->root){
		tree->root=new_node;
		return LAB2_SUCCESS;
	}

	lab2_node *p_node = NULL, *c_node = tree->root;

	while(c_node){
		if(new_node -> key == c_node -> key)
			return LAB2_ERROR;
		else if(new_node -> key < c_node -> key){
			p_node = c_node;
			c_node = c_node -> left;
		}
		else{
			p_node = c_node;
			c_node = c_node -> right;
		}
	}

	if(new_node->key < p_node->key)
		p_node -> left = new_node;
	else
		p_node -> right = new_node;

	return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
	// You need to implement lab2_node_insert_fg function.
START:
	if(!tree->root){
		if(pthread_mutex_lock(&mutex_global))
			goto START;
		tree->root=new_node;
		pthread_mutex_unlock(&mutex_global);
		return LAB2_SUCCESS;
	}
	else{
		lab2_node *p_node = NULL, *c_node = tree->root;

		while(c_node){
			if(p_node!=NULL)
				pthread_mutex_unlock(&p_node->mutex);
			pthread_mutex_lock(&c_node->mutex);
			if(new_node -> key == c_node -> key){
				pthread_mutex_unlock(&c_node->mutex);
				return LAB2_ERROR;
			}
			else if(new_node -> key < c_node -> key){
				p_node = c_node;
				c_node = c_node -> left;
			}
			else{
				p_node = c_node;
				c_node = c_node -> right;
			}
		}
		if(new_node->key < p_node->key)
			p_node -> left = new_node;
		else
			p_node -> right = new_node;
		pthread_mutex_unlock(&p_node->mutex);
	}
	return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
	// You need to implement lab2_node_insert_cg function.
	pthread_mutex_lock(&mutex_global);
	if(!tree->root){
		tree->root=new_node;
	}
	else{
		lab2_node *p_node = NULL, *c_node = tree->root;

		while(c_node){
			if(new_node -> key == c_node -> key){
				pthread_mutex_unlock(&mutex_global);
				return LAB2_ERROR;
			}
			else if(new_node -> key < c_node -> key){
				p_node = c_node;
				c_node = c_node -> left;
			}
			else{
				p_node = c_node;
				c_node = c_node -> right;
			}
		}

		if(new_node->key < p_node->key)
			p_node -> left = new_node;
		else
			p_node -> right = new_node;
	}

	pthread_mutex_unlock(&mutex_global);
	return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key) {
	// You need to implement lab2_node_remove function.
	lab2_node *p_node = NULL;
	lab2_node *c_node = tree->root;

	while(c_node){
		if(key == c_node->key)
			break;
		else if(key < c_node->key){
			p_node = c_node;
			c_node = c_node->left;
		}else{
			p_node = c_node;
			c_node = c_node->right;
		}
	}
	// If tree doesn't have a key
	if(!c_node)
		return LAB2_ERROR;

	if(c_node->left && c_node->right){
		lab2_node *r_p_node = c_node, *r_c_node = c_node->right;
		while(r_c_node->left){
			r_p_node = r_c_node;
			r_c_node = r_c_node->left;
		}
		c_node->key = r_c_node->key;
		if(r_p_node->right==r_c_node)
			r_p_node->right=r_c_node->right;
		else
			r_p_node->left=r_c_node->right;
		free(r_c_node);
	}
	else if(c_node -> left || c_node->right){
		if(c_node->left){
			if(c_node==tree->root)
				tree->root=c_node->left;
			else if(p_node->left==c_node)
				p_node->left=c_node->left;
			else
				p_node->right=c_node->left;
		}else{
			if(c_node==tree->root)
				tree->root=c_node->right;
			else if(p_node->left==c_node)
				p_node->left=c_node->right;
			else
				p_node->right=c_node->right;
		}
		free(c_node);
	}else{
		if(c_node==tree->root)
			tree->root=NULL;
		else if(p_node->left==c_node)
			p_node->left=NULL;
		else
			p_node->right=NULL;
		free(c_node);
	}

	return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
	// You need to implement lab2_node_remove_fg function.
	lab2_node *p_node, *c_node;
START:
	if(!tree->root){
		return LAB2_ERROR;
	}
	p_node = NULL;
	c_node = tree->root;

	while(c_node){
		if(pthread_mutex_lock(&c_node->mutex))
			goto START;
		if(key == c_node->key){
			pthread_mutex_unlock(&c_node->mutex);
			break;
		}
		else if(key < c_node->key){
			p_node = c_node;
			c_node = c_node->left;
		}else{
			p_node = c_node;
			c_node = c_node->right;
		}
		pthread_mutex_unlock(&p_node->mutex);
	}
	// If tree doesn't have a key
	if(!c_node)
		return LAB2_ERROR;
	
	if(pthread_mutex_lock(&c_node->mutex))
		goto START;
	if(c_node->left && c_node->right){
		lab2_node *r_p_node = c_node, *r_c_node = c_node->right;
		while(r_c_node->left){
			r_p_node = r_c_node;
			r_c_node = r_c_node->left;
		}
		c_node->key = r_c_node->key;
		if(r_p_node->right==r_c_node)
			r_p_node->right=r_c_node->right;
		else
			r_p_node->left=r_c_node->right;
		free(r_c_node);
		pthread_mutex_unlock(&c_node->mutex);
	}
	else if(c_node -> left || c_node->right){
		if(c_node->left){
			if(c_node==tree->root)
				tree->root=c_node->left;
			else if(p_node->left==c_node)
				p_node->left=c_node->left;
			else
				p_node->right=c_node->left;
		}else{
			if(c_node==tree->root)
				tree->root=c_node->right;
			else if(p_node->left==c_node)
				p_node->left=c_node->right;
			else
				p_node->right=c_node->right;
		}
		pthread_mutex_unlock(&c_node->mutex);
		free(c_node);
	}else{
		if(c_node==tree->root)
			tree->root=NULL;
		else if(p_node->left==c_node)
			p_node->left=NULL;
		else
			p_node->right=NULL;
		pthread_mutex_unlock(&c_node->mutex);
		free(c_node);
	}

	return LAB2_SUCCESS;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
	// You need to implement lab2_node_remove_cg function.
	pthread_mutex_lock(&mutex_global);
	
	lab2_node *p_node = NULL;
	lab2_node *c_node = tree->root;

	while(c_node){
		if(key == c_node->key)
			break;
		else if(key < c_node->key){
			p_node = c_node;
			c_node = c_node->left;
		}else{
			p_node = c_node;
			c_node = c_node->right;
		}
	}
	// If tree doesn't have a key
	if(!c_node){
		pthread_mutex_unlock(&mutex_global);
		return LAB2_ERROR;
	}

	if(c_node->left && c_node->right){
		lab2_node *r_p_node = c_node, *r_c_node = c_node->right;
		while(r_c_node->left){
			r_p_node = r_c_node;
			r_c_node = r_c_node->left;
		}
		c_node->key = r_c_node->key;
		if(r_p_node->right==r_c_node)
			r_p_node->right=r_c_node->right;
		else
			r_p_node->left=r_c_node->right;
		free(r_c_node);
	}
	else if(c_node -> left || c_node->right){
		if(c_node->left){
			if(c_node==tree->root)
				tree->root=c_node->left;
			else if(p_node->left==c_node)
				p_node->left=c_node->left;
			else
				p_node->right=c_node->left;
		}else{
			if(c_node==tree->root)
				tree->root=c_node->right;
			else if(p_node->left==c_node)
				p_node->left=c_node->right;
			else
				p_node->right=c_node->right;
		}
		free(c_node);
	}else{
		if(c_node==tree->root)
			tree->root=NULL;
		else if(p_node->left==c_node)
			p_node->left=NULL;
		else
			p_node->right=NULL;
		free(c_node);
	}

	pthread_mutex_unlock(&mutex_global);
	return LAB2_SUCCESS;
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
	// You need to implement lab2_tree_delete function.
	lab2_node_delete(tree->root);
	free(tree);
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
	// You need to implement lab2_node_delete function
	if(node){
		lab2_node *left_node = node->left;
		lab2_node *right_node = node->right;
		free(node);
		lab2_node_delete(left_node);
		lab2_node_delete(right_node);
	}
}

