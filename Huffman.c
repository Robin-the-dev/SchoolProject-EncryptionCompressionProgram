/******************************************************************************
File: Huffman.c
Author: Robin Hanjun Kwon
Date: Friday, 16th of October
Reference:
  - https://github.com/JiangxueHan529/Huffman-compression
  - https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
  - https://github.com/Robin-the-dev/Reference_HuffmanAlgorithm
******************************************************************************/

/* Libraries */
#include "Core.h"
#include "Huffman.h"

/* Global Variables */
MinHeapNode_t* node[MAX_ASCII]; /* nodes for building tree */
MinHeapNode_t* root; /* root node */
int nNode; /* number of nodes in total */
unsigned nFreq[MAX_ASCII]; /* number of frequencies for each ascii letter */
unsigned nCode[MAX_ASCII]; /* ascii code for each ascii letter */
int nLen[MAX_ASCII]; /* depth of the tree for each ascii code */
int bitLoc = -1; /* bit location to write or read binary code from tree */

/******************************************************************************
To get a bit from specific location that I want to extract for.
Author: Robin Hanjun Kwon
Input:
- x: target location in bit sequence
- y: current location in bit sequence
- z: length of bits that user wants to detect (put 1 to z means detect one bit)
Output:
- unsigned int to return specific bit
******************************************************************************/
unsigned getThisBit(int x, int y, int z)
{
	return (x >> y) & ~(~0 << z);
}

/******************************************************************************
To write a bit into the file.
Author: Robin Hanjun Kwon
Input:
- cur: current location in bit sequence
- src: the file to write a bit sequence
- flag: True to write in file, False to juts put a bit sequence in memory
Output:
- N/A
******************************************************************************/
void putBit(unsigned cur, FILE* src, int flag)
{
	static unsigned byte = 0;

	/* Save 8 bits */
	if (bitLoc < 0 || flag == TRUE)
	{
		putc(byte, src);
		bitLoc = 7;
		byte = 0;
	}
	byte |= cur << (bitLoc--);
}

/******************************************************************************
To get a bit from the file.
Author: Robin Hanjun Kwon
Input:
- src: the file to get a bit sequence from
Output:
- return a bit using getThisBit() function
******************************************************************************/
int getBit(FILE* src)
{
	static int cur = 0;

	if (bitLoc < 0)
	{
		cur = getc(src);
		bitLoc = 7;
	}

	return getThisBit(cur, bitLoc--, 1);
}

/******************************************************************************
To get frequency from the file.
Author: Robin Hanjun Kwon
Input:
- src: the file to get a frequency from
Output:
- return the frequency
******************************************************************************/
unsigned getFrequency(FILE* src)
{
	int i;
	unsigned len = 0;

	for (i = 0; i < MAX_ASCII; i++)
		nFreq[i] = 0L;
    
	rewind(src);
    
	while(!feof(src))
	{
		i = getc(src);
		len++;
		nFreq[i]++;
	}

	return len - 1; /* Return full frequency from the file */
}

/******************************************************************************
To find the node with the minimum frequency from the node array (min heap).
Author: Robin Hanjun Kwon
Input:
- N/A
Output:
- return the index of minimum node from the node array
******************************************************************************/
int findMinimum()
{
	int minIndex = 0;
	int i;

	/* Find the node which contains minimum frequency */
	for (i = 1; i < nNode; i++)
	{
		if (node[i]->freq < node[minIndex]->freq)
			minIndex = i;
	}

	return minIndex;
}

/******************************************************************************
To initialise the nodes.
Author: Robin Hanjun Kwon
Input:
- N/A
Output:
- N/A
******************************************************************************/
void initialiseNode()
{
	int i = 0;
	MinHeapNode_t* temp;

	for (i = nNode = 0; i < MAX_ASCII; i++)
	{
		/* Do not create a node if the frequency is zero */
		if (nFreq[i] != 0)
		{
			temp = (MinHeapNode_t*) malloc(sizeof(MinHeapNode_t));

			temp->freq = nFreq[i];
			temp->data = i;
			temp->left = temp->right = NULL;

			/* Save node */
			node[nNode++] = temp;
        }
    }
}

/******************************************************************************
To build Huffman tree with frequency array.
Author: Robin Hanjun Kwon
Input:
- N/A
Output:
- N/A
******************************************************************************/
void buildTree()
{
	int min;
	MinHeapNode_t* temp;
	MinHeapNode_t* h1;
	MinHeapNode_t* h2;
	
	initialiseNode();

	/* Min heapify */
	while (nNode > 1)
	{
		/* Find the node with minimum frequency at the moment */
		min = findMinimum();
		h1 = node[min];
		
		/* Locate last node to position of minimum frequency node 
		(There will be only node[0] at the last) */
		node[min] = node[--nNode];

		/* Find the node with second minimum frequency at the moment */
		min = findMinimum();
        h2 = node[min];
    
        /* Create new node to add h1 and h2 
		(New node doesn't have ascii letter) */
		temp = (MinHeapNode_t*) malloc(sizeof(MinHeapNode_t));
		temp->freq = h1->freq + h2->freq;
		temp->data = 0;
		temp->left = h1;
		temp->right = h2;
	
		/* Locate new node to position of node with second minimum frequency */
		node[min] = temp;
	}
	
	/* Put last node into root */
	root = node[0];
}

/******************************************************************************
To destruct Huffman tree.
Author: Robin Hanjun Kwon
Input:
- root: root node of Huffman tree
Output:
- N/A
******************************************************************************/
void destructTree(MinHeapNode_t* root)
{
	if (root != NULL)
	{
		destructTree(root->left);
		destructTree(root->right);
		free(root);
		root = NULL;
	}
}

/******************************************************************************
To get binary code from Huffman tree.
Author: Robin Hanjun Kwon
Input:
- node: the node to start to get binary code from
- code: to store binary code inside of this variable
- length: to store length of each binary code sequence
Output:
- N/A
******************************************************************************/
void getBinary(MinHeapNode_t* node, unsigned code, int length)
{
	/* If it is not a leaf node, keep searching and creating the binary code,
     if it is a leaf node, stop searching and save the binary code */
	if (node->left != NULL || node->right != NULL)
	{
		/* Shift 1 to the direction of left to add 0 into the binary code,
         and then increase the length of the binary code */
		code <<= 1;
		length++;

		/* Call recursion to the left child node */
		getBinary(node->left, code, length);

		/* Add 1 into the binary code using '1or' operation, 
		if left child node is visited */
		code |= 1;
		
		/* Call recursion to the right child node */
		getBinary(node->right, code, length);
		
		/* Go back to parent node, and then decreaset the length of 
		the binary code */
		code >>= 1;
		length--;
	}
	else
	{
		/* Save binary code and the length of binary code for each ascii letter */
		nCode[node->data] = code;
		nLen[node->data] = length;
    }
}

/******************************************************************************
To put binary code into the array using getBinary() function.
Author: Robin Hanjun Kwon
Input:
- N/A
Output:
- N/A
******************************************************************************/
void putBinary()
{
    int i;

    for (i = 0; i < MAX_ASCII; i++)
        nCode[i] = nLen[i] = 0;

    getBinary(root, 0u, 0);
}

/******************************************************************************
To compress with Huffman tree.
Author: Robin Hanjun Kwon
Input:
- src: original encrypted text file
- input[]: name of the file that user prefers
Output:
- N/A
******************************************************************************/
/* Compress with Huffman algorithm */
void compress(FILE* src, char input[])
{
	FILE* fp2;
	FILE* fp3;

	int cur;
	int i, j;
	unsigned length;

	char input1[MAX_SIZE];
	char input2[MAX_SIZE];

	strcpy(input1, input);
	strcpy(input2, input);

	strcat(input1, ".bin");
	strcat(input2, "_tree.txt");

#if DEBUG
	printf("[Debug mode] Name of the compressed file: '%s'\n", input1);
	printf("[Debug mode] Name of the tree info file: '%s'\n", input2);
#endif

	fp2 = fopen(input1, "wb");

	fp3 = fopen(input2, "w");

	/* Write header for compressed file */
    length = getFrequency(src);
	fwrite(&length, sizeof(unsigned int), 1, fp2); /* Write length of the file */

	buildTree();
	putBinary();

	/* Write information of nCode[] and nLen[] */
	for(i = 0; i < MAX_ASCII; i++)
	{
		fputc(nCode[i], fp3);
		fputc(nLen[i], fp3);
	}
	
    destructTree(root);

    rewind(src);
    bitLoc = 7; /* To store 8 bits in one byte */
    
	/* Write binary code into the compressed file */
	while (1)
    {
        cur = getc(src);
        
		/* if it is end of file, then break this infinite loop */
        if (feof(src))
            break;
        
        for (j = nLen[cur] - 1; j >= 0; j--)
            putBit(getThisBit(nCode[cur], j, 1), fp2, FALSE);
    }
    putBit(0, fp2, TRUE);
    
	fclose(fp2);
	fclose(fp3);
}

/******************************************************************************
To restruct Huffman tree.
Author: Robin Hanjun Kwon
Input:
- N/A
Output:
- N/A
******************************************************************************/
void restructTree()
{
	int i;

	/* Initialis root with NULL to restruct Huffman tree */
	root = NULL;

	for (i = 0; i < MAX_ASCII; i++)
	{
		if (nLen[i] > 0) 
			rebuildTree(i);
	}
}

/******************************************************************************
To rebuild Huffman tree using array of the length and array of ascii code.
Author: Robin Hanjun Kwon
Input:
- data: the index of ascii code array
Output:
- N/A
******************************************************************************/
void rebuildTree(int data)
{
	int i = nLen[data] - 1;
	MinHeapNode_t* t1;
	MinHeapNode_t* t2;

	if (root == NULL)
	{
		root = (MinHeapNode_t*) malloc(sizeof(MinHeapNode_t));
		root->left = root->right = NULL;
	}

	t1 = t2 = root;
	while (i >= 0)
	{
		if (getThisBit(nCode[data], i, 1) == 0)
		{
			t2 = t2->left;
			if (t2 == NULL)
			{
				t2 = (MinHeapNode_t*) malloc(sizeof(MinHeapNode_t));
				t2->left = t2->right = NULL;
				t1->left = t2;
			}
		}
		else
		{
			t2 = t2->right;
			if (t2 == NULL)
			{
				t2 = (MinHeapNode_t*) malloc(sizeof(MinHeapNode_t));
				t2->left = t2->right = NULL;
				t1->right = t2;
			}
		}
		t1 = t2;
		i--;
	}
	t2->data = data;
}

/******************************************************************************
To decompress Huffman tree.
Author: Robin Hanjun Kwon
Input:
- src1: compressed file
- src2: the file that contains Huffman tree information
- input[]: name of the file that user prefers to decompress with
Output:
- N/A
******************************************************************************/
void decompress(FILE* src1, FILE* src2, char input[])
{
	FILE* fp1;
	unsigned length;
	int n;
	MinHeapNode_t* h;
	int i = 0;

	rewind(src1);
	
	strcat(input, "_decompressed.txt");

	fp1 = fopen(input, "wb");

	fread(&length, sizeof(unsigned int), 1, src1);

#if DEBUG
	printf("[Debug mode] Length of Compressed file: %u\n", length);
#endif

	/* Read nCode[] and nLen[] */
	for(i = 0; i < MAX_ASCII; i++)
	{
		nCode[i] = fgetc(src2);
		nLen[i] = fgetc(src2);
	}	
    
	restructTree();

	n = 0;
	bitLoc = -1;

	while (n < length)
	{
		h = root;
		while (h->left && h->right)
		{
			if (getBit(src1) == 1)
				h = h->right;
			else
				h = h->left;
		}
		putc(h->data, fp1);
		n++;
	}
    
	destructTree(root);
    
	fclose(fp1);
}
