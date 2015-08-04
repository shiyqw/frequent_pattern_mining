// ����ϵر�֤��
// ���Լ��������������������ӷ�������Ƶ���������й����� 
// �ڴˣ��Ҹ�лʷ���Ω���ҵ������Ͱ���������ı����У��һ��������ᵽ
// �����ڸ����������ҵİ�����
// �ҵĳ������з������õ�����������ĵ�֮����
// ����̲ġ����ñʼǡ����ϵ�Դ�����Լ������ο����ϵĴ����,
// �Ҷ��Ѿ��ڳ����ע����������ע�������õĳ����� 
// �Ҵ�δ��Ϯ�����˵ĳ���Ҳû�е��ñ��˵ĳ���
// �������޸�ʽ�ĳ�Ϯ����ԭ�ⲻ���ĳ�Ϯ��
// �ұ�д������򣬴���û�����Ҫȥ�ƻ���������������ϵͳ��������ת��
// С���Ա��ʷ���Ω ��˫ 
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
vector<vector<int> > originalData;												// ԭʼ����תΪ���� 
vector<multiset<int> > originalSet;												// ԭʼ���ݼ� 
vector<vector<int> > candidateData;												// ��ѡ� 
vector<vector<int> > frequentData;												// Ƶ���
map<vector<int>, int> data_num;													// ͳ��֧�ֶȼ���
int orcount;																	// ��¼ԭʼ�������� 
int freq_count;																	// ��¼Ƶ��ģʽ���� 


bool Merge(vector<int> p, vector<int> q, int level);							// �ж�������Ƿ���������� 
bool freq(vector<int> p, int level);											// �ж��p���Ӽ��Ƿ�ȫ��Ƶ�����������p�����ѡ�� 
bool isFreq(vector<int> p);														// �жϺ�ѡ��p�ǲ���Ƶ���� 
bool isExist(vector<int> p);													// �ж�һ��Ƶ�����Ƿ��Ѿ����� 
int main(int argc, char *argv[])
{
	string s;
	vector<int> temp;															// ������������
	multiset<int> tset;
	fstream fin(argv[1]);
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
	// ����Ƶ��1�������вŻ��к������� 
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
	// ��1�������ɶ����ѡ�������м�֦�����ҳ�Ƶ���
	int level=1;																// levelΪ���
	vector<int> mergeItem;
	int t1=clock();
	while(!frequentData.empty())
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
				mergeItem.clear();
//				if(level == 1)
//				{
//					mergeItem.push_back(frequentData[i][0]);
//					mergeItem.push_back(frequentData[j][0]);
//				}
			    if(Merge(frequentData[i], frequentData[j], level))				// Merge�ж��˿ɷ������ӣ� ���������ӵ�ֱ��ȥ��ʵ�ּ�֦ 
				{
					for(int k=0; k<level-1; k++)
						mergeItem.push_back(frequentData[i][k]);
					int t1=frequentData[i][level-1];
					int t2=frequentData[j][level-1];								// ������k������Ǳ���ֻ�����һ�ͬ���ܺϲ�
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
		//��ʼ�Ӻ�ѡ���в���Ƶ��level+1�
//		int t1=time(NULL);
		frequentData.clear();													// �����һ�ֵ�Ƶ���
		l=candidateData.size();
		data_num.clear();														// �����һ�ֶ�Ƶ������֧�ֶȼ��� 
		for(int i=0; i != l; i++)
		{
			if(isFreq(candidateData[i]) && (!isExist(candidateData[i])))		// ���p��Ƶ���������������� 
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
bool Merge(vector<int> p, vector<int> q, int level)								// �ж�������Ƿ���������� 
{
	if(level == 1)
		return true;
	int i;
	for(i=level-2; i >= 0; i--)
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
		temp.erase(temp.begin()+i); 											// ÿ��ɾ��һ��Ԫ�أ���level���Ӽ�
		int left=0;
		int right=l-1;
		int middle=(left+right)/2;
		int ll=temp.size();
		flag=true;
		while(left<=right)													    // ����Ƶ�������Ⱦ���������������ö��ֲ��ҵķ�ʽ
																				// �����Ӽ�����ԭ���ز��ҷ�����ٶ��кܴ����� 
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
bool isFreq(vector<int> p)														// �ж��p�ǲ���Ƶ���� 
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
bool isExist(vector<int> p)								// �ж�ĳƵ�����Ƿ��Ѿ����� 
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
