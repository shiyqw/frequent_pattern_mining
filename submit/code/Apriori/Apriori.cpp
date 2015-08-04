#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h> 
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
using namespace std;
double minsup;
// use vector<vector<int> > to replace a[][]
vector<vector<int> > originalData;												// 原始数据转为事务集 
vector<multiset<int> > originalSet;												// 原始数据集 
vector<vector<int> > candidateData;												// 候选项集 
vector<vector<int> > frequentData;												// 频繁项集
map<string, int> data_num;														// 统计支持度计数
long long orcount;																// 记录原始事务总数 
long long freq_count;

bool Merge(vector<int> p, vector<int> q, int level);							// 判断两个项集是否可以自连接 
bool freq(vector<int> p, int level);											// 判断项集p的子集是否全部频繁，如果是则p加入候选集 
bool isFreq(vector<int> p);														// 判断候选集p是不是频繁集 
bool isExist(vector<int> p);													// 判断一个频繁集是否已经存在 
int main()
{
	string s;
	vector<int> temp;															// 处理读入的数据
	multiset<int> tset;
	fstream fin("F:\\CS\\datas\\mushroom.txt"); 
	ofstream fout("F:\\CS\\datas\\result.txt");
	if(!fin)
	{
		printf("File Open Error!\n");
		exit(1);
	}
	orcount=0; 
	while(getline(fin, s))														// 按行读入
	{
		orcount++;
		temp.clear();
		tset.clear();
		s.erase(0,s.find_first_not_of("\r\t\n "));   							//去除字符串首部的空格
        s.erase(s.find_last_not_of("\r\t\n")+1);        						//去除字符串尾部的空格
		int sum=0;
		int l=s.size();
		for(int i=0; i<l; i++)
		{
			if(s[i] == ' ')
			{
				temp.push_back(sum);
				tset.insert(sum);
				sum=0;
			}
			else
				sum=sum*10+(s[i]-'0');
		}
		originalData.push_back(temp);
		originalSet.push_back(tset);
	}
//	cout << "total cases:" << orcount << endl;
	for(int i=0; i<originalData.size(); i++)
		sort(originalData[i].begin(), originalData[i].end());
	printf("Please input the minsup\n");
	cin >> minsup;
	cin.get();
	// 先求频繁1项集，如果有才会有后续运算 
	map<int, int> count_freq;
	freq_count=0;
	for(int i=0; i != originalData.size(); i++)
	{
		for(int j=0; j != originalData[i].size(); j++)
			count_freq[originalData[i].at(j)]++;
	}
	map<int, int>::iterator ii;
	for(ii=count_freq.begin(); ii != count_freq.end(); ii++)
	{
		if(((double)(ii -> second)/(double)orcount >= minsup))
		{
			temp.clear();
			temp.push_back(ii -> first);
			frequentData.push_back(temp);
		}
	}
	if(frequentData.empty())
	{
		printf("No 1-level frequency set! THE END!\n");
		fin.close(); fout.close();
		return 0;
	}
	printf("The min_sup is %lf\n", minsup);
	printf("The 1-level frequency set:\n");
	int ll=frequentData.size(); 
	for(int i=0; i != ll; i++)
	{
		printf("{ ");
		for(int j=0; j != frequentData[i].size(); j++)
		{
			printf("%d } support number=%d\n", frequentData[i].at(j), count_freq[frequentData[i].at(j)]);
			freq_count ++;
		}
	}
	printf("number of cases: %d\n", ll);
	printf("Please Enter to continue the program\n");
	getchar();
	// 由1项集逐个生成多项候选集并进行剪枝，再找出频繁项集
	int level=1;																// level为层次
//	int find;																	// 判断是否包含了非频繁项的子集，如果包含需要进行剪枝 
	vector<int> mergeItem;
	do
	{
		//生成level+1项候选集 
		candidateData.clear();													// 清空上一轮的候选集
		int l=frequentData.size();
//		for(int i=0; i != l; i++)
//			sort(frequentData[i].begin(), frequentData[i].end());				// 为避免生成全排列进行判断，先对每个项集的元素进行排序 
		for(int i=0; i != l; i++)
		{
			for(int j=i+1; j != l; j++)
			{
				if(Merge(frequentData[i], frequentData[j], level))				// Merge判断了可否自连接， 不能自连接的直接去掉实现剪枝 
				{
					mergeItem.clear();
					int count=0;
					for(int k=0; count != level-1; k++)
					{
						count++;
						mergeItem.push_back(frequentData[i].at(k));
					}
					int t1=frequentData[i].at(level-1);
					int t2=frequentData[j].at(level-1);							// 对两个k项集，它们必须只有最后一项不同才能合并
					if(t1<t2)
					{
						mergeItem.push_back(t1);
						mergeItem.push_back(t2);
					}
					else
					{
						mergeItem.push_back(t2);
						mergeItem.push_back(t1);
					}
				}
				if(mergeItem.empty())
					continue;
//				vector<int>::iterator a=mergeItem.begin();
//				for(;a != mergeItem.end(); a++)
//					cout << *a << " ";
				cout << endl;
				if(freq(mergeItem, level))
					candidateData.push_back(mergeItem);
			}
		}
		//开始从候选集中产生频繁level+1项集
//		int t1=time(NULL);
		frequentData.clear();													// 清空上一轮的频繁项集
		l=candidateData.size();
		data_num.clear();														// 清空上一轮对频繁集的支持度计数 
		for(int i=0; i != l; i++)
		{
			vector<int> p=candidateData[i];
			if(isFreq(p) && !isExist(p))										// 如果p是频繁项集，则放入数组中 
				frequentData.push_back(p);
		}
		if(frequentData.empty())
		{
			printf("No level-%d frequency set! THE END!\n", level+1);
			printf("The sum of all frequency sets is %d\n", freq_count);
			exit(2);
		}
		printf("level-%d frequency set:\n", level+1);
		l=frequentData.size();
		for(int i=0; i != l; i++)
		{
			vector<int> tt=frequentData[i];
			vector<int>::iterator ii;
			char a[20];
			memset(a, 0, sizeof(a));
			printf("{ ");
			s.clear();
			for(ii=tt.begin(); ii != tt.end(); ii++)
			{
				itoa(*ii, a, 10);
				cout << *ii;
				printf(" ");
				s += a;
			}
			printf("} support number=%d\n", data_num[s]);
			freq_count ++;
		}
//		int t2=time(NULL);
		printf("num of cases: %d\n", l);
		printf ("Please Enter to continue the program.\n");
		level++;
		getchar();
	}while(!frequentData.empty());
	fin.close();
	fout.close();
	return 0;
}
bool Merge(vector<int> p, vector<int> q, int level)								// 判断两个项集是否可以自连接 
{
	if(level == 1)
		return true;
	int i;
	for(i=0; i<level-1; i++)
	{
		if(p[i] != q[i])
			return false;
	}																			// 经排序预处理，如果可以自连接，那必须只有两个项集的最后一项不同 
	return true;
}
bool freq(vector<int> p, int level)												// 判断level项集的所有子集是否都是level-1项频繁集
{
	if(level == 1)
		return true;
	vector<int> temp;
	int i;
	int l=frequentData.size();
	bool flag;
	for(i=0; i<level; i++)
	{
		temp=p;
		temp.erase(temp.begin()+i);												// 每次删除一个元素，共level个子集
		for(int j=0; j != l; j++)
		{
			flag=true;
			int len=frequentData[j].size();
			if(len != level)
				return false;
			for(int k=0; k != len; k++)
			{
				if(temp[k] != frequentData[j].at(k))
				{
					flag=false;
					break;
				}
			}
			if(flag)
				break;
			continue;
		}
		if(flag)
			continue;
		else
			return false;
	}
	return true;
} 
bool isFreq(vector<int> p)														// 判断项集p是不是频繁集 
{
	string s;
	bool flag=false;
	char a[20];
	vector<int>::iterator i=p.begin();
	for(; i != p.end(); i++)
	{
		memset(a, 0, sizeof(a));
		itoa(*i, a, 10);
		s += a;
	}																			// 转换为字符串方便计数 
	int l=p.size();
	int len=originalSet.size();
	int count=0;
	for(int j=0; j != len; j++)
	{
		int k;
		multiset<int> temp=originalSet[j];
		for(k=0; k != l; k++)
		{
			multiset<int>::iterator ii;
			ii=temp.find(p[k]);
			if(ii == temp.end())
				break;
		}
		if(k == l)
			count++;
	}
	if((double)count/(double)orcount >= minsup)
	{
		data_num[s]=count;
		return true;
	}
	return false; 
}
bool isExist(vector<int> p)								// 判断某频繁集是否已经存在 
{
	int len=frequentData.size();
	bool flag;
	for(int i=0; i != len; i++)
	{
		flag=true;
		vector<int> temp=frequentData[i];
		int l=temp.size();
		for(int j=0; j != l; j++)
		{
			if(p[j] != temp[j])
			{
				flag=false;
				break;
			}
		}
		if(flag)
			return true;
	}
	return false; 
}
