#include <string>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "Create.cpp"
using namespace std;


//判断是否建表
void create_table(string op, int &flag) //已完成
{
    string s="Create Table ";
    int i = 0;
    //字符串比较
    for(i=0; i<s.length(); i++)
    {
        if(op[i]!=s[i])
        {
            break;
        }
    }
    if(i == s.length()) //判断成功，是Create命令的正确起始字符
    {
        flag=1;
        Create_table(op);
    }
    return;
}
//判断是否建立视图
void create_view(string op, int &flag)  //已完成
{
    string s="Create View ";
    int i = 0;
    for(i=0; i<s.length(); ++i)
    {
        if(op[i]!=s[i])
        {
            break;
        }
    }
    if(i == s.length())
    {
        flag=1;
        Create_view(op);
    }
    return;
}
//判断是否建立索引
void create_index(string op, int &flag, int &creindex) //已完成
{
    string s="Create Index ";
    int i = 0;
    for(i=0; i<s.length(); ++i)
    {
        if(op[i]!=s[i])
        {
            break;
        }
    }
    if(i == s.length())
    {
        flag=1;
        creindex=1;
        Create_index(op);
    }
    return;
}

//插入
void inserts(string op, int &flag) //已完成
{
    string s="Insert into ";
    int f=4;
    for(int i=0; i<s.length(); i++)
    {
        if(op[i]!=s[i])
        {
            f=0;
            break;
        }
    }
    if(f==0)
        return;
    else
        flag=1;

    //确定往哪个表中插入数据,并判断表是否存在
    string tableName="";
    int i=s.length();
    for( ; i<op.length()&&op[i]!= ' '; i++)
        tableName+=op[i];

    int k=-1;
    for(int j=0; j<table.size(); j++)
    {
        if(table[j].tableName==tableName)
        {
            k=j;
            break;
        }
    }
    if(k==-1)
    {
        cout<<tableName<<"表不存在！"<<endl;
        flag=0;
        return;
    }

    //判断语句格式
    s="Values(";
    i++;
    for(int j=0; j<s.length(); ++j,++i)
    {
        if(op[i]!=s[j])
        {
            cout<<"insert 语法错误！"<<endl;
            flag=0;
            return;
        }
    }

    //确定插入的数据
    vector<string> vec;
    string tt="";
    while(i<op.length())
    {
        if(op[i]==','||op[i]==')')//以属性之间的逗号和最后的右括号为每个属性的结束标识符
        {
            vec.push_back(tt);
            tt="";
        }
        else if(op[i]=='\''|| op[i]==' ')
            i;//跳过去，继续读
        else
            tt += op[i];//提取有效的属性
        i++;
    }
    if(vec.size() != table[k].colName.size())//数量不对，肯定不对应
    {
        cout << "插入的元组与该表不对应！" << endl;
        flag = 0;
        return;
    }
    //判断约束条件
    for(int i = 0; i < table[k].colName.size(); i++)
    {
        if(table[k].condition[i] == 1)
        {
            if(vec[i]=="NULL")
            {
                cout<<"主键不能为空！"<<endl;
                flag=0;
                return;
            }
        }

        if(table[k].condition[i]==2)
        {
            if(vec[i]=="NULL")
            {
                cout<<"违反约束条件unique,不能做插入操作！"<<endl;
                flag=0;
                return;
            }
        }

        if(table[k].condition[i]==3)
        {
            if(vec[i]=="NULL")
            {
                cout<<"违反约束条件not null,不能做插入操作！"<<endl;
                flag=0;
                return;
            }
        }
    }

    ofstream fout;

    tt = table[k].tableName+".txt";
    char file[30];
    for(int i = 0; i < tt.length(); ++i)
        file[i] = tt[i];
    file[tt.length()] = '\0';

    fout.open(file, ios::app);
    for(int i = 0; i < vec.size()-1; ++i)
        fout << vec[i] << "\t\t";
    fout<<vec[vec.size()-1]<<"\t\t"<<endl;
    fout.close();
    cout<<"成功插入 1 个元组！"<<endl;
    table[k].remcount++;
    return;
}

//两种权限都可以操作
void help(string op, int& flag)//已完成
{
    int i,j,k,w;
    if(op=="Help Database")
    {
        flag = 1;
        int flag_01 = 0;

        //显示数据库中表的信息
        for(i=0; i<table.size(); i++)
        {
            flag_01 = 1;
            cout<<endl<<"////////////////////////////////////////////////////////"<<endl<<endl;
            cout<<"table："<<table[i].tableName<<":"<<endl;
            for(j = 0; j < table[i].colName.size(); j++) //显示属性
                cout<<table[i].colName[j]<<"\t\t";									///对齐方式还得继续完善
            cout<<endl;
            for(j = 0; j < table[i].colName.size(); j++) //显示属性类型
                cout<<table[i].colType[j]<<"\t";
            cout<<endl<<endl;
        }
        //显示数据库中视图的信息
        for(i=0; i<view.size(); i++)
        {
            flag_01=1;
            cout<<"view："<<view[i].viewName<<":"<<endl;
            cout<<"Create View "<<view[i].viewName<<endl;
            for(j=0; j<view[i].codes.size(); j++)
                cout<<view[i].codes[j]<<endl;
            cout<<endl<<endl;
        }
        //显示数据库中索引的信息
        for(i=0; i<index.size(); ++i)
        {
            flag_01=1;
            cout<<"index："<<index[i].indexName<<":"<<endl;
            cout<<"Create Index "<<index[i].indexName<<endl;
            for(j=0; j<index[i].code.size(); j++)
                cout<<index[i].code[j]<<endl;
            cout<<endl<<endl;
        }
        if(flag_01==0)
            cout<<"该数据库中暂无数据！"<<endl;
        if(flag_01 == 1)
            cout<<"////////////////////////////////////////////////////////"<<endl<<endl;
        return;
    }

    //单独查看表
    string s="Help Table ";
    for(i=0; i<s.length(); i++)
    {
        if(op[i]!=s[i])
        {
            break;
        }
    }
    if(i == s.length())//若成立，说明此语句是查看表的语句
    {
        flag = 1;
        int flag_02 = 0;
        string name;

        for(j=s.length(); j<op.length(); ++j)
            name+=op[j];

        for(k = 0; k < table.size(); k++)
        {
            if(table[k].tableName == name)
            {
                flag_02 = 1;
                cout<<"************table："<<table[k].tableName<<"\t"<<"************"<<endl;

                ifstream fin;
                string tt = table[k].tableName+".txt";
                char file[30];
                for(w=0; w<tt.length(); w++)
                    file[w]=tt[w];
                file[tt.length()]='\0';
                fin.open(file);

                //读取方式: 逐行读取, 将行读入字符串,行之间用回车换行区分,这种方法比较灵活
                string row;
                while(getline(fin, row))
                    cout<<row<<endl;
                cout<<endl;
                return;
            }
        }
        if(!flag_02)
            cout<<name<<"表不存在！"<<endl;
        return;
    }

    //单独查看视图
    s="Help View ";
    for(i=0; i<s.length(); ++i)
    {
        if(op[i]!=s[i])
        {
            break;
        }
    }
    if(i == s.length())
    {
        flag=1;
        j = 0,k = 0;
        string name;
        for(j=s.length(); j< op.length(); ++j)
            name+=op[j];
        int flag_03 = 0;
        for(k = 0; k < view.size(); k++)
        {
            if(view[k].viewName == name)
            {
                flag_03 = 1;
                cout <<"************view：" <<view[k].viewName<<"\t"<<"************"<< endl;
                cout<<"Create View "<<name<<endl;
                for(w=0; w<view[k].codes.size(); w++)
                    cout<<view[k].codes[w]<<endl;
                cout<<endl;
                return;
            }
        }
        if(!flag_03)
            cout<<name<<"视图不存在！"<<endl;
        return;
    }

    //单独查看索引
    s="Help Index ";
    for(i=0; i<s.length(); ++i)
    {
        if(op[i]!=s[i])
        {
            break;
        }
    }
    if(i == s.length())
    {
        flag=1;
        j = 0,k = 0;
        string name;
        for(j=s.length(); j< op.length(); ++j)
            name+=op[j];
        int flag_03 = 0;
        for(k = 0; k < index.size(); k++)
        {
            if(index[k].indexName == name)
            {
                flag_03 = 1;
                cout <<"************index：" <<index[k].indexName<<"\t"<<"************"<< endl;
                cout<<"Create Index "<<name<<endl;
                for(w=0; w<index[k].code.size(); w++)
                    cout<<index[k].code[w]<<endl;
                cout<<endl;
                return;
            }
        }
        if(!flag_03)
            cout<<name<<"索引不存在！"<<endl;
        return;
    }
    return;
}
//特针对只读权限
void limit(string op,int &flag) //已完成
{
    string s;
    s="Create";
    string temp="";
    for(int i=0; op[i]!=' '; ++i)
    {
        temp+=op[i];
        if(temp==s)
        {
            flag=1;
            cout<<"您没有create权限！"<<endl;
            return;
        }
    }
    s="Delete";
    temp="";
    for(int i=0; op[i]!=' '; ++i)
    {
        temp+=op[i];
        if(temp==s)
        {
            flag=1;
            cout<<"您没有delete权限！"<<endl;
            return;
        }
    }
    s="Insert";
    temp="";
    for(int i=0; op[i]!=' '; ++i)
    {
        temp+=op[i];
        if(temp==s)
        {
            flag=1;
            cout<<"您没有insert权限！"<<endl;
            return;
        }
    }
    s="Update";
    temp="";
    for(int i=0; op[i]!=' '; ++i)
    {
        temp+=op[i];
        if(temp==s)
        {
            flag=1;
            cout<<"您没有update权限！"<<endl;
            return;
        }
    }
}

//删除
void deletes(string op, int& flag, string &tt, string &gettabname) //已完成
{
    string s = "Delete";
    int i;
    for(i = 0 ; i < s.length(); i++)
    {
        if(s[i]!=op[i])
            return;
    }
    flag = 1;

    getline(cin, op);
    s = "from ";
    for(i = 0 ; i < s.length(); i++)
    {
        if(s[i]!=op[i])
        {
            cout << "delete 语法错误！"  << endl;
            return;
        }
    }

    string name = "";
    for( ; i < op.length(); i++)
        name += op[i];
    gettabname = name;

    int k = -1;
    for(int j = 0; j < table.size(); ++j)
    {
        if(table[j].tableName == name)
        {
            k = j;
            break;
        }
    }
    if(k == -1)
    {
        cout <<name<<"表不存在！" << endl;
        return;
    }

    getline(cin, op);
    s = "where ";
    for(i = 0; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
        {
            cout << "delete 语法错误！"  << endl;
            flag=0;
            return;
        }
    }

    string t = "";
    for( ; i < op.length()&&op[i]!='='; i++)
        t += op[i];
    i += 2;
    for( ; i < op.length()&&op[i]!='\''; i++)
        tt += op[i];


    gettabname+=".txt";             //ttname是文件名，tt是要查找的关键字
    fstream file;
    int found;

    file.open(gettabname.c_str());
    ofstream outfile("in2.txt",ios::out|ios::trunc);//ofstream是从内存到硬盘，ios::out表示写打开，ios::trunc是在向文件写之前擦掉所有之前数据
    string line;
    int remdelnum=0;
    while(!file.eof())
    {
        getline(file,line);		//逐条读取txt文件的内容
        found = line.find(tt);	//found的值为要查找的关键字在所在行的下标
        if(found==-1)			//如果某一行没有找到要查找的关键字，found为默认值-1.
            outfile<<line<<endl;//把没有关键字所在行的内容存入临时文件in2.txt，就相当于删除了带关键字的一行
        else
            remdelnum++;
    }
    outfile.close();
    file.close();

    ofstream outfile1;
    outfile1.open(gettabname.c_str());
    fstream file1("in2.txt");
    while(!file1.eof())
    {
        getline(file1,line);
        outfile1<<line<<endl;    //重写相应的txt文件，与line：387对应
    }
    outfile1.close();
    file1.close();
//    remove("in2.txt");
    system("del in2.txt");//删除临时文件

    TABLE ttt;
    for(int counts = 0; counts < table.size(); counts++)
    {
        if(gettabname == table[counts].tableName)
        {
            ttt = table[counts];
        }
    }
    ttt.remcount-=remdelnum;
    cout<<"成功删除了"<<remdelnum<<"个元组"<<endl;
}

//查询
void select(string op, int& flag,string &key_word,string &gettabname,string &colname) //已完成
{
    string s = "Select ";
    int i;

    for(i = 0; i < s.length(); i++)
    {
        if(s[i] != op[i])
            return;
    }
    flag = 1;

    string column_name = "";
    for( ; i < op.length(); ++i)
        column_name += op[i];
    colname=column_name;
    string table_name="";
    getline(cin, op);
    s = "from ";
    i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
        {
            cout << "select 语法错误！" << endl;
            flag=0;
            return;
        }
    }

    for( ; i < op.length(); ++i)
        table_name += op[i];
    gettabname=table_name;
    int k = -1;
    for(int j = 0; j < table.size(); ++j)
    {
        if(table[j].tableName== table_name)
        {
            k = j;
            break;
        }
    }
    if(k==-1)
    {
        cout << table_name<<"表不存在！" << endl;
        flag=0;
        return;
    }

    string where = "";
    getline(cin, op);
    s = "where ";
    i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
        {
            cout << "select 语法错误!" << endl;
            flag=0;
            return;
        }
    }

    for( ; i < op.length()&&op[i]!='='; ++i)
        where += op[i];
    i=i+2;
    for( ; i < op.length()&&op[i]!='\''; ++i)
        key_word += op[i];



    int counts = 0;
    TABLE t;
    for(counts = 0; counts < table.size(); counts++)
    {
        if(gettabname == table[counts].tableName)
        {
            t = table[counts];
        }
    }

    t.tableName+=".txt";//gettabname是文件名，key_word是要查找的关键字
    fstream file;
    int found;
    file.open(t.tableName.c_str());
    string line;
    while(!file.eof())
    {
        getline(file,line);//逐行读取txt文件的内容
        found = line.find(key_word);//found的值为要查找的关键字在所在行的下标
        if(found!=-1)//如果某一行没有找到要查找的关键字，found为默认值-1
        {
            stringstream ss(line);
            string s1;
            vector<string> in;
            while(ss >> s1)
                in.push_back(s1);
//            cout<<"////*****"<<t.colength<<"*****////";
            for(int i = 0; i < t.colength; i++)
            {
                if(t.colName[i]==colname)
                {
                    cout <<"查询结果：";
                    cout<<in[i]<<endl;
                }
            }
        }
    }
    file.close();
}

//更新
void update_w(string op, int& flag, string &up_name, string &t1, string &t2, string &where, string &rset)  ///未实现对属性进行更新
{
    string s = "Update ";
    int i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i] != op[i])
            return;
    }


    string name = "";
    for( ; i < op.length(); ++i)
        name += op[i];
    up_name=name;//获取表名
    int k=-1;
    for(int j=0; j < table.size(); ++j)
    {
        if(table[j].tableName == name)
        {
            k = j;
            break;
        }
    }
    if(k == -1)
    {
        cout << name<<"表不存在！" << endl;
        flag=0;
        return;
    }

    getline(cin, op);
    s = "set ";
    i = 0;
    for(int j = 0; j < s.length(); ++j, ++i)
    {
        if(s[j] != op[i])
        {
            cout << "update 语法错误！" << endl;
            flag=0;
            return;
        }
    }
    for( ; i < op.length()&&op[i]!='='; ++i)
        rset += op[i];//要被使用的属性的name，如sage等
    i=i+2;//跳过等号和引号
    for( ; i < op.length()&&op[i]!='\''; ++i)
        t1 += op[i];//要换成的内容

    getline(cin, op);
    s = "where ";
    i = 0;
    for(int j = 0; j < s.length(); ++j, ++i)
    {
        if(s[j] != op[i])
        {
            cout << "update 语法错误！" << endl;
            flag=0;
            return;
        }
    }

    for( ; i < op.length()&&op[i]!='='; ++i)
        where += op[i];//被换属性的name，如sage
    i+=2;
    for( ; i < op.length()&&op[i]!='\''; ++i)
        t2 += op[i];//被换内容的码
	flag = 10086;

}

//先找到rset（colname）的下标remrset和where（colaname）的下标remwhere
//挨行读出，找到与t2相同的那个属性in[remwhere]
//那么 in[rset]修改t1
void update_t(string &up_name, string &where, string &rset, string &t1,string &t2)
{
    int counts = 0,numcounts = 0;;
    TABLE t;
    for(counts = 0; counts < table.size(); counts++)
    {
        if(up_name == table[counts].tableName)
        {
            t = table[counts];
        }
    }
    up_name+=".txt";
    fstream file;
    int found;
    file.open(up_name.c_str());
    ofstream outfile("in2.txt",ios::out|ios::trunc);
    string line;
    while(!file.eof())
    {
        int i,j;
        getline(file,line);   //逐行读取txt文件的内容
        found = line.find(t2);//found的值为要查找的关键字在所在行的下标
        if(found==-1)         //如果某一行没有找到要查找的关键字，found为默认值-1
            outfile<<line<<endl;
        else
        {
            int pos,remrset;
            stringstream ss(line);
            string s1;
            vector<string> in;
            while(ss >> s1)
                in.push_back(s1);
            for(int i = 0; i < t.colength; i++)
            {
                if(t.colName[i]==rset)
                {
                    remrset = i;
                    break;
                }
            }
            string detail=in[remrset];
            pos=line.find(detail);
            while(pos!=-1)
            {
                stringstream ss(line);
                line.replace(pos,detail.length(),t1);//用新的串替换掉指定的串
                numcounts++;
                pos = line.find(detail);//继续查找指定的串，直到所有的都找到为止
            }
            outfile<<line<<endl;
        }
    }
    outfile.close();
    file.close();
    ofstream outfile1;
    outfile1.open(up_name.c_str());
    fstream file1("in2.txt");
    while(!file1.eof())
    {
        getline(file1,line);
        outfile1<<line<<endl;
    }
    outfile1.close();
    file1.close();
    system("del in2.txt");//删除临时文件
    cout<<"成功修改了"<<numcounts<<"个元组！"<< endl;

}

