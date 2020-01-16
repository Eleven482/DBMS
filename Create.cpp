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
    int colength;           //属性个数
    int remcount;			//元组个数
    string tableName;	    //表名
    vector<int> condition;  //0表示无约束条件
    vector<string> colName; //属性名
    vector<string> colType; //属性类型

};

class VIEW
{
public:
    string viewName;   	  //视图名
    vector<string> codes; //定义视图的代码

};

class INDEX
{
public:
    vector<string> code;	//定义索引的代码
    string indexName;		//索引名
};


vector<TABLE> table; //用TABLE类定义一个table数组，里面的元素均为表
vector<VIEW> view;   //用VIEW类定义一个view数组，里面的元素均为试图
vector<INDEX> index; //用INDEX类定义一个index数组，里面的元素均为索引

void Create_table(string op)
{

    string tmp = "Create Table ";
    TABLE t;

    t.colength=0;     //初始化属性个数为0
    t.remcount=0;
    t.tableName = ""; //初始化表名为空

    //提取新建的表的名字
    for(int k = tmp.length(); (op[k]!='\n' || op[k]!='(' || op[k]!=' ') && k < op.length(); k++)
        t.tableName += op[k];//挨个字符记录

    //获取表中字段名及字段类型
    int i = 0;
    string s;

    /*括号是命令输入结束的标志*/
    /*从create的下一行开始读*/
    ///这个地方有一个问题，在没有起始左括号的情况下，也可以成功建表
    while(getline(cin, s) && s != ");")
    {
        int f = 1;

        tmp = "";
        t.colength++;

        while(!(s[i]>='a'&&s[i]<='z') && !(s[i]>='A'&&s[i]<='Z'))
            i++;				//将i挪到每一行第一次出现字母的地方

        for( ; s[i] != ' ' && i < s.length(); i++)
        {
            tmp += s[i];
        }

        if(tmp.length() == 0)	//creat后输入为空，没有有效的属性定义
        {
            cout <<"create 语法错误!"<< endl;
            return;
        }

        tmp = "";

        while(!(s[i]>='a'&&s[i]<='z') && !(s[i]>='A'&&s[i]<='Z'))
            i++;

        for( ; s[i]!='(' && s[i]!=' '&& s[i]!=',' && i < s.length(); i++)
        {
            tmp += s[i];	//提取有效的属性类型定义
        }
        //判断字段类型是否正确
        if((tmp != "char")&&(tmp != "int")&&(tmp != "float")&&(tmp != "double")&&(tmp != "time")&&(tmp != "date"))
        {
            cout <<"类型错误!"<< endl;
            return;
        }
        if(tmp.length() == 0)
        {
            cout <<"create 语法错误!"<< endl;
            return;
        }


        //为了提取约束条件，把所有干扰字符先去掉
        for(i = 0; i < s.length(); i++)
            if(s[i] == ',')
                s[i] = ' ';
        if(s[0] == '(')
            s[0] = ' ';

        //判断约束条件
        stringstream ss(s);//将输入的第一行转换成流
        string s1;
        vector<string> in;
        while(ss >> s1)
            in.push_back(s1);
        t.colName.push_back(in[0]); //字段名
        t.colType.push_back(in[1]); //字段类型
        string yueshu = "";
        for(i = 2; i < in.size(); i++)
        {
            yueshu += in[i];//第二个字符串后面的全部为约束条件
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

    //存储整个数据表,在table里会有显示
    ofstream fout;
    fout.open("table.txt", ios::app);//以追加的方式打开文件,供写,文件不存在则创建,若文件已存在则在原文件内容后写入新的内容,指针位置总在最后
    fout <<"表名："<<t.tableName<<"\t"<<"，共有"<<t.colength<<"个属性"<< endl;
    for(i = 0; i < t.colName.size(); i++)
    {
        fout<<"字段名："<<t.colName[i]<<"\t";

        fout<<" 字段类型："<< t.colType[i]<<"\t";
        if(t.condition[i]==1)
            fout<<"约束条件:primary key\t"<<endl;
        else if(t.condition[i]==2)
            fout<<"约束条件:unique\t"<<endl;
        else if(t.condition[i]==3)
            fout<<"约束条件:not null\t"<<endl;
        else
            fout<<endl;
    }
    fout<<endl;
    fout.close();


//    存储数据表中的数据
    char file[30];
//    fstream file;
    string tt = t.tableName + ".txt";
    for(i = 0; i < tt.length(); i++)
        file[i] = tt[i];
    file[tt.length()]='\0';
    fout.open(file);		///这个地方需要把字符串转换成字符数组然后才能打开，如果直接用字符串当参数打不开文件
//    file.open(tt.c_str());
//    ofstream outfile("???.txt",ios::out|ios::trunc);
    //用c_str函数转换能达到打开文件的目的，但是上面这个函数写文件名的时候还是没法写，那就没法重写。所以只能用本办法了

    for(i = 0; i < t.colName.size()-1; i++)
        fout <<t.colName[i]<<"\t\t";
    fout <<t.colName[t.colName.size()-1]<<"\t\t"<<endl;
    fout.close();

    cout <<"建表成功！" << endl;
    return;
}

void Create_view(string s)
{
    string tmp;
    tmp="Create View ";
    int i=tmp.length();
    //先把长度留下来，之所以不直接定义i=12是为了方便后期的调试，
    //如果简单的自定义建视图命令为Cre View，那么长度就会变化，这样获取长度只需要修改tmp就可以了


    //获取view的名字
    tmp="";
    for(; i<s.length()&&s[i]!=' '; i++)
        tmp+=s[i];

    VIEW v;
    v.viewName=tmp;//存入view的名字

    string ttt,t1;
    ttt="as ";

    //读创建view的命令代码
    while(getline(cin,t1)&&t1!=";")
        v.codes.push_back(t1);

    v.codes.push_back(";");
    view.push_back(v); //把新创建的view压入vector

    //对相应文件进行操作
    //存储命令语句
    ofstream fout;
    fout.open("view.txt",ios::app);//以追加的方式打开文件,供写,文件不存在则创建,若文件已存在则在原文件内容后写入新的内容,指针位置总在最后
    fout <<"视图名:"<<v.viewName<<endl;
    fout<<"视图定义语句："<<endl;
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

    cout << "建立视图成功！" << endl;
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

    //读第二行
    string t1;
    getline(cin,t1);

    tmp="on ";
    i=tmp.length();
    tmp = "";
    for(; i<t1.length()&&t1[i]!='('; i++)
    {
        tmp+=t1[i];//确定table的名字
    }
    string ind_tab_name = "";
    ind_tab_name = tmp;

    i++;//跳过左括号
    tmp = "";
    for(; i < t1.length()&&t1[i]!=')'; i++)
    {
        tmp+=t1[i];//确定主码
    }
    string key_name;
    key_name = tmp;

    tmp = "";
    getline(cin,t1);
    for(i = 0; i < t1.length()&&t1[i]!=';'; i++)
    {
        tmp+=t1[i];//确定主码
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
    fout.open("index.txt",ios::app);//以追加的方式打开文件,供写,文件不存在则创建,若文件已存在则在原文件内容后写入新的内容,指针位置总在最后
    fout << "索引名："<<ind.indexName<<endl;
    fout<<"索引："<<endl;
    for(int i=0; i<ind.code.size(); ++i)
        fout<<ind.code[i]<<endl;
    fout.close();
    cout<<"建立索引成功！"<<endl;

//    cout<<"index如下："<<endl;
//    for(it1 = indexs.code.begin(); it1 != indexs.code.end(); it1++)
//    {
//    	cout<<*it1<<endl;
//    }
    return;
}

