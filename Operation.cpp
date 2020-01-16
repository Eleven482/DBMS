#include <string>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "Create.cpp"
using namespace std;


//�ж��Ƿ񽨱�
void create_table(string op, int &flag) //�����
{
    string s="Create Table ";
    int i = 0;
    //�ַ����Ƚ�
    for(i=0; i<s.length(); i++)
    {
        if(op[i]!=s[i])
        {
            break;
        }
    }
    if(i == s.length()) //�жϳɹ�����Create�������ȷ��ʼ�ַ�
    {
        flag=1;
        Create_table(op);
    }
    return;
}
//�ж��Ƿ�����ͼ
void create_view(string op, int &flag)  //�����
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
//�ж��Ƿ�������
void create_index(string op, int &flag, int &creindex) //�����
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

//����
void inserts(string op, int &flag) //�����
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

    //ȷ�����ĸ����в�������,���жϱ��Ƿ����
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
        cout<<tableName<<"�����ڣ�"<<endl;
        flag=0;
        return;
    }

    //�ж�����ʽ
    s="Values(";
    i++;
    for(int j=0; j<s.length(); ++j,++i)
    {
        if(op[i]!=s[j])
        {
            cout<<"insert �﷨����"<<endl;
            flag=0;
            return;
        }
    }

    //ȷ�����������
    vector<string> vec;
    string tt="";
    while(i<op.length())
    {
        if(op[i]==','||op[i]==')')//������֮��Ķ��ź�����������Ϊÿ�����ԵĽ�����ʶ��
        {
            vec.push_back(tt);
            tt="";
        }
        else if(op[i]=='\''|| op[i]==' ')
            i;//����ȥ��������
        else
            tt += op[i];//��ȡ��Ч������
        i++;
    }
    if(vec.size() != table[k].colName.size())//�������ԣ��϶�����Ӧ
    {
        cout << "�����Ԫ����ñ���Ӧ��" << endl;
        flag = 0;
        return;
    }
    //�ж�Լ������
    for(int i = 0; i < table[k].colName.size(); i++)
    {
        if(table[k].condition[i] == 1)
        {
            if(vec[i]=="NULL")
            {
                cout<<"��������Ϊ�գ�"<<endl;
                flag=0;
                return;
            }
        }

        if(table[k].condition[i]==2)
        {
            if(vec[i]=="NULL")
            {
                cout<<"Υ��Լ������unique,���������������"<<endl;
                flag=0;
                return;
            }
        }

        if(table[k].condition[i]==3)
        {
            if(vec[i]=="NULL")
            {
                cout<<"Υ��Լ������not null,���������������"<<endl;
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
    cout<<"�ɹ����� 1 ��Ԫ�飡"<<endl;
    table[k].remcount++;
    return;
}

//����Ȩ�޶����Բ���
void help(string op, int& flag)//�����
{
    int i,j,k,w;
    if(op=="Help Database")
    {
        flag = 1;
        int flag_01 = 0;

        //��ʾ���ݿ��б����Ϣ
        for(i=0; i<table.size(); i++)
        {
            flag_01 = 1;
            cout<<endl<<"////////////////////////////////////////////////////////"<<endl<<endl;
            cout<<"table��"<<table[i].tableName<<":"<<endl;
            for(j = 0; j < table[i].colName.size(); j++) //��ʾ����
                cout<<table[i].colName[j]<<"\t\t";									///���뷽ʽ���ü�������
            cout<<endl;
            for(j = 0; j < table[i].colName.size(); j++) //��ʾ��������
                cout<<table[i].colType[j]<<"\t";
            cout<<endl<<endl;
        }
        //��ʾ���ݿ�����ͼ����Ϣ
        for(i=0; i<view.size(); i++)
        {
            flag_01=1;
            cout<<"view��"<<view[i].viewName<<":"<<endl;
            cout<<"Create View "<<view[i].viewName<<endl;
            for(j=0; j<view[i].codes.size(); j++)
                cout<<view[i].codes[j]<<endl;
            cout<<endl<<endl;
        }
        //��ʾ���ݿ�����������Ϣ
        for(i=0; i<index.size(); ++i)
        {
            flag_01=1;
            cout<<"index��"<<index[i].indexName<<":"<<endl;
            cout<<"Create Index "<<index[i].indexName<<endl;
            for(j=0; j<index[i].code.size(); j++)
                cout<<index[i].code[j]<<endl;
            cout<<endl<<endl;
        }
        if(flag_01==0)
            cout<<"�����ݿ����������ݣ�"<<endl;
        if(flag_01 == 1)
            cout<<"////////////////////////////////////////////////////////"<<endl<<endl;
        return;
    }

    //�����鿴��
    string s="Help Table ";
    for(i=0; i<s.length(); i++)
    {
        if(op[i]!=s[i])
        {
            break;
        }
    }
    if(i == s.length())//��������˵��������ǲ鿴������
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
                cout<<"************table��"<<table[k].tableName<<"\t"<<"************"<<endl;

                ifstream fin;
                string tt = table[k].tableName+".txt";
                char file[30];
                for(w=0; w<tt.length(); w++)
                    file[w]=tt[w];
                file[tt.length()]='\0';
                fin.open(file);

                //��ȡ��ʽ: ���ж�ȡ, ���ж����ַ���,��֮���ûس���������,���ַ����Ƚ����
                string row;
                while(getline(fin, row))
                    cout<<row<<endl;
                cout<<endl;
                return;
            }
        }
        if(!flag_02)
            cout<<name<<"�����ڣ�"<<endl;
        return;
    }

    //�����鿴��ͼ
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
                cout <<"************view��" <<view[k].viewName<<"\t"<<"************"<< endl;
                cout<<"Create View "<<name<<endl;
                for(w=0; w<view[k].codes.size(); w++)
                    cout<<view[k].codes[w]<<endl;
                cout<<endl;
                return;
            }
        }
        if(!flag_03)
            cout<<name<<"��ͼ�����ڣ�"<<endl;
        return;
    }

    //�����鿴����
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
                cout <<"************index��" <<index[k].indexName<<"\t"<<"************"<< endl;
                cout<<"Create Index "<<name<<endl;
                for(w=0; w<index[k].code.size(); w++)
                    cout<<index[k].code[w]<<endl;
                cout<<endl;
                return;
            }
        }
        if(!flag_03)
            cout<<name<<"���������ڣ�"<<endl;
        return;
    }
    return;
}
//�����ֻ��Ȩ��
void limit(string op,int &flag) //�����
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
            cout<<"��û��createȨ�ޣ�"<<endl;
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
            cout<<"��û��deleteȨ�ޣ�"<<endl;
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
            cout<<"��û��insertȨ�ޣ�"<<endl;
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
            cout<<"��û��updateȨ�ޣ�"<<endl;
            return;
        }
    }
}

//ɾ��
void deletes(string op, int& flag, string &tt, string &gettabname) //�����
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
            cout << "delete �﷨����"  << endl;
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
        cout <<name<<"�����ڣ�" << endl;
        return;
    }

    getline(cin, op);
    s = "where ";
    for(i = 0; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
        {
            cout << "delete �﷨����"  << endl;
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


    gettabname+=".txt";             //ttname���ļ�����tt��Ҫ���ҵĹؼ���
    fstream file;
    int found;

    file.open(gettabname.c_str());
    ofstream outfile("in2.txt",ios::out|ios::trunc);//ofstream�Ǵ��ڴ浽Ӳ�̣�ios::out��ʾд�򿪣�ios::trunc�������ļ�д֮ǰ��������֮ǰ����
    string line;
    int remdelnum=0;
    while(!file.eof())
    {
        getline(file,line);		//������ȡtxt�ļ�������
        found = line.find(tt);	//found��ֵΪҪ���ҵĹؼ����������е��±�
        if(found==-1)			//���ĳһ��û���ҵ�Ҫ���ҵĹؼ��֣�foundΪĬ��ֵ-1.
            outfile<<line<<endl;//��û�йؼ��������е����ݴ�����ʱ�ļ�in2.txt�����൱��ɾ���˴��ؼ��ֵ�һ��
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
        outfile1<<line<<endl;    //��д��Ӧ��txt�ļ�����line��387��Ӧ
    }
    outfile1.close();
    file1.close();
//    remove("in2.txt");
    system("del in2.txt");//ɾ����ʱ�ļ�

    TABLE ttt;
    for(int counts = 0; counts < table.size(); counts++)
    {
        if(gettabname == table[counts].tableName)
        {
            ttt = table[counts];
        }
    }
    ttt.remcount-=remdelnum;
    cout<<"�ɹ�ɾ����"<<remdelnum<<"��Ԫ��"<<endl;
}

//��ѯ
void select(string op, int& flag,string &key_word,string &gettabname,string &colname) //�����
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
            cout << "select �﷨����" << endl;
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
        cout << table_name<<"�����ڣ�" << endl;
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
            cout << "select �﷨����!" << endl;
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

    t.tableName+=".txt";//gettabname���ļ�����key_word��Ҫ���ҵĹؼ���
    fstream file;
    int found;
    file.open(t.tableName.c_str());
    string line;
    while(!file.eof())
    {
        getline(file,line);//���ж�ȡtxt�ļ�������
        found = line.find(key_word);//found��ֵΪҪ���ҵĹؼ����������е��±�
        if(found!=-1)//���ĳһ��û���ҵ�Ҫ���ҵĹؼ��֣�foundΪĬ��ֵ-1
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
                    cout <<"��ѯ�����";
                    cout<<in[i]<<endl;
                }
            }
        }
    }
    file.close();
}

//����
void update_w(string op, int& flag, string &up_name, string &t1, string &t2, string &where, string &rset)  ///δʵ�ֶ����Խ��и���
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
    up_name=name;//��ȡ����
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
        cout << name<<"�����ڣ�" << endl;
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
            cout << "update �﷨����" << endl;
            flag=0;
            return;
        }
    }
    for( ; i < op.length()&&op[i]!='='; ++i)
        rset += op[i];//Ҫ��ʹ�õ����Ե�name����sage��
    i=i+2;//�����Ⱥź�����
    for( ; i < op.length()&&op[i]!='\''; ++i)
        t1 += op[i];//Ҫ���ɵ�����

    getline(cin, op);
    s = "where ";
    i = 0;
    for(int j = 0; j < s.length(); ++j, ++i)
    {
        if(s[j] != op[i])
        {
            cout << "update �﷨����" << endl;
            flag=0;
            return;
        }
    }

    for( ; i < op.length()&&op[i]!='='; ++i)
        where += op[i];//�������Ե�name����sage
    i+=2;
    for( ; i < op.length()&&op[i]!='\''; ++i)
        t2 += op[i];//�������ݵ���
	flag = 10086;

}

//���ҵ�rset��colname�����±�remrset��where��colaname�����±�remwhere
//���ж������ҵ���t2��ͬ���Ǹ�����in[remwhere]
//��ô in[rset]�޸�t1
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
        getline(file,line);   //���ж�ȡtxt�ļ�������
        found = line.find(t2);//found��ֵΪҪ���ҵĹؼ����������е��±�
        if(found==-1)         //���ĳһ��û���ҵ�Ҫ���ҵĹؼ��֣�foundΪĬ��ֵ-1
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
                line.replace(pos,detail.length(),t1);//���µĴ��滻��ָ���Ĵ�
                numcounts++;
                pos = line.find(detail);//��������ָ���Ĵ���ֱ�����еĶ��ҵ�Ϊֹ
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
    system("del in2.txt");//ɾ����ʱ�ļ�
    cout<<"�ɹ��޸���"<<numcounts<<"��Ԫ�飡"<< endl;

}

