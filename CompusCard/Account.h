#ifndef COURSEDESIGN_ACCOUNT_H
#define COURSEDESIGN_ACCOUNT_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include "Window.h"

// forward declaration
class Card;               //ǰ������

// Account
class Account
{
public:
    enum ReturnResult {WordError, LackAmount, Succeed, ReportLoss, SoldOut}; //ö�����ͱ�ʾ����״̬

private:
    enum {Morning, Noon, Evening};
    struct Node {unsigned int card_id; bool status; struct Node * next;};     //ʹ�õ���������Ƭ

    //��̬˽�г�Ա
    static unsigned int SerialNum;              //��ˮ��
    static std::vector<Card> * Member_point_to_cards;                                  //ָ���Ӧcard�ĵ�ַ
    static std::unordered_map<unsigned int, Account> * Member_point_to_accounts;
    static unsigned int final_card_id(unsigned int card_id);                  //���տ���

    //˽�г�Ա
    std::string Member_last_day;                         //��һ��
    unsigned char Member_last_period;                    //��һ��ʱ���
    unsigned int Member_acc_num;                  //ѧ��
    std::string Member_name;                             //������ʹ��string�ࣩ
    Node * Member_head;                                  //��������
    bool Member_status;                                  //У԰��״̬������������
    double Member_remaining_sum;                         //�������
    unsigned char Member_num_reissue_card;               //�Ѿ���������
    double Member_increased_amount;                     //��ǰʱ����ۼ����ѽ��
    unsigned short Member_password;                      //����
    std::ostream & Member_output;

    //˽�к���
    bool send_card();                   //����
    bool report_loss();                 //��ʧ
    bool solute_lossing();           //���
    bool reissue_card();                 //����
    bool clear_account();               //����
    bool recharge(double money);       //��ֵ
    ReturnResult consumption(double cost, unsigned short password);    //����
    void clear_amount() { Member_increased_amount = 0.0; }

    Account(const Account &);
    Account & operator=(const Account & temp);
public:
    explicit Account(std::ostream &output, unsigned int stu_num, std::string name);
    void Access_operating(operating_record & record);              //��������
    ~Account();                                                     //��������

    //��̬��Ա
    static std::vector<operating_record> operating_records;          //������¼
    static unsigned int location;                                //��ǰλ��

    //���к���
    account_information ShowStudent() const;
    unsigned int show_card_id() const ;   //���ؿ���


    //��̬��Ա����
    static void build_Member_p_to_cards(std::vector<Card> * point_to_cards) { Member_point_to_cards = point_to_cards; }; //����ѧ��������ָ��
    static void bulid_Member_p_to_accounts(std::unordered_map<unsigned int, Account> * point_to_accounts) { Member_point_to_accounts = point_to_accounts;};
    static void Reading_operating_Information(const char * filename);                  //��ȡ������¼
    static void open_account(const char * filename, std::ostream & out);        //����

    //��Ԫ��
    friend class Card;                                 //������Ԫ��
    friend std::ostream & operator<<(std::ostream & out, const Account & student); //��Ԫ�����������
};

//Card
class Card
{
private:
    bool Member_current_status;                   //����ǰ״̬
    unsigned int card_id;                    //����
    Account * Member_point_to_student;            //ָ���Ӧ��ѧ��

    //��̬��Ա
    static std::vector<Card> * Member_point_to_cards;
    static std::vector<Window> * Member_point_to_windows;
    static std::unordered_multimap<std::string, unsigned int> table_name;               //������
    static std::unordered_multimap<unsigned int, unsigned int> table_account_num;       //ѧ�ű�
    static std::map<std::string, unsigned int> table_date;                              //ʱ���
    static std::multimap<double, unsigned int> table_amount;                            //����
    static std::vector<std::vector<unsigned int> > table_window;                        //���ڱ�


public:
    explicit Card(Account * point_to_student = nullptr) : Member_current_status(true), Member_point_to_student(point_to_student), card_id(0)
    { if (Member_point_to_student) card_id = point_to_student->Member_head->card_id; };
    void process_consuming(consuming_record & record, unsigned short password = 8888);

    static std::set<std::string> given_name;       //��
    static std::set<std::string> family_name;       //��
    static void bulid_Member_p_to_windows(std::vector<Window> * point_to_windows) { Member_point_to_windows = point_to_windows; };
    static void build_Member_p_to_cards(std::vector<Card> * point_to_cards) { Member_point_to_cards = point_to_cards; };
    static void CreateTable(std::vector<consuming_record> & final_windows_records);        //����
    static void AllSearch(searching_information & search, std::vector<consuming_record> & resultRecords);      //��ѯ
    static void SearchAccount(std::string & acc_num, std::vector<account_information> & acc_informations);      //��ѯѧ��
    friend class Account;                  //�˻�����Ԫ��
};

#endif //COURSEDESIGN_ACCOUNT_H
