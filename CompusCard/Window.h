#ifndef COURSEDESIGN_WINDOW_H
#define COURSEDESIGN_WINDOW_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <sstream>

// structure
//���Ѽ�¼
struct consuming_record {
    unsigned short win_num;      //���ں�
    std::string time_pre, time_after;  //����
    unsigned int card_id;              //����
    double amount;                      //���ѽ��
};

//������¼
struct operating_record {
    std::string date, operating;         //���ڣ�����
    unsigned int stu_num;        //ѧ��
    double amount;                      //0
};

//������Ϣ
struct searching_information {
    std::string begin_time, end_time;
    double begin_cost, end_cost;
    std::string name;
    std::string stu_num;
};

//�˻���Ϣ
struct account_information{
    unsigned int stu_num;                    //ѧ��
    bool status;                            //У԰��״̬������������
    double remaining_sum;                //�������
    unsigned char num_reissue_card;        //��������
    double current_increased_costs;     //��ǰʱ����ۼ����ѽ��
};
std::ostream & operator<<(std::ostream & out, const operating_record & record);
std::ostream & operator<<(std::ostream & out, const consuming_record & record);

//Window
class Window
{
private:
    static const unsigned int MaxAmount = 60000;             //����¼��60000
    static std::vector<Window> * Member_point_to_windows;
    static std::vector<std::vector<consuming_record> > consume_records;   //���Ѽ�¼

    unsigned short Member_window_number;              //���ں�
    unsigned int Member_current_times;                    //�����ۼ����Ѵ���
    double Member_current_costs;                         //�����ۼ����ѽ��
    std::string Member_last_day;                      //��һ��
    unsigned int Member_begin;                       //������ʼ��¼λ��
    unsigned int Member_end;                         //������ֹ��¼λ��
    std::vector<consuming_record> Member_consumption_records;          //���Ѽ�¼

    void increase_consumpting_record(consuming_record & record);    //�������Ѽ�¼

public:
    explicit Window(unsigned short windowNumber = 0, unsigned int consuming_location = 0);

    static unsigned int location;
    static std::vector<consuming_record>  final_records;    //���ո��������Ѽ�¼����
    static std::vector<consuming_record> ordered_records;       //����������Ѽ�¼
    static std::vector<consuming_record>  final_quicksort_records;

    static void process_consumption_records(const char * filename);                //�������Ѽ�¼
    static void read_window_wz(const char * filename);           //��ȡ�ļ�λ��
    static void build_Member_point_to_windows(std::vector<Window> * point_to_windows) { Member_point_to_windows = point_to_windows; };
    static void K_Merge();                //K·�鲢
    static void Quick();

    friend class Card;
    friend std::ostream & operator<<(std::ostream & out, const Window & window);
};
#endif //COURSEDESIGN_WINDOW_H
