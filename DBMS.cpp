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

    //�û���ݵ�ȷ��

    char iden_input;//���������ݵ���ĸ
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
            cout<<"�밴��ʾ������ȷ�Ĵ���!"<<endl;
        }
    }

    //�û���¼�ɹ��Լ�Ȩ����ʾ
    string name,password;
    while(true)
    {
        cout<<"�������¼��:"<<endl;
        cin>>name;
        cout<<"����������:"<<endl;
        cin>>password;

        User tem_user;
        tem_user = user.query_user(name,password);

        if(tem_user.UserName != name || tem_user.password != password)
        {
            cout << "�û������������" << endl;
            continue;
        }

        user = tem_user;
        cout<<"*********************************************\n";

        cout<<"��ã�"<<user.UserName<<"!\n";
        if(user.permission_read == "true" && user.permission_write == "true")
        {
            cout<<"��ӵ������Ȩ�ޣ�\n" <<endl;;
            cout<<"a:Create       b:Help       c:Select\n";
            cout<<"e:Update       f:Insert     g:Delete\n";
        }
        else if(user.permission_read == "true")
        {
            cout<<"��ӵ������Ȩ�ޣ�\n";
            cout<<"a:Help         b:Select\n";
        }
        else
        {
            cout<<"��ӵ������Ȩ�ޣ�\n";
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

        string key_word = "";  //key_wordΪɾ�����Ĺؼ���
        string gettabname = "";//��ȡҪ��ɾ�������ı���
        string up_name="";     //��ȡҪ�����в����ı���
        string col_name="";    //��ȡҪ��ѯ������
        string up_key01="";	   //���µĹؼ���1
        string up_key02="";	   //���µĹؼ���2
        string where="";	   //���µĹؼ���3
        string rset="";		   //���µĹؼ���4

        grant(operations,flag);
        revoke(operations,flag);

        if(user.per_create=="true")
        {
            create_table(operations, flag);   		 //����
            create_view(operations, flag);   		 //������ͼ
            create_index(operations, flag, creindex);   		 //��������
        }
        if(user.per_help=="true")
        {
            //�鿴
            help(operations, flag);
        }
        if(user.per_select=="true")
        {
            //����
            select(operations, flag, key_word, gettabname, col_name);
        }
        if(user.per_insert == "true")
        {
            //����
            inserts(operations, flag);
        }
        if(user.per_delete == "true")
        {
            //ɾ��
            deletes(operations, flag, key_word, gettabname);
        }
        if(user.per_update == "true")
        {
            //����
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
    cout<<"              ��ӭʹ��DBMS                   \n";
    cout<<"                                             \n";
    cout<<"              �밴Ҫ����в�����             \n";
    cout<<"                                             \n";
    cout<<"             (1)�û���¼, ������1            \n";
    cout<<"                                             \n";
    cout<<"             (2)�û�ע��, ������2            \n";
    cout<<"                                             \n";
    cout<<"*********************************************\n";
}
void grant(string op,int & flag)
{

    string compstr="Grant ";
    vector <string> privilege;
    vector <string> tablesname;
    int i = 0;
    //�ַ����Ƚ�
    for(i=0; i<compstr.length(); i++)
    {
        if(op[i]!=compstr[i])
        {
            return;
        }
    }
    flag = 9;
    if(i == compstr.length()) //�жϳɹ�����Grant�������ȷ��ʼ�ַ�
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
            cout<<"�ɹ������û�CreateȨ��"<<endl;
        }
        else if(*itr  == "Insert")
        {
            tempuser.per_insert = "true";
            cout<<"�ɹ������û�InsertȨ��"<<endl;
        }
        else if(*itr  == "Delete")
        {
            tempuser.per_delete = "true";
            cout<<"�ɹ������û�DeleteȨ��"<<endl;
        }
        else if(*itr  == "Update")
        {
            tempuser.per_update = "true";
            cout<<"�ɹ������û�UpdateȨ��"<<endl;
        }
        else if(*itr  == "Select")
        {
            tempuser.per_select = "true";
            cout<<"�ɹ������û�SelectȨ��"<<endl;
        }
        else if(*itr  == "Help")
        {
            tempuser.per_help = "true";
            cout<<"�ɹ������û�HelpȨ��"<<endl;
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
    //�ַ����Ƚ�
    for(i=0; i<compstr.length(); i++)
    {
        if(op[i]!=compstr[i])
        {
            return;
        }
    }
    flag = 1;
    if(i == compstr.length()) //�жϳɹ���Revoke���������ȷ��ʼ�ַ�
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
            cout<<"�ɹ��ջ��û�CreateȨ��"<<endl;
        }
        else if(*itr  == "Insert")
        {
            tempuser.per_insert = "false";
            cout<<"�ɹ��ջ��û�InsertȨ��"<<endl;
        }
        else if(*itr  == "Delete")
        {
            tempuser.per_delete = "false";
            cout<<"�ɹ��ջ��û�DeleteȨ��"<<endl;
        }
        else if(*itr  == "Update")
        {
            tempuser.per_update = "false";
            cout<<"�ɹ��ջ��û�UpdateȨ��"<<endl;
        }
        else if(*itr  == "Select")
        {
            tempuser.per_select = "false";
            cout<<"�ɹ��ջ��û�SelectȨ��"<<endl;
        }
        else if(*itr  == "Help")
        {
            tempuser.per_help = "false";
            cout<<"�ɹ��ջ��û�HelpȨ��"<<endl;
        }
    }
    user = tempuser;
    return;
}
void check_index()
{
    cout<<"index �Ѹ���"<<endl;
}
