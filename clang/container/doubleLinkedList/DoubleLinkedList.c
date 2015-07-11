/*
	一个简单的双向链表结构的实现
	同时还可以当队列使用
*/

#include <stdio.h>
#include <stdlib.h>
#include "DoubleLinkedList.h"

#define _dllGetNodeData(_list_) (((struct DllNode *)(_list_))->data)

int _defaultMatchFun(void *a, void *b)
{
	return (a == b) ? 1 : 0;
}

struct DllList *createDllList(DllMatch match)
{
	struct DllList *ret = malloc(sizeof(struct DllList));

	if (ret == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	ret->count = 0;
	ret->head = NULL;
	ret->tail = NULL;
	ret->itering = DLL_NOT_ITER;

	if (match) {
		ret->match = match;
	} else {
		ret->match = _defaultMatchFun;
	}
	
	return ret;
}

int dllListAddTail(struct DllList *list, void *ptr)
{
	if (list->itering == DLL_ITERING) {
		return -1;
	}

	struct DllNode *node = malloc(sizeof(struct DllNode));
	if (node == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	node->next = NULL;
	node->previous = NULL;
	node->data = ptr;

	if (list->head == NULL && list->tail == NULL) {
		list->head = list->tail = node;
	} else {
		list->tail->next = node;
		node->previous = list->tail;
		list->tail = node;
	}
	list->count++;
	return 0;
}

int dllListAddHead(struct DllList *list, void *ptr)
{
	if (list->itering == DLL_ITERING) {
		return -1;
	}

	struct DllNode *node = malloc(sizeof(struct DllNode));
	if (node == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	node->next = NULL;
	node->previous = NULL;
	node->data = ptr;

	if (list->head == NULL && list->tail == NULL) {
		list->head = list->tail = node;
	} else {
		node->next = list->head;
		list->head->previous = node;
		list->head = node;
	}
	list->count++;
	return 0;
}

int dllIsEmpty(struct DllList *list)
{
	return list->count;
}

int dllListSize(struct DllList *list)
{
	return list->count;
}

int dllQueuePush(struct DllList *list, void *ptr)
{
	return dllListAddTail(list, ptr);
}

void *dllQueuePop(struct DllList *list)
{
	if (list->itering == DLL_ITERING) {
		return NULL;
	}

	struct DllNode *node = list->head;
	void *retval;

	if (node == NULL) {
		return NULL;
	}

	retval = _dllGetNodeData(node);
	list->head = node->next;

	if (list->head == NULL) {
		list->head = list->tail = NULL;
	} else {
		list->head->previous = NULL;
	}

	free(node);
	return retval;
}

static void _deleteItem(struct DllList *list, struct DllNode *node, int dofree)
{
	if (node == NULL || list->itering == DLL_ITERING) {
		return;
	}

	if (list->head == node) {
		list->head = node->next;
		if (node->next) {
			node->next->previous = NULL;
		}
	} else if (list->tail == node) {
		list->tail = node->previous;
		if (list->tail) {
			list->tail->next = NULL;
		}
	} else {
		if (node->previous) {
			node->previous->next = node->next;
		} else {
			fprintf(stderr, "链表构建出错！！\n");
			return;
		}

		if (node->next) {
			node->next->previous = node->previous;
		} else {
			fprintf(stderr, "链表构建出错！！\n");
			return;
		}
	}
	if (list->head == NULL || list->tail == NULL) {
		list->head = list->tail = NULL;
	}
	if (dofree) {
		free(node);
	}
}

void *dllListDelItem(struct DllList *list, void *ptr)
{
	if (list->itering == DLL_ITERING) {
		return NULL;
	}

	struct DllNode *node = list->head;
	void *retval = NULL;

	for (;node != NULL;node = node->next) {
		if (list->match(_dllGetNodeData(node),ptr) == 0) {
			retval = _dllGetNodeData(node);
			_deleteItem(list,node,1);
			list->count--;
			break;
		}
	}
	return retval;
}

int dllListClear(struct DllList *list)
{
	if (list->itering == DLL_ITERING) {
		return -1;
	}
	struct DllNode *node = list->head;
	struct DllNode *tempnode = NULL;
	void *retval = NULL;

	for (;node != NULL;node = node->next,free(tempnode)) {
		_deleteItem(list, node, 0);
		tempnode = node;
	}
	list->head = list->tail = NULL;
	list->count = 0;
	return 0;
}

int dllListInsertItemWithAfter(struct DllList *list, void *ptr)
{
	if (list->itering == DLL_ITERING) {
		return -1;
	}
}

int dllListInsertItemWithBefor(struct DllList *list, void *ptr)
{
	if (list->itering == DLL_ITERING) {
		return -1;
	}
}

void *dllListGetItemByIndex(struct DllList *list, int index)
{

}

struct DllIter *dllListGetIter(struct DllList *list, int iterOrder)
{
	struct DllIter *iter = malloc(sizeof(struct DllIter));
	if (iter == NULL) {
		fprintf(stderr, "oom\n");
		return NULL;
	}

	list->itering = DLL_ITERING;
	iter->list = list;
	iter->iterOrder = iterOrder;

	if (iterOrder == DLL_ITER_ORDER_POSITIVE) {
		iter->current = list->head;
	} else if (iterOrder == DLL_ITER_ORDER_REVERSE) {
		iter->current = list->tail;
	} else {
		fprintf(stderr, "参数错误！！\n");
	}
	
	return iter;
}

int dllListHasNext(struct DllIter *iter)
{
	if (iter == NULL || iter->current == NULL ||
		iter->list->itering != DLL_ITERING) {
		return 0;
	}
	if (iter->current) {
		return 1;
	}
	return 0;
}

void *dllListNext(struct DllIter *iter)
{
	if (iter == NULL || iter->current == NULL ||
		iter->list->itering != DLL_ITERING) {
		return NULL;
	}

	struct DllNode *node = iter->current;

	if (iter->iterOrder == DLL_ITER_ORDER_POSITIVE) {
		iter->current = iter->current->next;
	} else if (iter->iterOrder == DLL_ITER_ORDER_REVERSE) {
		iter->current = iter->current->previous;
	} else {
		fprintf(stderr, "参数错误！！\n");
	}
	
	return _dllGetNodeData(node);
}

void dllFreeIter(struct DllIter *iter)
{
	iter->list->itering = DLL_NOT_ITER;
	free(iter);
}

