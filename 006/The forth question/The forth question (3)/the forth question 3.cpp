#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// 结构体来存储数据
struct ProductData {
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

// 利润计算函数
double calculate_profits(ProductData& data, bool test1_parts, bool test2_parts, bool test_final, bool disassemble_defects, double L_l, double L_r) {
    double total_cost = 0;
    double profit = 0;

    // 基本成本
    total_cost += data.part1_price + data.part2_price + data.assembly_cost;

    // 检测成本
    if (test1_parts) {
        total_cost += data.part1_test_cost;
        data.part1_defect_rate = 0; // 如果检测，次品率降为0
    }
    if (test2_parts) {
        total_cost += data.part2_test_cost;
        data.part2_defect_rate = 0; // 如果检测，次品率降为0
    }

    // 计算次品率
    double defect_rate = (1 - (1 - data.part1_defect_rate) * (1 - data.part2_defect_rate) * (1 - data.final_product_defect_rate));

    if (test_final) {
        total_cost += data.final_product_test_cost;
        // 成品销售利润 (检测后没有调换损失)
        profit = (1 - defect_rate) * (data.market_price - total_cost);
    }
    else {
        // 未检测成品，需要考虑调换损失
        profit = data.market_price - total_cost - data.exchange_loss * defect_rate * 0.95 / (1 - defect_rate);
    }

    // 拆解成本 (只有在检测出次品后才会拆解)
    if (defect_rate > 0 && disassemble_defects && test_final) {
        total_cost += data.disassembly_cost * defect_rate;
        // 拆解回本
        total_cost -= (1 - data.part1_defect_rate) * (1 - data.part2_defect_rate) * (data.part1_price + data.part2_price) * defect_rate;
    }

    return profit;
}

int main() {
    // 六种情况的数据
    vector<ProductData> data_list = {
        {1, 0.10, 4, 2, 0.10, 18, 3, 0.10, 6, 3, 56, 6, 5},
        {2, 0.20, 4, 2, 0.20, 18, 3, 0.20, 6, 3, 56, 6, 5},
        {3, 0.10, 4, 2, 0.10, 18, 3, 0.10, 6, 3, 56, 30, 5},
        {4, 0.20, 4, 1, 0.20, 18, 1, 0.20, 6, 2, 56, 30, 5},
        {5, 0.10, 4, 8, 0.20, 18, 1, 0.10, 6, 2, 56, 10, 5},
        {6, 0.05, 4, 2, 0.05, 18, 3, 0.05, 6, 3, 56, 10, 40}
    };

    // 设置三种物流成本和信誉损失的组合
    vector<pair<double, double>> lr_list = {
        {1.0, 1.0},
        {10.0, 10.0},
        {2.0, 5.0}
    };

    // 遍历六种情况
    for (ProductData& data : data_list) {
        cout << "情况 " << data.case_number << ":" << endl;

        for (auto& lr : lr_list) {
            double L_l = lr.first;
            double L_r = lr.second;

            cout << "物流成本 L_l = " << L_l << ", 信誉损失 L_r = " << L_r << ":" << endl;

            double max_profit = -1e9; // 初始化最大利润为一个很小的值
            bool best_t1 = false, best_t2 = false, best_tf = false, best_d = false;

            // 穷举所有决策组合
            for (bool t1 : {false, true}) {
                for (bool t2 : {false, true}) {
                    for (bool tf : {false, true}) {
                        for (bool d : {false, true}) {
                            double profit = calculate_profits(data, t1, t2, tf, d, L_l, L_r);
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

            cout << "最优决策: T1=" << best_t1 << ", T2=" << best_t2
                << ", Tf=" << best_tf << ", D=" << best_d << endl;
            cout << "最大利润: " << max_profit << endl << endl;
        }
    }

    return 0;
}
