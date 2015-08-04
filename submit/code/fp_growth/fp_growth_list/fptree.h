//文件名：fptree.cpp

//作者：史杨勍惟 （学号：1200012741）

// 我真诚地保证：
// 我自己独立地完成了整个程序从分析、设计到编码的所有工作。
// 如果在上述过程中，我遇到了什么困难而求教于人，那么，我将在程序实习报告中
// 详细地列举我所遇到的问题，以及别人给我的提示。
// 在此，我感谢我的队友对我的启发和帮助。下面的报告中，我还会具体地提到
// 他们在各个方法对我的帮助。
// 我的程序里中凡是引用到其他程序或文档之处，
// 例如教材、课堂笔记、网上的源代码以及其他参考书上的代码段,
// 我都已经在程序的注释里很清楚地注明了引用的出处。
// 我从未抄袭过别人的程序，也没有盗用别人的程序，
// 不管是修改式的抄袭还是原封不动的抄袭。
// 我编写这个程序，从来没有想过要去破坏或妨碍其他计算机系统的正常运转。

#pragma once

#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<list>
#include<string>
#include<stack>
#include<utility>
#include<map>

using namespace std;

struct fp_Node {
	int ahead; // a pointer to its father
	int next;	// a pointer to the next node in ther header table
	int count;	// support counts of this node
	int ord;	// order value
	fp_Node():ahead(-1) , next(-1) , count(0) , ord(-1) {}
};

struct ST {

	// a sub data structure in a FP Tree(more details in report)

	list<int> base_count;
	list<int> branch;
	vector<int> count;
};

class fp_Tree {

private:
	int item_ord; // the owner of the database
	vector<fp_Node> tree; // tree
	vector<int> header; // header table
	vector<int> tailer; // tailer table , used to find the last node in the header table efficiently
	vector<int> item_count; // item counts
	ST stree; // a subtree for each pattern
	stack<int> item_stack; // a stack maintaining the pattern
	int freq;
	multimap<int , vector<int> > result;

	void list_insert(const vector<int> & ); // insert a list
	int item_insert(int , int); // insert an item
	void adjust(); // reverse the pointers
	void adjust(int , int);
	void st_update(int); // updating ST
	void st_build(int); // build ST
	bool exist_node(); // judging if there is remaining node for mining
	void display(); // for debug
	void output(int); // outputting results
	void mine();
	
public:
	fp_Tree(string , int , int );

	// the key interface
	multimap<int , vector<int> > &fp_mine();

};

