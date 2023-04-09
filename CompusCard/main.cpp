#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <unordered_map>
#include "Account.h"
#include "Window.h"

int main() {
    using namespace std;
    cout <<"          ------------------------欢迎使用学生校园卡管理系统----------------------------"
    << endl;
    cout <<"请选择使用功能:\n"
    <<"0.退出  " << "1.批量开户  " << "2.批量操作  " <<"3.充值第一阶段  " << "4.挂失补卡\n" <<"5.充值消费  "
    << "6.汇总分析过程  " << "7.查询操作  " << "8.单项操作  \n" <<"你所选择的功能为:";
    int choice;
    cin >> choice;
    ofstream fout("operation_log.txt");

    //准备工作
    unordered_map<unsigned int, Account> accounts;
    vector<Window> windows;
    vector<Card> cards;

    Account::bulid_Member_p_to_accounts(&accounts);
    Account::build_Member_p_to_cards(&cards);
    Card::bulid_Member_p_to_windows(&windows);
    Card::build_Member_p_to_cards(&cards);
    Window::build_Member_point_to_windows(&windows);

    Window::read_window_wz("wz003.txt");
    Window::process_consumption_records("xf014.txt");

    Account::Reading_operating_Information("cz002.txt");

    cout << "\n";
    while(choice != 0){
        switch(choice)
        {
            case 1:
            {
                fout  << "2021年09月01日" << "\t开户\n";
                Account::open_account("kh001.txt", fout);
                fout << "开户完成\n\n";
                cout <<"批量开户结束\n\n";
                break;
            }
            case 2:
            {
                fout << "2021年09月03日" << "\t挂失/解挂/销户\n";
                operating_record record = Account::operating_records[Account::location];
                while (record.date < "20210904")
                {
                    accounts.at(record.stu_num).Access_operating(record);
                    record = Account::operating_records[++Account::location];
                }
                fout << "挂失/解挂/销户完成\n\n";
                cout <<"批量操作结束\n\n";
                break;
            }
            case 3:
            {
                fout << "2021年09月04日全天" << "\t充值第一阶段\n";
                operating_record record = Account::operating_records[Account::location];
                while (record.date < "20210906")
                {
                    accounts.at(record.stu_num).Access_operating(record);
                    record = Account::operating_records[++Account::location];
                }
                fout << "充值第一阶段完成\n\n";
                cout <<"充值第一阶段结束\n\n";
                break;
            }
            case 4:
            {
                fout  << "2021年09月06日" << "\t(挂失)补卡\n";
                operating_record record = Account::operating_records[Account::location];
                while (record.date < "20210923")
                {
                    accounts.at(record.stu_num).Access_operating(record);
                    record = Account::operating_records[++Account::location];
                }
                fout << "(挂失)补卡完成\n\n";
                cout <<"挂失补卡结束\n\n";
                break;
            }
            case 5:
            {
                // 消费(1)
                fout  << "2021年09月23日" << "\t消费第一阶段\n";
                consuming_record record1 = Window::ordered_records[Window::location];
                while (record1.time_pre < "20211014")
                {
                    cards[(record1.card_id % 1000000 / 10) - 12346].process_consuming(record1);
                    record1 = Window::ordered_records[++Window::location];
                }
                fout << "消费第一阶段完成\n\n";

                // 充值(2)
                fout  << "2021年10月14日凌晨" << "\t充值第二阶段\n";
                operating_record record2 = Account::operating_records[Account::location];
                while (record2.date < "20211104")
                {
                    accounts.at(record2.stu_num).Access_operating(record2);
                    record2 = Account::operating_records[++Account::location];
                }
                fout << "充值第二阶段完成\n\n";

                // 消费(2)
                fout  << "2021年10月14日" << "\t消费第二阶段\n";
                consuming_record record3 = Window::ordered_records[Window::location];
                while (record3.time_pre < "20211104")
                {
                    cards[(record3.card_id % 1000000 / 10) - 12346].process_consuming(record3);
                    record3 = Window::ordered_records[++Window::location];
                }
                fout << "消费第二阶段完成\n\n";

                // 充值(3)
                fout  << "2021年11月04日凌晨" << "\t充值第三阶段\n";
                operating_record record4 = Account::operating_records[Account::location];
                while (record4.date < "20211125")
                {
                    accounts.at(record4.stu_num).Access_operating(record4);
                    record4 = Account::operating_records[++Account::location];
                }
                fout << "充值第三阶段完成\n\n";

                // 消费(3)
                fout  << "2021年11月04日" << "\t消费第三阶段\n";
                consuming_record record5 = Window::ordered_records[Window::location];
                while (record5.time_pre < "20211125")
                {
                    cards[(record5.card_id % 1000000 / 10) - 12346].process_consuming(record5);
                    record5 = Window::ordered_records[++Window::location];
                }
                fout << "消费第三阶段完成\n\n";

                // 充值(4)
                fout  << "2021年11月25日凌晨" << "\t充值第四阶段\n";
                operating_record record6 = Account::operating_records[Account::location];
                while (record6.date < "20211216")
                {
                    accounts.at(record6.stu_num).Access_operating(record6);
                    record6 = Account::operating_records[++Account::location];
                }
                fout << "充值第四阶段完成\n\n";


                // 消费(4)
                fout  << "2021年11月25日" << "\t消费第四阶段\n";
                consuming_record record7 = Window::ordered_records[Window::location];
                while (record7.time_pre < "20211216")
                {
                    cards[(record7.card_id % 1000000 / 10) - 12346].process_consuming(record7);
                    record7 = Window::ordered_records[++Window::location];
                }
                fout << "消费第四阶段完成\n\n";

                // 充值(5)
                fout  << "2021年12月16日凌晨" << "\t充值第五阶段\n";
                operating_record record8 = Account::operating_records[Account::location];
                while (record8.date < "20211231")
                {
                    accounts.at(record8.stu_num).Access_operating(record8);
                    record8 = Account::operating_records[++Account::location];
                }
                fout << "充值第五阶段完成\n\n";

                //消费(5)
                fout  << "2021年12月16日" << "\t消费第五阶段\n";
                consuming_record record9 = Window::ordered_records[Window::location];
                while (record9.time_pre < "20220101")
                {
                    cards[(record9.card_id % 1000000 / 10) - 12346].process_consuming(record9);
                    record9 = Window::ordered_records[++Window::location];
                }
                fout << "消费第五阶段完成\n\n";

                std::cout <<"批量充值消费结束\n\n";
                break;
            }
            case 6:
            {
                time_t start1, end1;
                time(&start1);
                Window::K_Merge();
                time(&end1);
                cout <<"99路归并排序所需时间为:"
                <<difftime(end1, start1) << endl;

                time_t start2, end2;
                time(&start2);
                Window::Quick();
                time(&end2);
                cout <<"快速排序所需时间为:"
                << difftime(end2, start2) << endl;

                Card::CreateTable(Window::final_records);
                cout <<"汇总过程结束\n\n";
                break;
            }
            case 7:
            {
                cout <<"请选择查询功能:\n0.退出查询  "<< "1.按学号模糊查询账户信息   "
                << "2.组合查询并显示当前累计金额   " << "3.查询指定学生余额\n";
                int choice1;
                cout <<"您选择的查询功能为:";
                cin >> choice1;
                while(choice1 != 0)
                {
                    switch (choice1)
                    {
                        case 1:
                        {
                            cout <<"请输入需要查询的学号(支持模糊查询):\n";
                            string stu_num, ch;
                            cin >> ch;
                            stu_num = ch.substr(4);
                            vector<account_information> account_records;
                            Card::SearchAccount(stu_num,account_records);
                            for(auto & record : account_records)
                            {
                                cout <<"学号:" << record.stu_num;
                                if(record.status == 1)
                                    cout << " 账户状态:正常 ";
                                else
                                    cout << " 账户状态:异常 ";
                                cout << "账户余额:" << record.remaining_sum << "  补卡次数:" << int(record.num_reissue_card)
                                     << " 当前时间段累计消费金额:"
                                     << record.current_increased_costs << endl;
                            }
                            break;
                        }
                        case 2:
                        {
                            cout << "请依次输入开始日期、截止日期、开始金额、截止金额、姓名、学号(支持姓名,学号模糊匹配):\n";
                            double start_cost, end_cost;
                            string begin_time, end_time, stu_name, stu_num, ch;
                            cin >> begin_time >> end_time >> start_cost >> end_cost >> stu_name >> ch;
                            stu_num = ch.substr(4);
                            vector<consuming_record> result_records;
                            searching_information search = {begin_time, end_time, start_cost
                                                   , end_cost, stu_name, stu_num};
                            Card::AllSearch(search, result_records);
                            double sum_account = 0.0;
                            for(auto & record : result_records)
                            {
                                cout << "日期:" << record.time_pre << record.time_after
                                     << " 卡号:" << record.card_id << " 消费金额:"
                                     << record.amount << "   消费窗口:" << record.win_num << endl;
                                sum_account += record.amount;
                            }
                            cout << "累积金额为:" << sum_account << endl;
                            break;
                        }
                        case 3:
                        {   
                            cout <<"请输入需要查询的学号:\n";
                            string num;
                            unsigned int stu_num;
                            cin >> num;
                            std::istringstream(num) >> stu_num;
                            account_information record = accounts.at(stu_num).ShowStudent();
                            cout << "学号:"
                                 << record.stu_num << " 余额:" << record.remaining_sum << endl;
                            break;
                        }
                        default:
                            break;
                    }
                    cout <<"请继续选择查询功能:";
                    cin >> choice1;
                }
                cout << "查询结束\n\n";
                break;
            }
            case 8:
            {
                cout <<"请选择单项操作的功能:\n0.退出单项操作   "<<"1.开户发卡  "
                << "2.挂失 \n" << "3.解挂 " << "4.补卡 " << "5.销户 " << "6.充值 " << "7.消费\n";
                int choice2;
                cout << "您所选择的单项操作的功能为:";
                cin >> choice2;
                cout << "\n";
                while(choice2 != 0){
                    switch (choice2) {
                        case 1:
                        {
                            unsigned int student_num;
                            string student_name;
                            cout << "请依次输入学号,姓名:\n";
                            cin >> student_num >> student_name;
                            accounts.emplace(std::piecewise_construct, std::forward_as_tuple(student_num),
                                             std::forward_as_tuple(fout, student_num, student_name));
                            cout << "开户成功 " << "卡号为:" << accounts.at(student_num).show_card_id() << "\n";
                            break;
                        }
                        case 2:
                        {
                            string time;
                            unsigned int student_num;
                            cout << "请依次输入日期、学号:\n";
                            cin >> time >> student_num;
                            operating_record record{time, "挂失", student_num, 0.0};
                            accounts.at(student_num).Access_operating(record);
                            cout << "挂失完成\n";
                            break;
                        }
                        case 3:
                        {
                            string time;
                            unsigned int student_num;
                            cout <<"请依次输入日期、学号:\n";
                            cin >> time >> student_num;
                            operating_record record{time, "解挂", student_num, 0.0};
                            accounts.at(student_num).Access_operating(record);
                            cout << "解挂完成\n";
                            break;
                        }
                        case 4:
                        {
                            string time;
                            unsigned int student_num;
                            cout <<"请依次输入日期、学号:\n";
                            cin >> time >> student_num;
                            operating_record record{time, "补卡", student_num, 0.0};
                            accounts.at(student_num).Access_operating(record);
                            cout << "补卡完成" << "  新卡号:"
                                 << accounts.at(record.stu_num).show_card_id() << endl;
                            break;
                        }
                        case 5:
                        {
                            string time;
                            unsigned int student_num;
                            cout <<"请依次输入日期、学号:\n";
                            cin >> time >> student_num;
                            operating_record record{time, "销户", student_num, 0.0};
                            accounts.at(student_num).Access_operating(record);
                            cout <<"销户完成\n";
                            break;
                        }
                        case 6:
                        {
                            string time;
                            unsigned int student_num;
                            double amount;
                            cout <<"请依次输入日期、学号、充值金额:\n";
                            cin >> time >> student_num >> amount;
                            operating_record record{time, "充值", student_num, amount};
                            accounts.at(student_num).Access_operating(record);
                            cout <<"充值完成\n";
                            break;
                        }
                        case 7:
                        {
                            string date1,date2;
                            unsigned short window;
                            unsigned int student_card_id;
                            double amount;
                            unsigned short password;
                            cout <<"请依次输入窗口号、年月日、时分秒、卡号、消费金额:\n";
                            cin >> window >> date1 >>date2 >> student_card_id >> amount;
                            if(amount > 20.0)
                            {
                                cout <<"请输入密码:";
                                cin >> password;
                            } else password = 8888;
                            consuming_record record{window, date1,
                                                    date2, student_card_id, amount};
                            cards[(record.card_id % 1000000 / 10) - 12346].process_consuming(record, password);
                            cout <<"消费结束\n";
                            break;
                        }
                        default:
                            break;
                    }
                    cout <<"请继续选择单项操作的功能:";
                    cin >> choice2;
                }
                cout <<"单项操作结束\n\n";
                break;
            }
            default:
                break;
        }
        cout <<"请继续选择功能   " << "你所选择的功能为:";
        cin >> choice;
    }
    cout <<"          ----------------------------退出系统成功----------------------------";
    fout.close();
    return 0;
}

