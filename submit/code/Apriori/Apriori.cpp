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
vector<vector<int> > originalData;												// ԭʼ����תΪ���� 
vector<multiset<int> > originalSet;												// ԭʼ���ݼ� 
vector<vector<int> > candidateData;												// ��ѡ� 
vector<vector<int> > frequentData;												// Ƶ���
map<string, int> data_num;														// ͳ��֧�ֶȼ���
long long orcount;																// ��¼ԭʼ�������� 
long long freq_count;

bool Merge(vector<int> p, vector<int> q, int level);							// �ж�������Ƿ���������� 
bool freq(vector<int> p, int level);											// �ж��p���Ӽ��Ƿ�ȫ��Ƶ�����������p�����ѡ�� 
bool isFreq(vector<int> p);														// �жϺ�ѡ��p�ǲ���Ƶ���� 
bool isExist(vector<int> p);													// �ж�һ��Ƶ�����Ƿ��Ѿ����� 
int main()
{
	string s;
	vector<int> temp;															// ������������
	multiset<int> tset;
	fstream fin("F:\\CS\\datas\\mushroom.txt"); 
	ofstream fout("F:\\CS\\datas\\result.txt");
	if(!fin)
	{
		printf("File Open Error!\n");
		exit(1);
	}
	orcount=0; 
	while(getline(fin, s))														// ���ж���
	{
		orcount++;
		temp.clear();
		tset.clear();
		s.erase(0,s.find_first_not_of("\r\t\n "));   							//ȥ���ַ����ײ��Ŀո�
        s.erase(s.find_last_not_of("\r\t\n")+1);        						//ȥ���ַ���β���Ŀո�
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
	// ����Ƶ��1�������вŻ��к������� 
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
	// ��1�������ɶ����ѡ�������м�֦�����ҳ�Ƶ���
	int level=1;																// levelΪ���
//	int find;																	// �ж��Ƿ�����˷�Ƶ������Ӽ������������Ҫ���м�֦ 
	vector<int> mergeItem;
	do
	{
		//����level+1���ѡ�� 
		candidateData.clear();													// �����һ�ֵĺ�ѡ��
		int l=frequentData.size();
//		for(int i=0; i != l; i++)
//			sort(frequentData[i].begin(), frequentData[i].end());				// Ϊ��������ȫ���н����жϣ��ȶ�ÿ�����Ԫ�ؽ������� 
		for(int i=0; i != l; i++)
		{
			for(int j=i+1; j != l; j++)
			{
				if(Merge(frequentData[i], frequentData[j], level))				// Merge�ж��˿ɷ������ӣ� ���������ӵ�ֱ��ȥ��ʵ�ּ�֦ 
				{
					mergeItem.clear();
					int count=0;
					for(int k=0; count != level-1; k++)
					{
						count++;
						mergeItem.push_back(frequentData[i].at(k));
					}
					int t1=frequentData[i].at(level-1);
					int t2=frequentData[j].at(level-1);							// ������k������Ǳ���ֻ�����һ�ͬ���ܺϲ�
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
		//��ʼ�Ӻ�ѡ���в���Ƶ��level+1�
//		int t1=time(NULL);
		frequentData.clear();													// �����һ�ֵ�Ƶ���
		l=candidateData.size();
		data_num.clear();														// �����һ�ֶ�Ƶ������֧�ֶȼ��� 
		for(int i=0; i != l; i++)
		{
			vector<int> p=candidateData[i];
			if(isFreq(p) && !isExist(p))										// ���p��Ƶ���������������� 
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
bool Merge(vector<int> p, vector<int> q, int level)								// �ж�������Ƿ���������� 
{
	if(level == 1)
		return true;
	int i;
	for(i=0; i<level-1; i++)
	{
		if(p[i] != q[i])
			return false;
	}																			// ������Ԥ����������������ӣ��Ǳ���ֻ������������һ�ͬ 
	return true;
}
bool freq(vector<int> p, int level)												// �ж�level��������Ӽ��Ƿ���level-1��Ƶ����
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
		temp.erase(temp.begin()+i);												// ÿ��ɾ��һ��Ԫ�أ���level���Ӽ�
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
bool isFreq(vector<int> p)														// �ж��p�ǲ���Ƶ���� 
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
	}																			// ת��Ϊ�ַ���������� 
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
bool isExist(vector<int> p)								// �ж�ĳƵ�����Ƿ��Ѿ����� 
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
