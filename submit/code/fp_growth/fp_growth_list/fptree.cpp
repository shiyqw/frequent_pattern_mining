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


#include"fptree.h"
#include<ctime>

fp_Tree::fp_Tree(string filename , int f , int i) : 
	freq(f) , item_ord(i) , item_count(i , 0) {
	ifstream infile(filename.c_str() , ios::in);
	string line;
	fp_Node root;
	tree.push_back(root);

	// insert every list
	int this_count = 0;
	while(getline(infile , line)) {
		istringstream instring(line);
		vector<int> v;
		int x;
		while(instring >> x) v.push_back(x);
		list_insert(v);
		++this_count;
	}

	// insert the owner single item
	result.insert(make_pair(this_count , vector<int>(1 , item_ord)));
	// reverse the pointers
	adjust();

	// get item_count
	for(int i = 0 ; i < item_ord ; ++i) {
		int now = header[i];
		while(now != -1) {
			item_count[i] += tree[now].count;
			now = tree[now].next;
		}
	}
}

void fp_Tree::display() {

	// used fot debugging

	cout << "------------------------------------------" << endl;
	for(int i = 0 ; i < tree.size() ; ++i) {
		cout << tree[i].ord << ' ' << tree[i].ahead << ' '
			<< tree[i].next << ' ' << tree[i].count << endl;
	}
	cout << "------------------------------------------" << endl;
}

void fp_Tree::list_insert(const vector<int> & v) {
	int now = 0;
	for(int i = 1 ; i < v.size() ; ++i) {
		now = item_insert(v[i] , now);
	}
}

int fp_Tree::item_insert(int x , int now) {

	// three conditions
	if(tree[now].ahead == -1) {
	
		// condition 1 : root node has no child
		fp_Node new_node;
		new_node.count = 1;
		new_node.ord = x;
		tree.push_back(new_node);
		int new_now = tree.size()-1;
		tree[now].ahead = new_now;
		return new_now;

	} else {

		int child = tree[now].ahead , pre = -1;
		while(child != -1) {
			if(tree[child].ord == x) break;
			pre = child;
			child = tree[child].next;
		}
		if(child == -1) {
			// condition 2 : root node has childs but none has the same order with the item 
			fp_Node new_node;
			new_node.count = 1;
			new_node.ord = x;
			tree.push_back(new_node);
			int new_now = tree.size()-1;
			tree[pre].next = new_now;
			return new_now;

		} else {

			// condition 3 : find a child whose order are same with the inserting item
			++tree[child].count;
			return child;
		}
	}
}

void fp_Tree::adjust() {
	header.resize(item_ord , -1);
	tailer.resize(item_ord , -1);
	if(tree[0].ahead == -1) return;
	int now = tree[0].ahead;

	// reverse in recursion
	while(now != -1) {
		adjust(0 , now);
		now = tree[now].next;
	}
	for(int i = 0 ; i < item_ord ; ++i) {
		if(tailer[i] != -1) {
			tree[tailer[i]].next = -1;
		}
	}
}

void fp_Tree::adjust(int p , int q) {

	// adjusting its childs and then reverse the ahead pointer
	int now = tree[q].ahead;
	while(now != -1) {
		adjust(q , now);
		now = tree[now].next;
	}
	tree[q].ahead = p;
	int ord = tree[q].ord;

	// link the last item in the item order's list to this node
	if(header[ord] == -1) {
		header[ord] = tailer[ord] = q;
	} else {
		tree[tailer[ord]].next = q;
		tailer[ord] = q;
	}
}

multimap<int , vector<int> > & fp_Tree::fp_mine() {

	// mine each ST(i)
	for(int i = item_ord-1 ; i >= 0 ; --i) {
		if(item_count[i] < freq) continue;
		item_stack.push(i);
		output(item_count[i]);

		// build ST(i) and mine
		st_build(i);
		if(exist_node()) mine();
		item_stack.pop();
	}
	return result;
}

bool fp_Tree::exist_node() {
	list<int>::iterator it = stree.branch.begin();
	for(; it != stree.branch.end() ; ++it) {
		if((*it) != 0) return true;
	}
	return false;
}

void fp_Tree::st_build(int x) {

	// resizing data
	stree.branch.clear();
	stree.base_count.clear();
	stree.count.clear();
	stree.count.resize(item_ord);
	int now = header[x];

	// build new data
	while(now != -1) {
		stree.branch.push_back(tree[now].ahead);
		stree.base_count.push_back(tree[now].count);
		int t_now = now;
		while (tree[t_now].ahead != 0) {
			stree.count[tree[tree[t_now].ahead].ord] 
				+= tree[now].count;
			t_now = tree[t_now].ahead;	
		}
		now = tree[now].next;
	}
}

void fp_Tree::mine() {

	// storing original data
	int t = item_stack.top();
	list<int> t_branch = stree.branch;
	list<int> t_base_count = stree.base_count;
	vector<int> t_count = stree.count;

	// mine each items in the ST(i) by recursion
	for(int i = t-1 ; i >= 0 ; --i) {
		if(stree.count[i] >= freq) {
			item_stack.push(i);
			output(stree.count[i]);
			st_update(i);
			if(exist_node()) mine();
			item_stack.pop();

			// restoring
			stree.branch = t_branch;
			stree.base_count = t_base_count;	
			stree.count = t_count;
		}
	}
}

void fp_Tree::st_update(int x) {

	// updating data after adding a new item into ST
	list<int>::iterator pt = stree.branch.begin();
	list<int>::iterator qt = stree.base_count.begin();
	stree.count.clear();
	stree.count.resize(item_ord , 0);
	while(pt != stree.branch.end()) {
		int now = (*pt);
		int p = tree[now].ahead;
		while(now != 0 && tree[now].ord != x) now = tree[now].ahead;
		if(tree[now].ord == x) {
			int t_now = now;
			while (tree[t_now].ahead != 0) {
				stree.count[tree[tree[t_now].ahead].ord] 
					+= (*qt);
				t_now = tree[t_now].ahead;	
			}
			++pt;
			++qt;
		} else {
			pt = stree.branch.erase(pt);
			qt = stree.base_count.erase(qt);
		}
	}
}

void fp_Tree::output(int x){

	// outputting a valid frequent pattern
	stack<int> t_stack = item_stack;
	vector<int> output_items;
	multimap<int , vector<int> >::iterator mit = result.insert(make_pair(x , output_items));
	while(!t_stack.empty()) {
		(mit->second).push_back(t_stack.top());
		t_stack.pop();
	}
	(mit->second).push_back(item_ord);
}
