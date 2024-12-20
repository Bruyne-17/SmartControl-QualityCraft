#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// �ṹ�����洢����
struct Data {
    int case_number;
    double part1_defect_rate;
    double part1_price;
    double part1_test_cost;
    double part2_defect_rate;
    double part2_price;
    double part2_test_cost;
    double final_product_defect_rate;
    double assembly_cost;
    double final_product_test_cost;
    double market_price;
    double exchange_loss;
    double disassembly_cost;
};

// ������㺯��
double calculate_profits(Data& data, bool test1_parts, bool test2_parts, bool test_final, bool disassemble_defects) {
    double total_cost = 0;
    double profit = 0;

    // �����ɱ�
    total_cost += data.part1_price + data.part2_price + data.assembly_cost;

    // ���ɱ�
    if (test1_parts) {
        total_cost += data.part1_test_cost;
        data.part1_defect_rate = 0; // �����⣬��Ʒ�ʽ�Ϊ0
    }
    if (test2_parts) {
        total_cost += data.part2_test_cost;
        data.part2_defect_rate = 0; // �����⣬��Ʒ�ʽ�Ϊ0
    }

    // �����Ʒ��
    double defect_rate = (1 - (1 - data.part1_defect_rate) * (1 - data.part2_defect_rate) * (1 - data.final_product_defect_rate));

    if (test_final) {
        total_cost += data.final_product_test_cost;
        // ��Ʒ�������� (����û�е�����ʧ)
        profit = (1 - defect_rate) * (data.market_price - total_cost);
    }
    else {
        // δ����Ʒ����Ҫ���ǵ�����ʧ
        profit = data.market_price - total_cost - data.exchange_loss * defect_rate / (1 - defect_rate);
    }

    // ���ɱ� (ֻ���ڼ�����Ʒ��Ż���)
    if (defect_rate > 0 && disassemble_defects && test_final) {
        total_cost += data.disassembly_cost * defect_rate;
        // ���ر�
        total_cost -= (1 - data.part1_defect_rate) * (1 - data.part2_defect_rate) * (data.part1_price + data.part2_price) * defect_rate;
    }

    return profit;

}

int main() {
    // �������������
    vector<Data> data_list = {
    {1, 0.11, 4, 2, 0.11, 18, 3, 0.11, 6, 3, 56, 6, 5},
    {2, 0.21, 4, 2, 0.21, 18, 3, 0.21, 6, 3, 56, 6, 5},
    {3, 0.11, 4, 2, 0.11, 18, 3, 0.11, 6, 3, 56, 30, 5},
    {4, 0.21, 4, 1, 0.21, 18, 1, 0.21, 6, 2, 56, 30, 5},
    {5, 0.11, 4, 8, 0.21, 18, 1, 0.11, 6, 2, 56, 10, 5},
    {6, 0.06, 4, 2, 0.06, 18, 3, 0.06, 6, 3, 56, 10, 40}
    };

    // �����������
    for (Data& data : data_list) {
        cout << "��� " << data.case_number << ":" << endl;
        double max_profit = -1e9; // ��ʼ���������Ϊһ����С��ֵ
        bool best_t1, best_t2, best_tf, best_d;

        // ������о������
        for (bool t1 : {false, true}) {
            for (bool t2 : {false, true}) {
                for (bool tf : {false, true}) {
                    for (bool d : {false, true}) {
                        double profit = calculate_profits(data, t1, t2, tf, d);
                        if (profit > max_profit) {
                            max_profit = profit;
                            best_t1 = t1;
                            best_t2 = t2;
                            best_tf = tf;
                            best_d = d;
                        }
                    }
                }
            }
        }

        cout << "���ž���: T1=" << best_t1 << ", T2=" << best_t2
            << ", Tf=" << best_tf << ", D=" << best_d << endl;
        cout << "�������: " << max_profit << endl << endl;
    }

    return 0;


}
