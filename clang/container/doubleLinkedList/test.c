#include <stdio.h>
#include <stdlib.h>
#include "DoubleLinkedList.h"

struct DllList *test1()
{
	struct DllList *list = createDllList(NULL);
	int *u;

	for (int i = 0; i < 100; i++) {
		u = malloc(sizeof(*u));
		*u = i;
		dllListAddTail(list, u);
	}

	struct DllIter *iter = dllListGetIter(list, DLL_ITER_ORDER_POSITIVE);

	while (dllListHasNext(iter)) {
		u = dllListNext(iter);
		fprintf(stderr, "%d\n", *u);
	}
	dllFreeIter(iter);
	fprintf(stderr, "------------\n");
	for (int i = 1; i < 100; i++) {
		u = malloc(sizeof(*u));
		*u = i;
		dllListAddHead(list, u);
	}

	iter = dllListGetIter(list, DLL_ITER_ORDER_REVERSE);

	while (dllListHasNext(iter)) {
		u = dllListNext(iter);
		fprintf(stderr, "%d\n", *u);
	}
	dllFreeIter(iter);
	return list;
}

int match(void *a, void *b)
{
	int *a1 = a;
	int *b1 = b;
	return *a1 - *b1;
}

void test2()
{
	fprintf(stderr, "-------queue-----\n");
	struct DllList *list = createDllList(match);
	int *u;

	for (int i = 0; i < 100; i++) {
		u = malloc(sizeof(*u));
		*u = i;
		dllQueuePush(list, u);
	}


	for (int i = 0; i < 100; i++) {
		if (i % 2 > 0) {
			dllListDelItem(list, &i);
		}
	}

	while (u = dllQueuePop(list)) {
		fprintf(stderr, "%d\n", *u);
	}

	dllListClear(list);
	fprintf(stderr, "-------queue end-----\n");
}

int main(int argc, char const *argv[])
{
	test2();
	//test1();
	return 0;
}
