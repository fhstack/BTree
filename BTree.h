#pragma once
#include <stack>
#include <iostream>
using namespace std;
template <class K,class V,size_t M>
struct  BTreeNode
{
    pair<K,V> _kvs[M];
    BTreeNode<K,V,M>* _subs[M+1];
    BTreeNode<K,V,M>* _parent;
    size_t size;
    BTreeNode()
    :size(0)
    ,_parent(NULL)
    {
    for(size_t i = 0;i<M+1;i++)
    _subs[i] = NULL;
    }
};

template<class K,class V,size_t M>
class BTree
{
typedef BTreeNode<K,V,M> Node;
public:
    BTree()
        :_root(NULL)
    {}
    pair<Node*,int> Find(const K& key)
    {
        Node* parent = NULL;
        Node* cur = _root;
        int i = 0;
        while(cur)
        {
            for( i = 0;i<cur->size;i++)
            {
                parent = cur;
                if(key == cur->_kvs[i].first)
                return make_pair(cur,i);
                else if(key<cur->_kvs[i].first)
                break;
            }
            cur = cur->_subs[i];
        }
        return make_pair(parent,-1);
    }
    bool Insert(const pair<K,V>& kv)
    {
        if(_root == NULL)
        {
            _root = new Node;
            _root->_kvs[0] = kv;
            _root->size++;
            return true;
        }
        pair<Node*,int> ret = Find(kv.first);
        if(ret.second >= 0)
        return false;
        Node* cur = ret.first;
        Node* sub = NULL;
        pair<K,V> newkv = kv;
        while(1)
        {
            //插入kv与sub
            Insertkv(cur,newkv,sub);
            if(cur->size < M)
            return true;
            Node* new_node = new Node;
            size_t mid = cur->size/2;
            size_t i = 0;
            size_t j = 0;
            for(i = mid+1;i<cur->size;i++)
            {
                new_node->_kvs[j] = cur->_kvs[i];
                new_node->_subs[j] = cur->_subs[i];
                if(cur->_subs[i])
                cur->_subs[i]->_parent = new_node;
                j++;
                new_node->size++;
            }
            new_node->_subs[j] = cur->_subs[i];
            if(cur->_subs[i])
            cur->_subs[i]->_parent = new_node;
            cur->size -= (new_node->size+1);
            if(cur->_parent == NULL)
            {
                _root =  new Node;
                _root->_kvs[0] = cur->_kvs[mid];
                _root->_subs[0] = cur;
                _root->_subs[1] = new_node;
                _root->size = 1;
                cur->_parent = _root;
                new_node->_parent = _root;
                return true;
            }
            else
            {
                newkv = cur->_kvs[mid];
                sub = new_node;
                cur = cur->_parent;
            }
        }
    }

    void Insertkv(Node* cur,const pair<K,V>& kv,Node* sub)
    {
        int end = cur->size-1;
        while(end >= 0)
        {
            //挪动key
            //挪动孩子
            if(cur->_kvs[end].first > kv.first)
            {
                cur->_kvs[end+1] = cur->_kvs[end];
                cur->_subs[end+2] = cur->_subs[end+1];
                --end;
            }
            else
            {
                break;
            }
        }
        cur->_kvs[end+1] = kv;
        //插入的孩子在key的右边
        cur->_subs[end+2] = sub;
        if(sub)
        sub->_parent = cur;
        cur->size++;
    }
    
    /*void InOrder()
    {
        stack<Node*> s;
        Node* root = _root;
        while(root || !s.empty())
        {
            if(root||)
            {
                s.push(root);
                root = root->_subs[0];
            }
            else
            {
                Node*  tmp = s.top();
                s.pop();
                for(size_t i = 0;i<tmp->size;i++)
                cout<<tmp->_kvs[i];
                root = tmp->_parent;
            }
        }
    }*/
    void InOrder()
    {
        _InOrder(_root);
    }
    void _InOrder(Node* root)
    {
        if(root == NULL)
        return;
        size_t i = 0;
        for(i = 0;i<root->size;i++)
        {
            _InOrder(root->_subs[i]);   
            cout<<root->_kvs[i].first<<" ";
        }
        _InOrder(root->_subs[i]);
    }
protected:
    Node* _root;
};

void Test()
{
    BTree<int,int,3> t;
    int a[] = {53, 75, 139, 49, 145, 36, 101};
    for(size_t i = 0;i<sizeof(a)/sizeof(a[0]);i++)
    {
        cout<<a[i]<<"插入成功？"<<t.Insert(make_pair(a[i],0))<<endl;
        t.InOrder();
        cout<<endl;
    }
}
