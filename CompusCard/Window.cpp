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

//多路归并(采用败者树)
static const unsigned short k = num_windows;
static const char * maxkey = "3";            //设置最大值
static const char * minkey = "0";            //设置最小值
typedef std::string KeyType;
typedef std::vector<std::vector<consuming_record> > Input;
typedef std::vector<consuming_record> Output;
typedef unsigned short LoserTree[k];  //败者树是完全二叉树且不含叶子，可采用顺序存储结构
struct ExNode { KeyType key1; KeyType key2; };
typedef ExNode External[k+1];    //外结点，只存放待归并记录的关键字

//重载运算符
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

//读取各消费窗口位置read_window_consumption_location
void Window::read_window_wz(const char * filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())     std::cout << "打开文件失败";
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

//处理消费记录
void Window::process_consumption_records(const char * filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) std::cout << "打开文件失败";
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

    //多路归并排序
    {
        ::K_Merge(consume_records, ordered_records);
    }
    fin.clear();
    fin.close();
}

//插入消费记录
void Window::increase_consumpting_record(consuming_record & record) {
    //实现循环覆盖
    Member_end = (Member_end + 1) % (MaxAmount + 1);

    Member_consumption_records[Member_end] = record;
    if ((Member_end + 1) % (MaxAmount + 1) == Member_begin)
        Member_begin = (Member_begin + 1) % (MaxAmount + 1);
    if (record.time_pre != Member_last_day) {        //需要跳转天数
        Member_last_day = record.time_pre;
        Member_current_costs = record.amount;
        Member_current_times = 1;
    }
    else{
        Member_current_costs += record.amount;
        Member_current_times += 1;
    }
}

//归并排序插入函数
inline void input(ExNode &key, consuming_record & record)
{
    key.key1 = record.time_pre, key.key2 = record.time_after;
}

//归并排序输出函数
inline void output(Output & output_records, consuming_record & record)
{
    output_records.emplace_back(record);
}

//消费开始前按照时间进行排序
void K_Merge(Input & input_records, Output & output_records)
{//利用败者树ls将编号从0到k-1的k个输入归并段中的记录归并到输出归并段
    // b[0]至b[k-1]为败者树上的k个叶子结点，分别存放k个输入归并段中当前记录的关键字
    LoserTree ls;
    External b;
    unsigned long index[k] = {0};
    unsigned short q;
    for(unsigned short i = 0; i < k; ++i)               //分别从k个输入归并段读入该段当前第一个记录的关键字到外结点
        input(b[i], input_records[i][index[i]]);
    CreateLoserTree(ls, b);                //建败者树ls，选得最小关键字为b[ls[0]].key
    while(b[ls[0]].key1 != maxkey){
        q = ls[0];                      //q指示当前最小关键字所在归并段
        output(output_records, input_records[q][index[q]]);//将编号为q的归并段中当前（关键字为b[q].key）的记录
        ++index[q];
        //写至输出归并段
        input(b[q], input_records[q][index[q]]);             //从编号为q的输入归并段中读入下一个记录的关键字
        Adjust(ls, b, q);                  //调整败者树，选择新的最小关键字
    }// while
    output(output_records, input_records[ls[0]][index[ls[0]]]);                      //将含最大关键字MAXKEY的记录写至输出归并段
}// K_Merge

//对消费成功的记录进行排序汇总
void Window::K_Merge() {
    LoserTree ls;
    External b;
    unsigned long index[k] = {0};
    unsigned short q;
    for(unsigned short i = 0; i < k; ++i) //分别从k个输入归并段读入该段当前第一个记录的关键字到外结点
    {
        (*Member_point_to_windows)[i].Member_consumption_records[((*Member_point_to_windows)[i].Member_end + 1) % (MaxAmount + 1)].time_pre = maxkey;
        index[i] = (*Member_point_to_windows)[i].Member_begin;
        input(b[i], (*Member_point_to_windows)[i].Member_consumption_records[index[i]]);
    }
    CreateLoserTree(ls, b);                //建败者树ls，选得最小关键字为b[ls[0]].key
    while(b[ls[0]].key1 != maxkey){
        q = ls[0];                      //q指示当前最小关键字所在归并段
        output(final_records, (*Member_point_to_windows)[q].Member_consumption_records[index[q]]);//将编号为q的归并段中当前（关键字为b[q].key）的记录
        index[q] = (index[q] + 1) % (MaxAmount + 1);
        //写至输出归并段
        input(b[q], (*Member_point_to_windows)[q].Member_consumption_records[index[q]]);             //从编号为q的输入归并段中读入下一个记录的关键字
        Adjust(ls, b, q);                  //调整败者树，选择新的最小关键字
    }// while
    output(final_records, (*Member_point_to_windows)[ls[0]].Member_consumption_records[index[ls[0]]]);                   //将含最大关键字MAXKEY的记录写至输出归并
}

//调整过程
void Adjust(LoserTree &ls, External &b, unsigned short s)
{//沿从叶子结点b[s]到根节点ls[0]的路径调整败者树
    unsigned short t = (s + k) / 2;                      //ls[t]是b[s]的双亲结点
    unsigned short temp;
    while(t > 0){
        if(b[s] > b[ls[t]])
            temp = s, s = ls[t], ls[t] = temp;// s指示新的胜者
        t = t / 2;
    }
    ls[0] = s;
}// Adjust

void CreateLoserTree(LoserTree &ls, External &b)
{//已知b[0]到b[k-1]为完全二叉树ls的叶子结点存有k个关键字，沿从叶子到根的k条路径将ls调整成为败者树
    b[k].key1 = minkey;     //设MINKEY为关键字可能的最小值
    short i;
    for(i = 0; i < k; ++i)  //设置ls中“败者”的初值
        ls[i] = k;
    for(i = k-1; i >= 0; --i) //依次从b[k-1],b[k-2],...,b[0]出发调整败者
        Adjust(ls, b, i);
}// CreateLoserTree

//快速排序
//先将枢轴暂存在r[0],一趟排序结束后在移至正确位置
unsigned int Partition(std::vector<consuming_record> & quick_records, unsigned int low, unsigned int high){
    //交换顺序表L中子表L.r[low..high]的记录，使枢轴记录到位，并返回其所在位置在它之前(后)的记录均不大(小)于它
    quick_records[0] = quick_records[low];
    std::string pivotkey = quick_records[low].time_pre + quick_records[low].time_after;//用子表的第一个记录作枢轴记录(pivotkey为枢轴关键字)
    while(low < high){                                  //从表的两端交替地向中间扫描
        while(low < high && (quick_records[high].time_pre + quick_records[high].time_after) >= pivotkey) --high;
        quick_records[low] = quick_records[high];                           //将比枢轴记录小的记录交换到低端
        while(low < high && (quick_records[low].time_pre + quick_records[low].time_after) <= pivotkey) ++low;
        quick_records[high] = quick_records[low];                           //将比枢轴记录大的记录交换到高端
    }
    quick_records[low] = quick_records[0];                                  //枢轴记录到位
    return low;                                         //返回枢轴所在位置
}// Partition


void QuickSort(std::vector<consuming_record> & quick_records, unsigned int low, unsigned int high){ // NOLINT
    // 对顺序表L中的子序列L.r[low..high]作快速排序
    unsigned int pivotloc;
    if(low < high){
        pivotloc = Partition(quick_records, low, high);
        QuickSort(quick_records, low, pivotloc - 1);
        QuickSort(quick_records, pivotloc + 1, high);
    }
}//QuickSort

//快速排序
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



