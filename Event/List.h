#ifndef LIST_NODE_H
#define LIST_NODE_H

#include <iostream.h>

template<class T> class List;

template<class T> class ListNode { //nodes to be contained with a list
	friend class List<T> ;
public:
	ListNode(T);
	~ListNode() {
		nextPtr = NULL;
		data = NULL;
	}
	T getData();
	ListNode* getNext();
	void setData(T);

private:
	T data; //templatized data stored in node
	ListNode* nextPtr; //pointer to the next node in list
};

template<class T> ListNode<T>* ListNode<T>::getNext() {
	return this->nextPtr;
}

template<class T> ListNode<T>::ListNode(T dataIn) {
	data = dataIn; //stores data in node
	nextPtr = NULL; //initializes point to next node to null
}

template<class T> T ListNode<T>::getData() { //returns data stored in node
	return data;
}

template<class T> void ListNode<T>::setData(T data) { //returns data stored in node
	this->data = data;
}

template<class T> class List { //linked list of ListNode objects
public:
	List();
	~List();
	void insertNewNode(T); //fucntion used to insert new node in order in the list
	void print(); //prints the contents of the linked list
	ListNode<T>* search(T); //searches for a value in the linked list and returns the point to object that contains that value
	// bilo je private
	void insertBegin(T); //inserts new node before the first node in the list
	void insertEnd(T); //inserts new node after the last node in the list
	T removeLast();
	T removeFirst();
	ListNode<T>* getFirst();
	void freeList();
	static ListNode<T>* GetFirstNode(List*);
	void swap(T, T);
	unsigned getSize();
	List<T> copyList();
private:
	ListNode<T> *startPtr; //stores the pointer of first object in the linked list
	ListNode<T> *endPtr; //stored the pointer of the last object in the linked list
	char isEmpty(); //utility functions used to see if the list contains no elements
	unsigned size;
};

template<class T> ListNode<T>* List<T>::getFirst() {
	return startPtr;
}

template<class T> List<T>::List() { //creates list with start and end as NULL
	this->startPtr = NULL;
	this->endPtr = NULL;
	this->size = 0;
}

template<class T> List<T>::~List() {
	if (!isEmpty()) { // List is not empty

		ListNode<T> *currentPtr = startPtr;
		ListNode<T> *tempPtr;

		while (currentPtr != NULL) { // delete remaining nodes
			tempPtr = currentPtr;
			currentPtr = currentPtr->nextPtr;
			delete tempPtr;
		}
	}
}

template<class T> char List<T>::isEmpty() {
	if (startPtr == NULL && endPtr == NULL) //if the start pointer and end pointer are NULL then the list is empty
		return 1;
	else
		return 0;
}

template<class T> void List<T>::insertBegin(T dataIn) {
	ListNode<T> * newPtr = new ListNode<T>(dataIn); //creates new node
	if (isEmpty()) { //if the list is empty create first element of the list
		startPtr = newPtr; //start and end pointer are same becuase there is only one object in list
		endPtr = newPtr;
	} else { //if node(s) exist in list insert additional object before the first
		newPtr->nextPtr = startPtr; //the next pointer of the new node points to the node that was previously first
		startPtr = newPtr; //the pointer for the new node is now the starting node
	}
	this->size++;
}

template<class T> void List<T>::insertEnd(T dataIn) {
	ListNode<T> * newPtr = new ListNode<T>(dataIn);
	if (isEmpty()) { //if the list is empty create first element of the list (same as first case in insert at begin)
		startPtr = newPtr;
		endPtr = newPtr;
	} else { //if node(s) exist in the list then insert new node at the end of the list
		endPtr->nextPtr = newPtr; //the current last node's next pointer points to the new node
		endPtr = newPtr; //the new node is now the last node in the list
	}
	this->size++;
}

template<class T> void List<T>::print() {
	if (isEmpty())
		cout << "The list is empty" << endl;

	else {
		ListNode<T> * currentPtr = startPtr;

		cout << "The contents of the list is: ";
		while (currentPtr != NULL) { //prints until the end of the list is reached
			cout << currentPtr->getData() << " ->" << endl;
			currentPtr = currentPtr->nextPtr; //moves to next node in list
		}
		cout << "END LIST" << endl;
	}

}

template<class T> ListNode<T>* List<T>::search(T key) { //search functions that searches for node that contains data equal to the key
	ListNode<T>* nodePtr = startPtr;

	while (nodePtr!= NULL) { //runs through list until data is found within a node or end of list is reached
		if (nodePtr->data == key) //if the node's data equals the key then the node has been found
			return nodePtr; //moves to next node in list
		nodePtr = nodePtr->nextPtr;
	}
	return NULL; //returns pointer to the node that contains data equal to key (NULL if not found)
}

template<class T> T List<T>::removeLast() {
	if (isEmpty())
		return NULL;
	ListNode<T> * currentPtr = endPtr;
	T ret = currentPtr->getData();
		// cout << "The list is empty" << endl;
	if (startPtr == endPtr) {
		startPtr = NULL;
		endPtr = NULL;
		this->size--;
	} else {
		endPtr = startPtr;
		while (endPtr->nextPtr != currentPtr)
			endPtr = endPtr->nextPtr;
		endPtr->nextPtr = NULL;
		this->size--;
	}
	delete currentPtr;
	return ret;
}

template<class T> T List<T>::removeFirst() {
	if (isEmpty())
		return NULL;
	ListNode<T> * currentPtr = startPtr;
	T ret = currentPtr->getData();
		// cout << "The list is empty" << endl;
	if (startPtr == endPtr) {
		startPtr = NULL;
		endPtr = NULL;
		this->size--;
	} else {
		startPtr = startPtr->nextPtr;
		this->size--;
	}
	delete currentPtr;
	return ret;
}

template<class T> void List<T>::freeList() {
	ListNode<T>* node = startPtr;
	while (startPtr != NULL) {
		startPtr = node->nextPtr;
		delete node;
	}
	this->size = 0;
	this->endPtr = NULL;
}

template<class T> ListNode<T>* List<T>::GetFirstNode(List<T>* list) {
	return list->getFirst();
}

template<class T> void List<T>::swap(T t1, T t2) {
	ListNode<T>* first = NULL, *second = NULL, *start = startPtr;

	while (start != NULL && (first == NULL || second == NULL)) {
		if (start->getData() == t1)
			first = start;
		if (start->getData() == t2)
			second = start;
	}
	if (first != NULL && second != NULL) {
		first->setData(t2);
		second->setData(t1);
	}
}

template<class T> unsigned List<T>::getSize() {
	return this->size;
}

//template <class T> List<T> List<T>::copyList(){
//	List<T> newList;
//	return newList;
//}

#endif
