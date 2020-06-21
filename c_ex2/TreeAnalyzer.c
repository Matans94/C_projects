#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_ROW 1024
#define NUMBER_OF_ARGS 4
#define WRONG_ARGC "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n"
#define INVALID_INPUT "Invalid input\n"
#define ROOT_ANNOUNCE "Root Vertex: "
#define VERTICES_ANNOUNCE "Vertices Count: "
#define EDGES_ANNOUNCE "Edges Count: "
#define LENGTH_MIN_BRANCH "Length of Minimal Branch: "
#define LENGTH_MAX_BRANCH_ANNOUNCE "Length of Maximal Branch: "
#define DIAMETER_ANNOUNCE "Diameter Length: "
#define PATH_ANNOUNCE "Shortest Path Between "
#define ERROR_MALLOC "Memory allocation failed\n"
#define WIN_END_LINE "\r\n"
#define LINUX_END_LINE "\n"
#define DELIMITER_TEXT " \r\n"
#define TRUE 1
#define FALSE 0

/**
 * struct for every node. contain all the detail for the program to run well.
 */
struct Vertex
{
    int key;
    struct Vertex *parent;
    struct Vertex **childArray;
    struct Vertex *prev;
    int isLeaf;
    int sonFlag;
    int canBeRoot;
    int childNum;
    int dist;
    int inArray;
};

/**
 * The struct of the tree that the code pass between the functions.
 */
struct Tree
{
    int numOfInputVertex;
    int numOfVertex;
    int EdgeNum;
    int vertexU;
    int vertexV;
    struct Vertex *vertexArray;
    struct Vertex *lowestNode;
};

/**
 * Checks if the input char can be an int.
 * @param checkChar - the char that the method check.
 * @return - 0 if it fail, 1 for success.
 */
int canBeInteger(const char checkChar[], int *result)
{
    char *ptr = NULL;
    *result = (int) strtol(checkChar, &ptr, 10);
    if (*ptr == '\0' && *result >= 0)
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * Swetch between char to num.
 * @param str - the char target to switch.
 * @return the int number from the char.
 */
int strToInt(const char str[])
{
    int num;
    sscanf(str, "%d", &num);
    return num;
}

/**
 * Checks of the input from the CLI is valid.
 * 4 args should be in the input, 1 path and 2 integers. (another one for /main)
 * @param argc - the number of the args.
 * @param argv - the array of chars.
 * @return - 0 if its fail, otherwise 1.
 */
int isValidCli(const int argc, const char *argv[], struct Tree *newTree)
{
    if ((argc != NUMBER_OF_ARGS))
    {
        fprintf(stderr, WRONG_ARGC);
        return FALSE;
    }
    int vertexU = 0, vertexV = 0;
    if (!canBeInteger(argv[2], &vertexU) || !canBeInteger(argv[3], &vertexV))
    {
        fprintf(stderr, INVALID_INPUT);
        return FALSE;
    }
    newTree->vertexU = strToInt(argv[2]);
    newTree->vertexV = strToInt(argv[3]);
    return TRUE;
}

/**
 * Initialize array for every parent tree. It allocate a new storage for array of pointers to
 * nodes struct that points to the vertex array in the tree's struct.
 * Free of the space will be done in other method because it use that storage in the proccess of
 * the building tree and all the prints. If there is any problem after the initialize, the code
 * would call the free func.
 * @param newTree - The tree struct that always updates and changing.
 * @param parentVertex - the parent that need to sign up all the children.
 */
int buildChildArray(struct Tree *newTree, struct Vertex **parentVertex)
{
    (*parentVertex)->childArray = (struct Vertex **) malloc(sizeof(struct Vertex *) *
                                                            (*parentVertex)->childNum);
    if ((*parentVertex)->childArray != NULL)
    {
        int index = 0;
        for (int i = 0; i < newTree->numOfInputVertex; i++)
        {
            if (newTree->vertexArray[i].sonFlag == 1)
            {
                (*parentVertex)->childArray[index] = &newTree->vertexArray[i];
                newTree->vertexArray[i].sonFlag = 0;
                newTree->vertexArray[i].canBeRoot = 0;
                index++;
            }
        }
        return TRUE;
    }
    else
    {
        printf("%s", ERROR_MALLOC);
        return FALSE;
    }

}

/**
 * This function parse the row. take every char of the input line, and separate by the checks by
 * spaces. The main input that should be valid is "-" and an integer or null line. other wise, it
 * will be an invalid input and it will be failed.
 * @param line - The line in the text.
 * @param newTree - The tree struct that always updates and changing.
 * @param lineNumber - The line number from the txt file. it should be the key of the father node.
 * @return - True if all pass and initialize right, otherwise, false.
 */
int rowParser(char *line, struct Tree *newTree, int lineNumber)
{
    int parentVertex = lineNumber - 2;
    const char *parse;
    parse = strtok(line, DELIMITER_TEXT);
    int parseInt = 0;
    if (parse == NULL || *parse == '\0')  //empty line is illegal!
    {
        fprintf(stderr, INVALID_INPUT);
        return FALSE;
    }
    else if (!strcmp(parse, "-"))  //equals
    {
        {
            parse = strtok(NULL, " ");
            if (parse == NULL)
            {
                newTree->vertexArray[parentVertex].isLeaf = 1;
            }
            else if (strcmp(parse, LINUX_END_LINE) != 0 && strcmp(parse, WIN_END_LINE) != 0)
            {
                fprintf(stderr, INVALID_INPUT);
                return FALSE;
            }
        }
        return TRUE;

    }
    else if (canBeInteger(parse, &parseInt))
    {
        while ((parse == NULL) == 0)
        {
            int newVertexKey = strToInt(parse);
            if (0 <= newVertexKey && newVertexKey < newTree->numOfInputVertex &&
                newTree->vertexArray[newVertexKey].sonFlag != 1 &&
                newTree->vertexArray[newVertexKey].inArray == 0)
            {
                //newTree->vertexArray[newVertexKey].isLeaf = 0;
                newTree->vertexArray[newVertexKey].parent = &newTree->vertexArray[parentVertex];
                newTree->vertexArray[newVertexKey].sonFlag = 1;
                newTree->vertexArray[newVertexKey].inArray = 1;
                newTree->numOfVertex++, newTree->EdgeNum++;
                newTree->vertexArray[parentVertex].childNum++;
                parse = strtok(NULL, " \r\n");
            }
            else
            {
                fprintf(stderr, INVALID_INPUT);
                return FALSE;
            }
        }
        struct Vertex *daddy = &newTree->vertexArray[parentVertex];
        if (buildChildArray(newTree, &daddy))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        fprintf(stderr, INVALID_INPUT);
        return FALSE;
    }
}

/**
 * Initialize all the vertex in the tree's array. It use memory allocate for full dynamic array
 * by the n' number from the first line of a valid text. The free function will be used when the
 * program fails and then it will free all the array, or when the program print all of the prints
 * and then free all the array.
 * @param newTree - The tree struct that always updates and changing.
 */
int initializeArrayWithVertex(struct Tree *newTree)
{
    newTree->vertexArray = malloc(sizeof(struct Vertex) * newTree->numOfInputVertex);

    if (newTree->vertexArray != NULL)
    {
        for (int i = 0; i < newTree->numOfInputVertex; i++)
        {
            newTree->vertexArray[i].key = i;
            newTree->vertexArray[i].canBeRoot = 1;
            newTree->vertexArray[i].sonFlag = 0;
            newTree->vertexArray[i].childNum = 0;
            newTree->vertexArray[i].isLeaf = 0;
            newTree->vertexArray[i].parent = NULL;
            newTree->vertexArray[i].prev = NULL;
            newTree->vertexArray[i].childArray = NULL;
            newTree->vertexArray[i].dist = -1;
            newTree->vertexArray[i].inArray = 0;
        }
        return TRUE;
    }
    else
    {
        printf("%s", ERROR_MALLOC);
        return FALSE;
    }
}

/**
 * This method open the file and first check if it is a valid route. Then it parse every line in
 * the text file and check if it a valid input. All the nodes should be positive or 0 and smaller
 * than the n' number that in the first line of the text.
 * @param textDirectory - The route to find the text file.
 * @param newTree - The tree struct that always updates and changing.
 * @return - True if all success or False if there is any problem with the input.
 */
int buildingTree(const char *textDirectory, struct Tree *newTree)
{
    FILE *textFile = fopen(textDirectory, "r");
    if (textFile == NULL)
    {
        fprintf(stderr, INVALID_INPUT);
        return FALSE;
    }
    char line[MAX_ROW];
    int row = 1;
    fgets(line, sizeof(line), textFile);
    char *check;
    check = strtok(line, DELIMITER_TEXT);
    int checkInt = 0;
    if (!canBeInteger(check, &checkInt) || strToInt(line) <= 0)
    {
        fprintf(stderr, INVALID_INPUT);
        fclose(textFile);
        return FALSE;
    }
    newTree->numOfInputVertex = strToInt(line);
    initializeArrayWithVertex(newTree);
    if (newTree->vertexU >= newTree->numOfInputVertex || newTree->vertexV >=
        newTree->numOfInputVertex)
    {
        fprintf(stderr, INVALID_INPUT);
        fclose(textFile);
        return FALSE;
    }
    //building array in length of the n' number. Every cell will include vertex.
    row++;
    while ((fgets(line, sizeof(line), textFile) != NULL))
    {
        if (!rowParser(line, newTree, row))
        {
            fclose(textFile);
            return FALSE;
        }
        row++;
    }
    fclose(textFile);
    if (newTree->numOfInputVertex != newTree->numOfVertex + 1 ||
        row != newTree->numOfInputVertex + 2)
    {
        fprintf(stderr, INVALID_INPUT);
        return FALSE;
    }
    return TRUE;
}

/**
 * This function pass through all the nodes and check if the canBeRoot value the initialize in
 * every node is true. If there is more than 1 root, it will be fails.
 * @param newTree The tree struct that always updates and changing.
 * @param rootValue - The key that update to be the root's key.
 * @return - True if all success or False otherwise.
 */
int findTreeRoot(struct Tree *newTree, int *const rootValue)
{
    int rootCounter = 0;
    for (int i = 0; i < newTree->numOfInputVertex; i++)
    {
        if (newTree->vertexArray[i].canBeRoot == 1)
        {
            *rootValue = newTree->vertexArray[i].key;
            rootCounter++;
        }
    }
    if (rootCounter != 1)
    {
        return FALSE;
    }
    return TRUE;
}

/**
 * This function checks for the minimal branch and the maximal branch. It's take two pointers and
 * pass through all the nodes that are leafs and go up until they meet the root. then it compare
 * for the bigger value or a smaller value.
 * @param newTree - The tree struct that always updates and changing.
 * @param minBranch - The pointer that should be update outside the function.
 * @param maxBranch - The pointer that should be update outside the function.
 */
void findMinMaxBranch(struct Tree *newTree, int *const minBranch, int *const maxBranch)
{
    if (newTree->numOfInputVertex == 1)
    {
        *maxBranch = 0;
        *minBranch = 0;
    }
    for (int i = 0; i < newTree->numOfInputVertex; i++)
    {
        if (newTree->vertexArray[i].isLeaf == 1)
        {
            int routeLen = 0;
            struct Vertex node = newTree->vertexArray[i];
            while (node.parent != NULL)
            {
                routeLen++;
                node = *node.parent;
            }
            if (routeLen > *maxBranch)
            {
                newTree->lowestNode = &newTree->vertexArray[i];
                *maxBranch = routeLen;
            }
            if (routeLen < *minBranch || *minBranch == 0)
            {
                *minBranch = routeLen;
            }
        }
    }
}

/**
 * BFS algorithm. It has queue. It start from the starting node and checks for children or parent
 * and insert it to the queue. It is run like BFS algorithm.
 * @param newTree - The tree struct that always updates and changing.
 * @param startNode - The node that the bfs run and insert to the queue first.
 * @param routeLen - The longest route pointer that update from out side the function.
 */
void BFS(struct Tree *newTree, struct Vertex *startNode, int *const routeLen)
{
    for (int i = 0; i < newTree->numOfInputVertex; i++)
    {
        newTree->vertexArray[i].dist = -1;
        newTree->vertexArray[i].prev = NULL;
    }
    startNode->dist = 0;
    Queue *queue = allocQueue();
    enqueue(queue, startNode->key);
    struct Vertex *u;
    while (queue->head != NULL)
    {
        u = &newTree->vertexArray[dequeue(queue)];
        for (int i = 0; i < u->childNum; i++)
        {
            if (u->childNum > 0 && u->childArray[i]->dist == -1)
            {
                enqueue(queue, u->childArray[i]->key);
                u->childArray[i]->prev = u;
                u->childArray[i]->dist = u->dist + 1;
            }
        }
        if (u->parent != NULL && u->parent->dist == -1)
        {
            enqueue(queue, u->parent->key);
            u->parent->prev = u;
            u->parent->dist = 0;
            u->parent->dist = u->dist + 1;
        }
        *routeLen = u->dist; // the last from the queue.
    }
    freeQueue(&queue);
}

/**
 * Find the diameter of the tree. It first check if there is only one node in the tree. if it is'
 * the diameter should be 0, else, it called to bfs algorithm with the lowest node that this
 * value was find before.
 * @param newTree - The tree struct that always updates and changing.
 * @param diameter - The longest route pointer that update from out side the function.
 */
void findDiameter(struct Tree *newTree, int *const diameter)
{
    if (newTree->numOfInputVertex == 1)
    {
        *diameter = 0;
    }
    else
    {
        *diameter = 0;
        BFS(newTree, newTree->lowestNode, diameter);
    }
}

/**
 * This functions run the bfs from vertexU that it gets from the input and then print, node by
 * node, the prev node that the node holds.
 * @param newTree - The tree struct that always updates and changing.
 * @param diameter - The longest route pointer that update from out side the function.
 */
void findAndPrintShortestPath(struct Tree *newTree, int *const diameter)
{
    BFS(newTree, &newTree->vertexArray[newTree->vertexV], diameter);
    struct Vertex w = newTree->vertexArray[newTree->vertexU]; //const Vertex
    if (newTree->vertexU != newTree->vertexV)
    {
        printf("%d ", w.key);
        w = *(w.prev);
        while (w.key != newTree->vertexV)
        {
            printf("%d ", w.key);
            w = *(w.prev);
        }
        printf("%d\n", w.key);
    }
    else
    {
        printf("%d\n", w.key);
    }
}

/**
 * A function that print all the values according to the tree.
 * @param newTree - The tree struct that always updates and changing.
 */
void programPrint(struct Tree *newTree)
{
    int rootValue;
    if (!findTreeRoot(newTree, &rootValue))
    {
        fprintf(stderr, INVALID_INPUT);
    }
    printf("%s%d\n", ROOT_ANNOUNCE, rootValue);
    printf("%s%d\n", VERTICES_ANNOUNCE, newTree->numOfInputVertex);
    printf("%s%d\n", EDGES_ANNOUNCE, newTree->EdgeNum);

    int minBranch = 0, maxBranch = 0;
    findMinMaxBranch(newTree, &minBranch, &maxBranch);
    printf("%s%d\n", LENGTH_MIN_BRANCH, minBranch);
    printf("%s%d\n", LENGTH_MAX_BRANCH_ANNOUNCE, maxBranch);

    int diameter;
    findDiameter(newTree, &diameter);
    printf("%s%d\n", DIAMETER_ANNOUNCE, diameter);

    printf("%s%d and %d: ", PATH_ANNOUNCE, newTree->vertexU, newTree->vertexV);

    findAndPrintShortestPath(newTree, &diameter);
}

/**
 * Free all the malloc that find before. It is go first and clear the children array if there is
 * any, and then free the space in the cell of the vertexArray.
 * @param newTree - The tree struct that always updates and changing.
 */
void freeSpace(struct Tree *newTree)
{
    for (int i = 0; i < newTree->numOfInputVertex; i++)
    {
        if (newTree->vertexArray[i].childArray != NULL)
        {
            free(newTree->vertexArray[i].childArray);
            newTree->vertexArray[i].childArray = NULL;
        }
    }
    free(newTree->vertexArray);
    newTree->vertexArray = NULL;
}

/**
 * The main function the gets the input from the CLI and checks if the input is valid. Then it
 * checks for the info in the text file and try to build a tree. It there is any problem it will
 * print a invalid input msg, else it build the tree and print it information.
 * @param argc - The number of the inputs.
 * @param argv - The chars from the CLI.
 * @return - EXIT_FAILURE if it finds any problem or invalid input (CLI or text) or EXIT_SUCCESS
 * if everything is ok and the information of the tree printed well.
 */
int main(const int argc, const char *argv[])
{
    /**
     * init tree int vals.
     */
    struct Tree newTree =
    {
            .EdgeNum = 0,
            .numOfVertex = 0
    };
    if (!isValidCli(argc, argv, &newTree))
    {
        return EXIT_FAILURE;
    }
    if (!buildingTree(argv[1], &newTree))
    {
        freeSpace(&newTree);
        return EXIT_FAILURE;
    }
    programPrint(&newTree);
    freeSpace(&newTree);
    return EXIT_SUCCESS;
}