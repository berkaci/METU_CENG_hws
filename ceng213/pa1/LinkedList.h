#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

template <class T>
class LinkedList
{
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &obj);
    ~LinkedList();

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

    int getSize() const;
    bool isEmpty() const;
    bool containsNode(Node<T> *node) const;
    int getIndex(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void append(const T &data);
    void prepend(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAtIndex(const T &data, int index);
    void moveToIndex(int currentIndex, int newIndex);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeNodeAtIndex(int index);
    void removeAllNodes();

    void print() const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.


void appendNode(Node<T> *yeni)
{
    if (head==NULL)
    {
        head=yeni;
        head->next=head;
        head->prev=head;
    }
    else
    {
        yeni->prev=head->prev;
        yeni->next=head;
        head->prev->next=yeni;
        head->prev=yeni;

    }
    size++;
}

void prependNode(Node<T> *yeni)
{
    if (head==NULL)
    {
        head=yeni;
        head->next=head;
        head->prev=head;
    }
    else
    {
        yeni->prev=head->prev;
        yeni->next=head;
        head->prev->next=yeni;
        head->prev=yeni;
        head=yeni;
    }
    size++;
}
void insertNodeAtIndex(Node<T> *yeni, int index)
{
    if(index>size) return;
    if(index==0)
    {
        prependNode(yeni);
        return;
    }
    if(index==size)
    {
        appendNode(yeni);
        return;
    }
    yeni->next=getNodeAtIndex(index);
    yeni->prev=yeni->next->prev;
    yeni->next->prev=yeni;
    yeni->prev->next=yeni;
    size++;
}


private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    int size;
};





template<class T>
LinkedList<T>::LinkedList()
{
    size=0;
    head=NULL;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj)
{
    if(obj.size==0)
    {
        size=0;
        head=NULL;
        return;
    }

    int i;
    size=obj.size;
    head=new Node<T>(obj.head->data);
    Node<T>* hsim=head,*sim=obj.head;
    for(i=0;i<size-1;i++)
    {
        hsim->next=new Node<T>(sim->next->data);
        hsim->next->prev=hsim;
        hsim=hsim->next;
        sim=sim->next;
    }
    hsim->next=head;
    head->prev=hsim;
}

template<class T>
LinkedList<T>::~LinkedList()
{
    if(size==0) return;
    if(size==1)
    {
        delete head;
        head=NULL;
        size=0;
        return;
    }
    Node<T> *it,*son;
    son=head->prev;
    for(it=head->next;it!=son;it=it->next)
    {
        delete it->prev;
    }
    if (it->prev!=it) delete it->prev;
    delete it;
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs)
{
    if(&rhs==this)
    {
        return *this;
    }
    removeAllNodes();
    Node<T> *it,*yit;
    it=rhs.head;
    if( !it )
    {
        return *this;
    }
    head = new Node<T>(rhs.head->data);
    yit=head;
    it=it->next;
    for(;it!=rhs.head; it=it->next , yit=yit->next)
    {
        yit->next=new Node<T>(it->data);
        yit->next->prev=yit;
    }
    yit->next=head;
    head->prev=yit;
    size=rhs.size;
    return *this;
}

template<class T>
int LinkedList<T>::getSize() const
{
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const
{
    return size==0;
}

template<class T>
bool LinkedList<T>::containsNode(Node<T> *node) const
{
    if(head==NULL) return false;
    Node<T> *it;
    for(it=head;it!=head->prev;it=it->next)
    {
        if(it==node) return true;
    }
    if(it==node) return true;
    else return false;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    if(head==NULL) return NULL;
    return head;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    if(head==NULL) return NULL;
    return head->prev;
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const
{
	if (head==NULL) return NULL;
    Node<T> *it;
    for(it=head;it!=head->prev;it=it->next)
    {
        if(it->data==data) return it;
    }
    if(it->data==data) return it;
    else return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    if(index>=size||index<0) return NULL;
    int i;
    Node<T> *it;
    it=head;
    for(i=0;i<index;i++,it=it->next);
    return it;
}

template<class T>
void LinkedList<T>::append(const T &data)
{
    if (head==NULL)
    {
        head=new Node<T>(data);
        head->next=head;
        head->prev=head;
    }
    else
    {
        Node<T>* yeni;
        yeni = new Node<T>(data, head->prev, head);
        head->prev->next=yeni;
        head->prev=yeni;

    }
    size++;
}

template<class T>
void LinkedList<T>::prepend(const T &data)
{
    if (head==NULL)
    {
        head=new Node<T>(data);
        head->next=head;
        head->prev=head;
    }
    else
    {
        Node<T>* yeni;
        yeni = new Node<T>(data, head->prev, head);
        head->prev->next=yeni;
        head->prev=yeni;
        head=yeni;
    }
    size++;
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    if(!containsNode(node)) return;
    Node<T> *yeni;
    yeni= new Node<T>(data);
    yeni->next=node->next;
    yeni->prev=node;
    node->next=yeni;
    yeni->next->prev=yeni;
    size++;
}

template<class T>
void LinkedList<T>::insertAtIndex(const T &data, int index)
{
    if(index>size) return;
    if(index==0)
    {
        prepend(data);
        return;
    }
    if(index==size)
    {
        append(data);
        return;
    }
    Node<T>* yeni;
    yeni=new Node<T>(data);
    yeni->next=getNodeAtIndex(index);
    yeni->prev=yeni->next->prev;
    yeni->next->prev=yeni;
    yeni->prev->next=yeni;
    size++;
}

template<class T>
int LinkedList<T>::getIndex(Node<T> *node) const
{
    Node<T> *it;
    int i;
    for(it=head , i=0 ; it!=head->prev;it=it->next, i++)
    {
        if(it==node) return i;
    }
    if(it==node) return i;
    else return -1;
}

template<class T>
void LinkedList<T>::moveToIndex(int currentIndex, int newIndex)
{
    if(currentIndex==newIndex || currentIndex>=size || currentIndex<0)
    {
        return;
    }
    if(newIndex>=size)
    {
        newIndex=size-1;
    }
    Node<T> *tn;
    tn=getNodeAtIndex(currentIndex);
    if(currentIndex==0)
    {
        head=head->next;
    }
    tn->prev->next=tn->next;
    tn->next->prev=tn->prev;
    size--;
    insertNodeAtIndex(tn,newIndex);


}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    Node<T> *it,*tmp;
    if(size==0) return;
    if(head->next==head && head==node)
    {
        delete head;
        head=NULL;
        size--;
        return;
    }
    if(head==node)
    {
        head->prev->next=head->next;
        head->next->prev=head->prev;
        tmp=head->next;
        delete head;
        head=tmp;
        size--;
        return;
    }
    for(it=head;it!=head->prev;it=it->next)
    {
        if(it==node)
        {
            it->prev->next=it->next;
            it->next->prev=it->prev;
            delete it;
            size--;
            return;
        }
    }
    if(it==node)
    {
        it->prev->next=it->next;
        it->next->prev=it->prev;
        delete it;
        size--;
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data)
{
    Node<T> *it,*tmp;
    if(size==0) return;
    while(1)
    {
        if(head->next==head && head->data==data)
        {
            delete head;
            head=NULL;
            size--;
            return;
        }
        if(head->data==data)
        {
            head->prev->next=head->next;
            head->next->prev=head->prev;
            tmp=head->next;
            delete head;
            head=tmp;
            size--;
        }
        else break;
    }
    for(it=head;it!=head->prev;it=it->next)
    {
        if(it->data==data)
        {
            it->prev->next=it->next;
            it->next->prev=it->prev;
            tmp=it->prev;
            delete it;
            it=tmp;
            size--;
        }
    }
    if(it->data==data)
    {
        it->prev->next=it->next;
        it->next->prev=it->prev;
        delete it;
        size--;
    }
}







template<class T>
void LinkedList<T>::removeNodeAtIndex(int index)
{
    removeNode(getNodeAtIndex(index));
}

template<class T>
void LinkedList<T>::removeAllNodes()
{
    if(size==0) return;
    if(size==1)
    {
        delete head;
        head=NULL;
        size=0;
        return;
    }
    Node<T> *it,*son;
    son=head->prev;
    for(it=head->next;it!=son;it=it->next)
    {
        delete it->prev;
    }
    delete it->prev;
    delete it;
    head=NULL;
    size=0;
}

template<class T>
void LinkedList<T>::print() const
{
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    Node<T> *node = this->getFirstNode();

    do {
        std::cout << *node << std::endl;
        node = node->next;
    }
    while (node != this->getFirstNode());
}

#endif //LINKEDLIST_H

