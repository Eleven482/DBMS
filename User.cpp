#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;
class User
{
public:
    string UserName;
    string password;
    string permission_write;
    string permission_read;
    string per_create;
    string per_insert;
    string per_select;
    string per_delete;
    string per_update;
    string per_help;
    User() //默认构造函数
    {
        this->UserName= "\0";
        this->password= "\0";

        this->permission_write = "false";
        this->permission_read = "false";

        this->per_create = "false";
        this->per_insert = "false";
        this->per_delete = "false";
        this->per_update = "false";

        this->per_select = "false";
        this->per_help = "false";
    }

    void CreateUser()
    {
        User u;
        string name;
        cout << "请输入用户名：" << endl;
        cin >> name;

        User tmp;
        ifstream fin;
        fin.open("user.txt");
        bool find = false;
        string s;

        int flag = 0;
        while(!fin.eof())
        {
            getline(fin, s);
            tmp = stringToUSER(s);
            if(tmp.UserName == name)
            {
                flag = 1;
                fin.close();
                break;
            }
        }
        fin.close();

        if(flag)
        {
            cout<<"用户已存在"<<endl;
            return;
        }

        string passwd;
        cout << "请输入密码：" << endl;
        cin >> passwd;

        string privilege;
        cout << "请输入要赋予用户的权限（r - 读，w - 写，rw - 读写）：" << endl;
        cin >> privilege;

        u.UserName = name;
        u.password = passwd;
        for(int i = 0; i < privilege.length(); ++i)
        {
            if(privilege[i]=='w')
            {
                u.permission_write = "true";
                u.per_create = "true";
                u.per_insert = "true";
                u.per_delete = "true";
                u.per_update = "true";
            }

            else if(privilege[i]=='r')
            {
                u.permission_read = "true";
                u.per_select = "true";
                u.per_help = "true";
            }
        }

        ofstream fout;
        fout.open("user.txt", ios::app);
        fout << "用户姓名："<<u.UserName<<" 用户密码："<< u.password<<" 用户权限：写 "<< u.permission_write << ",读 " << u.permission_read<< ".具体权限："<<" 创建--"<<u.per_create<<", 插入--"<< u.per_insert<<", 删除--"<<u.per_delete<<", 更新--"<<u.per_update<<", 筛选--"<<u.per_select<<", 查看--"<<u.per_help<<endl<<endl;
        fout.close();
        cout<<"用户创建成功，请登录！"<<endl;
    }
    User stringToUSER(string s)
    {
        User temp;

        string t = "";
        int i = 0;
        int len = s.length();

        for( i=10; i < len; ++i)
            if(s[i] == ' ')
                break;
            else
                t += s[i];
        temp.UserName = t;
        t = "";

        i=i+11;
        for( ; i < len; ++i)
            if(s[i] == ' ')
                break;
            else
                t += s[i];
        temp.password = t;
        t = "";

        i=i+14;
        for( ; i < len; ++i)
            if(s[i] == ',')
                break;
            else
                t += s[i];
        temp.permission_write = t;
        t = "";

        i=i+4;
        for( ; i < len; ++i)
            if(s[i] == '.')
                break;
            else
                t += s[i];
        temp.permission_read = t;
        t = "";


        i=i+18;
        for( ; i < len; ++i)
            if(s[i] == ',')
                break;
            else
                t += s[i];
        temp.per_create = t;
        t = "";


        i=i+8;
        for( ; i < len; ++i)
            if(s[i] == ',')
                break;
            else
                t += s[i];
        temp.per_insert = t;
        t = "";

        i=i+8;
        for( ; i < len; ++i)
            if(s[i] == ',')
                break;
            else
                t += s[i];
        temp.per_delete = t;
        t = "";


        i=i+8;
        for( ; i < len; ++i)
            if(s[i] == ',')
                break;
            else
                t += s[i];
        temp.per_update = t;
        t = "";

        i=i+8;
        for( ; i < len; ++i)
            if(s[i] == ',')
                break;
            else
                t += s[i];
        temp.per_select = t;
        t = "";


        i=i+8;
        for( ; i < len; ++i)
            if(s[i] == ',')
                break;
            else
                t += s[i];
        temp.per_help = t;
        t = "";

        return temp;
    }

// 读取文件并查询，成功返回该用户结构体，否则返回名为null的用户
    User query_user(string name, string passwd)
    {

        User tmp;
        ifstream fin;
        fin.open("user.txt");
        bool find = false;
        string s;

        while(!fin.eof())
        {
            getline(fin, s);
            tmp = stringToUSER(s);
            if(tmp.UserName == name && tmp.password == passwd)
            {
                fin.close();
                return tmp;
            }
        }
        fin.close();
        User t;
        return t;
    }
};
