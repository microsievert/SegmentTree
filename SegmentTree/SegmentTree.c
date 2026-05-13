#include "SegmentTree.h"

void BuildSegmentTreeInternal(SegmentTree* tree, int* source, int node, int l, int r, int (*buildfunc)(int, int))
{
	if (l == r)
	{
		tree->arr[node] = source[l];

		return;
	}

	int newRangeCenter = (l + r) / 2;
	
	BuildSegmentTreeInternal(tree, source, node * 2, l, newRangeCenter, buildfunc);
	BuildSegmentTreeInternal(tree, source, node * 2 + 1, newRangeCenter + 1, r, buildfunc);

	tree->arr[node] = buildfunc(tree->arr[node * 2], tree->arr[node*2+1]);
}

int GetRangeInternal(SegmentTree* tree, int l, int r, int node, int cl, int cr)
{
	if (cl > r || cr < l)
		return tree->combinerDefault;

	if (cl >= l && cr <= r)
		return tree->arr[node];

	int newRangeCenter = (cl + cr) / 2;

	return tree->combiner(GetRangeInternal(tree, l, r, node * 2, cl, newRangeCenter), GetRangeInternal(tree, l, r, node * 2 + 1, newRangeCenter + 1, cr));
}

void UpdateSegmentTreeInternal(SegmentTree* tree, int index, int newVal, int node, int l, int r)
{
	if (l == r) 
	{
		tree->arr[node] = newVal;

		return;
	}

	int newRangeCenter = (l + r) / 2;

	if (index <= newRangeCenter)
		UpdateSegmentTreeInternal(tree, index, newVal, node * 2, l, newRangeCenter);
	else
		UpdateSegmentTreeInternal(tree, index, newVal, node * 2 + 1, newRangeCenter + 1, r);

	tree->arr[node] = tree->combiner(tree->arr[node * 2], tree->arr[node * 2 + 1]);
}

void TreeToArrInternal(SegmentTree* tree, int node, int l, int r, int* sourceArr)
{
	if (l == r)
	{
		sourceArr[l] = tree->arr[node];

		return;
	}

	int newRangeCenter = (l + r) / 2;

	TreeToArrInternal(tree, node * 2, l, newRangeCenter, sourceArr);
	TreeToArrInternal(tree, node * 2 + 1, newRangeCenter + 1, r, sourceArr);
}

SegmentTree* BuildSegmentTree(int* source, size_t len, int (*buildfunc)(int, int), int combinerDefault)
{
	if (source == NULL)
		return NULL;

	SegmentTree* newTree = calloc(1, sizeof(SegmentTree));

	if (newTree == NULL)
		return NULL;

	newTree->len = len;
	newTree->arr = calloc(4 * len, sizeof(int));

	newTree->combinerDefault = combinerDefault;
	newTree->combiner = buildfunc;

	if (newTree->arr == NULL)
		return NULL;

	BuildSegmentTreeInternal(newTree, source, 1, 0, len - 1, buildfunc);

	return newTree;
}

void UpdateSegmentTree(SegmentTree* tree, int index, int newVal)
{
	UpdateSegmentTreeInternal(tree, index, newVal, 1, 0, tree->len - 1);
}

void AddToSegmentTree(SegmentTree** tree, int value)
{
	SegmentTree* curTree = *tree;

	int* sourceArr = calloc(curTree->len + 1, sizeof(int));

	TreeToArrInternal(curTree, 1, 0, curTree->len - 1, sourceArr);

	sourceArr[curTree->len] = value;

	SegmentTree* newTree = BuildSegmentTree(sourceArr, curTree->len + 1, curTree->combiner, curTree->combinerDefault);

	DisposeSegmentTree(curTree);

	*tree = newTree;
}

void DisposeSegmentTree(SegmentTree* tree)
{
	if (tree == NULL)
		return;

	free(tree->arr);
	free(tree);
}

int GetSegRange(SegmentTree* tree, int l, int r)
{
	return GetRangeInternal(tree, l, r, 1, 0, tree->len - 1);
}

int GetSegValue(SegmentTree* tree, int index)
{
	return GetSegRange(tree, index, index);
}