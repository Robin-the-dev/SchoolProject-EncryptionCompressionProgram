/******************************************************************************
File: Huffman.h
Author: Robin Hanjun Kwon
Date: Friday, 16th of October
Reference:
  - https://github.com/JiangxueHan529/Huffman-compression
  - https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
  - https://github.com/Robin-the-dev/Reference_HuffmanAlgorithm
******************************************************************************/

#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Macros */
#define MAX_ASCII 256

/* Structures */
struct MinHeapNode {
	unsigned freq;
	int data;
    struct MinHeapNode* left;
    struct MinHeapNode* right;
};
typedef struct MinHeapNode MinHeapNode_t;

/* Function Prototypes */
unsigned getThisBit(int x, int y, int z);
void putBit(unsigned cur, FILE* src, int flag);
int getBit(FILE* src);
unsigned getFrequency(FILE* src);
int findMinimum();
void initialiseNode();
void buildTree();
void destructTree(MinHeapNode_t* root);
void getBinary(MinHeapNode_t* node, unsigned code, int length);
void putBinary();
void compress(FILE* src, char input[]);
void restructTree();
void rebuildTree(int data);
void decompress(FILE* src1, FILE* src2, char input[]);

#endif
