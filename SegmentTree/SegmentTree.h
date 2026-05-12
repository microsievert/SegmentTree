#ifndef SEGMENTTREE 
#define SEGMENTTREE

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	size_t len;

	int combinerDefault;

	int (*combiner)(int, int);

	int* arr;
} SegmentTree;

SegmentTree* BuildSegmentTree(int* source, size_t len, int (*buildfunc)(int, int), int combinerDefault);

void UpdateSegmentTree(SegmentTree* tree, int index, int newVal);

void DisposeSegmentTree(SegmentTree* tree);

int GetSegRange(SegmentTree* tree, int l, int r);

int GetSegValue(SegmentTree* tree, int index);

#endif