//文件名：main.cpp

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


#include<iostream>
#include"data.h"
#include"fptree.h"
#include<string>
#include<ctime>
//#include"fp.h"
//#include"fptree.h"

using namespace std;

int main(int argc , char * argv[]) {

	string s = argv[1];
	cout << "Input Threhold: ";
	int f;
	cin >> f;
	
	//Initialize data
	Data d(s , f);
	int tm = clock();


	//scan data twice
	
	d.first_scan();
	d.second_scan();

	//build fptree and mine
	d.fp_mine();
//	d.d_output();

	cout << "Done!" << endl;

	//Calculating executing time
	int etm = clock();
	cout << "using time: " << double(etm-tm)/double(1000000) << endl;


	//Calculating executing memory
	string checkvm = "pgrep \"fp\" > temp.txt";
	system(checkvm.c_str());
	ifstream infile("temp.txt");
	string pnum;
	infile >> pnum;
	cout << pnum << endl;
	checkvm = "cat /proc/"+pnum+"/status | grep \"VmPeak\"";
	system(checkvm.c_str());
	infile.close();
	system("rm temp.txt"); 
	return 0;
}
