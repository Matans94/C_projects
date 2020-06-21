//
// Created by matans on 09/12/2019.
//

#include "Structs.h"
#include <string.h>
#include <stdlib.h>

#define POW(x) (x*x)
#define SUCCESS 1
#define FAILURE 0;

/**
 * Compare enum for results of the vector's compare.
 */
typedef enum Compare
{
    LOWER = -1,
    EQUAL = 0,
    GREATER = 1
} Compare;


/**
 * It's compare between the vector double array value by value. if number is bigger than a numer
 * in b, its' return 1. if its equals its return 0, else -1.
 * @param a - first vector.
 * @param b - second vector.
 * @param n - The equal number of arguments to check.
 * @return - return the result of the compare by enum.
 */
int vectorCompareN(const Vector *a, const Vector *b, int n)
{
    for (int i = 0; i<n ; i++)
    {
        if (a->vector[i]< b->vector[i])
        {
            return LOWER;
        }
        if (a->vector[i] > b->vector[i])
        {
            return GREATER;
        }
    }
    return EQUAL;
}


/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    if (a == NULL || b == NULL)
    {
        return FAILURE
    }
    Vector *A = (Vector*) a;
    Vector *B = (Vector*) b;

    if (A->len == B->len)
    {
        return vectorCompareN(A, B, A->len);
    }
    else
    {
        int len = A->len < B->len ? A->len : B->len;
        int res = vectorCompareN(A, B, len);
        if (res == EQUAL)
        {
            if (len == B->len)
            {
                return GREATER;
            }
            return LOWER;
        }
        return res;
    }
}

/**
 * find the norma of the vector.
 * @param vector - The vector to calculate the norma
 * @return - returns the norma of the vector.
 */
double findVectorNormal(Vector *vector)
{
    double res = 0;
    for (int i = 0; i < vector->len; i++)
    {
        res += POW(vector->vector[i]);
    }
    return res;
}


/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    Vector *aVector = (Vector*)pVector;
    Vector *bVector = (Vector*)pMaxVector;

    if (aVector == NULL || bVector == NULL || aVector->vector == NULL)
    {
        return FAILURE
    }

    if (bVector->vector == NULL)
    {
        bVector->vector = (double *) malloc(aVector->len * sizeof(double));
        if (bVector->vector == NULL)
        {
            return FAILURE
        }
        memcpy(bVector->vector, aVector->vector, aVector->len * sizeof(double));
        bVector->len = aVector->len;
        return SUCCESS;
    }
    else
    {
        double aNorm = 0, bNorm = 0;
        aNorm = findVectorNormal(aVector);
        bNorm = findVectorNormal(bVector);

        if (aNorm > bNorm)
        {
            bVector->vector = (double *) realloc(bVector->vector, aVector->len * sizeof(double));
            if (bVector->vector == NULL)
            {
                return FAILURE
            }
            memcpy(bVector->vector, aVector->vector, aVector->len * sizeof(double));
            bVector->len = aVector->len;
            return SUCCESS;
        }
    }
    return FAILURE
}


/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    Vector *vector = (Vector*) malloc(sizeof(Vector));

    if (vector == NULL)
    {
        return NULL;
    }
    vector->vector = NULL;
    vector->len = 0;
    forEachRBTree(tree, copyIfNormIsLarger, vector);

    return vector;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    if (pVector != NULL)
    {
        Vector *aVector = (Vector *) pVector;
        if (aVector->vector != NULL)
        {
            free(aVector->vector);
            aVector->vector = NULL;
        }
        free(aVector);
    }
}


/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    char *A = (char*) a;
    char *B = (char*) b;

    return (strcmp(A, B));
}

/**
 * ForEach function that concatenates the given word to pConcatenated.
 * pConcatenated is already allocated with
 * enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    char* res = NULL;
    res = (strcat(pConcatenated, word));
    if (res == NULL)
    {
        return FAILURE
    }
    res = strcat(res, "\n");
    if (res == NULL)
    {
        return FAILURE
    }
    return SUCCESS;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    if (s != NULL)
    {
        free(s);
    }
}
