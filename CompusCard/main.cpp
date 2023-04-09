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
    cout <<"          ------------------------��ӭʹ��ѧ��У԰������ϵͳ----------------------------"
    << endl;
    cout <<"��ѡ��ʹ�ù���:\n"
    <<"0.�˳�  " << "1.��������  " << "2.��������  " <<"3.��ֵ��һ�׶�  " << "4.��ʧ����\n" <<"5.��ֵ����  "
    << "6.���ܷ�������  " << "7.��ѯ����  " << "8.�������  \n" <<"����ѡ��Ĺ���Ϊ:";
    int choice;
    cin >> choice;
    ofstream fout("operation_log.txt");

    //׼������
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
                fout  << "2021��09��01��" << "\t����\n";
                Account::open_account("kh001.txt", fout);
                fout << "�������\n\n";
                cout <<"������������\n\n";
                break;
            }
            case 2:
            {
                fout << "2021��09��03��" << "\t��ʧ/���/����\n";
                operating_record record = Account::operating_records[Account::location];
                while (record.date < "20210904")
                {
                    accounts.at(record.stu_num).Access_operating(record);
                    record = Account::operating_records[++Account::location];
                }
                fout << "��ʧ/���/�������\n\n";
                cout <<"������������\n\n";
                break;
            }
            case 3:
            {
                fout << "2021��09��04��ȫ��" << "\t��ֵ��һ�׶�\n";
                operating_record record = Account::operating_records[Account::location];
                while (record.date < "20210906")
                {
                    accounts.at(record.stu_num).Access_operating(record);
                    record = Account::operating_records[++Account::location];
                }
                fout << "��ֵ��һ�׶����\n\n";
                cout <<"��ֵ��һ�׶ν���\n\n";
                break;
            }
            case 4:
            {
                fout  << "2021��09��06��" << "\t(��ʧ)����\n";
                operating_record record = Account::operating_records[Account::location];
                while (record.date < "20210923")
                {
                    accounts.at(record.stu_num).Access_operating(record);
                    record = Account::operating_records[++Account::location];
                }
                fout << "(��ʧ)�������\n\n";
                cout <<"��ʧ��������\n\n";
                break;
            }
            case 5:
            {
                // ����(1)
                fout  << "2021��09��23��" << "\t���ѵ�һ�׶�\n";
                consuming_record record1 = Window::ordered_records[Window::location];
                while (record1.time_pre < "20211014")
                {
                    cards[(record1.card_id % 1000000 / 10) - 12346].process_consuming(record1);
                    record1 = Window::ordered_records[++Window::location];
                }
                fout << "���ѵ�һ�׶����\n\n";

                // ��ֵ(2)
                fout  << "2021��10��14���賿" << "\t��ֵ�ڶ��׶�\n";
                operating_record record2 = Account::operating_records[Account::location];
                while (record2.date < "20211104")
                {
                    accounts.at(record2.stu_num).Access_operating(record2);
                    record2 = Account::operating_records[++Account::location];
                }
                fout << "��ֵ�ڶ��׶����\n\n";

                // ����(2)
                fout  << "2021��10��14��" << "\t���ѵڶ��׶�\n";
                consuming_record record3 = Window::ordered_records[Window::location];
                while (record3.time_pre < "20211104")
                {
                    cards[(record3.card_id % 1000000 / 10) - 12346].process_consuming(record3);
                    record3 = Window::ordered_records[++Window::location];
                }
                fout << "���ѵڶ��׶����\n\n";

                // ��ֵ(3)
                fout  << "2021��11��04���賿" << "\t��ֵ�����׶�\n";
                operating_record record4 = Account::operating_records[Account::location];
                while (record4.date < "20211125")
                {
                    accounts.at(record4.stu_num).Access_operating(record4);
                    record4 = Account::operating_records[++Account::location];
                }
                fout << "��ֵ�����׶����\n\n";

                // ����(3)
                fout  << "2021��11��04��" << "\t���ѵ����׶�\n";
                consuming_record record5 = Window::ordered_records[Window::location];
                while (record5.time_pre < "20211125")
                {
                    cards[(record5.card_id % 1000000 / 10) - 12346].process_consuming(record5);
                    record5 = Window::ordered_records[++Window::location];
                }
                fout << "���ѵ����׶����\n\n";

                // ��ֵ(4)
                fout  << "2021��11��25���賿" << "\t��ֵ���Ľ׶�\n";
                operating_record record6 = Account::operating_records[Account::location];
                while (record6.date < "20211216")
                {
                    accounts.at(record6.stu_num).Access_operating(record6);
                    record6 = Account::operating_records[++Account::location];
                }
                fout << "��ֵ���Ľ׶����\n\n";


                // ����(4)
                fout  << "2021��11��25��" << "\t���ѵ��Ľ׶�\n";
                consuming_record record7 = Window::ordered_records[Window::location];
                while (record7.time_pre < "20211216")
                {
                    cards[(record7.card_id % 1000000 / 10) - 12346].process_consuming(record7);
                    record7 = Window::ordered_records[++Window::location];
                }
                fout << "���ѵ��Ľ׶����\n\n";

                // ��ֵ(5)
                fout  << "2021��12��16���賿" << "\t��ֵ����׶�\n";
                operating_record record8 = Account::operating_records[Account::location];
                while (record8.date < "20211231")
                {
                    accounts.at(record8.stu_num).Access_operating(record8);
                    record8 = Account::operating_records[++Account::location];
                }
                fout << "��ֵ����׶����\n\n";

                //����(5)
                fout  << "2021��12��16��" << "\t���ѵ���׶�\n";
                consuming_record record9 = Window::ordered_records[Window::location];
                while (record9.time_pre < "20220101")
                {
                    cards[(record9.card_id % 1000000 / 10) - 12346].process_consuming(record9);
                    record9 = Window::ordered_records[++Window::location];
                }
                fout << "���ѵ���׶����\n\n";

                std::cout <<"������ֵ���ѽ���\n\n";
                break;
            }
            case 6:
            {
                time_t start1, end1;
                time(&start1);
                Window::K_Merge();
                time(&end1);
                cout <<"99·�鲢��������ʱ��Ϊ:"
                <<difftime(end1, start1) << endl;

                time_t start2, end2;
                time(&start2);
                Window::Quick();
                time(&end2);
                cout <<"������������ʱ��Ϊ:"
                << difftime(end2, start2) << endl;

                Card::CreateTable(Window::final_records);
                cout <<"���ܹ��̽���\n\n";
                break;
            }
            case 7:
            {
                cout <<"��ѡ���ѯ����:\n0.�˳���ѯ  "<< "1.��ѧ��ģ����ѯ�˻���Ϣ   "
                << "2.��ϲ�ѯ����ʾ��ǰ�ۼƽ��   " << "3.��ѯָ��ѧ�����\n";
                int choice1;
                cout <<"��ѡ��Ĳ�ѯ����Ϊ:";
                cin >> choice1;
                while(choice1 != 0)
                {
                    switch (choice1)
                    {
                        case 1:
                        {
                            cout <<"��������Ҫ��ѯ��ѧ��(֧��ģ����ѯ):\n";
                            string stu_num, ch;
                            cin >> ch;
                            stu_num = ch.substr(4);
                            vector<account_information> account_records;
                            Card::SearchAccount(stu_num,account_records);
                            for(auto & record : account_records)
                            {
                                cout <<"ѧ��:" << record.stu_num;
                                if(record.status == 1)
                                    cout << " �˻�״̬:���� ";
                                else
                                    cout << " �˻�״̬:�쳣 ";
                                cout << "�˻����:" << record.remaining_sum << "  ��������:" << int(record.num_reissue_card)
                                     << " ��ǰʱ����ۼ����ѽ��:"
                                     << record.current_increased_costs << endl;
                            }
                            break;
                        }
                        case 2:
                        {
                            cout << "���������뿪ʼ���ڡ���ֹ���ڡ���ʼ����ֹ��������ѧ��(֧������,ѧ��ģ��ƥ��):\n";
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
                                cout << "����:" << record.time_pre << record.time_after
                                     << " ����:" << record.card_id << " ���ѽ��:"
                                     << record.amount << "   ���Ѵ���:" << record.win_num << endl;
                                sum_account += record.amount;
                            }
                            cout << "�ۻ����Ϊ:" << sum_account << endl;
                            break;
                        }
                        case 3:
                        {   
                            cout <<"��������Ҫ��ѯ��ѧ��:\n";
                            string num;
                            unsigned int stu_num;
                            cin >> num;
                            std::istringstream(num) >> stu_num;
                            account_information record = accounts.at(stu_num).ShowStudent();
                            cout << "ѧ��:"
                                 << record.stu_num << " ���:" << record.remaining_sum << endl;
                            break;
                        }
                        default:
                            break;
                    }
                    cout <<"�����ѡ���ѯ����:";
                    cin >> choice1;
                }
                cout << "��ѯ����\n\n";
                break;
            }
            case 8:
            {
                cout <<"��ѡ��������Ĺ���:\n0.�˳��������   "<<"1.��������  "
                << "2.��ʧ \n" << "3.��� " << "4.���� " << "5.���� " << "6.��ֵ " << "7.����\n";
                int choice2;
                cout << "����ѡ��ĵ�������Ĺ���Ϊ:";
                cin >> choice2;
                cout << "\n";
                while(choice2 != 0){
                    switch (choice2) {
                        case 1:
                        {
                            unsigned int student_num;
                            string student_name;
                            cout << "����������ѧ��,����:\n";
                            cin >> student_num >> student_name;
                            accounts.emplace(std::piecewise_construct, std::forward_as_tuple(student_num),
                                             std::forward_as_tuple(fout, student_num, student_name));
                            cout << "�����ɹ� " << "����Ϊ:" << accounts.at(student_num).show_card_id() << "\n";
                            break;
                        }
                        case 2:
                        {
                            string time;
                            unsigned int student_num;
                            cout << "�������������ڡ�ѧ��:\n";
                            cin >> time >> student_num;
                            operating_record record{time, "��ʧ", student_num, 0.0};
                            accounts.at(student_num).Access_operating(record);
                            cout << "��ʧ���\n";
                            break;
                        }
                        case 3:
                        {
                            string time;
                            unsigned int student_num;
                            cout <<"�������������ڡ�ѧ��:\n";
                            cin >> time >> student_num;
                            operating_record record{time, "���", student_num, 0.0};
                            accounts.at(student_num).Access_operating(record);
                            cout << "������\n";
                            break;
                        }
                        case 4:
                        {
                            string time;
                            unsigned int student_num;
                            cout <<"�������������ڡ�ѧ��:\n";
                            cin >> time >> student_num;
                            operating_record record{time, "����", student_num, 0.0};
                            accounts.at(student_num).Access_operating(record);
                            cout << "�������" << "  �¿���:"
                                 << accounts.at(record.stu_num).show_card_id() << endl;
                            break;
                        }
                        case 5:
                        {
                            string time;
                            unsigned int student_num;
                            cout <<"�������������ڡ�ѧ��:\n";
                            cin >> time >> student_num;
                            operating_record record{time, "����", student_num, 0.0};
                            accounts.at(student_num).Access_operating(record);
                            cout <<"�������\n";
                            break;
                        }
                        case 6:
                        {
                            string time;
                            unsigned int student_num;
                            double amount;
                            cout <<"�������������ڡ�ѧ�š���ֵ���:\n";
                            cin >> time >> student_num >> amount;
                            operating_record record{time, "��ֵ", student_num, amount};
                            accounts.at(student_num).Access_operating(record);
                            cout <<"��ֵ���\n";
                            break;
                        }
                        case 7:
                        {
                            string date1,date2;
                            unsigned short window;
                            unsigned int student_card_id;
                            double amount;
                            unsigned short password;
                            cout <<"���������봰�ںš������ա�ʱ���롢���š����ѽ��:\n";
                            cin >> window >> date1 >>date2 >> student_card_id >> amount;
                            if(amount > 20.0)
                            {
                                cout <<"����������:";
                                cin >> password;
                            } else password = 8888;
                            consuming_record record{window, date1,
                                                    date2, student_card_id, amount};
                            cards[(record.card_id % 1000000 / 10) - 12346].process_consuming(record, password);
                            cout <<"���ѽ���\n";
                            break;
                        }
                        default:
                            break;
                    }
                    cout <<"�����ѡ��������Ĺ���:";
                    cin >> choice2;
                }
                cout <<"�����������\n\n";
                break;
            }
            default:
                break;
        }
        cout <<"�����ѡ����   " << "����ѡ��Ĺ���Ϊ:";
        cin >> choice;
    }
    cout <<"          ----------------------------�˳�ϵͳ�ɹ�----------------------------";
    fout.close();
    return 0;
}

