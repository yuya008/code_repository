#ifndef __DOUBLELINKEDLIST_H__
#define __DOUBLELINKEDLIST_H__

struct DllNode
{
	/* 下一个元素 */
	struct DllNode *next;
	/* 上一个元素 */
	struct DllNode *previous;
	/* 数据指针 */
	void *data;
};

/* 0 是匹配 整数 是 a>b 负数是 b > a*/
typedef int (*DllMatch)(void *a,void *b);

struct DllList
{
	/* 链表元素总数 */
	int count;
	/* 头 */
	struct DllNode *head;
	/* 尾 */
	struct DllNode *tail;
	/* 比较回调函数 */
	DllMatch match;
	/* 迭代标志 */
	int itering;
};

struct DllIter
{
	struct DllList *list;
	struct DllNode *current;
	int iterOrder;
};

#ifdef DLL_THREAD_SAFE

	#define DLL_LOCK() do { 			\
		/* 还没实现 */					\
	} while(0);

	#define DLL_UNLOCK() do { 			\
		/* 还没实现 */		  			\
	} while(0);

#else

	#define DLL_LOCK()
	#define DLL_UNLOCK()

#endif

#define DLL_ITER_ORDER_POSITIVE 0
#define DLL_ITER_ORDER_REVERSE 1

#define DLL_ITERING 1
#define DLL_NOT_ITER 0

/* 创建链表 */
struct DllList *createDllList(DllMatch match);
/* 添加数据到链表尾 */
int dllListAddTail(struct DllList *list, void *ptr);
/* 添加数据到链表头 */
int dllListAddHead(struct DllList *list, void *ptr);
/* 判断链表是否为空 */
int dllIsEmpty(struct DllList *list);
/* 链表的元素数目 */
int dllListSize(struct DllList *list);
/* 入队列 */
int dllQueuePush(struct DllList *list, void *ptr);
/* 出队列 */
void *dllQueuePop(struct DllList *list);
/* 删除元素 */
void *dllListDelItem(struct DllList *list, void *ptr);
/* 清空列表 */
int dllListClear(struct DllList *list);
/* 插入元素之后 */
int dllListInsertItemWithAfter(struct DllList *list, void *ptr);
/* 插入元素之前 */
int dllListInsertItemWithBefor(struct DllList *list, void *ptr);
/* 获取索引位置的元素 */
void *dllListGetItemByIndex(struct DllList *list, int index);
/* 获得迭代器 */
struct DllIter *dllListGetIter(struct DllList *list, int iterOrder);
/* 迭代器中是否拥有元素 */
int dllListHasNext(struct DllIter *);
/* 返回迭代器中的下一个元素 */
void *dllListNext(struct DllIter *);
/* 注销迭代器 */
void dllFreeIter(struct DllIter *);

#endif



