/**
 * 双向链表头文件
 */

#ifndef _LIST_H
#define _LIST_H


/**
 * 初始化双向环状链表 
 * list指向头结点，由于是双相链表，因此初始时prev和next都是指向头节点本身
 * notation：这种方式的双向链是环状的，环状链的好处是，节点是平等的，谁都可以是头或位，删除也不用担心空指针异常
 */
KLINE void list_init(list_h_t *list){
	list->prev = list;
	list->next = list;
	return;
}

/**
 * 移除两个节点中间的子链
 * 只需给出子链的前驱节点和后继结点prev->(sub list)->next
 */
KLINE void __list_del(list_h_t *prev, list_h_t *next){
	next->prev = prev;
	prev->next = next;
	return;
}

/**
 * 在指定的两个节点之间插入新节点
 * 由于是双向链表，与prev和next分别建立两个连接，共四个赋值语句
 */
KLINE void __list_add(list_h_t *new, list_h_t *prev, list_h_t *next){
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
	return;
}

/**
 * 前插法插入新节点（默认） 
 * prev = head
 * next = head->next
 */
KLINE void list_add(list_h_t *new, list_h_t *head){
	__list_add(new, head, head->next);
	return;
}

/**
 * 尾插法插入新节点
 * prev = head->prev
 * next = head
 */
KLINE void list_add_tail(list_h_t *new, list_h_t *head){
	__list_add(new, head->prev, head);
	return;
}

/**
 * 将entry节点从所在链表中移除
 */
KLINE void __list_del_entry(list_h_t *entry){
	__list_del(entry->prev, entry->next);
	return;
}

/**
 * 将entry从所在链表中移除，并将entry节点初始化为一个链表
 */
KLINE void list_del(list_h_t *entry){
	__list_del(entry->prev, entry->next);
	list_init(entry);
	return;
}

/**
 * 将list节点移动到head节点后
 * 节点移动可以是跨链表，也可以是在同一个链表中
 */
KLINE void list_move(list_h_t *list, list_h_t *head){
	list_del(list);
	list_add(list, head);
	return;
}

/**
 * 将list节点移动到head节点前
 * 节点移动可以跨链表，也可以是在同一链表中
 */
KLINE void list_move_tail(list_h_t *list, list_h_t *head){
	list_del(list);					//从list中移出list节点
	list_add_tail(list, head);		//将list插入到head前面
	return;
}

/**
 * 判断链表是否为空
 */
KLINE bool_t list_is_empty(const list_h_t *head){
	if (head->next == head){
		return TRUE;
	}
	return FALSE;
}

/**
 * 仔细判断链表是否为空
 */
KLINE bool_t list_is_empty_careful(const list_h_t *head){
	list_h_t *next = head->next;
	if (next == head && next == head->prev){	//头结点指向自己
		return TRUE;
	}
	return FALSE;
}

/**
 * 遍历这个链表
 * 从head的下一个开始，一直向后(next)走，直到到达head。cur只是循环变量 
 * head指向的节点不会被处理
 */
#define list_for_each(pos, head) for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * 从head的下一个开始，直到head的下一个等于head（在循环的过程中需要改变head，否则死循环）
 */
#define list_for_each_head_dell(pos, head) for (pos = (head)->next; pos != (head); pos = (head)->next)

/**
 * 根据结构体成员地址获取结构体起始地址
 */
#define list_entry(ptr, type, member) ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


#endif
