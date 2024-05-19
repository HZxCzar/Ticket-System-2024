/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include "exceptions.hpp"
#include "utility.hpp"
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>

namespace sjtu {
template <class T> struct ListNode {
  const T *_data;
  ListNode *next;
  ListNode *last;
  ListNode(const T *data = nullptr, ListNode *_next = nullptr,
           ListNode *_last = nullptr)
      : _data(data), next(_next), last(_last) {}
  ~ListNode() {} //不需要delete，data数据在红黑树中进行维护
};
template <class T> class DoubleLinkedList {

private:
  ListNode<T> *Head;
  ListNode<T> *Tail;
  size_t count;

public:
  DoubleLinkedList() : count(0) {
    Head = new ListNode<T>();
    Tail = new ListNode<T>();
    Head->next = Tail;
    Tail->last = Head;
  }
  ~DoubleLinkedList() {
    Clear();
    delete Head;
    delete Tail;
  }
  void Clear();
  ListNode<T> *Insert(T *data, const int &type,
                      ListNode<T> *CloseNode = nullptr);
  void Remove(ListNode<T> *cur);
  ListNode<T> *end() const { return Tail; }
  ListNode<T> *begin() const { return Head->next; }
};
template <class T> void DoubleLinkedList<T>::Clear() {
  ListNode<T> *cur = Head;
  while (cur) {
    ListNode<T> *temp = cur;
    cur = cur->next;
    delete temp;
  }
  count = 0;
  Head = new ListNode<T>();
  Tail = new ListNode<T>();
  Head->next = Tail;
  Tail->last = Head;
}

template <class T>
ListNode<T> *DoubleLinkedList<T>::Insert(T *data, const int &type,
                                         ListNode<T> *CloseNode) {
  // type=-1表示前插,type=0表示根节点,type=1表示后插
  if (type == 0) {
    //assert(count == 0);
    ListNode<T> *AddNode = new ListNode<T>(data);
    Head->last = nullptr;
    Tail->next = nullptr;
    Head->next = AddNode;
    AddNode->last = Head;
    Tail->last = AddNode;
    AddNode->next = Tail;
    ++count;
    return AddNode;
  }
  //assert(count > 0);
  if (type == -1) {
    ListNode<T> *AddNode = new ListNode<T>(data);
    ListNode<T> *BefNode = CloseNode->last;
    BefNode->next = AddNode;
    AddNode->last = BefNode;
    CloseNode->last = AddNode;
    AddNode->next = CloseNode;
    ++count;
    return AddNode;
  } else if (type == 1) {
    ListNode<T> *AddNode = new ListNode<T>(data);
    ListNode<T> *AftNode = CloseNode->next;
    AftNode->last = AddNode;
    AddNode->next = AftNode;
    CloseNode->next = AddNode;
    AddNode->last = CloseNode;
    ++count;
    return AddNode;
  }
  throw;
}

template <class T> void DoubleLinkedList<T>::Remove(ListNode<T> *cur) {
  //assert(count >= 1);
  if (count == 1) {
    Clear();
  } else {
    if(cur->last)
    {
      cur->last->next = cur->next;
    }
    if(cur->next)
    {
      cur->next->last = cur->last;
    }
    --count;
    delete cur;
  }
}

enum Color {
  RED,
  BLACK,
};

template <class T> struct RBTreeNode {
  ListNode<RBTreeNode<T>> *LisN;
  RBTreeNode<T> *_parent;
  RBTreeNode<T> *_left;
  RBTreeNode<T> *_right;
  T *_data;

  Color _col;
  RBTreeNode(const T *data = nullptr)
      : _parent(nullptr), _left(nullptr), _right(nullptr), _col(RED) {
    if (data == nullptr) {
      _data = nullptr;
    } else {
      _data = new T(*data);
    }
  }
  ~RBTreeNode() { delete _data; }
  void release() {
    this->_parent = nullptr;
    if (this->_left) {
      this->_left->release();
    }
    if (this->_right) {
      this->_right->release();
    }
    delete this;
  }
  void Show(int &num)
  {
    if(this==nullptr)
    {
      return;
    }
    ++num;
    std::cout<<_data->first<<' '<<_col<<' '<<_data->second<<'\n';
    if(_left)
    {
      _left->Show(num);
    }
    if(_right)
    {
      _right->Show(num);
    }
  }
};
template <class T, class Compare> class RBTree {
  typedef RBTreeNode<T> Node;
  friend class DoubleLinkedList<Node>;

private:
  Node *_root;
  Compare cmp;
  size_t count;
  DoubleLinkedList<Node> List;
  void RotateL(Node *data);
  void RotateR(Node *data);
  void MaintainAfterRemove(const Node *node);
  void SwapInfo(Node *lhs, Node *rhs);
  void Copy(Node *lhs, const Node *rhs);

public:
  RBTree() : _root(nullptr), count(0) {}
  RBTree(const RBTree &other);
  RBTree &operator=(const RBTree &other);
  ~RBTree() { clear(); }
  void clear();
  bool Insert(const T &data);
  bool Remove(const T &data);
  ListNode<Node> *begin() const { return List.begin(); }
  ListNode<Node> *end() const { return List.end(); }
  Node *Get(const decltype(T::first) &key) const;
  Node *GetNode(const T &data)const;
  int Size() const { return count; }
  void ShowTree()const;
  // to be continued.
};

// body
template <class T, class Compare>
void RBTree<T, Compare>::ShowTree()const{
  int num=0;
  if(_root)
  {
    std::cerr<<"This is Root "<<_root->_data->first<<'\n';
  }
  else
  {
    std::cerr<<"empty\n";
  }
  _root->Show(num);
  std::cerr<<"num : "<<num<<'\n';
  assert(num==count);
}
template <class T, class Compare>
RBTreeNode<T> *RBTree<T, Compare>::GetNode(const T &data) const {
  if (!_root) {
    return nullptr;
  }
  Node *tmp = _root;
  while (tmp) {
    if (cmp(data.first, tmp->_data->first)) {
      tmp = tmp->_left;
    } else if (cmp(tmp->_data->first, data.first)) {
      tmp = tmp->_right;
    } else {
      if(&data==tmp->_data)
      {
        return tmp;
      }
      else {
      return nullptr;
      }
    }
  }
  return nullptr;
}

template <class T, class Compare>
RBTreeNode<T> *RBTree<T, Compare>::Get(const decltype(T::first) &key) const {
  if (!_root) {
    return nullptr;
  }
  Node *tmp = _root;
  while (tmp) {
    if (cmp(key, tmp->_data->first)) {
      tmp = tmp->_left;
    } else if (cmp(tmp->_data->first, key)) {
      tmp = tmp->_right;
    } else {
      return tmp;
    }
  }
  return nullptr;
}

template <class T, class Compare>
void RBTree<T, Compare>::Copy(Node *lhs, const Node *rhs) {
  if (rhs->_left) {
    Node *AddNode = new Node(rhs->_left->_data);
    AddNode->_col=rhs->_left->_col;
    //assert(AddNode->_data!=rhs->_left->_data);
    ++count;
    lhs->_left = AddNode;
    AddNode->_parent = lhs;
    AddNode->LisN = List.Insert(AddNode, -1, lhs->LisN);
    Copy(AddNode, rhs->_left);
  }
  if (rhs->_right) {
    Node *AddNode = new Node(rhs->_right->_data);
    AddNode->_col=rhs->_right->_col;
    //assert(AddNode->_data!=rhs->_right->_data);
    ++count;
    lhs->_right = AddNode;
    AddNode->_parent = lhs;
    AddNode->LisN = List.Insert(AddNode, 1, lhs->LisN);
    Copy(AddNode, rhs->_right);
  }
}

template <class T, class Compare>
RBTree<T, Compare>::RBTree(const RBTree &other) {
  count = 0;
  _root = nullptr;
  if (other._root) {
    _root = new Node(other._root->_data);
    _root->LisN = List.Insert(_root, 0);
    ++count;
    Copy(_root, other._root);
  }
}

template <class T, class Compare>
RBTree<T, Compare> &
RBTree<T, Compare>::operator=(const RBTree<T, Compare> &other) {
  if (this == &other) {
    return *this;
  }
  clear();
  if (other._root) {
    _root = new Node(other._root->_data);
    _root->_col=BLACK;
    _root->LisN = List.Insert(_root, 0);
    ++count;
    Copy(_root, other._root);
  }
  return *this;
}

template <class T, class Compare>
void RBTree<T, Compare>::SwapInfo(Node *lhs, Node *rhs) {
  ListNode<Node> *TempNode=lhs->LisN;
  lhs->LisN->_data=rhs;
  lhs->LisN=rhs->LisN;
  rhs->LisN->_data=lhs;
  rhs->LisN=TempNode;
  T *tmp = lhs->_data;
  lhs->_data = rhs->_data;
  rhs->_data = tmp;
}

template <class T, class Compare> void RBTree<T, Compare>::RotateR(Node *data) {
  Node *parent = data->_parent;
  Node *left = data->_left;
  if (parent && parent->_left == data) {
    parent->_left = left;
  } else if (parent && parent->_right == data) {
    parent->_right = left;
  } else {
    _root = left;
  }
  left->_parent = parent;
  data->_left = left->_right;
  if (data->_left) {
    data->_left->_parent = data;
  }
  left->_right = data;
  data->_parent = left;
}

template <class T, class Compare> void RBTree<T, Compare>::RotateL(Node *data) {
  Node *parent = data->_parent;
  Node *right = data->_right;
  if (parent && parent->_left == data) {
    parent->_left = right;
  } else if (parent && parent->_right == data) {
    parent->_right = right;
  } else {
    _root = right;
  }
  right->_parent = parent;
  data->_right = right->_left;
  if (data->_right) {
    data->_right->_parent = data;
  }
  right->_left = data;
  data->_parent = right;
}

template <class T, class Compare>
bool RBTree<T, Compare>::Insert(const T &data) {
  if (_root == nullptr) {
    _root = new Node(&data);
    _root->_col = BLACK;
    ++count;
    _root->LisN = List.Insert(_root, 0);
    return true;
  }
  Node *parent = nullptr;
  Node *cur = _root;

  // search
  while (cur) {
    if (cmp(data.first, cur->_data->first)) {
      parent = cur;
      cur = cur->_left;
    } else if (cmp(cur->_data->first, data.first)) {
      parent = cur;
      cur = cur->_right;
    } else {
      return false;
    }
  }
  ++count;
  cur = new Node(&data);
  if (cmp(data.first, parent->_data->first)) {
    parent->_left = cur;
  } else if (cmp(parent->_data->first, data.first)) {
    parent->_right = cur;
  }

  cur->_parent = parent;

  //加入链表
  if (parent->_left == cur) {
    cur->LisN = List.Insert(cur, -1, parent->LisN);
  } else if (parent->_right == cur) {
    cur->LisN = List.Insert(cur, 1, parent->LisN);
  }else {
  assert(0);
  }

  //插入维护
  while (parent && parent->_col == RED) {
    // case3，当前父节点为root且为RED
    if (parent == _root) {
      parent->_col = BLACK;
      continue;
    }

    Node *grandfather = parent->_parent;
    if (parent == grandfather->_left) {
      Node *uncle = grandfather->_right;
      // uncle存在且为RED
      //上溢LL&LR
      if (uncle && uncle->_col == RED) {
        parent->_col = BLACK;
        uncle->_col = BLACK;
        grandfather->_col = RED;

        cur = grandfather;
        parent = cur->_parent;
      }
      // uncle为NIL黑或普黑
      //旋转降高
      else if (!uncle || uncle->_col == BLACK) {
        // case5
        if (cur == parent->_right) {
          RotateL(parent);
          cur = parent;
          parent = cur->_parent;
        }
        // case6
        else if (cur == parent->_left) {
          RotateR(grandfather);
          grandfather->_col = RED;
          parent->_col = BLACK;
        }
      }
    }

    else if (parent == grandfather->_right) {
      Node *uncle = grandfather->_left;
      // uncle存在且为RED
      //上溢RL&RR
      if (uncle && uncle->_col == RED) {
        parent->_col = BLACK;
        uncle->_col = BLACK;
        grandfather->_col = RED;

        cur = grandfather;
        parent = cur->_parent;
      }

      // unclde为NIL黑或普黑
      //旋转降高
      else if (!uncle || uncle->_col == BLACK) {
        if (cur == parent->_left) {
          RotateR(parent);
          cur = parent;
          parent = cur->_parent;
        } else if (cur == parent->_right) {
          RotateL(grandfather);
          grandfather->_col = RED;
          parent->_col = BLACK;
        }
      }
    }
  }
  _root->_col = BLACK;
  return true;
}

template <class T, class Compare>
bool RBTree<T, Compare>::Remove(const T &data) {
  if (_root == nullptr) {
    return false;
  }
  Node *parent = nullptr;
  Node *cur = _root;
  while (cur) {
    if (cmp(data.first, cur->_data->first)) {
      parent = cur;
      cur = cur->_left;
    } else if (cmp(cur->_data->first, data.first)) {
      parent = cur;
      cur = cur->_right;
    } else {
      break;
    }
  }
  if (!cur) {
    return false;
  }

  // case0
  if (cur == _root && this->Size() == 1) {
    //从链表删除
  List.Remove(cur->LisN);
    --count;
    delete _root;
    _root = nullptr;
    return true;
  }

  // case1
  if (cur->_left && cur->_right) {
    //寻找后继节点
    Node *successor = cur->_right;
    Node *suc_parent = cur;
    while (successor->_left) {
      suc_parent = successor;
      successor = successor->_left;
    }
    //交换键值
    SwapInfo(cur, successor);
    cur = successor;
    parent = suc_parent;
    //后接case3
  }
  //从链表删除
  List.Remove(cur->LisN);
  // case2
  if (!cur->_left && !cur->_right) {
    if (cur->_col == BLACK) {
      MaintainAfterRemove(cur);
    }
    if (cur == parent->_left) {
      parent->_left = nullptr;
      delete cur;
    } else if (cur == parent->_right) {
      parent->_right = nullptr;
      delete cur;
    }
    --count;
    return true;
  }

  // case3
  //assert((cur->_left == nullptr && cur->_right != nullptr) ||
  //       (cur->_left != nullptr && cur->_right == nullptr));

  Node *replacement = cur->_left ? cur->_left : cur->_right;
  if (cur == _root) {
    _root = replacement;
    replacement->_col = BLACK;
    delete cur;
  } else if (cur == parent->_left) {
    replacement->_parent = parent;
    parent->_left = replacement;
    replacement->_col = BLACK;
    delete cur;
  } else if (cur == parent->_right) {
    replacement->_parent = parent;
    parent->_right = replacement;
    replacement->_col = BLACK;
    delete cur;
  }
  --count;
  return true;
}

template <class T, class Compare>
void RBTree<T, Compare>::MaintainAfterRemove(const Node *cur) {
  if (cur == _root) {
    return;
  }
  Node *Sibling;
  Node *parent = cur->_parent;
  if (parent->_left == cur) {
    Sibling = parent->_right;
  } else {
    Sibling = parent->_left;
  }
  //assert(Sibling);
  // case1
  if (Sibling->_col == RED) {
    if (parent->_left == cur) {
      RotateL(parent);
      Sibling->_col = BLACK;
      parent->_col = RED;
      Sibling=parent->_right;
    } else if (parent->_right == cur) {
      RotateR(parent);
      Sibling->_col = BLACK;
      parent->_col = RED;
      Sibling=parent->_left;
    }
    // to case3
  }

  // case2
  if (Sibling->_col == BLACK && parent->_col == RED &&
      (!Sibling->_left || Sibling->_left->_col == BLACK) &&
      (!Sibling->_right || Sibling->_right->_col == BLACK)) {
    Sibling->_col = RED;
    parent->_col = BLACK;
    return;
  }

  // case3
  if (Sibling->_col == BLACK && parent->_col == BLACK &&
      (!Sibling->_left || Sibling->_left->_col == BLACK) &&
      (!Sibling->_right || Sibling->_right->_col == BLACK)) {
    Sibling->_col = RED;
    MaintainAfterRemove(parent);
    return;
  }

  // case4
  if (Sibling->_col == BLACK) {
    if (cur == parent->_left &&
        (Sibling->_left && Sibling->_left->_col == RED) &&
        (!Sibling->_right || Sibling->_right->_col == BLACK)) {
      RotateR(Sibling);
      Sibling->_col = RED;
      Sibling = Sibling->_parent;
      Sibling->_col = BLACK;
    } else if (cur == parent->_right &&
               (Sibling->_right && Sibling->_right->_col == RED) &&
               (!Sibling->_left || Sibling->_left->_col == BLACK)) {
      RotateL(Sibling);
      Sibling->_col = RED;
      Sibling = Sibling->_parent;
      Sibling->_col = BLACK;
    }
  }

  // case5
  if (Sibling->_col == BLACK) {
    if (cur == parent->_left &&
        (!Sibling->_left || Sibling->_left->_col == BLACK) &&
        (Sibling->_right && Sibling->_right->_col == RED)) {
      RotateL(parent);
      Sibling->_col = parent->_col;
      parent->_col = BLACK;
      Sibling->_right->_col = BLACK;
      return;
    } else if (cur == parent->_right &&
               (!Sibling->_right || Sibling->_right->_col == BLACK) &&
               (Sibling->_left && Sibling->_left->_col == RED)) {
      RotateR(parent);
      Sibling->_col = parent->_col;
      parent->_col = BLACK;
      Sibling->_left->_col = BLACK;
      return;
    } 
  }

  //case6
  if(Sibling->_col==BLACK && (Sibling->_left && Sibling->_left->_col==RED) && (Sibling->_right && Sibling->_right->_col==RED))
  {
    if(cur==parent->_right)
    {
      RotateR(parent);
      Sibling->_col=parent->_col;
      Sibling->_left->_col=BLACK;
      Sibling->_right->_col=BLACK;
    }
    else if(cur==parent->_left)
    {
      RotateL(parent);
      Sibling->_col=parent->_col;
      Sibling->_left->_col=BLACK;
      Sibling->_right->_col=BLACK;
    }
  }
}

template <class T, class Compare> void RBTree<T, Compare>::clear() {
  List.Clear();
  if (this->_root != nullptr && count > 0) {
    _root->release();
    _root = nullptr;
  }
  count = 0;
}

template <class Key, class T, class Compare = std::less<Key>> class map {
  public:
  typedef pair<const Key, T> value_type;
  typedef RBTreeNode<value_type> Node;

private:
  RBTree<value_type, Compare> RBmap;

public:
  class const_iterator;
  class iterator {
  private:
    friend class const_iterator;
    ListNode<Node> *current;

  public:
  bool isNull()
  {
    return current==nullptr;
  }
    iterator() : current(nullptr) {}
    iterator(ListNode<Node> *cur) : current(cur) {}
    iterator(const iterator &other) { current = other.current; }
    iterator operator++(int) {
      ListNode<Node> *tempNode = current;
      if (!tempNode->next) {
        throw invalid_iterator();
      }
      current = current->next;
      iterator res(tempNode);
      return res;
    }
    iterator &operator++() {
      if (!current->next) {
        throw invalid_iterator();
      }
      current = current->next;
      return *this;
    }
    iterator operator--(int) {
      ListNode<Node> *tempNode = current;
      if (!tempNode->last || !tempNode->last->last) {
        throw invalid_iterator();
      }
      current = current->last;
      iterator res(tempNode);
      return res;
    }
    iterator &operator--() {
      if (!current->last || !current->last->last) {
        throw invalid_iterator();
      }
      current = current->last;
      return *this;
    }
    value_type &operator*() const { return *current->_data->_data; }
    bool operator==(const iterator &rhs) const {
      return current == rhs.current;
    }
    bool operator==(const const_iterator &rhs) const {
      return current == rhs.current;
    }
    bool operator!=(const iterator &rhs) const {
      return current != rhs.current;
    }
    bool operator!=(const const_iterator &rhs) const {
      return current != rhs.current;
    }
    value_type *operator->() const noexcept { 
      return current->_data->_data; 
      }
  };
  class const_iterator {
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
  private:
    // data members.
    friend class iterator;
    const ListNode<Node> *current;

  public:
    const_iterator() : current(nullptr) {}
    const_iterator(const ListNode<Node> *cur) : current(cur) {}
    const_iterator(const const_iterator &other) { current = other.current; }
    const_iterator(const iterator &other) { current = other.current; }

    const_iterator operator++(int) {
      const ListNode<Node> *tempNode = current;
      if (!tempNode->next) {
        throw invalid_iterator();
      }
      current = current->next;
      const_iterator res(tempNode);
      return res;
    }

    const_iterator &operator++() {
      if (!current->next) {
        throw invalid_iterator();
      }
      current = current->next;
      return *this;
    }

    const_iterator operator--(int) {
      const ListNode<Node> *tempNode = current;
      if (!tempNode->last || !tempNode->last->last) {
        throw invalid_iterator();
      }
      current = current->last;
      const_iterator res(tempNode);
      return res;
    }

    const_iterator &operator--() {
      if (!current->last || !current->last->last) {
        throw invalid_iterator();
      }
      current = current->last;
      return *this;
    }
    const value_type &operator*() const { return *current->_data->_data; }
    bool operator==(const iterator &rhs) const {
      return current == rhs.current;
    }
    bool operator==(const const_iterator &rhs) const {
      return current == rhs.current;
    }
    bool operator!=(const iterator &rhs) const {
      return current != rhs.current;
    }
    bool operator!=(const const_iterator &rhs) const {
      return current != rhs.current;
    }
    const value_type *operator->() const noexcept {
      return current->_data->_data;
    }
  };

  map() {}
  map(const map &other) { RBmap = other.RBmap; }

  map &operator=(const map &other) {
    if (this == &other) {
      return *this;
    } else {
      RBmap = other.RBmap;
      return *this;
    }
  }

  ~map() {}

  T &at(const Key &key) {
    Node *res = RBmap.Get(key);
    if (res == nullptr) {
      throw index_out_of_bound();
    }
    return res->_data->second;
  }
  const T &at(const Key &key) const {
    Node *res = RBmap.Get(key);
    if (res == nullptr) {
      throw index_out_of_bound();
    }
    return res->_data->second;
  }

  T &operator[](const Key &key) {
    Node *res = RBmap.Get(key);
    //std::cerr<<"GET\n";
    if (res == nullptr) {
      value_type *AddNode = new value_type(key, T());
      RBmap.Insert(*AddNode);
      delete AddNode;
      res = RBmap.Get(key);
    }
    //assert(res != nullptr);
    return res->_data->second;
  }

  const T &operator[](const Key &key) const {
    Node *res = RBmap.Get(key);
    if (res == nullptr) {
      throw index_out_of_bound();
    }
    return res->_data->second;
  }

  iterator begin() {
    iterator it(RBmap.begin());
    return it;
  }
  const_iterator cbegin() const {
    const_iterator cit(RBmap.begin());
    return cit;
  }

  iterator end() {
    iterator it(RBmap.end());
    return it;
  }
  const_iterator cend() const {
    const_iterator cit(RBmap.end());
    return cit;
  }

  bool empty() const { return RBmap.Size() == 0; }

  size_t size() const { return RBmap.Size(); }

  void clear() { RBmap.clear(); }

  pair<iterator, bool> insert(const value_type &value) {
    bool if_insert;
    if_insert = RBmap.Insert(value);
    iterator it(RBmap.Get(value.first)->LisN);
    pair<iterator, bool> res(it, if_insert);
    return res;
  }

  void erase(iterator pos) {

    if (pos == this->end()) {
      throw runtime_error();
    }
    Node *tmp=RBmap.GetNode((*pos));
    if(tmp==nullptr)
    {
      throw runtime_error();
    }
    bool if_remove;
    //assert(tmp!=nullptr);
    if_remove = RBmap.Remove(*pos);
    if (!if_remove) {
      std::cerr << "if_remove\n";
      throw runtime_error();
    }
  }

  size_t count(const Key &key) const {
    Node *check = RBmap.Get(key);
    return check ? 1 : 0;
  }

  iterator find(const Key &key) {
    Node *res = RBmap.Get(key);
    if (!res) {
      return end();
    } else {
      //assert(res->LisN!=nullptr);
      iterator it(res->LisN);
      return it;
    }
  }
  const_iterator find(const Key &key) const {
    Node *res = RBmap.Get(key);
    if (!res) {
      return cend();
    } else {
      const_iterator it(res->LisN);
      return it;
    }
  }

  void Traverse() {
    for (iterator it = begin(); it != end(); ++it) {
      std::cout << (*it).first << " | " << (*it).second << '\n';
    }
    std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    RBmap.ShowTree();
  }
};

} // namespace sjtu

#endif