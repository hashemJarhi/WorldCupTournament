//
// Created by hashem on 11/29/2022.
//

#ifndef WET1_AVL_TREE_H
#define WET1_AVL_TREE_H

#include <stdlib.h>
#include "iostream"


using namespace std;


template <class Key , class Data>
class NodeInTree
{
public:
    int height;
    Key *key;
    Data *data;
    NodeInTree *leftSon;
    NodeInTree *rightSon;
    NodeInTree *father;

    NodeInTree(): height(0)
    , key(nullptr)
    , data(nullptr)
    , leftSon(nullptr),
    rightSon(nullptr),
    father(nullptr)
    {}

    NodeInTree(Key &newKey, Data &newData, NodeInTree *father = nullptr)
            : height(0),
            leftSon(nullptr),
            rightSon(nullptr),
            father(father)
    {
        this->key = new Key(newKey);
        this->data = new Data(newData);
    }

    ~NodeInTree()
    {
        father = nullptr;
        leftSon = nullptr;
        rightSon = nullptr;
    }
    bool hasTwoSons()
    {
        if(rightSon != nullptr && leftSon != nullptr)
        {
            return true;
        }
        return false;
    }
    bool isLeaf()
    {
        if(rightSon == nullptr && leftSon == nullptr)
        {
            return true;
        }
        return false;
    }

    void updateHeight()
    {
        int right_height = (rightSon == nullptr ? -1 : this->rightSon->height);
        int left_height = (leftSon == nullptr ? -1 : leftSon->height);

        this->height = (right_height > left_height ? right_height : left_height) + 1;
    }


    int balanceFactor()
    {
        return ((getHeight(leftSon)) - (getHeight(rightSon)));
    }


    bool hasLeftSon()
    {
        return (leftSon != nullptr);
    }

    bool hasRightSon()
    {
        return (rightSon!= nullptr);
    }
    void swap(NodeInTree* to_swap)
    {
        auto tmp= to_swap->key;
        auto tmpData= to_swap->data;

        (to_swap->data) = (this->data);
        (to_swap->key) = (this->key);

        (this->data) = (tmpData);
        (this->key) = (tmp);
    }

    bool hasOneSon()
    {
        if((rightSon != nullptr && leftSon == nullptr) ||
           (leftSon != nullptr && rightSon == nullptr))
        {
            return true;
        }
        return false;
    }

    int getHeight(NodeInTree *node)
    {
        if(node == nullptr)
            return -1;

        node->updateHeight();
        return node->height;
    }

};


template<class Key , class Data>
class Tree
{
private:
    int size;
    NodeInTree<Key,Data> *root;
    NodeInTree<Key,Data> *max;


public:

    Tree()
    {
        size = 0;
        root = nullptr;
        max= nullptr;
    }

    class Delete
    {
    public:
        void operator()( NodeInTree<Key,Data>  *node)
        {
            delete node->key;
            delete node->data;
            delete node;
        }
    };

    ~Tree()
    {
        treeClear();
    }
    NodeInTree<Key,Data>* getRoot()
    {
        return this->root;
    }

    NodeInTree<Key,Data>* getMax()
    {

    }
    int getSize()
    {
        return this->size;
    }
    void insert(Key &new_key , Data &new_data) {
        NodeInTree<Key,Data>  *itr = this->root;
        NodeInTree<Key,Data>  *father = nullptr;

        while (itr != nullptr)
        {
            if(new_key == (*itr->key))
            {
                return;
            }
            father = itr;

            if(new_key > *(itr->key))
            {
                itr = itr->rightSon;
            }
            else
            {
                itr = itr->leftSon;
            }
        }

        NodeInTree<Key,Data> *new_node = new NodeInTree<Key,Data>(new_key, new_data, father);

        if (father == nullptr)
        {
            this->root = new_node;
        } else if(new_key > *(father->key))
        {
            father->rightSon = new_node;
        }
        else
        {
            father->leftSon = new_node;
        }

        if (new_node->leftSon)
        {
            std::cout << new_node->leftSon->key << std::endl;
        }

        fixTheTree(new_node); //we start fixing the Tree from the temp father by updating the height and calculating the balance factor and rotate right and left
        if (max == nullptr )
        {
            max =new_node;
        }
        else if(new_key > (*max->key))
        {
            max = new_node;
        }
        size = size + 1;
    }
    void remove(Key &key) {
        NodeInTree<Key,Data>  *to_delete = find(key);
        if( to_delete == nullptr)
        {
            return;
        }

        NodeInTree<Key,Data>  *temp_father = to_delete->father;
        if(to_delete->rightSon && to_delete->leftSon)
        {
            NodeInTree<Key,Data>  *itr=to_delete->rightSon;
            while(itr->leftSon != nullptr)
            {
                itr=itr->leftSon;
            }

            to_delete->swap(itr);
            to_delete=itr;

        }
        if (to_delete->rightSon)
        {
            to_delete->swap(to_delete->rightSon);
            to_delete = to_delete->rightSon;
            to_delete->father->rightSon = nullptr;

        }
        else if (to_delete->leftSon)
        {
            to_delete->swap(to_delete->leftSon);
            to_delete = to_delete->leftSon;
            to_delete->father->leftSon = nullptr;

        }

        temp_father = to_delete->father;
        to_delete->father= nullptr;

        if(temp_father)
        {
            if (temp_father->rightSon && temp_father->rightSon->key == to_delete->key)
            {
                temp_father->rightSon = nullptr;
            }

            if (temp_father->leftSon && *(temp_father->leftSon->key) == *(to_delete->key))
            {
                temp_father->leftSon = nullptr;
            }
        }
        else
        {
            root = nullptr;
            max = nullptr;
        }

        if (temp_father && (*to_delete->key) == (*max->key))
        {
            max = temp_father;
        }

        delete to_delete->key;
        delete to_delete->data;
        delete to_delete;
        size = size - 1;


        fixTheTree(temp_father);

        return;

    }

    void rotateRightRight( NodeInTree<Key,Data> *node)
    {
        NodeInTree<Key,Data>  *temp_root = node->rightSon;
        node->rightSon = temp_root->leftSon;
        if(node->rightSon)
        {
            node->rightSon->father = node;
        }

        temp_root->father = node->father;
        node->father = temp_root;
        temp_root->leftSon = node;

        if(temp_root->father)
        {
            if(temp_root->father->leftSon == node){
                temp_root->father->leftSon = temp_root;
            } else
            {
                temp_root->father->rightSon = temp_root;
            }
        } else
        {
            this->root = temp_root;
        }

        node->updateHeight();
        temp_root->updateHeight();

        if(temp_root->father)
        {
            temp_root->updateHeight();
        }
    }
    void rotateLeftLeft( NodeInTree<Key,Data> *node)
    {
        NodeInTree<Key,Data>  *temp_root = node->leftSon;

        node->leftSon = temp_root->rightSon;
        if(node->leftSon)
        {
            node->leftSon->father = node;
        }

        temp_root->father = node->father;
        node->father = temp_root;
        temp_root->rightSon = node;


        if(temp_root->father)
        {
            if(temp_root->father->rightSon == node){
                temp_root->father->rightSon = temp_root;
            } else
            {
                temp_root->father->leftSon = temp_root;
            }
        } else
        {
            this->root = temp_root;
        }
        node->updateHeight();
        temp_root->updateHeight();

        if(temp_root->father)
        {
            temp_root->updateHeight();
        }

    }

    void fixTheTree( NodeInTree<Key,Data> *node)
    {
        postOrderSearch(this->root , Update());
        NodeInTree<Key,Data> * itr= node;
        while (itr != nullptr)
        {
            if (itr->balanceFactor() > 1)
            {
                if (itr->leftSon->balanceFactor() >= 0)
                {
                    rotateLeftLeft(itr); //LL
                }
                else
                {
                    rotateLeftRight(itr);//LR
                }
            }
            else if (itr->balanceFactor() < -1)
            {
                if (itr->rightSon->balanceFactor() == 1)
                {
                    rotateRightLeft(itr); //RL
                } else
                {
                    rotateRightRight(itr); //RR
                }
            }
            itr->updateHeight();
            itr = itr->father;
        }
    }
    void treeClear();
    void setRoot( NodeInTree<Key,Data> *node)
    {
        this->root = node;
    }

    void setSize(int sizex)
    {
        this->size = sizex;
    }

    void inOrderSearch(NodeInTree<Key,Data> * node,NodeInTree<Key,Data>  array[])
    {

        if (node == nullptr)
            return;

        inOrderSearch(node->leftSon,array);
        array[0]= *node;
        array++;
        inOrderSearch(node->rightSon,array);
    }


    template <class Func>
    void preOrderSearch( NodeInTree<Key,Data>  *node, Func func);
    template <class Func>
    void postOrderSearch( NodeInTree<Key,Data>  *node, Func function);

    void TreeKey_to_array(NodeInTree<Key,Data>* root, Key arr_key[], int* count);
    void TreeData_to_array(NodeInTree<Key,Data>* root, Data arr_Data[], int* count);
    void rotateRightLeft( NodeInTree<Key,Data> *node);
    void rotateLeftRight( NodeInTree<Key,Data> *node);
    NodeInTree<Key,Data>* find(Key &key);
   // NodeInTree<Key,Data>* arrayToTree(NodeInTree<Key,Data>**array, int start, int end,int* cnt,NodeInTree<Key,Data>* root );
   NodeInTree<Key,Data>* arrayToTree(Key arr_key[], Data arr_data[], int start, int end, int* cnt);
   NodeInTree<Key,Data>* temp(Key arr_key[], Data arr_data[], int start, int end, int* cnt, NodeInTree<Key,Data>* root);

    class Update
    {
    public:
        void operator()( NodeInTree<Key,Data>  *node)
        {
            node->updateHeight();
        }

    };
};


template<class Key,class Data>
void Tree<Key,Data>::TreeKey_to_array(NodeInTree<Key,Data> *node, Key arr_key[], int* count)
{
    if(node == nullptr)
        return;

    TreeKey_to_array(node->leftSon,arr_key,count);
    arr_key[(*count)] = *(node->key);
    (*count)++;
    TreeKey_to_array(node->rightSon, arr_key,count);
}

template<class Key,class Data>
void Tree<Key,Data>::TreeData_to_array(NodeInTree<Key,Data>* node, Data arr_Data[], int* count)
{
    if(node == nullptr)
        return;

    TreeData_to_array(node->leftSon,arr_Data,count);
    arr_Data[(*count)] = *(node->data);
    (*count)++;
    TreeData_to_array(node->rightSon, arr_Data,count);
}

template<class Key,class Data>
NodeInTree<Key,Data>* Tree<Key,Data>::arrayToTree(Key arr_key[], Data arr_data[], int start, int end, int* cnt)
{
    if (start > end) {
        return nullptr;
    }

    int mid = (start + end) / 2;
    auto newNode = new NodeInTree<Key,Data>(arr_key[mid], arr_data[mid]);

    if (*cnt == 0)
    {
        this->size = end + 1;
        this->root = newNode;
    }
    if (*cnt == end)
    {
        this->max = newNode;
    }

    (*cnt) = (*cnt) + 1;

    newNode->leftSon = arrayToTree(arr_key,arr_data, start, mid-1,cnt);
    if (newNode->leftSon)
        newNode->leftSon->father = newNode ;
    newNode->rightSon = arrayToTree(arr_key, arr_data, mid + 1, end, cnt);
    if (newNode->rightSon)
        newNode->rightSon->father = newNode ;

    return newNode;
}


template<class Key,class Data>
NodeInTree<Key,Data>* Tree<Key,Data>::temp(Key arr_key[], Data arr_data[], int start, int end, int* cnt, NodeInTree<Key,Data>* root)
{
    if(start > end)
    {
        return nullptr;
    }
    int mid = (start +end)/2;
    auto newNode = new NodeInTree<Key,Data>(arr_key[mid], arr_data[mid]);

    *(cnt)++;
    newNode->father = root;
    /*if(root == nullptr)
    {
       this->root = newNode;
    }else{
        newNode->father = root;
    }*/
    newNode->leftSon = temp(arr_key,arr_data,start,mid-1,cnt,newNode);
    newNode->rightSon = temp(arr_key,arr_data,mid+1,end,cnt,newNode);
    if(newNode->hasRightSon() && newNode->hasLeftSon())
    {

        if (newNode->leftSon->height > newNode->rightSon->height)
        {
            newNode->height = newNode->leftSon->height +1;
        }else{
            newNode->height = newNode->rightSon->height+1;
        }

    }else if(newNode->hasRightSon()){
        newNode->height = newNode->rightSon->height +1;
    }
    else if(newNode->hasLeftSon()){
        newNode->height = newNode->leftSon->height +1;
    }
    return newNode;
}

template<class Key,class Data>
void Tree<Key,Data>::treeClear()
{
    if (this->root != nullptr)
    {
        remove(*this->root->key);
        treeClear();
    }
    max = nullptr;

}



template<class Key,class Data>
NodeInTree<Key,Data> *Tree<Key,Data>::find(Key &key)
{
    NodeInTree<Key,Data>  *itr = this->root;
    while (itr != nullptr  )
    {
        if((itr->key))
        {
            if ((*itr->key) != key)
            {
                if (*(itr->key) > key)
                    itr = itr->leftSon;
                else
                    itr = itr->rightSon;
            } else
            {
                return itr;
            }
        }else{
            return nullptr;
        }
    }
    if(itr == nullptr)
    {
        return nullptr;
    }
    return itr;
}


template<class Key,class Data>
template<class Func>
void Tree<Key,Data>::postOrderSearch(NodeInTree<Key,Data>  *node, Func function)
{
    if (node == nullptr)
        return;
    postOrderSearch(node->leftSon,function);
    postOrderSearch(node->rightSon, function);
    function(node);
}

template<class Key,class Data>
template<class Func>
void Tree<Key,Data>::preOrderSearch(NodeInTree<Key,Data>  *node, Func func)
{
    if (node == nullptr)
        return;
    func(node);
    preOrderSearch(node->leftSon,func);
    preOrderSearch(node->rightSon, func);
}

template<class Key , class Data>
void Tree<Key,Data>::rotateRightLeft(NodeInTree<Key,Data>  *node)
{
    rotateLeftLeft(node->rightSon);
    return rotateRightRight(node);
}

template<class Key , class Data>
void Tree<Key,Data>::rotateLeftRight(NodeInTree<Key,Data>  *node)
{
    rotateRightRight(node->leftSon);
    return rotateLeftLeft(node);
}

#endif //WET1_AVL_TREE_H
