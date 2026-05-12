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

int GetRangeInternal(SegmentTree* tree, int l, int r, int node, int cl, int cr, int (*aggregator)(int, int))
{
	if (cl > r || cr < l)
		return tree->combinerDefault;

	if (cl >= l && cr <= r)
		return tree->arr[node];

	int newRangeCenter = (cl + cr) / 2;

	return aggregator(GetRangeInternal(tree, l, r, node * 2, cl, newRangeCenter, aggregator), GetRangeInternal(tree, l, r, node * 2 + 1, newRangeCenter + 1, cr, aggregator));
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

int GetSegRange(SegmentTree* tree, int l, int r)
{
	return GetRangeInternal(tree, l, r, 1, 0, tree->len - 1, tree->combiner);
}

int GetSegValue(SegmentTree* tree, int index)
{
	return GetSegRange(tree, index, index);
}