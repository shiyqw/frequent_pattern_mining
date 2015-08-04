//文件名：data.cpp

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


#include"data.h"
#include"fptree.h"
#include<ctime>

Data::Data(string s , int f):set_num(0) , item_num(0) , freq(f){
	int pos = s.find('.');
	sub = s.substr(0 , pos);
	is = s;
	os = sub + "_o.dat";
//	cout << sub << endl;
//	cout << is << endl;
//	cout << ds << endl;
//	cout << os << endl;
}

Data::~Data() {
//	clear_data();
}


void Data::output(const multimap<int , vector<int> > & result , ofstream & outfile){

	// output results
	multimap<int , vector<int> >::const_iterator it;
	map<vector<int> , int , mycmp>::iterator dit;

	for(it = result.begin() ; it != result.end() ; ++it) {
		vector<int> ordv;
		for(int i = 0 ; i < (it->second).size() ; ++i) {
			ordv.push_back(ord_to_item[(it->second)[i]]);
		}
		sort(ordv.begin() , ordv.end());
//		debugmap.insert(make_pair(ordv , it->first));
		for(int i = 0 ; i < ordv.size() ; ++i) 
		  outfile << ordv[i] << ' ';
		outfile << ':' << ' ' << it->first << endl;
	}
}

void Data::d_output() {
	
	// sort all results and output together
	map<vector<int> , int , mycmp>::iterator dit;
	ofstream outfile(os.c_str() , ios::out | ios::app);
	for(dit = debugmap.begin() ; dit != debugmap.end() ; ++dit) {
		for(int i = 0 ; i < dit->first.size() ; ++i) {
			outfile << (dit->first)[i] << ' ';
		}
		outfile << ':' << ' ' << dit->second << endl;
	}
	outfile.close();
}

void Data::first_scan() {
	infile.open(is.c_str());
	string line;

	// stage 1 : get item numbers and each support number
	while(getline(infile , line)) {
		istringstream instring(line);
		int x;
		while(instring >> x) {
			if(item_count.count(x) == 0) item_count[x] = 1;
			else ++item_count[x];
		}
		if(x > item_num) item_num = x;
		++set_num;
	}
	double d_freq = double(freq*set_num)/double(100);
	if(double(int(d_freq)) < (d_freq)) freq = int(d_freq)+1;
	else freq = int(d_freq);

	// stage 2 : sort the items b its support number
	map<int , int>::iterator it = item_count.begin();
	for(; it != item_count.end() ; ++it) {
		item_list.push_back(make_pair(it->second , it->first));
	}
	sort(item_list.begin() , item_list.end() , greater<pair<int , int> >() );
	for(int i = 0 ; i < item_list.size() ; ++i) {
		if(item_list[i].first < freq) {
			item_list.erase(item_list.begin()+i , item_list.end());
			break;
		}
	}

	// stage 3 : generate ord to item table and item to ord table
	item_to_ord.clear();
	item_to_ord.resize(item_num+1 , -1);
	ord_to_item.clear();
	valid_item_num = item_list.size();
	ord_to_item.resize(valid_item_num);
	ord_to_item.resize(item_list.size());
	for(int i = 0 ; i < item_list.size() ; ++i) {
		item_to_ord[item_list[i].second] = i;
		ord_to_item[i] = item_list[i].second;
	}
	
	// stage 4 : generating detached database (initialize but not input data)
	for(int i = 0 ; i < valid_item_num ; ++i) {
		char c[5];
		sprintf(c , "%d" , i);
		string outdata = sub+"_d"+c+".dat";
		ofstream out_data(outdata.c_str());
		out_data.close();
	}
	ofstream outfile(os.c_str());
	outfile.close();
	infile.close();
}


void Data::second_scan() {

	// scan the origin database again
	infile.open(is.c_str());
	string line;
	while(getline(infile , line)) {
		istringstream instring(line);
		vector<int> v;
		int x;
		while(instring >> x) {
			if(item_to_ord[x] != -1) v.push_back(item_to_ord[x]);
		}

		// input the list into its related database
		sort(v.begin() , v.end());
		if(!v.empty()) {
			int head = v[v.size()-1];
			store_data(head , v);
		}
	}
	infile.close();
}


void Data::store_data(int head , vector<int> v) {
	
	// store data into the "head"'s database
	char c[5];
	sprintf(c , "%d" , head);
	string outdata = sub+"_d"+c+".dat";
	ofstream out_data(outdata.c_str() , ios::out | ios::app);
	out_data << -1 << ' ';
	for(int i = 0 ; i < v.size()-1 ; ++i) {
		out_data << v[i] << ' ';
	}
	out_data << endl;
	out_data.close();
}

void Data::fp_mine() {
	ofstream outfile(os.c_str() , ios::out | ios::app);
//	cout << valid_item_num << endl;
	for(int i = valid_item_num-1 ; i >= 0 ; --i) {

		// scan one detached databse
		char c[5];
		sprintf(c , "%d" , i);
		string filename = sub+"_d"+c+".dat";
		
		// build FP Tree
		fp_Tree T(filename , freq , i);
		// mine it
		multimap<int , vector<int> > & result = T.fp_mine();

		// output and update database
//		output(result , outfile);
		update_data(i);
	}
	outfile.close();
}

void Data::update_data(int x) {


	// open every database
	vector<int> v;
	ofstream of[x];
	for(int i = 0 ; i < x ; ++i) {
		char c[5];
		sprintf(c , "%d" , i);
		string outfilename = sub+"_d"+c+".dat";
		(of[i]).open(outfilename.c_str() , ios::out | ios::app);
	}
	char c[5];
	sprintf(c , "%d" , x);
	string filename = sub+"_d"+c+".dat";
	ifstream infile(filename.c_str() , ios::in);
	string line;

	// get each list and update it into another database
	while(getline(infile , line)) {
		istringstream instring(line);
		vector<int> v;
		int x;
		while(instring >> x) v.push_back(x);
		if(v.size() == 1) continue;
		sort(v.begin() , v.end());
		int head = v[v.size()-1];
		for(int i = 0 ; i < v.size()-1 ; ++i) {
			(of[head]) << v[i] << ' ';
		}
		of[head] << endl;
	}
	for(int i = 0 ; i < x ; ++i) of[i].close();
	infile.close();

	// remove this database
	string command = "rm "+filename;
	system(command.c_str());
}
