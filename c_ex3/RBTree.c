//
// Created by matans on 09/12/2019.
//
#include "RBTree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SUCCESS 1;
#define FAILURE 0;

/**
 * Four types of rotations.
 */
typedef enum Rotation
{
    LR_ROTATION__,
    RL_ROTATION__,
    R_ROTATION__,
    L_ROTATION__
} Rotation;

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *newRBTree = malloc(sizeof(RBTree));
    if (newRBTree == NULL)
    {
        return NULL;
    }
    newRBTree->compFunc = compFunc;
    newRBTree->freeFunc = freeFunc;
    newRBTree->root = NULL;
    newRBTree->size = 0;
    return newRBTree;
}

/**
 * Method that find where to insert the new node by the compare func of the tree.
 * It is a recursive function that search for a node in the tree that is a leaf.
 * @param tree -the tree to add an item to.
 * @param root - The root of the sub-tree.
 * @param data - The new value that the new node will have.
 * @param newNode - The pointer for the new node.
 */
void recursiveInsertion(RBTree *tree, Node *root, void *data, Node *newNode)
{
    if (root == NULL)
    {
        return;
    }
    if (tree->compFunc(root->data, data) > 0)
    {
        if (root->left == NULL)
        {
            root->left = newNode;
            newNode->parent = root;
            return;
        }
        else
        {
            recursiveInsertion(tree, root->left, data, newNode);
        }
    }
    else
    {
        if (root->right == NULL)
        {
            root->right = newNode;
            newNode->parent = root;
            return;
        }
        else
        {
            recursiveInsertion(tree, root->right, data, newNode);
        }
    }
}


/**
 * Insert node to the tree according to regular laws of the binary search tree.
 * @param tree : the tree to add an item to.
 * @param data: item to add to the tree.
 */
Node *insertNode(RBTree *tree, void *data)
{
    if (containsRBTree(tree, data))
    {
        return NULL;
    }
    Node *newNode = malloc(sizeof(Node));
    tree->size++;
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->parent = NULL;
    newNode->color = RED;
    newNode->data = data;

    if (tree->root == NULL)
    {
        tree->root = newNode;
        newNode->color = BLACK;
    }
    else
    {
        recursiveInsertion(tree, tree->root, data, newNode);
    }

    return newNode;
}

/**
 * func that do the rotations in the tree. If it is double circle in the tree, it do it right
 * when its end.
 * @param node - The node that needed evaluate the iteration.
 * @param rotationType - The enum type of the rotation.
 */
void rotation(Node *node, int rotationType)
{
    switch (rotationType)
    {
        case LR_ROTATION__:
        {
            node->parent = node->parent->parent;
            node->parent->left->right = node->left;

            if (node->left != NULL)
            {
                node->left->parent = node->parent->left;
            }
            node->left = node->parent->left;
            node->left->parent = node;
            node->parent->left = node;
            rotation(node, R_ROTATION__);
            break;
        }


        case R_ROTATION__:
        {
            node->color = BLACK;
            node->parent->color = RED;

            node->parent->left = node->right;
            if (node->right != NULL)
            {
                node->right->parent = node->parent;
            }
            node->right = node->parent;
            node->parent = node->parent->parent;
            if (node->parent != NULL)
            {
                if (node->parent->left == node->right)
                {
                    node->parent->left = node;
                }
                else
                {
                    node->parent->right = node;
                }
            }
            node->right->parent = node;
            break;
        }
        case RL_ROTATION__:
        {
            node->parent = node->parent->parent;
            node->parent->right->left = node->right;

            if (node->right != NULL)
            {
                node->right->parent = node->parent->right;
            }
            node->right = node->parent->right;
            node->right->parent = node;
            node->parent->right = node;
            rotation(node, L_ROTATION__);
            break;
        }

        case L_ROTATION__:
        {
            node->color = BLACK;
            node->parent->color = RED;

            node->parent->right = node->left;
            if (node->left != NULL)
            {
                node->left->parent = node->parent;
            }
            node->left = node->parent;
            node->parent = node->parent->parent;
            if (node->parent != NULL)
            {
                if (node->parent->right == node->left)
                {
                    node->parent->right = node;
                }
                else
                {
                    node->parent->left = node;
                }
            }
            node->left->parent = node;

            break;
        }
        default:
            printf("Wrong rotation type");
    }
}

/**
 * function that updates the tree. It changes color of the nodes if it needed and check if any
 * rotation is needed.
 * @param node - The new node that last inserted.
 */
void updateTree(Node *node)
{
    if (node->parent != NULL && node->parent->color == RED)
    {
        if (node->parent->parent != NULL)
        {  //3
            Node *grandpa = node->parent->parent;
            Node *uncle = grandpa->left;
            if (uncle == node->parent)
            {
                uncle = grandpa->right;
            }
            if (uncle != NULL && uncle->color == RED)
            {
                uncle->color = BLACK;
                node->parent->color = BLACK;
                grandpa->color = RED;
                updateTree(grandpa);
            }
            else  //4
            {
                if (grandpa->left != NULL && node == grandpa->left->right)
                {
                    rotation(node, LR_ROTATION__);
                }
                else if (grandpa->right != NULL && node == grandpa->right->left)
                {
                    rotation(node, RL_ROTATION__);
                }
                else if (grandpa->left != NULL && node == grandpa->left->left)
                {
                    rotation(node->parent, R_ROTATION__);
                }
                else if (grandpa->right != NULL && node == grandpa->right->right)
                {
                    rotation(node->parent, L_ROTATION__);
                }
            }
        }
    }
}

/**
 * update the root pointer of the tree.
 * @param tree
 * @return
 */
int updateRoot(RBTree *tree)
{
    Node *ptr = tree->root;
    while (ptr->parent != NULL)
    {
        ptr = ptr->parent;
    }
    tree->root = ptr;
    tree->root->color = BLACK;
    return SUCCESS
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return FAILURE
    }
    Node *newNode = insertNode(tree, data);
    if (newNode == NULL)
    {
        return FAILURE
    }
    updateTree(newNode);
    updateRoot(tree);
    return SUCCESS
}

/**
 * check whether the tree contains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int containsRBTree(RBTree *tree, void *data)
{
    Node *node = tree->root;
    while (node != NULL)
    {
        if (node->data == data)
        {
            return SUCCESS
        }
        else if (tree->compFunc(node->data, data) > 0)
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    return FAILURE
}

/**
 * find the smallest data node in the tree.
 * @param start - The root of the subtree to calculate on the smallest value.
 * @return - a pointer to that node.
 */
Node *smallestDataNode(Node *start)
{
    if (start == NULL)
    {
        return NULL;
    }
    Node *runner = start;
    while (runner->left != NULL)
    {
        runner = runner->left;
    }
    return runner;
}

/**
 * successor function that scan what is the smallest big value of the node.
 * @param node - The node that we want the successor of.
 * @return - a pointer to a successor.
 */
Node *successor(Node *node)
{
    if (node->right != NULL)
    {
        return smallestDataNode(node->right);
    }
    Node *parent = node->parent;
    while (parent != NULL && node == parent->right)
    {
        node = parent;
        parent = node->parent;
    }
    return parent;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order.
 * if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args)
{
    Node *node = smallestDataNode(tree->root);
    while (node != NULL)
    {
        if (!func(node->data, args))
        {
            return FAILURE
        }
        node = successor(node);
    }
    return SUCCESS
}

/**
 * recursive func that free all the node from the subtree of the given node,
 * @param tree - the tree to free that all the nodes are there.
 * @param node - The node that we want to scan from the subtree and free all the subtree.
 */
void recursiveFree(RBTree *tree, Node *node)
{
    if (node == NULL)
    {
        return;
    }
    recursiveFree(tree, node->left);
    recursiveFree(tree, node->right);

    tree->freeFunc(node->data);

    free(node);
    node = NULL;
}

/**
 * free all memory of the data structure.
 * @param tree: the tree to free.
 */
void freeRBTree(RBTree *tree)
{
    recursiveFree(tree, tree->root);
    free(tree);
    tree = NULL;
}