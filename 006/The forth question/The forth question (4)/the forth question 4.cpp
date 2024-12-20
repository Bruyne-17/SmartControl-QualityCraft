#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct ProductData {
    int node_id;
    double market_price;
    double exchange_loss;
    double test_cost;
    double assembly_cost;
    double disassembly_cost;
    double buy_cost;
    double exchange_saving;
    double original_defect_rate;
    double actual_total_cost;
    double actual_defect_rate;
    bool is_leaf;
    bool is_root;
    bool is_disassemble;
    bool is_check;
    vector<int> son;
} dAta[105];

int a[20];
double max_profit;
double L_l, L_r; // 新增用于物流和信誉损失的全局变量

void initial_set() {
    // 设置所有节点的原始次品率为 11%
    for (int i = 1; i <= 12; i++) {
        dAta[i].original_defect_rate = 0.11;
    }

    // 初始化部分参数
    int r[2][8] = { {2, 8, 12, 2, 8, 12, 8, 12}, {1, 1, 2, 1, 1, 2, 1, 2} };
    for (int i = 1; i <= 8; i++) {
        dAta[i].buy_cost = r[0][i - 1];
        dAta[i].test_cost = r[1][i - 1];
        dAta[i].is_leaf = true;
    }

    int rr[3][4] = { {8, 8, 8, 8}, {4, 4, 4, 6}, {6, 6, 6, 10} };
    for (int i = 9; i <= 12; i++) {
        dAta[i].assembly_cost = rr[0][i - 9];
        dAta[i].test_cost = rr[1][i - 9];
        dAta[i].disassembly_cost = rr[2][i - 9];
    }

    dAta[12].market_price = 200;
    dAta[12].exchange_loss = 40;
    dAta[12].is_root = true;
}

void connection() {
    for (int i = 1; i <= 8; i++) {
        dAta[(i - 1) / 3 + 9].son.push_back(i);
    }
    for (int i = 9; i <= 11; i++)
        dAta[12].son.push_back(i);
}

void print() {
    cout << "最大利润为：" << max_profit << endl;
    cout << "此时参数为：" << endl;
    for (int i = 1; i <= 12; i++) {
        if (i <= 8) {
            cout << "是否检查零配件" << i << "：";
            cout << (a[i] ? "是" : "不是") << endl;
        }
        else if (i > 8 && i < 12) {
            cout << "是否检查半成品" << i - 8 << "：";
            cout << (a[i] ? "是" : "不是") << endl;
        }
        else {
            cout << "是否检查成品：";
            cout << (a[i] ? "是" : "不是") << endl;
        }
    }
    for (int i = 13; i <= 15; i++) {
        cout << "是否拆散不合格半成品" << i - 12 << "：";
        cout << (a[i] ? "是" : "不是") << endl;
    }
    cout << "是否拆散不合格成品：";
    cout << (a[16] ? "是" : "不是") << endl;
}

double calculate_profits(int id);

void is_set() {
    double profit;
    for (int i = 0; i < pow(2, 16); i++) {
        int j = 1, cnt = 1;
        while (cnt <= 8) {
            dAta[cnt].is_check = i & j;
            j *= 2;
            cnt++;
        }
        while (cnt <= 16) {
            dAta[cnt].is_check = i & j;
            j *= 2;
            cnt++;
            dAta[cnt].is_disassemble = i & j;
            j *= 2;
            cnt++;
        }
        initial_set();
        for (j = 1; j <= 12; j++)
            profit = calculate_profits(j);

        if (profit >= max_profit) {
            for (j = 1; j <= 12; j++)
                a[j] = dAta[j].is_check;
            for (j = 13; j <= 16; j++)
                a[j] = dAta[j - 4].is_disassemble;
            max_profit = profit;
        }
    }
}

double calculate_profits(int id) {
    double total_cost = 0.0;
    double profit = 0.0;
    double defect_rate = 1;
    vector<double> defect_son_rate(dAta[id].son.size());

    // 基本成本
    total_cost = (dAta[id].is_leaf ? dAta[id].buy_cost : dAta[id].assembly_cost);
    if (dAta[id].is_check)
        total_cost += dAta[id].test_cost;

    for (auto& p : dAta[id].son) {
        total_cost += dAta[p].actual_total_cost;
        if (!dAta[p].is_check)
            defect_rate *= 1 - dAta[p].actual_defect_rate;
    }

    defect_rate = 1 - defect_rate * (1 - dAta[id].original_defect_rate);
    dAta[id].actual_defect_rate = defect_rate;
    for (int i = 0; i < dAta[id].son.size(); i++) {
        int p = dAta[id].son[i];
        defect_son_rate[i] = dAta[p].actual_defect_rate / defect_rate;
    }

    if (defect_rate > 0 && (!dAta[id].is_leaf)) {
        if (dAta[id].is_disassemble) {
            dAta[id].exchange_saving = -dAta[id].disassembly_cost;
            for (int i = 0; i < dAta[id].son.size(); i++) {
                int p = dAta[id].son[i];
                dAta[id].exchange_saving += dAta[p].actual_total_cost * defect_son_rate[i];
            }
            if (!(dAta[id].is_root && dAta[id].is_check)) {
                total_cost -= defect_rate * dAta[id].exchange_saving;
            }
            else {
                total_cost -= defect_rate * dAta[id].exchange_saving / (1 - defect_rate);
            }
        }
    }

    if (dAta[id].is_root) {
        if (dAta[id].is_check) {
            profit = (1 - defect_rate) * (dAta[id].market_price - dAta[id].actual_total_cost);
        }
        else {
            profit = dAta[id].market_price;
            dAta[id].exchange_loss = defect_rate * (dAta[id].exchange_loss + L_l + L_r) / ((1 - defect_rate) * (1 - defect_rate));
            total_cost /= (1 - defect_rate);
            profit -= dAta[id].exchange_loss * 0.95 + total_cost;
        }
    }
    dAta[id].actual_total_cost = total_cost;
    return profit;
}

int main() {
    initial_set();
    connection();

    vector<pair<double, double>> lr_list = { {1.0, 1.0}, {10.0, 10.0}, {2.0, 5.0} };

    for (auto& lr : lr_list) {
        L_l = lr.first;
        L_r = lr.second;
        cout << "物流成本 L_l = " << L_l << ", 信誉损失 L_r = " << L_r << ":" << endl;

        max_profit = 0.0;
        is_set();
        print();
        cout << endl;
    }

    return 0;
}
