// Created by jhelland (1/11/19)
//


#ifndef __SINGLY_LINKED_LIST_H__
#define __SINGLY_LINKED_LIST_H__


template<typename T>
class SinglyLinkedList {
public:
	struct Node {
		T data;
		Node* next;
	};

	Node* head;

public:
	SinglyLinkedList() {}

	void insert(Node* prevNode, Node* newNode) {
		if (prevNode == nullptr) {  // first node
			if (head != nullptr)
				newNode->next = head;
			else
				newNode->next = nullptr;

			head = newNode;
		}
		else {
			if (prevNode->next == nullptr) {  // last node
				prevNode->next = newNode;
				newNode->next = nullptr;
			}
			else {  // middle node
				newNode->next = prevNode->next;
				prevNode->next = newNode;
			}
		}
	}

	void remove(Node* prevNode, Node* deleteNode) {
		if (prevNode == nullptr) {
			if (deleteNode->next == nullptr)
				head = nullptr;
			else
				head = deleteNode->next;
		}
		else
			prevNode->next = deleteNode->next;
	}
};


#endif