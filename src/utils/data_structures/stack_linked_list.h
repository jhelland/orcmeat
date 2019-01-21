// Created by jhelland (1/11/19)
//


#ifndef __STACK_LINKED_LIST_H__
#define __STACK_LINKED_LIST_H__


template<typename T>
class StackLinkedList {
private:
	StackLinkedList(StackLinkedList& stackLinkedList);

public:
	struct Node {
		T data;
		Node* next;
	};
	Node* head;

public:
	StackLinkedList() {}

	void push(Node* node) {
		if (node) {
			node->next = head;
		} else {
			node = head;
		}
		head = node;
	}

	Node* pop() {
		Node* top = head;
		head = head->next;
		return top;
	}
};


#endif // STACK_LINKED_LIST
