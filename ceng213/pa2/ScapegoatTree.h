#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cmath>

#include "Node.h"
#include "NoSuchItemException.h"

enum TraversalMethod {preorder, inorder, postorder};

template<class T>
class ScapegoatTree {
public: // DO NOT CHANGE THIS PART.
    ScapegoatTree();

    ScapegoatTree(const ScapegoatTree<T> &obj);

    ~ScapegoatTree();

    bool isEmpty() const;

    int getHeight() const;

    int getSize() const;

    bool insert(const T &element);

    bool remove(const T &element);

    void removeAllNodes();

    const T &get(const T &element) const;

    void print(TraversalMethod tp=inorder) const;

    void printPretty() const;

    ScapegoatTree<T> &operator=(const ScapegoatTree<T> &rhs);

    void balance();

    const T &getCeiling(const T &element) const;

    const T &getFloor(const T &element) const;

    const T &getMin() const;

    const T &getMax() const;

    const T &getNext(const T &element) const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(Node<T> *node, TraversalMethod tp) const;
    void printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const;


    Node<T>* kopyala(const Node<T> *k)
    {
        Node<T> *h;
        if(k==NULL)
        {
            return NULL;
        }
        h=new Node<T>(k->element, kopyala(k->left), kopyala(k->right));
        return h;
    }

    void sil(Node<T> *s)
    {
        if(!s) return;
        sil(s->right);
        sil(s->left);
        delete s;
    }

    int height(const Node<T> *n) const
    {
        if(!n) return -1;
        int sag,sol;
        sol=height(n->left);
        sag=height(n->right);
        if (sag>sol) return sag+1;
        else return sol+1;
    }

    int nsayisi(const Node<T> *n) const
    {
        if(!n) return 0;
        return nsayisi(n->left) + nsayisi(n->right) + 1;
    }

    bool ekle(Node<T>* &n, const T yeni)
    {
        if(!n)
        {
            n=new Node<T>(yeni, NULL, NULL);
            return true;
        }
        if(n->element == yeni) return false;
        if(yeni < n->element) return ekle(n->left, yeni);
        if(yeni > n->element) return ekle(n->right, yeni);
    }

    long int keciyibul(Node<T>* &n, const T yeni) // /2%2(&2) var mi,  %2 degisti mi, /4 size
    {
        if(!n) return 0;
        if(n->element==yeni) return 6;
        int sag, sol, cf;
        sol=keciyibul(n->left, yeni);
        sag=keciyibul(n->right, yeni);
        bool bakilacak;
        bakilacak = !(sol%2) && !(sag%2) && ( sag&2 || sol&2);
        if(bakilacak)
        {
            if(sag&2)
            {
                //cocuk=n->right;
                cf=sag;
            }
            else
            {
                //cocuk=n->left;
                cf=sol;
            }

            if( 2*(sag/4+sol/4+1) < 3*(cf/4) )
            {
                kesipye(n);
                return (sag/4+sol/4+1)*4+1;
            }
        }
        if(sag&2 || sol&2) return (sag/4+sol/4+1)*4 + 2;
        else return (sag/4+sol/4+1)*4;
    }

    void kesipye(Node<T>* &n)
    {
        int byt=nsayisi(n), in=0;
        if(!n) return;
        T *arr;
        arr=new T[byt];
        paketle(n, in, arr);
        sil(n);
        yerlestir(n, arr, 0, byt-1);
        delete [] arr;
    }


    void paketle(Node<T>* n, int &in, T *arr)
    {
        if(!n) return;
        paketle(n->left, in, arr);
        arr[in++]=n->element;
        paketle(n->right, in, arr);
    }

    void yerlestir(Node<T>* &n, T *arr, int bas, int son)
    {
        if(bas>son)
        {
            n=NULL;
            return;
        }
        n=new Node<T>(arr[bas+(son-bas)/2], NULL, NULL);
        yerlestir(n->left, arr, bas, bas+(son-bas)/2-1);
        yerlestir(n->right, arr, bas+(son-bas)/2+1, son);
    }

    Node<T>* &buyukNode(Node<T>* &n)
    {
        if(!n) return n;
        if(!(n->right)) return n;
        return buyukNode(n->right);
    }

    Node<T>* &bul(Node<T>* &n, const T bulunacak, Node<T>* &nul) const
    {
        if(!n) return n;
        if(bulunacak==n->element) return n;
        Node<T> *&sol=bul(n->left, bulunacak, nul);
        if(sol) return sol;
        Node<T> *&sag=bul(n->right, bulunacak, nul);
        if(sag) return sag;
        return nul;
    }

    const Node<T>* bulconst(const Node<T>* n, const T bulunacak) const
    {
        if(!n) return n;
        if(bulunacak==n->element) return n;
        const Node<T> *nl=n->left, *nr=n->right;
        const Node<T> *sol=bulconst(nl, bulunacak);
        if(sol) return sol;
        const Node<T> *sag=bulconst(nr, bulunacak);
        if(sag) return sag;
        return NULL;
    }

    const Node<T>* gc(const Node<T>* n, T bulunacak) const
    {
        if(!n) return NULL;
        if(n->element==bulunacak)
        {
            return n;
        }
        if(bulunacak < n->element)
        {
            const Node<T> *p=gc(n->left, bulunacak);
            if(p) return p;
            return n;
        }
        else
        {
            const Node<T> *p=gc(n->right, bulunacak);
            if(p) return p;
            return NULL;
        }
    }

    const Node<T>* gf(const Node<T>* n, T bulunacak) const
    {
        if(!n) return NULL;
        if(n->element==bulunacak)
        {
            return n;
        }
        if(bulunacak > n->element)
        {
            const Node<T> *p=gf(n->right, bulunacak);
            if(p) return p;
            return n;
        }
        else
        {
            const Node<T> *p=gf(n->left, bulunacak);
            if(p) return p;
            return NULL;
        }
    }

    const Node<T>* gmin(const Node<T>* p) const
    {
        for(; p->left ; p=p->left);
        return p;
    }

    const Node<T>* gn(const Node<T>* n, T bulunacak) const
    {
        if(!n) return NULL;
        if(n->element==bulunacak)
        {
            if(n->right) return gmin(n->right);
            return NULL;
        }
        if(bulunacak < n->element)
        {
            const Node<T> *p=gn(n->left, bulunacak);
            if(p) return p;
            return n;
        }
        else
        {
            const Node<T> *p=gn(n->right, bulunacak);
            if(p) return p;
            return NULL;
        }
    }



private: // DO NOT CHANGE THIS PART.
    Node<T> *root;

    int upperBound;
};

#endif //TREE_H








template<class T>
ScapegoatTree<T>::ScapegoatTree()
{
    root=NULL;
    upperBound=0;
}

template<class T>
ScapegoatTree<T>::ScapegoatTree(const ScapegoatTree<T> &obj)
{
    root=kopyala(obj.root);
    upperBound=obj.upperBound;
}

template<class T>
ScapegoatTree<T>::~ScapegoatTree()
{
    sil(root);
}

template<class T>
bool ScapegoatTree<T>::isEmpty() const
{
    return !root;
}

template<class T>
int ScapegoatTree<T>::getHeight() const
{
    return height(root);
}

template<class T>
int ScapegoatTree<T>::getSize() const
{
    return nsayisi(root);
}

template<class T>
bool ScapegoatTree<T>::insert(const T &element)
{
    bool r;
    r=ekle(root, element);
    upperBound++;
    int hg=getHeight();
    if( upperBound < pow(1.5, hg) )
    {
        keciyibul(root, element);
    }
    return r;
}

template<class T>
bool ScapegoatTree<T>::remove(const T &element)
{
    Node<T> *tmpn;
    Node<T>* nul=NULL;
    Node<T> *&silinecek=bul(root, element, nul);
    if(!silinecek) return false;
    if(!silinecek->right)
    {
        tmpn=silinecek;
        silinecek=silinecek->left;
        delete tmpn;
    }
    else
    {
        Node<T> *&yerinegelecek=buyukNode(silinecek->left);
        if(!yerinegelecek || yerinegelecek==silinecek)
        {
            tmpn=silinecek;
            silinecek=silinecek->right;
            delete tmpn;
        }
        else
        {
            silinecek->element=yerinegelecek->element;
            tmpn=yerinegelecek;
            yerinegelecek=yerinegelecek->left;
            delete tmpn;
        }
    }
    int boyut=getSize();
    if(2*boyut<upperBound || boyut>upperBound)
    {
        kesipye(root);
        upperBound=boyut;
    }
    return true;
}

template<class T>
void ScapegoatTree<T>::removeAllNodes()
{
    upperBound=0;
    sil(root);
    root=NULL;
}

template<class T>
const T &ScapegoatTree<T>::get(const T &element) const
{
    const Node<T> *rc=root;
    const Node<T> *bln=bulconst(rc, element);
    if(!bln) throw NoSuchItemException();
    return bln->element;
}







template<class T>
void ScapegoatTree<T>::print(TraversalMethod tp) const {

    if (tp == preorder)
    {
        if (isEmpty()) {
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }
        std::cout << "BST_preorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    }
    else if (tp == inorder) {
        // check if the tree is empty?
        if (isEmpty()) {
            // the tree is empty.
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }

        // the tree is not empty.

        // recursively output the tree.
        std::cout << "BST_inorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == postorder)
    {
        if (isEmpty()) {
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }
        std::cout << "BST_postorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;

    }
}

template<class T>
void ScapegoatTree<T>::print(Node<T> *node, TraversalMethod tp) const {

    if (tp == preorder)
    {
        if (node == NULL)
          return;

        std::cout << "\t" << node->element;
        if (node->left) {
            std::cout << "," << std::endl;
        }
        print(node->left, preorder);

        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, preorder);
    }
    else if (tp == inorder) {
        // check if the node is NULL?
        if (node == NULL)
          return;

        // first, output left subtree and comma (if needed).
        print(node->left, inorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }

        // then, output the node.
        std::cout << "\t" << node->element;

        // finally, output comma (if needed) and the right subtree.
        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    }
    else if (tp == postorder)
    {
        if (node == NULL)
          return;


        print(node->left, postorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }
        print(node->right, postorder);

        if (node->right) {
            std::cout << "," << std::endl;
        }
        std::cout << "\t" << node->element;

    }
}

template<class T>
void ScapegoatTree<T>::printPretty() const {

    // check if the tree is empty?
    if (isEmpty()) {
        // the tree is empty.
        std::cout << "BST_pretty{}" << std::endl;
        return;
    }

    // the tree is not empty.

    // recursively output the tree.
    std::cout << "BST_pretty{" << std::endl;
    printPretty(root, 0, false);
    std::cout << "}" << std::endl;

}

template<class T>
void ScapegoatTree<T>::printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const {

    // check if the node is NULL?
    if (node == NULL)
        return;

    // output the indentation and the node.
    std::cout << "\t";
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "---";
    }
    std::cout << (indentLevel == 0 ? "root:" : (isLeftChild ? "l:" : "r:")) << node->element << std::endl;

    // first, output left subtree with one more indentation level.
    printPretty(node->left, indentLevel + 1, true);

    // then, output right subtree with one more indentation level.
    printPretty(node->right, indentLevel + 1, false);
}







template<class T>
ScapegoatTree<T> &ScapegoatTree<T>::operator=(const ScapegoatTree<T> &rhs)
{
    if(this==&rhs) return *this;
    else removeAllNodes();
    root=kopyala(rhs.root);
    upperBound=rhs.upperBound;
}

template<class T>
void ScapegoatTree<T>::balance()
{
    kesipye(root);
}

template<class T>
const T &ScapegoatTree<T>::getCeiling(const T &element) const
{
    const Node<T> *p=gc(root, element);
    if(!p) throw NoSuchItemException();
    return p->element;
}

template<class T>
const T &ScapegoatTree<T>::getFloor(const T &element) const
{
    const Node<T> *p=gf(root, element);
    if(!p) throw NoSuchItemException();
    return p->element;
}

template<class T>
const T &ScapegoatTree<T>::getMin() const
{
    const Node<T> *p = root;
    if(!p) throw NoSuchItemException();
    for(; p->left ; p=p->left);
    return p->element;
}

template<class T>
const T &ScapegoatTree<T>::getMax() const
{
    const Node<T> *p = root;
    if(!p) throw NoSuchItemException();
    for(; p->right ; p=p->right);
    return p->element;
}

template<class T>
const T &ScapegoatTree<T>::getNext(const T &element) const
{
    const Node<T> *p=gn(root, element);
    if(!p) throw NoSuchItemException();
    return p->element;
}