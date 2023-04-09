#ifndef COURSEDESIGN_WINDOW_H
#define COURSEDESIGN_WINDOW_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <sstream>

// structure
//消费记录
struct consuming_record {
    unsigned short win_num;      //窗口号
    std::string time_pre, time_after;  //日期
    unsigned int card_id;              //卡号
    double amount;                      //消费金额
};

//操作记录
struct operating_record {
    std::string date, operating;         //日期，操作
    unsigned int stu_num;        //学号
    double amount;                      //0
};

//搜索信息
struct searching_information {
    std::string begin_time, end_time;
    double begin_cost, end_cost;
    std::string name;
    std::string stu_num;
};

//账户信息
struct account_information{
    unsigned int stu_num;                    //学号
    bool status;                            //校园卡状态（开户销户）
    double remaining_sum;                //卡中余额
    unsigned char num_reissue_card;        //补卡次数
    double current_increased_costs;     //当前时间段累计消费金额
};
std::ostream & operator<<(std::ostream & out, const operating_record & record);
std::ostream & operator<<(std::ostream & out, const consuming_record & record);

//Window
class Window
{
private:
    static const unsigned int MaxAmount = 60000;             //最大记录数60000
    static std::vector<Window> * Member_point_to_windows;
    static std::vector<std::vector<consuming_record> > consume_records;   //消费记录

    unsigned short Member_window_number;              //窗口号
    unsigned int Member_current_times;                    //当天累计消费次数
    double Member_current_costs;                         //当天累计消费金额
    std::string Member_last_day;                      //上一次
    unsigned int Member_begin;                       //消费起始记录位置
    unsigned int Member_end;                         //消费终止记录位置
    std::vector<consuming_record> Member_consumption_records;          //消费记录

    void increase_consumpting_record(consuming_record & record);    //插入消费记录

public:
    explicit Window(unsigned short windowNumber = 0, unsigned int consuming_location = 0);

    static unsigned int location;
    static std::vector<consuming_record>  final_records;    //最终各窗口消费记录汇总
    static std::vector<consuming_record> ordered_records;       //排序过的消费记录
    static std::vector<consuming_record>  final_quicksort_records;

    static void process_consumption_records(const char * filename);                //处理消费记录
    static void read_window_wz(const char * filename);           //读取文件位置
    static void build_Member_point_to_windows(std::vector<Window> * point_to_windows) { Member_point_to_windows = point_to_windows; };
    static void K_Merge();                //K路归并
    static void Quick();

    friend class Card;
    friend std::ostream & operator<<(std::ostream & out, const Window & window);
};
#endif //COURSEDESIGN_WINDOW_H
