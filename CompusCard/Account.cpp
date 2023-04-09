#include "Account.h"
#include <utility>
#include <fstream>
#include <algorithm>
#include <sstream>

static const unsigned short num_of_windows = 99;        //窗口总数
//计算hash值
inline int stringHash(std::string & string) { return  string[1]  + string[3]; }
constexpr inline int const_StringHash(const char* string) { return string[1] + string[3]; }
void convert_num(std::string & string, std::vector<unsigned int> & acc_nums);
void convert_name(std::string & string, std::vector<std::string> & strings);


// Card
std::vector<Window> * Card::Member_point_to_windows = nullptr;
std::vector<Card> * Card::Member_point_to_cards = nullptr;
std::set<std::string> Card::given_name;
std::set<std::string> Card::family_name;
std::unordered_multimap<std::string, unsigned int> Card::table_name;
std::unordered_multimap<unsigned int, unsigned int> Card::table_account_num;
std::map<std::string, unsigned int> Card::table_date;
std::multimap<double, unsigned int> Card::table_amount;
std::vector<std::vector<unsigned int> > Card::table_window(num_of_windows);//NOLINT

//Account
std::vector<Card> * Account::Member_point_to_cards = nullptr;
unsigned int Account::SerialNum = 12346;
unsigned int Account::location = 0;
std::vector<operating_record> Account::operating_records;
std::unordered_map<unsigned int, Account> * Account::Member_point_to_accounts;


Account::Account(std::ostream & output, unsigned int stu_num, std::string name): Member_output(output)
{
    Member_password = 8888;
    Member_increased_amount = 0.0;
    Member_num_reissue_card = 0;
    Member_remaining_sum = 0.0;
    Member_last_day = "0";
    Member_last_period = Morning;
    Member_status = true;
    Member_acc_num = stu_num;
    Member_name = std::move(name);
    Member_head =new Node {0, false, nullptr};
    send_card();
    Member_output << "姓名: "<<Member_name << "  学号:" << Member_acc_num << "  开户成功"
    << "  发卡卡号:" << Member_head->card_id << "\n";
}


//开户
void Account::open_account(const char * filename, std::ostream & out)
{
    std::ifstream fin(filename);
    if (!fin.is_open())     std::cout << "打开文件失败";
    unsigned int stu_num;                                        //学号
    std::string name;
    fin.ignore(255, '\n');
    while (fin >> stu_num)                                      //读取学号
    {
        fin.get();
        getline(fin, name, ';');
        fin.get();
        (*Member_point_to_accounts).emplace(std::piecewise_construct, std::forward_as_tuple(stu_num),
                                       std::forward_as_tuple(out, stu_num, name));
    }
    fin.clear();
    fin.close();
}

//销户
bool Account::clear_account()
{
    if (!Member_status)
        return false;
    Member_status = false;
    return true;
}

//计算最终卡号
unsigned int Account::final_card_id(unsigned int card_id)
{
    unsigned int pre = 3000000 + card_id * 10;
    unsigned int final_card_id = pre;
    unsigned int sum = 0;//定义各位数之和
    unsigned int right_num ;//最右边数字
    while (pre > 0)
    {
        right_num = pre % 10;//最右边数字3
        sum = sum + right_num;//各位数相加
        pre = pre / 10;//剔除最右边数字
    }
    final_card_id += 9 - (sum % 10);
    return final_card_id;
}

//发卡
bool Account::send_card()
{
    Node * temp = new (std::nothrow)Node {final_card_id(SerialNum), true, Member_head->next};
    if (!temp)
        return false;
    Member_head = temp;
    (*Member_point_to_cards).emplace_back(this);
    ++SerialNum;
    return true;
}

//挂失
bool Account::report_loss()
{
    if (!Member_status or !Member_head->status)
        return false;
    Member_head->status = false;
    (*Member_point_to_cards)[(Member_head->card_id % 1000000) / 10 - 12346].Member_current_status = false;
    return true;
}

//解挂
bool Account::solute_lossing()
{
    if (!Member_status or Member_head->status)
        return false;
    Member_head->status = true;
    (*Member_point_to_cards)[(Member_head->card_id % 1000000) / 10 - 12346].Member_current_status = true;
    return true;
}

//补卡
bool Account::reissue_card()
{
    if (!Member_status or (Member_num_reissue_card == 100))
        return false;
    report_loss();
    if (!send_card())
        return false;
    Member_num_reissue_card += 1;
    return true;
}

//充值
bool Account::recharge(double money)
{
    if (!Member_status or ((Member_remaining_sum + money) > 999.99))
        return false;
    Member_remaining_sum += money;
    return true;
}

//消费
Account::ReturnResult Account::consumption(double cost, unsigned short password)
{
    if ((Member_remaining_sum - cost) < 0.0)
        return LackAmount;
    if (Member_increased_amount + cost > 20.0)
    {
        if (password != Member_password) return WordError;
        clear_amount();
        Member_remaining_sum -= cost;
        return Succeed;
    }
    Member_remaining_sum -= cost;
    Member_increased_amount += cost;
    return Succeed;
}

//运算符重载
std::ostream & operator<<(std::ostream & out, const Account & student)
{
    out << student.Member_name << ' ' << student.Member_acc_num << ' ' << student.Member_head->card_id
        << ' ' << student.Member_increased_amount << '\n';
    return out;
}


//读取操作记录
void Account::Reading_operating_Information(const char * filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())     std::cout << "打开文件失败";
    std::string time, operating;
    unsigned int acc_num;
    double cost;
    char ch;
    fin.ignore(255, '\n');
    bool flag = true;
    while(flag)
    {
        getline(fin, time, ',');
        getline(fin, operating, ',');
        fin >> acc_num >> ch;
        if (ch != ';')  fin >> cost >> ch;
        else cost = 0;
        fin.get();
        if (fin.eof())  flag = false;
        else operating_records.emplace_back(operating_record{time, operating, acc_num, cost});
    }
    fin.clear();
    fin.close();
    operating_records.emplace_back(operating_record{"3", "none", 0, 0.0});
    //在最后加上结束标志”3“
}

//操作处理
void Account::Access_operating(operating_record & record)
{
    bool operationResult = false;
    switch (stringHash(record.operating))
    {
        case const_StringHash("挂失"):
            operationResult = report_loss();
            break;
        case const_StringHash("解挂"):
            operationResult = solute_lossing();
            break;
        case const_StringHash("补卡"):
            operationResult = reissue_card();
            break;
        case const_StringHash("充值"):
            operationResult = recharge(record.amount);
            break;
        case const_StringHash("销户"):
            operationResult = clear_account();
            break;
        default:
            std::cout << "非法操作";
            break;
    }
    Member_output << record.date << ' ' << record.operating;
    if (!operationResult) Member_output << "失败";
    else Member_output << "成功";
    Member_output << ' ' << record.stu_num << "卡号:" << Member_head->card_id << " ";
    if (record.amount != 0.0) Member_output << "充值金额:"<< record.amount;
    Member_output << '\n';
}

//析构函数
Account::~Account()
{
    Node * pNode;
    while (Member_head)
    {
        pNode = Member_head;
        Member_head = Member_head->next;
        delete pNode;
    }
}



//消费过程
void Card::process_consuming(consuming_record & record, unsigned short password)
{
    unsigned char current_time;
    if (record.time_after < "10") current_time = Account::Morning;
    else if (record.time_after < "14") current_time = Account::Noon;
    else current_time = Account::Evening;
    if (record.time_pre == Member_point_to_student->Member_last_day) {
        if (current_time != Member_point_to_student->Member_last_period) {
            Member_point_to_student->Member_last_period = current_time;
            Member_point_to_student->clear_amount();
        }
    } else {
        Member_point_to_student->Member_last_day = record.time_pre;
        Member_point_to_student->Member_last_period = current_time;
        Member_point_to_student->clear_amount();
    }
    Account::ReturnResult process_result;
    if (!Member_current_status)
        process_result = Account::ReportLoss;
    else if (!(Member_point_to_student->Member_status))
        process_result = Account::SoldOut;
    else
        process_result = Member_point_to_student->consumption(record.amount, password);
    Member_point_to_student->Member_output << record.time_pre + record.time_after << ' ' << "消费";
    if (process_result == Account::Succeed)
        (*Member_point_to_windows)[record.win_num - 1].increase_consumpting_record(record);
    else if (process_result == Account::LackAmount)
        Member_point_to_student->Member_output << "失败 原因:余额不足";
    else if (process_result == Account::WordError)
        Member_point_to_student->Member_output << "失败 原因:密码错误";
    else if (process_result == Account::ReportLoss)
        Member_point_to_student->Member_output << "失败 原因:已挂失 ";
    else
        Member_point_to_student->Member_output << "失败 原因:已销户 ";
    Member_point_to_student->Member_output << " 学号:" << Member_point_to_student->Member_acc_num
                                 << "  卡号:" << card_id << "  消费金额: " << record.amount
                                 << "  消费窗口:" << record.win_num << "\n";
}


//建表
void Card::CreateTable(std::vector<consuming_record> & final_windows_records){
    unsigned int length = final_windows_records.size() - 1;
    for (unsigned int i = 0; i < length; ++i)
    {
        auto student = (*Member_point_to_cards)[(final_windows_records[i].card_id % 1000000 / 10) - 12346].Member_point_to_student;
        int len = (int)student->Member_name.size() / 2;
        family_name.emplace(student->Member_name, 0, 2);
        for (int j = 1; j < len; ++j)
            given_name.emplace(student->Member_name, j * 2, 2);
        table_name.emplace(student->Member_name, i);
        table_account_num.emplace(student->Member_acc_num, i);
        table_date.emplace(final_windows_records[i].time_pre + final_windows_records[i].time_after , i);
        table_amount.emplace(final_windows_records[i].amount, i);
        table_window[final_windows_records[i].win_num].emplace_back(i);
    }
}

//查询
void Card::AllSearch(searching_information & search, std::vector<consuming_record> & resultRecords) {
    std::vector<std::set<unsigned int> > result_table(7);
    std::vector<std::string> names;
    std::vector<unsigned int> acc_nums;

    convert_name(search.name, names);
    convert_num(search.stu_num, acc_nums);
    for (auto & name : names) {
        for (auto pos = table_name.equal_range(name); pos.first != pos.second; ++pos.first)
            result_table[1].emplace(pos.first->second);
    }
    for (auto & num : acc_nums) {
        for (auto pos = table_account_num.equal_range(num); pos.first != pos.second; ++pos.first)
            result_table[2].emplace(pos.first->second);
    }
    auto start_amount = table_amount.lower_bound(search.begin_cost);
    auto end_amount = table_amount.upper_bound(search.end_cost);
    while (start_amount != end_amount) {
        result_table[3].emplace(start_amount->second);
        ++start_amount;
    }
    auto start_date = table_date.lower_bound(search.begin_time);
    auto end_date = table_date.upper_bound(search.end_time);
    while (start_date != end_date) {
        result_table[4].emplace(start_date->second);
        ++start_date;
    }
    if (result_table[1].empty() or result_table[2].empty())
        set_union(result_table[1].begin(), result_table[1].end(), result_table[2].begin(), result_table[2].end(), inserter(result_table[5], result_table[5].begin()));
    else
        set_intersection(result_table[1].begin(), result_table[1].end(), result_table[2].begin(), result_table[2].end(), inserter(result_table[5], result_table[5].begin()));
    if (result_table[3].empty() or result_table[4].empty())
        set_union(result_table[3].begin(), result_table[3].end(), result_table[4].begin(), result_table[4].end(), inserter(result_table[6], result_table[6].begin()));
    else
        set_intersection(result_table[3].begin(), result_table[3].end(), result_table[4].begin(), result_table[4].end(), inserter(result_table[6], result_table[6].begin()));
    if (result_table[5].empty() or result_table[6].empty())
        set_union(result_table[5].begin(), result_table[5].end(), result_table[6].begin(), result_table[6].end(), inserter(result_table[0], result_table[0].begin()));
    else
        set_intersection(result_table[5].begin(), result_table[5].end(), result_table[6].begin(), result_table[6].end(), inserter(result_table[0], result_table[0].begin()));
    for (auto & i : result_table[0])
        resultRecords.emplace_back(Window::final_records[i]);
}


//显示学生信息
account_information Account::ShowStudent() const
{
    account_information accountInformation{};
    accountInformation.stu_num = Member_acc_num;
    accountInformation.status = Member_status;
    accountInformation.remaining_sum = Member_remaining_sum;
    accountInformation.num_reissue_card = Member_num_reissue_card;
    accountInformation.current_increased_costs = Member_increased_amount;
    return accountInformation;
}

//显示卡号
unsigned int Account::show_card_id() const
{
    return Member_head->card_id;
}


//查询模糊学号信息
void Card::SearchAccount(std::string & acc_num, std::vector<account_information> & acc_informations)
{
    std::vector<unsigned int> acc_nums;
    convert_num(acc_num, acc_nums);
    auto last_student = (*Account::Member_point_to_accounts).end();
    for (auto & account: acc_nums)
        if ((*Account::Member_point_to_accounts).find(account) != last_student)
            acc_informations.emplace_back((*Account::Member_point_to_accounts).at(account).ShowStudent());
}


//递归替换学号中的？字符
void convert_num(std::string & string, std::vector<unsigned int> & acc_nums)  // NOLINT
{
    int pos = (int)string.find('?');
    if (pos != std::string::npos) {
        std::string temporary_string;
        for (int i = 0; i <= 9; ++i)
        {
            temporary_string = string;
            temporary_string.replace(pos, 1, 1, char('0' + i));
            convert_num(temporary_string, acc_nums);
        }
    }
    else{
        unsigned int number;
        std::istringstream("2020" + string) >> number;
        acc_nums.emplace_back(number);
    }
}

//递归替换姓名中的？字符
void convert_name(std::string & string, std::vector<std::string> & strings)   // NOLINT
{
    int pos = (int)string.find("？");
    if (pos != std::string::npos) {
        std::string tmp_str;
        auto set_name = &Card::given_name;
        if (pos == 0) set_name = &Card::family_name;
        for (auto & name : *set_name)
        {
            tmp_str = string;
            tmp_str.replace(pos, 2, name);
            convert_name(tmp_str, strings);
        }
    }
    else{
        strings.emplace_back(string);
    }
}

