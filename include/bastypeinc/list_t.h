/**
 * 链表抽象结构的定义
 */

#ifndef _LIST_T_H
#define _LIST_T_H

//定义链表节点结构
typedef struct s_LIST_H {
       struct s_LIST_H *prev,*next;//前驱指针，后继指针
}list_h_t;

//定义树节点
typedef struct s_TREE{
	u16_t tr_type;
	u16_t tr_color;
	//spinlock_t tr_lock;
	u64_t tr_hight;
	struct s_TREE* tr_left;	//左
	struct s_TREE* tr_right;	//右
	struct s_TREE* tr_paret;	//父节点
	struct s_TREE* tr_subs;	//孩子结点
}tree_t;


#endif
