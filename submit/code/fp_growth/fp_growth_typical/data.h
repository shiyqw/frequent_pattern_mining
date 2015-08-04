//文件名：data.h

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
#include<fstream>
#include<cstdlib>
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include<utility>
#include<algorithm>
#include<set>
#include<functional>

using namespace std;

// A compare function , used to sorting results

struct mycmp{
	bool operator () (vector<int> a , vector<int> b) {
		if(a.size() != b.size()) return (a.size() < b.size());
		return (a<b);
	}
};

// A data structure used to collect imformations the data and controlling input and output

class Data{
private:
	string sub; //a substring of filename
	string is;	//input file name
	string os;	//output file name

	//filestreams
	ifstream infile;
	ofstream outfile;

	int freq; // frequency
	int item_num;	//item numbers
	int valid_item_num;	//the number of the items whose support number is valid
	int set_num;	// number of sets
	vector<int> item_to_ord;	// transform item number to order(increase with support numbers)
	vector<int> ord_to_item; // transform order to item numbers
	vector<pair<int,int> > item_list; // item number and its support numer using vector
	map<int , int> item_count; // item number and its support numberusing map

	map<vector<int> , int , mycmp> debugmap; 

public:
	Data(string , int);
	~Data();

	// output result to files
	void output(const multimap<int , vector<int> > & , ofstream &);

	// first scan input file , get the number of the items, sets, and sort the items by their supports.
	void first_scan();
	// second scan input file , delete the items whose support is too low, change item numbers to orders and divide the database.
	void second_scan();

	// store a list into a file
	void store_data(int , vector<int>);

	// build FP Tree and mine
	void fp_mine();

	// after dealing with one database, delete this one and update others.
	void update_data(int);

	// output sorted 
	void d_output();
};


