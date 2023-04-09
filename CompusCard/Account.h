#ifndef COURSEDESIGN_ACCOUNT_H
#define COURSEDESIGN_ACCOUNT_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include "Window.h"

// forward declaration
class Card;               //前向声明

// Account
class Account
{
public:
    enum ReturnResult {WordError, LackAmount, Succeed, ReportLoss, SoldOut}; //枚举类型表示消费状态

private:
    enum {Morning, Noon, Evening};
    struct Node {unsigned int card_id; bool status; struct Node * next;};     //使用单链表创建卡片

    //静态私有成员
    static unsigned int SerialNum;              //流水号
    static std::vector<Card> * Member_point_to_cards;                                  //指向对应card的地址
    static std::unordered_map<unsigned int, Account> * Member_point_to_accounts;
    static unsigned int final_card_id(unsigned int card_id);                  //最终卡号

    //私有成员
    std::string Member_last_day;                         //上一天
    unsigned char Member_last_period;                    //上一次时间段
    unsigned int Member_acc_num;                  //学号
    std::string Member_name;                             //姓名（使用string类）
    Node * Member_head;                                  //卡号链表
    bool Member_status;                                  //校园卡状态（开户销户）
    double Member_remaining_sum;                         //卡中余额
    unsigned char Member_num_reissue_card;               //已经补卡次数
    double Member_increased_amount;                     //当前时间段累计消费金额
    unsigned short Member_password;                      //密码
    std::ostream & Member_output;

    //私有函数
    bool send_card();                   //发卡
    bool report_loss();                 //挂失
    bool solute_lossing();           //解挂
    bool reissue_card();                 //补卡
    bool clear_account();               //销户
    bool recharge(double money);       //充值
    ReturnResult consumption(double cost, unsigned short password);    //消费
    void clear_amount() { Member_increased_amount = 0.0; }

    Account(const Account &);
    Account & operator=(const Account & temp);
public:
    explicit Account(std::ostream &output, unsigned int stu_num, std::string name);
    void Access_operating(operating_record & record);              //操作处理
    ~Account();                                                     //析构函数

    //静态成员
    static std::vector<operating_record> operating_records;          //操作记录
    static unsigned int location;                                //当前位置

    //公有函数
    account_information ShowStudent() const;
    unsigned int show_card_id() const ;   //返回卡号


    //静态成员函数
    static void build_Member_p_to_cards(std::vector<Card> * point_to_cards) { Member_point_to_cards = point_to_cards; }; //构建学生到卡的指针
    static void bulid_Member_p_to_accounts(std::unordered_map<unsigned int, Account> * point_to_accounts) { Member_point_to_accounts = point_to_accounts;};
    static void Reading_operating_Information(const char * filename);                  //读取操作记录
    static void open_account(const char * filename, std::ostream & out);        //开户

    //友元类
    friend class Card;                                 //卡的友元类
    friend std::ostream & operator<<(std::ostream & out, const Account & student); //友元类运算符重载
};

//Card
class Card
{
private:
    bool Member_current_status;                   //卡当前状态
    unsigned int card_id;                    //卡号
    Account * Member_point_to_student;            //指向对应的学生

    //静态成员
    static std::vector<Card> * Member_point_to_cards;
    static std::vector<Window> * Member_point_to_windows;
    static std::unordered_multimap<std::string, unsigned int> table_name;               //姓名表
    static std::unordered_multimap<unsigned int, unsigned int> table_account_num;       //学号表
    static std::map<std::string, unsigned int> table_date;                              //时间表
    static std::multimap<double, unsigned int> table_amount;                            //金额表
    static std::vector<std::vector<unsigned int> > table_window;                        //窗口表


public:
    explicit Card(Account * point_to_student = nullptr) : Member_current_status(true), Member_point_to_student(point_to_student), card_id(0)
    { if (Member_point_to_student) card_id = point_to_student->Member_head->card_id; };
    void process_consuming(consuming_record & record, unsigned short password = 8888);

    static std::set<std::string> given_name;       //名
    static std::set<std::string> family_name;       //姓
    static void bulid_Member_p_to_windows(std::vector<Window> * point_to_windows) { Member_point_to_windows = point_to_windows; };
    static void build_Member_p_to_cards(std::vector<Card> * point_to_cards) { Member_point_to_cards = point_to_cards; };
    static void CreateTable(std::vector<consuming_record> & final_windows_records);        //建表
    static void AllSearch(searching_information & search, std::vector<consuming_record> & resultRecords);      //查询
    static void SearchAccount(std::string & acc_num, std::vector<account_information> & acc_informations);      //查询学号
    friend class Account;                  //账户的友元类
};

#endif //COURSEDESIGN_ACCOUNT_H
