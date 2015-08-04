// 我真诚地保证：
// 我自己独立地完成了整个程序从分析、设计到编码的所有工作。 
// 在此，我感谢史杨勍惟对我的启发和帮助。下面的报告中，我还会具体地提到
// 他们在各个方法对我的帮助。
// 我的程序里中凡是引用到其他程序或文档之处，
// 例如教材、课堂笔记、网上的源代码以及其他参考书上的代码段,
// 我都已经在程序的注释里很清楚地注明了引用的出处。 
// 我从未抄袭过别人的程序，也没有盗用别人的程序，
// 不管是修改式的抄袭还是原封不动的抄袭。
// 我编写这个程序，从来没有想过要去破坏或妨碍其他计算机系统的正常运转。
// 小组成员：史杨勍惟 姜双 
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
#include <time.h>
using namespace std;
double minsup;
// use vector<vector<int> > to replace a[][]
vector<vector<int> > originalData;												// 原始数据转为事务集 
vector<multiset<int> > originalSet;												// 原始数据集 
vector<vector<int> > candidateData;												// 候选项集 
vector<vector<int> > frequentData;												// 频繁项集
map<vector<int>, int> data_num;													// 统计支持度计数
int orcount;																	// 记录原始事务总数 
int freq_count;																	// 记录频繁模式总数 


bool Merge(vector<int> p, vector<int> q, int level);							// 判断两个项集是否可以自连接 
bool freq(vector<int> p, int level);											// 判断项集p的子集是否全部频繁，如果是则p加入候选集 
bool isFreq(vector<int> p);														// 判断候选集p是不是频繁集 
bool isExist(vector<int> p);													// 判断一个频繁集是否已经存在 
int main(int argc, char *argv[])
{
	string s;
	vector<int> temp;															// 处理读入的数据
	multiset<int> tset;
	fstream fin(argv[1]);
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
				sum=0;
			}
			else if(s[i] == '\n')
				break;
			else
			{
				sum=sum*10+(s[i]-'0');
			}
		}
		originalData.push_back(temp);
	}
//	cout << "total cases:" << orcount << endl;
	int length=originalData.size();
	for(int i=0; i<length; i++)
	{
		sort(originalData[i].begin(), originalData[i].end());
		multiset<int>tset;
		tset.clear();
		int ll=originalData[i].size();
		for(int j=0; j<ll; j++)
			tset.insert(originalData[i][j]);
		originalSet.push_back(tset);
	}
	printf("Please input the minsup\n");
	cin >> minsup;
	freopen("result.txt", "w", stdout);
//	cin.get();
	// 先求频繁1项集，如果有才会有后续运算 
	map<int, int> count_freq;
	count_freq.clear();
	freq_count=0;
	for(int i=0; i != length; i++)
	{
		int lll=originalData[i].size();
		for(int j=0; j != lll; j++)
			count_freq[originalData[i][j]]++;
	}
	map<int, int>::iterator ii;
//	printf("mapsize=%d\n",count_freq.size());
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
//		printf("No 1-level frequency set! NO frequency rules!\n");
		fin.close(); fclose(stdout);
		return 0;
	}
	printf("The min_sup is %lf\n", minsup);
//	printf("The 1-level frequency set:\n");
	int ll=frequentData.size(); 
	for(int i=0; i != ll; i++)
	{
		int lll=frequentData[i].size();
//		printf("{ ");
		for(int j=0; j != lll; j++)
		{
//			printf("%d } support number=%d\n", frequentData[i][j], count_freq[frequentData[i][j]]);
			printf("%d :%d\n", frequentData[i][j], count_freq[frequentData[i][j]]);
			freq_count++;
		}
			
	}
//	printf("number of cases: %d\n", ll);
//	printf("Please Enter to continue the program\n");
//	getchar();
	// 由1项集逐个生成多项候选集并进行剪枝，再找出频繁项集
	int level=1;																// level为层次
	vector<int> mergeItem;
	int t1=clock();
	while(!frequentData.empty())
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
				mergeItem.clear();
//				if(level == 1)
//				{
//					mergeItem.push_back(frequentData[i][0]);
//					mergeItem.push_back(frequentData[j][0]);
//				}
			    if(Merge(frequentData[i], frequentData[j], level))				// Merge判断了可否自连接， 不能自连接的直接去掉实现剪枝 
				{
					for(int k=0; k<level-1; k++)
						mergeItem.push_back(frequentData[i][k]);
					int t1=frequentData[i][level-1];
					int t2=frequentData[j][level-1];								// 对两个k项集，它们必须只有最后一项不同才能合并
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
//				vector<int>::iterator a=mergeItem.begin();
//				for(;a != mergeItem.end(); a++)
//					cout << *a << " ";
//				cout << endl;
//				sort(mergeItem.begin(), mergeItem.end());
				if(!mergeItem.empty() && freq(mergeItem, level))
					candidateData.push_back(mergeItem);
			}
		}
//		printf("cansize=%d\n", candidateData.size());
		//开始从候选集中产生频繁level+1项集
//		int t1=time(NULL);
		frequentData.clear();													// 清空上一轮的频繁项集
		l=candidateData.size();
		data_num.clear();														// 清空上一轮对频繁集的支持度计数 
		for(int i=0; i != l; i++)
		{
			if(isFreq(candidateData[i]) && (!isExist(candidateData[i])))		// 如果p是频繁项集，则放入数组中 
				frequentData.push_back(candidateData[i]);
		}
		if(frequentData.empty())
		{
//			printf("No level-%d frequency set! THE END!\n", level+1);
			printf("The sum of all frequency rules is %d\n", freq_count);
			int t2=clock();
			printf("time: %lfs\n", ((double)t2-(double)t1)/1000);
			exit(0);
		}
//		printf("level-%d frequency set:\n", level+1);
		l=frequentData.size();
//		cout << "fresize=" << l << endl;
		for(int i=0; i != l; i++)
		{
			vector<int>::iterator ii=frequentData[i].begin();
//			printf("{ ");
			for(; ii != frequentData[i].end(); ii++)
				printf("%d ", *ii);
//			printf("} support number=%d\n", data_num[frequentData[i]]);
			printf(":%d\n", data_num[frequentData[i]]);
			freq_count ++;
		}
//		int t2=time(NULL);
//		printf("num of cases: %d\n", l);
		level++;
//		printf ("Please Enter to continue the program.\n");
//		getchar();
	}
	fin.close();
	fclose(stdout);
	return 0;
}
bool Merge(vector<int> p, vector<int> q, int level)								// 判断两个项集是否可以自连接 
{
	if(level == 1)
		return true;
	int i;
	for(i=level-2; i >= 0; i--)
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
		temp.erase(temp.begin()+i); 											// 每次删除一个元素，共level个子集
		int left=0;
		int right=l-1;
		int middle=(left+right)/2;
		int ll=temp.size();
		flag=true;
		while(left<=right)													    // 由于频繁集事先经过排序处理，这里采用二分查找的方式
																				// 搜索子集，和原朴素查找法相比速度有很大的提高 
		{
			int k;
			for(k=0; k != ll; k++)
			{
				if(temp[k]<frequentData[middle][k])
				{
					flag=false;
					right=middle-1;
					middle=(left+right)/2;
					break;
				}
				else if(temp[k]>frequentData[middle][k])
				{
					flag=false;
					left=middle+1;
					middle=(left+right)/2;
					break;
				}
			}
			if(k == ll)
			{
				flag=true;
				break;
			}
		}
		if(flag == false)
			return false;
	}
	return true;
} 
bool isFreq(vector<int> p)														// 判断项集p是不是频繁集 
{
	bool flag=false;										
	int l=p.size();
	int len=originalSet.size();
	int count=0;
	for(int j=0; j != len; j++)
	{
		int k;
		for(k=0; k != l; k++)
		{
			multiset<int>::iterator ii;
			ii=originalSet[j].find(p[k]);
			if(ii == originalSet[j].end())
				break;
		}
		if(k == l)
			count++;
	}
	if((double)count/(double)orcount >= minsup)
	{
		data_num[p]=count;
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
		int l=frequentData[i].size();
		for(int j=0; j != l; j++)
		{
			if(p[j] != frequentData[i][j])
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
