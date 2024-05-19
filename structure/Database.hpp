#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>
#include <utility>
using std::string;
// constexpr int Block_Size = 40;
constexpr int LRU_MAX = 20;
// constexpr int M = 1000000000000000031;
// constexpr int B = 233;

template <class key, class Value, int Block_Size> class BPT {
private:
  typedef std::pair<key, Value> Elem;
  struct Node {
    int size;
    int pos;
    int ptr[Block_Size + 1];
    Elem data[Block_Size];
    bool IS_LEAF = false;
  };
  class List {
  private:
    struct ListNode {
      Node elem;
      ListNode *next, *prev;
      int num;
      int pos;
      ListNode() : next(nullptr), prev(nullptr), num(0) {}
      ListNode(const Node &_elem)
          : elem(_elem), next(nullptr), prev(nullptr), num(0) {}
    };

    int GetMini() {
      ListNode *cur = Head->next;
      int res = cur->num;
      while (cur) {
        if (cur->num < res) {
          res = cur->num;
        }
        cur = cur->next;
      }
      return res;
    }

    ListNode *Head;
    int ord;
    int _len;
    int mini_ord;

  public:
    List() : ord(0), _len(0), mini_ord(0) {
      Head = new ListNode();
      Head->prev = nullptr;
      Head->prev = nullptr;
    }
    ~List() {
      ListNode *cur = Head;
      while (cur) {
        ListNode *tmp = cur;
        cur = cur->next;
        delete tmp;
      }
      ord = 0;
      _len = 0;
      mini_ord = 0;
      Head = nullptr;
    }
    void Modify(const Node &elem, const int &pos) {
      if (_len == 0) {
        ListNode *lis_node = new ListNode(elem);
        lis_node->prev = Head;
        lis_node->next = nullptr;
        Head->next = lis_node;
        lis_node->pos = pos;
        lis_node->num = ++ord;
        mini_ord = lis_node->num;
        ++_len;
        return;
      } else {
        ListNode *cur = Head->next;
        ListNode *last;
        while (cur && cur->pos != pos) {
          last = cur;
          cur = cur->next;
        }
        if (!cur) {
          if (_len < LRU_MAX) {
            ListNode *lis_node = new ListNode(elem);
            lis_node->prev = last;
            lis_node->next = nullptr;
            last->next = lis_node;
            lis_node->pos = pos;
            lis_node->num = ++ord;
            ++_len;
          } else {
            ListNode *lis_node = Head->next;
            while (lis_node) {
              if (lis_node->num == mini_ord) {
                break;
              }
              lis_node = lis_node->next;
            }
            assert(lis_node);
            lis_node->elem = elem;
            lis_node->num = ++ord;
            lis_node->pos = pos;
            mini_ord = GetMini();
          }
        } else {
          cur->elem = elem;
          if (cur->num == mini_ord) {
            cur->num = ++ord;
            mini_ord = GetMini();
          } else {
            cur->num = ++ord;
          }
          cur->pos = pos;
          return;
        }
      }
    }
    bool Get(Node &t, const int &pos) {
      if (_len == 0) {
        return false;
      } else {
        ListNode *cur = Head->next;
        while (cur) {
          if (cur->pos == pos) {
            t = cur->elem;
            return true;
          }
          cur = cur->next;
        }
        return false;
      }
    }
  };

  class FileWriting {
  private:
    std::fstream file;
    string file_name;
    List LRU;
    int Elem_Size = sizeof(Node);

  public:
    FileWriting() = default;
    ~FileWriting() {
      if (file.is_open()) {
        file.close();
      }
    }
    FileWriting(const string &file_name) : file_name(file_name) {}
    void Setfile(const string &name) { file_name = name; }
    void initialise() {
      file.open(file_name, std::ios::out);
      int beg = 0;
      file.write(reinterpret_cast<char *>(&beg), sizeof(int));
      file.close();
    }
    void Clear() {
      file.close();
      file.open(file_name, std::ios::out);
      file.close();
    }
    void Close() { file.close(); }
    //检查是否是第一次打开
    bool IS_NEW() {
      file.open(file_name, std::ios::in | std::ios::out| std::ios::ate);
      if (!file.is_open()) {
        std::ofstream file(file_name);
        file.open(file_name, std::ios::in | std::ios::out| std::ios::ate);
        file.close();
        return true;
      }
      if (0 < file.tellp()) {
        int tmp = get_info();
        if (!tmp) {
          file.close();
          return true;
        }
        file.close();
        return false;
      } else {
        file.close();
        return true;
      }
    }
    int get_info() {
      // file.close();
      if (!file.is_open()) {
        file.open(file_name, std::ios::in | std::ios::out);
      }
      // file.open(file_name, std::ios::in | std::ios::out);
      file.seekg(0);
      int res;
      file.read(reinterpret_cast<char *>(&res), sizeof(int));
      // file.close();
      return res;
    }
    void write_info(int tmp) {
      // file.close();
      // file.open(file_name, std::ios::in | std::ios::out);
      if (!file.is_open()) {
        file.open(file_name, std::ios::in | std::ios::out);
      }
      file.seekp(0);
      file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
      // file.close();
    }
    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    void writeA(Node &t) {
      file.close();
      // if(!file.is_open())
      // {
      file.open(file_name, std::ios::out | std::ios::in | std::ios::ate);
      // }
      int pos = file.tellp();
      t.pos = pos;
      // std::cerr<<"tpos: "<<t.pos<<'\n';
      file.write(reinterpret_cast<char *>(&t), Elem_Size);
      LRU.Modify(t, t.pos);
      file.close();
    }
    void write_root(Node &t) {
      file.close();
      // if (!file.is_open()) {
      //   file.open(file_name, std::ios::out | std::ios::in | std::ios::ate);
      // }
      file.open(file_name, std::ios::out | std::ios::in | std::ios::ate);
      int pos = file.tellp();
      t.pos = pos;
      file.write(reinterpret_cast<char *>(&t), Elem_Size);
      write_info(pos);
      LRU.Modify(t, t.pos);
      // assert(t.pos==pos);
      file.close();
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void updateA(Node &t, const int index) {
      // file.close();
      if (!file.is_open()) {
        file.open(file_name, std::ios::out | std::ios::in);
      }
      // file.open(file_name, std::ios::in | std::ios::out);
      file.seekp(index);
      // std::cerr<<"update: "<<t.size<<'\n';
      file.write(reinterpret_cast<char *>(&t), Elem_Size);
      assert(t.pos == index);
      LRU.Modify(t, index);
      // file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void readA(Node &t, const int index) {
      // file.close();
      if (LRU.Get(t, index)) {
        return;
      }
      if (!file.is_open()) {
        file.open(file_name, std::ios::in | std::ios::out);
      }
      // file.open(file_name, std::ios::in | std::ios::out);
      file.seekg(index);
      file.read(reinterpret_cast<char *>(&t), Elem_Size);
      LRU.Modify(t, index);
      // file.close();
    }
  };

  // member
  FileWriting F;
  Node root;

  // function
  // int Get_Ind(const string &index);
  Elem make_pair(const key &index, const Value &value) {
    Elem res;
    // std::memset(res.first.info, 0, sizeof(res.first.info));
    // index.copy(res.first.info, sizeof(res.first.info) - 1);
    res.first = index;
    res.second = value;
    return res;
  }
  bool Less(const Elem &lhs, const Elem &rhs) {
    if (lhs.first < rhs.first) {
      return true;
    } else {
      if (lhs.first > rhs.first) {
        return false;
      } else if (lhs.second < rhs.second) {
        return true;
      } else {
        return false;
      }
    }
  }
  bool Ins(const Elem &elem);
  int insertval(const Elem &elem, Node &node);
  bool split(const Elem &elem, int *stack, int &dep, Node &node);
  void insert_Interval(const Elem &elem, const int &pos, const int &lhs,
                       const int &rhs, int *stack, int &dep);
  bool Del(const Elem &elem);
  int deleteval(const Elem &elem, Node &node);
  void ReAlloc(int *stack, int &dep, Node &cur);
  void DeletInterval(const Elem &elem, Node &node, int *stack, int &dep);
  // void traverse(const Node &cur);

  void find(const key &index, Value *_seq, int &size);

public:
  BPT(const string &_FileName);
  ~BPT() {}
  bool Insert(const key &index, const Value &value);
  bool Delete(const key &index, const Value &value);
  void Find(const key &index, Value *_set, int &size);
  void Modify(const key &index, Value &_set);
  void clear() {
    F.Clear();
    F.initialise();
    root.size = 0;
    root.IS_LEAF = true;
    root.ptr[0] = 0;
    root.ptr[Block_Size] = 0;
    F.write_root(root);
  }
  // void Traverse();
  // void DepTraverse();
};

template <class key, class Value, int Block_Size>
BPT<key, Value, Block_Size>::BPT(const string &_FileName) {
  F.Setfile(_FileName);
  if (F.IS_NEW()) {
    F.initialise();
    root.size = 0;
    root.IS_LEAF = true;
    root.ptr[0] = 0;
    root.ptr[Block_Size] = 0;
    F.write_root(root);
  } else {
    F.readA(root, F.get_info());
  }
}

// inline int BPT::Get_Ind(const string &index) {
//   int res = 0;
//   for (int pos = 0; pos < index.length(); ++pos) {
//     res = ((int)res * B + index[pos]) % M;
//   }
//   return res;
// }
template <class key, class Value, int Block_Size>
bool BPT<key, Value, Block_Size>::Insert(const key &index, const Value &value) {
  F.readA(root, F.get_info());
  // assert(root.pos == F.get_info());
  Elem cur = make_pair(index, value);
  return Ins(cur);
}

template <class key, class Value, int Block_Size>
bool BPT<key, Value, Block_Size>::Ins(const Elem &elem) {
  if (root.size == 0) {
    root.data[root.size++] = elem;
    root.IS_LEAF = true;
    root.ptr[0] = 0;
    root.ptr[Block_Size] = 0;
    F.updateA(root, root.pos);
    return true;
  } else {
    Node cur = root;
    int stack[10000];
    int dep = 0;
    int ch = 0;
    while (!cur.IS_LEAF) {
      // std::cerr<<"turn"<<ch<<"\n";
      stack[dep++] = cur.pos;
      for (int i = 0; i < cur.size; ++i) {
        if (Less(elem, cur.data[i])) {
          // std::cerr<<"turn1\n";
          ch = 1;
          // std::cerr<<cur.data[i].second<<"road\n";
          F.readA(cur, cur.ptr[i]);
          break;
        }
        if (i == cur.size - 1) {
          // std::cerr<<"road\n";
          // std::cerr<<"turn2\n"<<cur.ptr[i + 1];
          ch = cur.ptr[i + 1];
          F.readA(cur, cur.ptr[i + 1]);
          break;
        }
      }
    }
    // assert(cur.IS_LEAF);
    if (cur.size + 1 < Block_Size) {
      int res = insertval(elem, cur);
      if (res == -1)
        return false;
      F.updateA(cur, cur.pos);
    } else {
      split(elem, stack, dep, cur);
    }
    return true;
  }
}

template <class key, class Value, int Block_Size>
int BPT<key, Value, Block_Size>::insertval(const Elem &elem, Node &node) {
  int i = 0;
  int l = 0, r = node.size, mid;
  while (l < r) {
    mid = (l + r) / 2;
    if (Less(node.data[mid], elem)) {
      l = mid + 1;
    } else {
      r = mid;
    }
  }
  i = r;
  // while (i < node.size && Less(node.data[i], elem)) {
  //   ++i;
  // }
  if (i < node.size && node.data[i].first == elem.first &&
      node.data[i].second == elem.second) {
    return -1;
  }
  for (int j = node.size; j > i; --j) {
    node.data[j] = node.data[j - 1];
  }
  node.data[i] = elem;
  ++node.size;
  // assert(node.size<=Block_Size);
  return i;
}

template <class key, class Value, int Block_Size>
bool BPT<key, Value, Block_Size>::split(const Elem &elem, int *stack, int &dep,
                                        Node &node) {
  Node Rleaf, Lleaf;
  Rleaf.IS_LEAF = true;
  Lleaf.IS_LEAF = true;
  int res = insertval(elem, node);
  if (res == -1) {
    return false;
  }
  Lleaf.size = Block_Size / 2;
  Rleaf.size = Block_Size - Block_Size / 2;
  for (int i = 0; i < Lleaf.size; ++i) {
    Lleaf.data[i] = node.data[i];
  }
  for (int i = 0; i < Rleaf.size; ++i) {
    Rleaf.data[i] = node.data[Block_Size / 2 + i];
  }
  Lleaf.pos = node.pos;
  Rleaf.ptr[0] = Lleaf.pos;
  Rleaf.ptr[Block_Size] = node.ptr[Block_Size];
  Lleaf.ptr[0] = node.ptr[0];
  F.writeA(Rleaf);
  if (Lleaf.ptr[0]) {
    Node prev;
    F.readA(prev, Lleaf.ptr[0]);
    prev.ptr[Block_Size] = Lleaf.pos;
    F.updateA(prev, prev.pos);
  }
  if (Rleaf.ptr[Block_Size]) {
    Node next;
    F.readA(next, Rleaf.ptr[Block_Size]);
    next.ptr[0] = Rleaf.pos;
    F.updateA(next, next.pos);
  }
  Lleaf.ptr[Block_Size] = Rleaf.pos;
  F.updateA(Lleaf, Lleaf.pos);

  if (node.pos == root.pos) {
    Node newRoot;
    newRoot.data[0] = node.data[Block_Size / 2];
    newRoot.ptr[0] = Lleaf.pos;
    newRoot.ptr[1] = Rleaf.pos;
    newRoot.IS_LEAF = false;
    newRoot.size = 1;
    F.write_root(newRoot);
    root = newRoot;
    return true;
  } else {
    --dep;
    insert_Interval(node.data[Block_Size / 2], stack[dep], Lleaf.pos, Rleaf.pos,
                    stack, dep);
    return true;
  }
}

template <class key, class Value, int Block_Size>
void BPT<key, Value, Block_Size>::insert_Interval(const Elem &elem,
                                                  const int &pos,
                                                  const int &lhs,
                                                  const int &rhs, int *stack,
                                                  int &dep) {
  // std::cerr << elem.second << "interval\n";
  Node cur;
  F.readA(cur, pos);
  // std::cerr <<"begin_check"<<cur.data[0].first<<" "<< cur.data[0].second <<
  // '\n';
  if (cur.size + 1 < Block_Size) {
    int i = insertval(elem, cur);
    // std::cerr << "i: " << i << '\n';
    for (int j = cur.size; j > i + 1; --j) {
      cur.ptr[j] = cur.ptr[j - 1];
    }
    // if(pos==F.get_info())std::cerr<<"This changes root\n";
    cur.ptr[i] = lhs;
    cur.ptr[i + 1] = rhs;
    F.updateA(cur, pos);
    return;
  } else {
    Node Lchild;
    Node Rchild;
    Lchild.IS_LEAF = false;
    Rchild.IS_LEAF = false;
    int i = insertval(elem, cur);
    // std::cerr <<"begin_check2"<<cur.data[0].first<<" "<< cur.data[0].second
    // << '\n'; std::cerr<<"test:"<<cur.size<<'\n';
    for (int j = cur.size; j > i + 1; --j) {
      cur.ptr[j] = cur.ptr[j - 1];
    }
    // std::cerr<<"cur00:"<<cur.data[0].first<<"!"<<cur.data[0].second<<'\n';
    cur.ptr[i] = lhs;
    cur.ptr[i + 1] = rhs;
    Lchild.size = (Block_Size - 1) / 2;
    Rchild.size = Block_Size - Lchild.size - 1;
    // std::cerr<<"cur01:"<<cur.data[0].first<<"!"<<cur.data[0].second<<'\n';
    for (int p = 0; p < Lchild.size; ++p) {
      Lchild.data[p] = cur.data[p];
      Lchild.ptr[p] = cur.ptr[p];
    }
    // std::cerr<<"cur02:"<<cur.data[0].first<<"!"<<cur.data[0].second<<'\n';
    Lchild.ptr[Lchild.size] = cur.ptr[Lchild.size];
    for (int p = 0; p < Rchild.size; ++p) {
      Rchild.data[p] = cur.data[Lchild.size + 1 + p];
      Rchild.ptr[p] = cur.ptr[Lchild.size + 1 + p];
    }
    Rchild.ptr[Rchild.size] = cur.ptr[Lchild.size + 1 + Rchild.size];
    Rchild.pos = cur.pos;
    F.writeA(Lchild);
    F.updateA(Rchild, pos);
    if (pos == root.pos) {
      Node newRoot;
      newRoot.data[0] = cur.data[Lchild.size];
      newRoot.ptr[0] = Lchild.pos;
      newRoot.ptr[1] = Rchild.pos;
      newRoot.IS_LEAF = false;
      newRoot.size = 1;
      F.write_root(newRoot);
      root = newRoot;
      return;
      // std::cerr<<Lchild.data[0].first<<"check<<\n";
    } else {
      if (dep <= 0)
        exit(0);
      --dep;
      insert_Interval(cur.data[Lchild.size], stack[dep], Lchild.pos, Rchild.pos,
                      stack, dep);
    }
  }
}

template <class key, class Value, int Block_Size>
bool BPT<key, Value, Block_Size>::Delete(const key &index, const Value &value) {
  F.readA(root, F.get_info());
  // assert(root.pos == F.get_info());
  //  std::cerr << "former root pos" << root.pos << '\n';
  Elem cur = make_pair(index, value);
  return Del(cur);
}

template <class key, class Value, int Block_Size>
int BPT<key, Value, Block_Size>::deleteval(
    const Elem &elem,
    Node &node) //-1不存在，i找到并删除,删除数据在原i位(0-base)
{
  int i = 0;
  int l = 0, r = node.size, mid;
  while (l < r) {
    mid = (l + r) / 2;
    if (Less(node.data[mid], elem)) {
      l = mid + 1;
    } else {
      r = mid;
    }
  }
  i = r;
  // while (i < node.size && Less(node.data[i], elem)) {
  //   ++i;
  // }
  if (i < node.size && node.data[i].first == elem.first &&
      node.data[i].second == elem.second) {
    for (int j = i; j < node.size - 1; ++j) {
      node.data[j] = node.data[j + 1];
    }
    // node.data[node.size - 1].second.val = -1141514;
    --node.size;
    // if(!(node.pos==root.pos || node.size>0))exit(0);
    return i;
  }
  // std::cerr << "Can not find\n";
  return -1;
}

template <class key, class Value, int Block_Size>
bool BPT<key, Value, Block_Size>::Del(const Elem &elem) {
  if (root.size == 0) {
    // std::cerr << "root_sizs is 0\n";
    return false;
  } else {
    Node cur = root;
    int stack[10000];
    int dep = 0;
    int ch = 0;
    while (!cur.IS_LEAF) {
      stack[dep++] = cur.pos;
      // std::cerr<<"Turn"<<ch<<"\n";
      for (int i = 0; i < cur.size; ++i) {
        if (Less(elem, cur.data[i])) {
          // std::cerr<<cur.data[i].second<<"road\n";
          // std::cerr<<"Turn1\n";
          ch = 1;
          F.readA(cur, cur.ptr[i]);
          break;
        }
        if (i == cur.size - 1) {
          // std::cerr<<"road\n";
          F.readA(cur, cur.ptr[i + 1]);
          // std::cerr<<"Turn2\n";
          ch = 2;
          break;
        }
      }
    }
    int statue = deleteval(elem, cur);
    // std::cerr<<cur.data[0].second<<" |||
    // "<<cur.data[cur.size-1].second<<'\n'; std::cerr << "finish deletval\n";
    if (statue == -1)
      return false;
    if (statue == 0 && dep >= 1) {
      Node parent;
      F.readA(parent, stack[dep - 1]);
      int i = 0;
      for (; i <= parent.size; ++i) {
        if (parent.ptr[i] == cur.pos) {
          break;
        }
      }
      if (i != 0) {
        parent.data[i - 1] = cur.data[0];
        F.updateA(parent, parent.pos);
      } else if (cur.ptr[0] != 0) {
        int curdep = dep;
        bool run = true;
        Node currentgrand;
        F.readA(currentgrand, stack[curdep - 2]);
        while (run) {
          for (int j = 1; j <= currentgrand.size; ++j) {
            if (currentgrand.ptr[j] == parent.pos) {
              // assert(currentgrand.data[j - 1].first == elem.first &&
              //        currentgrand.data[j - 1].second == elem.second);
              // std::cerr<<cur.data[0].second<<"run good
              // !!!!!!!!!!!!!!!!!!!!!\n";
              currentgrand.data[j - 1] = cur.data[0];
              run = false;
              F.updateA(currentgrand, currentgrand.pos);
              break;
            }
          }
          if (run) {
            --curdep;
            parent = currentgrand;
            F.readA(currentgrand, stack[curdep - 2]);
          }
        }
      }
    }
    if (cur.size >= Block_Size / 2) {
      F.updateA(cur, cur.pos);
      return true;
    } else {
      if (cur.pos == root.pos) {
        // std::cerr<<"!!!!!!!!!!!!!!!!!!!!\n";
        F.updateA(cur, cur.pos);
        return true;
      }
      ReAlloc(stack, dep, cur);
    }
    return true;
  }
}

template <class key, class Value, int Block_Size>
void BPT<key, Value, Block_Size>::ReAlloc(int *stack, int &dep, Node &cur) {
  --dep;
  // assert(dep >= 0);
  Node parent;
  Node Silbing;
  F.readA(parent, stack[dep]);
  int p = -1;
  for (int i = 0; i <= parent.size; ++i) {
    if (parent.ptr[i] == cur.pos) {
      p = i;
      break;
    }
  }
  // assert(p != -1);
  if (p != 0) {
    F.readA(Silbing, parent.ptr[p - 1]);
    if (Silbing.size > Block_Size / 2) {
      insertval(Silbing.data[Silbing.size - 1], cur);
      --Silbing.size;
      parent.data[p - 1] = cur.data[0];
      // std::cerr << " >>" << parent.data[p - 1].second << '\n';
      //  if (parent.pos == root.pos)
      //    std::cerr << root.pos << "Change root\n";
      F.updateA(Silbing, Silbing.pos);
      F.updateA(parent, parent.pos);
      F.updateA(cur, cur.pos);
      // std::cerr << cur.data[1].second << "<<check\n";
      return;
    }
  }
  if (p != parent.size) {
    F.readA(Silbing, parent.ptr[p + 1]);
    if (Silbing.size > Block_Size / 2) {
      insertval(Silbing.data[0], cur);
      for (int i = 0; i < Silbing.size - 1; ++i) {
        Silbing.data[i] = Silbing.data[i + 1];
      }
      --Silbing.size;
      parent.data[p] = Silbing.data[0];
      F.updateA(Silbing, Silbing.pos);
      F.updateA(parent, parent.pos);
      F.updateA(cur, cur.pos);
      return;
    }
  }
  if (p != parent.size) {
    // std::cerr << p << " " << parent.size << "special case happen\n";
    if (cur.size == 0 && p - 1 >= 0) {
      // std::cerr << "special case happen\n";
      parent.data[p - 1] = Silbing.data[0];
      F.updateA(parent, parent.pos);
    }
    for (int i = 0; i < Silbing.size; ++i) {
      cur.data[cur.size] = Silbing.data[i];
      ++cur.size;
    }
    cur.ptr[Block_Size] = Silbing.ptr[Block_Size];
    if (cur.ptr[Block_Size] != 0) {
      Node next;
      F.readA(next, cur.ptr[Block_Size]);
      next.ptr[0] = cur.pos;
      F.updateA(next, next.pos);
    }
    F.updateA(cur, cur.pos);
    DeletInterval(Silbing.data[0], parent, stack, dep);
    return;
  }
  if (p != 0) {
    // std::cerr << "!!!special case happen\n";
    for (int i = 0; i < cur.size; ++i) {
      Silbing.data[Silbing.size] = cur.data[i];
      ++Silbing.size;
    }
    Silbing.ptr[Block_Size] = cur.ptr[Block_Size];
    if (Silbing.ptr[Block_Size] != 0) {
      Node next;
      F.readA(next, Silbing.ptr[Block_Size]);
      next.ptr[0] = Silbing.pos;
      F.updateA(next, next.pos);
    }
    F.updateA(Silbing, Silbing.pos);
    DeletInterval(cur.data[0], parent, stack, dep);
    return;
  }
  // assert(0);
}

template <class key, class Value, int Block_Size>
void BPT<key, Value, Block_Size>::DeletInterval(const Elem &elem, Node &node,
                                                int *stack, int &dep) {
  // std::cerr<<"you need debug\n";
  int i = deleteval(elem, node);
  // std::cerr<<"debug0\n"<<node.size;
  for (; i < node.size + 1; ++i) {
    node.ptr[i + 1] = node.ptr[i + 2];
  }
  if (node.size >= Block_Size / 2 - 1) {
    // std::cerr<<"debug01\n"<<node.pos;
    F.updateA(node, node.pos);
    return;
  } else {
    if (node.pos == root.pos) { // std::cerr<<"debug1\n";
      if (node.size == 0) {     // std::cerr<<"debug11\n";
        Node child;
        F.readA(child, node.ptr[0]);
        child.pos = node.pos;
        F.updateA(child, node.pos);
      } else {
        F.updateA(node, node.pos);
      }
      return;
    } else { // std::cerr<<"debug2\n";
      --dep;
      // assert(dep >= 0);
      Node parent;
      Node Silbing;
      int p = -1;
      F.readA(parent, stack[dep]);
      for (int i = 0; i <= parent.size; ++i) {
        if (parent.ptr[i] == node.pos) {
          p = i;
          break;
        }
      }
      if (p != 0) {
        F.readA(Silbing, parent.ptr[p - 1]);
        if (Silbing.size > Block_Size / 2) {
          for (int j = node.size; j > 0; --j) {
            node.data[j] = node.data[j - 1];
            node.ptr[j + 1] = node.ptr[j];
          }
          node.ptr[1] = node.ptr[0];
          node.ptr[0] = Silbing.ptr[Silbing.size];
          node.data[0] = parent.data[p - 1];
          ++node.size;
          parent.data[p - 1] = Silbing.data[Silbing.size - 1];
          --Silbing.size;
          F.updateA(parent, parent.pos);
          F.updateA(Silbing, Silbing.pos);
          F.updateA(node, node.pos);
          return;
        }
      }
      if (p != parent.size) {
        F.readA(Silbing, parent.ptr[p + 1]);
        if (Silbing.size > Block_Size / 2) {
          node.data[node.size] = parent.data[p];
          ++node.size;
          node.ptr[node.size] = Silbing.ptr[0];
          parent.data[p] = Silbing.data[0];
          --Silbing.size;
          for (int j = 0; j < Silbing.size; ++j) {
            Silbing.data[j] = Silbing.data[j + 1];
            Silbing.ptr[j] = Silbing.ptr[j + 1];
          }
          Silbing.ptr[Silbing.size] = Silbing.ptr[Silbing.size + 1];
          F.updateA(parent, parent.pos);
          F.updateA(node, node.pos);
          F.updateA(Silbing, Silbing.pos);
          return;
        }
      }
      if (p != parent.size) //右边合并
      {
        node.data[node.size] = parent.data[p];
        ++node.size;
        for (int j = 0; j < Silbing.size; ++j) {
          node.data[node.size] = Silbing.data[j];
          node.ptr[node.size] = Silbing.ptr[j];
          ++node.size;
        }
        node.ptr[node.size] = Silbing.ptr[Silbing.size];
        F.updateA(node, node.pos);
        DeletInterval(parent.data[p], parent, stack, dep);
        return;
      }
      if (p != 0) //左边合并
      {
        Silbing.data[Silbing.size] = parent.data[p - 1];
        ++Silbing.size;
        for (int j = 0; j < node.size; ++j) {
          Silbing.data[Silbing.size] = node.data[j];
          Silbing.ptr[Silbing.size] = node.ptr[j];
          ++Silbing.size;
        }
        Silbing.ptr[Silbing.size] = node.ptr[node.size];
        F.updateA(Silbing, Silbing.pos);
        DeletInterval(parent.data[p - 1], parent, stack, dep);
        return;
      }
    }
  }
}

template <class key, class Value, int Block_Size>
void BPT<key, Value, Block_Size>::Modify(const key &index, Value &_set) {
  F.readA(root, F.get_info());
  // assert(root.pos == F.get_info());
  key _index(index);
  if (root.size == 0) {
    return;
  }
  Node cur = root;
  while (!cur.IS_LEAF) {
    for (int i = 0; i < cur.size; ++i) {
      if (index < cur.data[i].first || index == cur.data[i].first) {
        // std::cerr<<cur.data[i].second<<"road\n";
        F.readA(cur, cur.ptr[i]);
        break;
      }
      if (i == cur.size - 1) {
        // std::cerr<<"road\n";
        F.readA(cur, cur.ptr[i + 1]);
        break;
      }
    }
  }
  int pos = 0;
  for (; pos < cur.size; ++pos) {
    if (index == cur.data[pos].first) {
      break;
    }
  }
  // std::cerr<<'\n'<<cur.data[pos-1].first<<'\n';
  if (pos == cur.size) {
    F.readA(cur, cur.ptr[Block_Size]);
    pos = 0;
    assert(index == cur.data[0].first);
  }
  assert(pos < cur.size && index == cur.data[pos].first);
  cur.data[pos].second = _set;
  F.updateA(cur, cur.pos);
  return;
}

template <class key, class Value, int Block_Size>
void BPT<key, Value, Block_Size>::Find(const key &index, Value *_seq,
                                       int &size) {
  F.readA(root, F.get_info());
  // assert(root.pos == F.get_info());
  key _index(index);
  find(_index, _seq, size);
}

template <class key, class Value, int Block_Size>
void BPT<key, Value, Block_Size>::find(const key &index, Value *_seq,
                                       int &size) {
  // std::cerr<<root.size<<'\n';
  size = 0;
  if (root.size == 0) {
    // std::cout << "null" << '\n';
    return;
  }
  Node cur = root;
  while (!cur.IS_LEAF) {
    for (int i = 0; i < cur.size; ++i) {
      if (index < cur.data[i].first || index == cur.data[i].first) {
        // std::cerr<<cur.data[i].second<<"road\n";
        F.readA(cur, cur.ptr[i]);
        break;
      }
      if (i == cur.size - 1) {
        // std::cerr<<"road\n";
        F.readA(cur, cur.ptr[i + 1]);
        break;
      }
    }
  }
  // assert(cur.IS_LEAF);
  int pos = 0;
  for (; pos < cur.size; ++pos) {
    if (index == cur.data[pos].first) {
      break;
    }
  }
  bool find_all = false;
  bool if_has = false;
  if (pos == cur.size && cur.data[pos - 1].first > index) {
    // std::cout << "null\n";
    size = 0;
    return;
  }
  while (!find_all) {
    // std::cerr<<pos<<" "<<cur.size<<'\n';
    for (; pos < cur.size; ++pos) {
      if (!(index == cur.data[pos].first)) {
        find_all = true;
        break;
      }
      _seq[size++] = cur.data[pos].second;
      // std::cout << cur.data[pos].second << " ";
      if_has = true;
    }
    if (!find_all) {
      if (cur.ptr[Block_Size] == 0) {
        if (!if_has) {
          // std::cout << "null\n";
          size = 0;
          return;
        } else {
          // std::cout << '\n';
        }
        return;
      }
      // std::cerr<<cur.ptr[Block_Size]<<"<<<\n";
      F.readA(cur, cur.ptr[Block_Size]);
      pos = 0;
    }
  }
  if (if_has) {
    // std::cout << '\n';
    return;
  }
  // std::cout << "null\n";
  size = 0;
  return;
}

// void BPT::Traverse() {
//   F.readA(root, F.get_info());
//   Node cur = root;
//   std::cerr << "show root\n";
//   for (int i = 0; i < cur.size; ++i) {
//     std::cerr << cur.data[i].first << '|' << cur.data[i].second << ' ';
//   }
//   std::cerr << "\n";
//   while (cur.ptr[0] != 0) {
//     F.readA(cur, cur.ptr[0]);
//   }
//   // std::cerr << "check\n";
//   while (cur.ptr[Block_Size] != 0) {
//     std::cerr << "size: " << cur.size << '\n';
//     for (int i = 0; i < cur.size; ++i) {
//       std::cout << cur.data[i].first << '|' << cur.data[i].second << ' ';
//     }
//     std::cout << '\n';
//     F.readA(cur, cur.ptr[Block_Size]);
//   }
//   std::cerr << "size: " << cur.size << '\n';
//   for (int i = 0; i < cur.size; ++i) {
//     std::cout << cur.data[i].first << '|' << cur.data[i].second << ' ';
//   }
//   std::cout << '\n';
// }

// void BPT::DepTraverse() {
//   F.readA(root, F.get_info());
//   traverse(root);
// }

// void BPT::traverse(const Node &cur) {
//   for (int i = 0; i < cur.size; ++i) {
//     std::cerr << cur.data[i].first << "|" << cur.data[i].second << " ";
//   }
//   std::cerr << '\n';
//   Node son;
//   if (!cur.IS_LEAF) {
//     for (int i = 0; i <= cur.size; ++i) {
//       if (cur.ptr[i] != 0) {
//         F.readA(son, cur.ptr[i]);
//         std::cerr << "Down to " << i << '\n';
//         traverse(son);
//         std::cerr << "Up\n";
//       }
//     }
//   }
// }

// test
// void test1() {
//   BPT a;
//   int n, m, k, value;
//   string index;
//   std::cin >> n >> m >> k;
//   for (int i = 0; i < n; ++i) {
//     std::cin >> index >> value;
//     a.Insert(index, value);
//     a.Traverse();
//     std::cerr << "____________________________________\n";
//     a.DepTraverse();
//   }
//   for (int i = 0; i < m; ++i) {
//     std::cin >> index >> value;
//     a.Delete(index, value);
//     a.Traverse();
//     std::cerr << "____________________________________\n";
//     a.DepTraverse();
//   }
//   for (int i = 0; i < k; ++i) {
//     std::cin >> index;
//     a.Find(index);
//     a.DepTraverse();
//   }
// }

// void test2() {
//   string instr, index;
//   int value;
//   std::fstream file;
//   file.open("BPToutput", std::ios::out);
//   file.close();
//   BPT a;
//   while (true) {
//     std::cin >> instr;
//     if (instr == "1") {
//       std::cin >> index >> value;
//       a.Insert(index, value);
//     } else if (instr == "2") {
//       std::cin >> index >> value;
//       a.Delete(index, value);
//     } else if (instr == "3") {
//       std::cin >> index;
//       a.Find(index);
//     } else if (instr == "quit") {
//       return;
//     } else {
//       std::cout << "man,what can I say!\n";
//     }
//     // a.DepTraverse();
//   }
// }

// int HASH(const string &index) {
//   int res = 0;
//   for (int pos = 0; pos < index.length(); ++pos) {
//     res = ((int)res * B + index[pos]) % M;
//   }
//   return res;
// }
// void test3() {
//   std::map<int, std::set<int>> b;
//   string instr, index;
//   int value;
//   std::fstream file;
//   file.open("BPToutput", std::ios::out);
//   file.close();
//   BPT a;
//   while (true) {
//     std::cin >> instr;
//     if (instr == "1") {
//       std::cin >> index >> value;
//       b[HASH(index)].insert(value);
//       a.Insert(index, value);
//     } else if (instr == "2") {
//       std::cin >> index >> value;
//       b[HASH(index)].erase(value);
//       a.Delete(index, value);
//     } else if (instr == "3") {
//       std::cin >> index;
//       a.Find(index);
//       for (auto i = b[HASH(index)].begin(); i != b[HASH(index)].end(); ++i) {
//         std::cout << *i << ' ';
//       }
//       std::cout << '\n';
//     } else if (instr == "quit") {
//       return;
//     } else {
//       std::cout << "man,what can I say!\n";
//     }
//     // a.DepTraverse();
//   }
// }

// #include <map>
// #include <set>
// #include <random>
// void test4() {
//   std::map<int, std::set<int>> b;
//   string instr, index;
//   int value;
//   std::fstream file;
//   file.open("BPToutput", std::ios::out);
//   file.close();
//   BPT a;
//   // 使用随机设备作为种子
//   std::random_device rd;
//   // 使用 Mersenne Twister 引擎
//   std::mt19937 gen(rd());
//   // 定义随机数分布，这里是整型范围为 [0, 10000] 的均匀分布
//   std::uniform_int_distribution<int> dis(0, 10000);
//   std::uniform_int_distribution<int> disindex(0, 3);

//   // 存储随机数的容器
//   std::vector<int> randomNumbers;

//   // 生成 10000 个随机数
//   for (int i = 0; i < 100000; ++i) {
//     randomNumbers.push_back(dis(gen));
//   }
//   string name = "1";
//   for (int i = 0; i < 5000; ++i) {
//     int num = dis(gen);
//     int n = disindex(gen);
//     if (i % 2) {

//       std::cerr << "1 " << n << " " << randomNumbers[num] << '\n';
//       a.Insert(std::to_string(n), randomNumbers[num]);
//       b[HASH(std::to_string(n))].insert(randomNumbers[num]);
//     } else {
//       std::cerr << "2 " << n << " " << randomNumbers[num] << '\n';
//       a.Delete(std::to_string(n), randomNumbers[num]);
//       b[HASH(std::to_string(n))].erase(randomNumbers[num]);
//     }
//   }
//   while (true) {
//     std::cin >> instr;
//     if (instr == "1") {
//       std::cin >> index >> value;
//       b[HASH(index)].insert(value);
//       a.Insert(index, value);
//     } else if (instr == "2") {
//       std::cin >> index >> value;
//       b[HASH(index)].erase(value);
//       a.Delete(index, value);
//     } else if (instr == "3") {
//       std::cin >> index;
//       a.Find(index);
//       for (auto i = b[HASH(index)].begin(); i != b[HASH(index)].end(); ++i) {
//         std::cout << *i << ' ';
//       }
//       std::cout << '\n';
//     } else if (instr == "quit") {
//       return;
//     } else {
//       std::cout << "man,what can I say!\n";
//     }
//     // a.DepTraverse();
//   }
// }
#endif