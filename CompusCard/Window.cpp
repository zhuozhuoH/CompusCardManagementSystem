#include "Window.h"
#include <fstream>

static const unsigned short num_windows = 99;

std::ostream & operator<<(std::ostream & out, const consuming_record & record)
{
    out << record.card_id << ' ' << record.time_pre << ' ' << record.time_after << ' '
        << record.amount << ' ' << record.win_num << '\t';
    return out;
}

unsigned int Window::location = 0;
std::vector<Window> * Window::Member_point_to_windows = nullptr;
std::vector<consuming_record>  Window::final_records;
std::vector<consuming_record>  Window::final_quicksort_records;
std::vector<std::vector<consuming_record> > Window::consume_records(num_windows);   /* NOLINT */
std::vector<consuming_record> Window::ordered_records;

//��·�鲢(���ð�����)
static const unsigned short k = num_windows;
static const char * maxkey = "3";            //�������ֵ
static const char * minkey = "0";            //������Сֵ
typedef std::string KeyType;
typedef std::vector<std::vector<consuming_record> > Input;
typedef std::vector<consuming_record> Output;
typedef unsigned short LoserTree[k];  //����������ȫ�������Ҳ���Ҷ�ӣ��ɲ���˳��洢�ṹ
struct ExNode { KeyType key1; KeyType key2; };
typedef ExNode External[k+1];    //���㣬ֻ��Ŵ��鲢��¼�Ĺؼ���

//���������
inline bool operator>(const ExNode & exNode1,const ExNode & exNode2)
{
    return (exNode1.key1 + exNode1.key2) > (exNode2.key1 + exNode2.key2);
}
void K_Merge(Input & input_records, Output & output_records);
void Adjust(LoserTree &ls, External &b, unsigned short s);
void CreateLoserTree(LoserTree &ls, External &b);
void input(ExNode &key, consuming_record & record);
void output(Output & output_records, consuming_record & record);

Window::Window(unsigned short windowNumber, unsigned int consuming_location)
        : Member_window_number(windowNumber), Member_current_times(0), Member_current_costs(0), Member_consumption_records(MaxAmount + 1),
          Member_begin(consuming_location), Member_end(consuming_location - 1), Member_last_day("0") {}

std::ostream & operator<<(std::ostream & out, const Window & window)
{
    out << window.Member_window_number << ' ' << window.Member_begin << '\n';
    return out;
}

//��ȡ�����Ѵ���λ��read_window_consumption_location
void Window::read_window_wz(const char * filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())     std::cout << "���ļ�ʧ��";
    unsigned short window_num;
    unsigned int start_wz;
    char ch;
    fin.ignore(255, '\n');
    while (fin >> window_num >> ch >> start_wz >> ch)
        (*Member_point_to_windows).emplace_back(window_num, start_wz);
    fin.clear();
    fin.close();
    fin.clear();
}

//�������Ѽ�¼
void Window::process_consumption_records(const char * filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) std::cout << "���ļ�ʧ��";
    unsigned int card_id;
    std::string date_pre, date_after;
    double amount;
    unsigned short window_number = 0;
    const unsigned short WindowNum = num_windows;
    while ((window_number <= WindowNum) and !fin.eof())
    {
        fin.clear();
        fin.ignore(255, '\n');
        while (fin >> card_id)
        {
            fin.get();
            getline(fin, date_pre, ',');
            getline(fin, date_after, ',');
            fin >> amount;
            fin.get();
            fin.get();
            consume_records[window_number - 1].emplace_back(
                    consuming_record{window_number, date_pre, date_after, card_id, amount });
        }
        window_number++;
    }
    for (auto & line_record : consume_records)
        line_record.emplace_back(consuming_record{0, "3", "0", 0, 0.0});

    //��·�鲢����
    {
        ::K_Merge(consume_records, ordered_records);
    }
    fin.clear();
    fin.close();
}

//�������Ѽ�¼
void Window::increase_consumpting_record(consuming_record & record) {
    //ʵ��ѭ������
    Member_end = (Member_end + 1) % (MaxAmount + 1);

    Member_consumption_records[Member_end] = record;
    if ((Member_end + 1) % (MaxAmount + 1) == Member_begin)
        Member_begin = (Member_begin + 1) % (MaxAmount + 1);
    if (record.time_pre != Member_last_day) {        //��Ҫ��ת����
        Member_last_day = record.time_pre;
        Member_current_costs = record.amount;
        Member_current_times = 1;
    }
    else{
        Member_current_costs += record.amount;
        Member_current_times += 1;
    }
}

//�鲢������뺯��
inline void input(ExNode &key, consuming_record & record)
{
    key.key1 = record.time_pre, key.key2 = record.time_after;
}

//�鲢�����������
inline void output(Output & output_records, consuming_record & record)
{
    output_records.emplace_back(record);
}

//���ѿ�ʼǰ����ʱ���������
void K_Merge(Input & input_records, Output & output_records)
{//���ð�����ls����Ŵ�0��k-1��k������鲢���еļ�¼�鲢������鲢��
    // b[0]��b[k-1]Ϊ�������ϵ�k��Ҷ�ӽ�㣬�ֱ���k������鲢���е�ǰ��¼�Ĺؼ���
    LoserTree ls;
    External b;
    unsigned long index[k] = {0};
    unsigned short q;
    for(unsigned short i = 0; i < k; ++i)               //�ֱ��k������鲢�ζ���öε�ǰ��һ����¼�Ĺؼ��ֵ�����
        input(b[i], input_records[i][index[i]]);
    CreateLoserTree(ls, b);                //��������ls��ѡ����С�ؼ���Ϊb[ls[0]].key
    while(b[ls[0]].key1 != maxkey){
        q = ls[0];                      //qָʾ��ǰ��С�ؼ������ڹ鲢��
        output(output_records, input_records[q][index[q]]);//�����Ϊq�Ĺ鲢���е�ǰ���ؼ���Ϊb[q].key���ļ�¼
        ++index[q];
        //д������鲢��
        input(b[q], input_records[q][index[q]]);             //�ӱ��Ϊq������鲢���ж�����һ����¼�Ĺؼ���
        Adjust(ls, b, q);                  //������������ѡ���µ���С�ؼ���
    }// while
    output(output_records, input_records[ls[0]][index[ls[0]]]);                      //�������ؼ���MAXKEY�ļ�¼д������鲢��
}// K_Merge

//�����ѳɹ��ļ�¼�����������
void Window::K_Merge() {
    LoserTree ls;
    External b;
    unsigned long index[k] = {0};
    unsigned short q;
    for(unsigned short i = 0; i < k; ++i) //�ֱ��k������鲢�ζ���öε�ǰ��һ����¼�Ĺؼ��ֵ�����
    {
        (*Member_point_to_windows)[i].Member_consumption_records[((*Member_point_to_windows)[i].Member_end + 1) % (MaxAmount + 1)].time_pre = maxkey;
        index[i] = (*Member_point_to_windows)[i].Member_begin;
        input(b[i], (*Member_point_to_windows)[i].Member_consumption_records[index[i]]);
    }
    CreateLoserTree(ls, b);                //��������ls��ѡ����С�ؼ���Ϊb[ls[0]].key
    while(b[ls[0]].key1 != maxkey){
        q = ls[0];                      //qָʾ��ǰ��С�ؼ������ڹ鲢��
        output(final_records, (*Member_point_to_windows)[q].Member_consumption_records[index[q]]);//�����Ϊq�Ĺ鲢���е�ǰ���ؼ���Ϊb[q].key���ļ�¼
        index[q] = (index[q] + 1) % (MaxAmount + 1);
        //д������鲢��
        input(b[q], (*Member_point_to_windows)[q].Member_consumption_records[index[q]]);             //�ӱ��Ϊq������鲢���ж�����һ����¼�Ĺؼ���
        Adjust(ls, b, q);                  //������������ѡ���µ���С�ؼ���
    }// while
    output(final_records, (*Member_point_to_windows)[ls[0]].Member_consumption_records[index[ls[0]]]);                   //�������ؼ���MAXKEY�ļ�¼д������鲢
}

//��������
void Adjust(LoserTree &ls, External &b, unsigned short s)
{//�ش�Ҷ�ӽ��b[s]�����ڵ�ls[0]��·������������
    unsigned short t = (s + k) / 2;                      //ls[t]��b[s]��˫�׽��
    unsigned short temp;
    while(t > 0){
        if(b[s] > b[ls[t]])
            temp = s, s = ls[t], ls[t] = temp;// sָʾ�µ�ʤ��
        t = t / 2;
    }
    ls[0] = s;
}// Adjust

void CreateLoserTree(LoserTree &ls, External &b)
{//��֪b[0]��b[k-1]Ϊ��ȫ������ls��Ҷ�ӽ�����k���ؼ��֣��ش�Ҷ�ӵ�����k��·����ls������Ϊ������
    b[k].key1 = minkey;     //��MINKEYΪ�ؼ��ֿ��ܵ���Сֵ
    short i;
    for(i = 0; i < k; ++i)  //����ls�С����ߡ��ĳ�ֵ
        ls[i] = k;
    for(i = k-1; i >= 0; --i) //���δ�b[k-1],b[k-2],...,b[0]������������
        Adjust(ls, b, i);
}// CreateLoserTree

//��������
//�Ƚ������ݴ���r[0],һ�������������������ȷλ��
unsigned int Partition(std::vector<consuming_record> & quick_records, unsigned int low, unsigned int high){
    //����˳���L���ӱ�L.r[low..high]�ļ�¼��ʹ�����¼��λ��������������λ������֮ǰ(��)�ļ�¼������(С)����
    quick_records[0] = quick_records[low];
    std::string pivotkey = quick_records[low].time_pre + quick_records[low].time_after;//���ӱ�ĵ�һ����¼�������¼(pivotkeyΪ����ؼ���)
    while(low < high){                                  //�ӱ�����˽�������м�ɨ��
        while(low < high && (quick_records[high].time_pre + quick_records[high].time_after) >= pivotkey) --high;
        quick_records[low] = quick_records[high];                           //���������¼С�ļ�¼�������Ͷ�
        while(low < high && (quick_records[low].time_pre + quick_records[low].time_after) <= pivotkey) ++low;
        quick_records[high] = quick_records[low];                           //���������¼��ļ�¼�������߶�
    }
    quick_records[low] = quick_records[0];                                  //�����¼��λ
    return low;                                         //������������λ��
}// Partition


void QuickSort(std::vector<consuming_record> & quick_records, unsigned int low, unsigned int high){ // NOLINT
    // ��˳���L�е�������L.r[low..high]����������
    unsigned int pivotloc;
    if(low < high){
        pivotloc = Partition(quick_records, low, high);
        QuickSort(quick_records, low, pivotloc - 1);
        QuickSort(quick_records, pivotloc + 1, high);
    }
}//QuickSort

//��������
void Window::Quick(){
    unsigned int i;
    final_quicksort_records.emplace_back(
            consuming_record{0, "0", "0", 0, 0.0});
    for (auto & window : (*Member_point_to_windows))
        if (window.Member_end != window.Member_begin - 1) {
            for (i = window.Member_begin; i != window.Member_end; i = (i + 1) % (MaxAmount + 1))
                final_quicksort_records.emplace_back(window.Member_consumption_records[i]);
            final_quicksort_records.emplace_back(window.Member_consumption_records[i]);
        }
    QuickSort(final_quicksort_records, 1, final_quicksort_records.size() - 1);
}// Quick



