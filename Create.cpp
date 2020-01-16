#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include "User.cpp"

using namespace std;


class TABLE
{
public:
    int colength;           //���Ը���
    int remcount;			//Ԫ�����
    string tableName;	    //����
    vector<int> condition;  //0��ʾ��Լ������
    vector<string> colName; //������
    vector<string> colType; //��������

};

class VIEW
{
public:
    string viewName;   	  //��ͼ��
    vector<string> codes; //������ͼ�Ĵ���

};

class INDEX
{
public:
    vector<string> code;	//���������Ĵ���
    string indexName;		//������
};


vector<TABLE> table; //��TABLE�ඨ��һ��table���飬�����Ԫ�ؾ�Ϊ��
vector<VIEW> view;   //��VIEW�ඨ��һ��view���飬�����Ԫ�ؾ�Ϊ��ͼ
vector<INDEX> index; //��INDEX�ඨ��һ��index���飬�����Ԫ�ؾ�Ϊ����

void Create_table(string op)
{

    string tmp = "Create Table ";
    TABLE t;

    t.colength=0;     //��ʼ�����Ը���Ϊ0
    t.remcount=0;
    t.tableName = ""; //��ʼ������Ϊ��

    //��ȡ�½��ı������
    for(int k = tmp.length(); (op[k]!='\n' || op[k]!='(' || op[k]!=' ') && k < op.length(); k++)
        t.tableName += op[k];//�����ַ���¼

    //��ȡ�����ֶ������ֶ�����
    int i = 0;
    string s;

    /*������������������ı�־*/
    /*��create����һ�п�ʼ��*/
    ///����ط���һ�����⣬��û����ʼ�����ŵ�����£�Ҳ���Գɹ�����
    while(getline(cin, s) && s != ");")
    {
        int f = 1;

        tmp = "";
        t.colength++;

        while(!(s[i]>='a'&&s[i]<='z') && !(s[i]>='A'&&s[i]<='Z'))
            i++;				//��iŲ��ÿһ�е�һ�γ�����ĸ�ĵط�

        for( ; s[i] != ' ' && i < s.length(); i++)
        {
            tmp += s[i];
        }

        if(tmp.length() == 0)	//creat������Ϊ�գ�û����Ч�����Զ���
        {
            cout <<"create �﷨����!"<< endl;
            return;
        }

        tmp = "";

        while(!(s[i]>='a'&&s[i]<='z') && !(s[i]>='A'&&s[i]<='Z'))
            i++;

        for( ; s[i]!='(' && s[i]!=' '&& s[i]!=',' && i < s.length(); i++)
        {
            tmp += s[i];	//��ȡ��Ч���������Ͷ���
        }
        //�ж��ֶ������Ƿ���ȷ
        if((tmp != "char")&&(tmp != "int")&&(tmp != "float")&&(tmp != "double")&&(tmp != "time")&&(tmp != "date"))
        {
            cout <<"���ʹ���!"<< endl;
            return;
        }
        if(tmp.length() == 0)
        {
            cout <<"create �﷨����!"<< endl;
            return;
        }


        //Ϊ����ȡԼ�������������и����ַ���ȥ��
        for(i = 0; i < s.length(); i++)
            if(s[i] == ',')
                s[i] = ' ';
        if(s[0] == '(')
            s[0] = ' ';

        //�ж�Լ������
        stringstream ss(s);//������ĵ�һ��ת������
        string s1;
        vector<string> in;
        while(ss >> s1)
            in.push_back(s1);
        t.colName.push_back(in[0]); //�ֶ���
        t.colType.push_back(in[1]); //�ֶ�����
        string yueshu = "";
        for(i = 2; i < in.size(); i++)
        {
            yueshu += in[i];//�ڶ����ַ��������ȫ��ΪԼ������
            if(i != in.size()-1) yueshu += " ";
        }
        if(yueshu == "primary key")
            t.condition.push_back(1);
        else if(yueshu == "unique")
            t.condition.push_back(2);
        else if(yueshu == "not null")
            t.condition.push_back(3);
        else
            t.condition.push_back(0);
    }
    table.push_back(t);

    //�洢�������ݱ�,��table�������ʾ
    ofstream fout;
    fout.open("table.txt", ios::app);//��׷�ӵķ�ʽ���ļ�,��д,�ļ��������򴴽�,���ļ��Ѵ�������ԭ�ļ����ݺ�д���µ�����,ָ��λ���������
    fout <<"������"<<t.tableName<<"\t"<<"������"<<t.colength<<"������"<< endl;
    for(i = 0; i < t.colName.size(); i++)
    {
        fout<<"�ֶ�����"<<t.colName[i]<<"\t";

        fout<<" �ֶ����ͣ�"<< t.colType[i]<<"\t";
        if(t.condition[i]==1)
            fout<<"Լ������:primary key\t"<<endl;
        else if(t.condition[i]==2)
            fout<<"Լ������:unique\t"<<endl;
        else if(t.condition[i]==3)
            fout<<"Լ������:not null\t"<<endl;
        else
            fout<<endl;
    }
    fout<<endl;
    fout.close();


//    �洢���ݱ��е�����
    char file[30];
//    fstream file;
    string tt = t.tableName + ".txt";
    for(i = 0; i < tt.length(); i++)
        file[i] = tt[i];
    file[tt.length()]='\0';
    fout.open(file);		///����ط���Ҫ���ַ���ת�����ַ�����Ȼ����ܴ򿪣����ֱ�����ַ����������򲻿��ļ�
//    file.open(tt.c_str());
//    ofstream outfile("???.txt",ios::out|ios::trunc);
    //��c_str����ת���ܴﵽ���ļ���Ŀ�ģ����������������д�ļ�����ʱ����û��д���Ǿ�û����д������ֻ���ñ��취��

    for(i = 0; i < t.colName.size()-1; i++)
        fout <<t.colName[i]<<"\t\t";
    fout <<t.colName[t.colName.size()-1]<<"\t\t"<<endl;
    fout.close();

    cout <<"����ɹ���" << endl;
    return;
}

void Create_view(string s)
{
    string tmp;
    tmp="Create View ";
    int i=tmp.length();
    //�Ȱѳ�����������֮���Բ�ֱ�Ӷ���i=12��Ϊ�˷�����ڵĵ��ԣ�
    //����򵥵��Զ��彨��ͼ����ΪCre View����ô���Ⱦͻ�仯��������ȡ����ֻ��Ҫ�޸�tmp�Ϳ�����


    //��ȡview������
    tmp="";
    for(; i<s.length()&&s[i]!=' '; i++)
        tmp+=s[i];

    VIEW v;
    v.viewName=tmp;//����view������

    string ttt,t1;
    ttt="as ";

    //������view���������
    while(getline(cin,t1)&&t1!=";")
        v.codes.push_back(t1);

    v.codes.push_back(";");
    view.push_back(v); //���´�����viewѹ��vector

    //����Ӧ�ļ����в���
    //�洢�������
    ofstream fout;
    fout.open("view.txt",ios::app);//��׷�ӵķ�ʽ���ļ�,��д,�ļ��������򴴽�,���ļ��Ѵ�������ԭ�ļ����ݺ�д���µ�����,ָ��λ���������
    fout <<"��ͼ��:"<<v.viewName<<endl;
    fout<<"��ͼ������䣺"<<endl;
    fout<<"Create View "<<v.viewName<<endl;
    for(i=0; i<v.codes.size(); i++)
        fout<<v.codes[i]<<endl;

    char file[30];
    string tt=v.viewName+"_view.txt";
    for(i=0; i<tt.length(); ++i)
        file[i]=tt[i];
    file[tt.length()]='\0';
    fout.open(file);
    for(int i=0; i<v.codes.size(); ++i)
        fout<<v.codes[i]<<endl;
    fout.close();

    cout << "������ͼ�ɹ���" << endl;
    return;
}
void Create_index(string s)
{
    string tmp;
    tmp="Create Index ";
    int i=tmp.length();

    tmp="";
    for(; i<s.length()&&s[i]!=' '; i++)
        tmp+=s[i];

    INDEX ind;
    ind.indexName=tmp;

    //���ڶ���
    string t1;
    getline(cin,t1);

    tmp="on ";
    i=tmp.length();
    tmp = "";
    for(; i<t1.length()&&t1[i]!='('; i++)
    {
        tmp+=t1[i];//ȷ��table������
    }
    string ind_tab_name = "";
    ind_tab_name = tmp;

    i++;//����������
    tmp = "";
    for(; i < t1.length()&&t1[i]!=')'; i++)
    {
        tmp+=t1[i];//ȷ������
    }
    string key_name;
    key_name = tmp;

    tmp = "";
    getline(cin,t1);
    for(i = 0; i < t1.length()&&t1[i]!=';'; i++)
    {
        tmp+=t1[i];//ȷ������
    }

    ind_tab_name+=".txt";
    fstream file;
    file.open(ind_tab_name.c_str());
    string line;

    int countline = 0;
    while(!file.eof())
    {
        countline++;
        getline(file,line);
        if(countline > 1)
        {
            ind.code.push_back(line);
        }
    }

    vector <string> :: iterator it1,it2;
    for(it1 = ind.code.begin(); it1 != ind.code.end(); it1++)
    {
        for(it2 = ind.code.begin(); it2 != ind.code.end(); it2++)
        {
            if(*it1 > *it2)
            {
                string thtmp = "";
                thtmp = *it1;
                *it1 = *it2;
                *it2 = thtmp;
            }
        }
    }

    ofstream fout;
    fout.open("index.txt",ios::app);//��׷�ӵķ�ʽ���ļ�,��д,�ļ��������򴴽�,���ļ��Ѵ�������ԭ�ļ����ݺ�д���µ�����,ָ��λ���������
    fout << "��������"<<ind.indexName<<endl;
    fout<<"������"<<endl;
    for(int i=0; i<ind.code.size(); ++i)
        fout<<ind.code[i]<<endl;
    fout.close();
    cout<<"���������ɹ���"<<endl;

//    cout<<"index���£�"<<endl;
//    for(it1 = indexs.code.begin(); it1 != indexs.code.end(); it1++)
//    {
//    	cout<<*it1<<endl;
//    }
    return;
}

