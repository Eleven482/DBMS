#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <fstream>
#include "Operation.cpp"

using namespace std;

User user;
void Initialize();
void grant(string op,int & flag);
void revoke(string op,int & flag);
void check_index();
int main()
{

    //用户身份的确认

    char iden_input;//输入代表身份的字母
    Initialize();
    while(true)
    {
        cin>>iden_input;
        if(iden_input == '1')
            break;
        else if(iden_input == '2')
        {
            user.CreateUser();
            break;
        }
        else
        {
            cout<<"请按提示输入正确的代号!"<<endl;
        }
    }

    //用户登录成功以及权限显示
    string name,password;
    while(true)
    {
        cout<<"请输入登录名:"<<endl;
        cin>>name;
        cout<<"请输入密码:"<<endl;
        cin>>password;

        User tem_user;
        tem_user = user.query_user(name,password);

        if(tem_user.UserName != name || tem_user.password != password)
        {
            cout << "用户名或密码错误" << endl;
            continue;
        }

        user = tem_user;
        cout<<"*********************************************\n";

        cout<<"你好，"<<user.UserName<<"!\n";
        if(user.permission_read == "true" && user.permission_write == "true")
        {
            cout<<"您拥有所有权限：\n" <<endl;;
            cout<<"a:Create       b:Help       c:Select\n";
            cout<<"e:Update       f:Insert     g:Delete\n";
        }
        else if(user.permission_read == "true")
        {
            cout<<"您拥有以下权限：\n";
            cout<<"a:Help         b:Select\n";
        }
        else
        {
            cout<<"您拥有以下权限：\n";
            cout<<"a:Create       b:Update       c:Insert     e:Delete\n";
        }
        break;
    }

    ofstream fout;
    fout.open("table.txt") ;
    fout.close();
    fout.open("view.txt") ;
    fout.close();
    fout.open("index.txt") ;
    fout.close();

    string operations;
    cout << "*********************************************" << endl<<endl;

    cout << "Please enter a valid SQL command:" << endl;
    cout << "*********************************************" << endl;

    int creindex = 0;
    while(getline(cin, operations))
    {
        if(operations.length()==0)
            continue;

        int flag = 0;

        string key_word = "";  //key_word为删除语句的关键字
        string gettabname = "";//获取要做删除操作的表名
        string up_name="";     //获取要做更行操作的表名
        string col_name="";    //获取要查询的列名
        string up_key01="";	   //更新的关键词1
        string up_key02="";	   //更新的关键词2
        string where="";	   //更新的关键词3
        string rset="";		   //更新的关键词4

        grant(operations,flag);
        revoke(operations,flag);

        if(user.per_create=="true")
        {
            create_table(operations, flag);   		 //建表
            create_view(operations, flag);   		 //建立视图
            create_index(operations, flag, creindex);   		 //建立索引
        }
        if(user.per_help=="true")
        {
            //查看
            help(operations, flag);
        }
        if(user.per_select=="true")
        {
            //查找
            select(operations, flag, key_word, gettabname, col_name);
        }
        if(user.per_insert == "true")
        {
            //插入
            inserts(operations, flag);
        }
        if(user.per_delete == "true")
        {
            //删除
            deletes(operations, flag, key_word, gettabname);
        }
        if(user.per_update == "true")
        {
            //更新
            update_w(operations, flag, up_name, up_key01, up_key02, where, rset);
            if(up_key01!="" && up_key02!="")
            {
                update_t(up_name, where, rset, up_key01, up_key02);
            }
            if(flag == 10086&&creindex == 1)
			{
				check_index();
			}
        }
        if(flag == 0)
            cout<<"Illegal input! Please enter again:" << endl;
        else
            cout <<"Please enter a valid SQL command:" << endl;
        cout << "*********************************************" << endl;
    }
    return 0;
}


void Initialize()
{
    cout<<"*********************************************\n";
    cout<<"              欢迎使用DBMS                   \n";
    cout<<"                                             \n";
    cout<<"              请按要求进行操作：             \n";
    cout<<"                                             \n";
    cout<<"             (1)用户登录, 请输入1            \n";
    cout<<"                                             \n";
    cout<<"             (2)用户注册, 请输入2            \n";
    cout<<"                                             \n";
    cout<<"*********************************************\n";
}
void grant(string op,int & flag)
{

    string compstr="Grant ";
    vector <string> privilege;
    vector <string> tablesname;
    int i = 0;
    //字符串比较
    for(i=0; i<compstr.length(); i++)
    {
        if(op[i]!=compstr[i])
        {
            return;
        }
    }
    flag = 9;
    if(i == compstr.length()) //判断成功，是Grant命令的正确起始字符
    {
        string rempri="";
        for(; i < op.length(); i++)
        {
            if(op[i] != ',' && op[i] != '\n')
            {
                rempri+=op[i];
            }
            else if(op[i] == ',')
            {
                privilege.push_back(rempri);
                rempri = "";
            }
//                else if(op[i] == '\n')
//                {
//                	cout<<123<<endl;
//                    privilege.push_back(rempri);
//                    break;
//                }
        }
    }
    getline(cin,op);
    compstr = "On Table ";

    for(i = 0; i < compstr.length(); i++)
    {
        if(compstr[i] != op[i])
            return;
    }

    for( ; i <= op.length(); i++)
    {
        int index = 0;
        string remstr = "";
        if(op[i] != ',' && op[i] != '\n')
        {
            remstr+=op[i];
        }
        else if(op[i] == ',')
        {
            tablesname.push_back(remstr);
            remstr = "";
        }
//            else if(op[i] == '\n')
//            {
//                tablesname.push_back(remstr);
//            }
    }

    getline(cin,op);
    compstr = "To ";

    for(i = 0; i < compstr.length(); i++)
    {
        if(compstr[i] != op[i])
            return;
    }
    compstr="";
    for( ; i <= op.length(); i++)
    {
        compstr+=op[i];
    }
    string judgement = user.UserName+";";
    for(int len = 0; len < judgement.length(); len++)
    {
        if(compstr[len] != judgement[len])
            return;
    }
//        for(j = 0; j <= tablesname.size(); j++)
//        {
//            for(k = 0; k <= table.size(); k++)
//            {
//                if(tablesname[j] == table[k].tableName)
//                {
//                    for(w = 0; w <= privilege.size(); w++)
//                    {
//                        tempuser.setpriv(privilege[w]);
//                    }
//                }
//            }
//        }

    int j,k,w;
    User tempuser = user;
    vector <string> :: iterator itr;
    for(itr = privilege.begin() ; itr!=privilege.end(); itr++)
    {
        if(*itr == "Create")
        {
            tempuser.per_create = "true";
            cout<<"成功授予用户Create权限"<<endl;
        }
        else if(*itr  == "Insert")
        {
            tempuser.per_insert = "true";
            cout<<"成功授予用户Insert权限"<<endl;
        }
        else if(*itr  == "Delete")
        {
            tempuser.per_delete = "true";
            cout<<"成功授予用户Delete权限"<<endl;
        }
        else if(*itr  == "Update")
        {
            tempuser.per_update = "true";
            cout<<"成功授予用户Update权限"<<endl;
        }
        else if(*itr  == "Select")
        {
            tempuser.per_select = "true";
            cout<<"成功授予用户Select权限"<<endl;
        }
        else if(*itr  == "Help")
        {
            tempuser.per_help = "true";
            cout<<"成功授予用户Help权限"<<endl;
        }
    }
    user = tempuser;
    return;
}
void revoke(string op,int & flag)
{

    string compstr="Revoke ";
    vector <string> privilege;
    vector <string> tablesname;
    int i = 0;
    //字符串比较
    for(i=0; i<compstr.length(); i++)
    {
        if(op[i]!=compstr[i])
        {
            return;
        }
    }
    flag = 1;
    if(i == compstr.length()) //判断成功，Revoke是命令的正确起始字符
    {
        string rempri="";
        for(; i < op.length(); i++)
        {
            if(op[i] != ',' && op[i] != '\n')
            {
                rempri+=op[i];
            }
            else if(op[i] == ',')
            {
                privilege.push_back(rempri);
                rempri = "";
            }
//                else if(op[i] == '\n')
//                {
//                	cout<<123<<endl;
//                    privilege.push_back(rempri);
//                    break;
//                }
        }
    }
    getline(cin,op);
    compstr = "On Table ";

    for(i = 0; i < compstr.length(); i++)
    {
        if(compstr[i] != op[i])
            return;
    }

    for( ; i <= op.length(); i++)
    {
        int index = 0;
        string remstr = "";
        if(op[i] != ',' && op[i] != '\n')
        {
            remstr+=op[i];
        }
        else if(op[i] == ',')
        {
            tablesname.push_back(remstr);
            remstr = "";
        }
//            else if(op[i] == '\n')
//            {
//                tablesname.push_back(remstr);
//            }
    }

    getline(cin,op);
    compstr = "From ";

    for(i = 0; i < compstr.length(); i++)
    {
        if(compstr[i] != op[i])
            return;
    }
    compstr="";
    for( ; i <= op.length(); i++)
    {
        compstr+=op[i];
    }
    string judgement = user.UserName+";";
    for(int len = 0; len < judgement.length(); len++)
    {
        if(compstr[len] != judgement[len])
            return;
    }
    int j,k,w;
    User tempuser = user;
    vector <string> :: iterator itr;
    for(itr = privilege.begin() ; itr!=privilege.end(); itr++)
    {
        if(*itr == "Create")
        {
            tempuser.per_create = "false";
            cout<<"成功收回用户Create权限"<<endl;
        }
        else if(*itr  == "Insert")
        {
            tempuser.per_insert = "false";
            cout<<"成功收回用户Insert权限"<<endl;
        }
        else if(*itr  == "Delete")
        {
            tempuser.per_delete = "false";
            cout<<"成功收回用户Delete权限"<<endl;
        }
        else if(*itr  == "Update")
        {
            tempuser.per_update = "false";
            cout<<"成功收回用户Update权限"<<endl;
        }
        else if(*itr  == "Select")
        {
            tempuser.per_select = "false";
            cout<<"成功收回用户Select权限"<<endl;
        }
        else if(*itr  == "Help")
        {
            tempuser.per_help = "false";
            cout<<"成功收回用户Help权限"<<endl;
        }
    }
    user = tempuser;
    return;
}
void check_index()
{
    cout<<"index 已更新"<<endl;
}
