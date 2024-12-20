#include <iostream>
#include <vector>  // 必须包含 vector 头文件
#include <cmath>
using namespace std;

// 计算二项分布的累积分布函数 P(X <= k)
double binomcdf(int n, int k, double p) {
    double cdf = 0.0;
    vector<double> C(n + 1);
    C[0] = 1;
    for (int i = 0; i <= k; ++i) {
        if (i)
            C[i] = C[i - 1] * (n - i + 1) / i;  // 计算组合数 C(n, i)
        cdf += C[i] * pow(p, i) * pow(1 - p, n - i);  // 累积概率
    }
    return cdf;
}

int main() {
    double p0 = 0.10;  // 标称值
    int max_n = 100;   // 最大样本量

    cout << "情形一 (α = 0.05):" << endl;
    double alpha1 = 0.05;  // 显著性水平
    bool found_case1 = false;  // 标志变量用于替代 goto
    for (int n = 1; n <= max_n && !found_case1; ++n) {
        for (int k = 1; k <= n; ++k) {
            double p_value = 1.0 - binomcdf(n, k - 1, p0);
            if (p_value <= alpha1 && alpha1 - p_value < 0.005) {
                cout << "n = " << n << ", k = " << k << endl;
                found_case1 = true;  // 找到满足条件的 n 和 k 后退出循环
                break;
            }
        }
    }

    cout << "情形二 (α = 0.10):" << endl;
    double alpha2 = 0.10;  // 显著性水平
    bool found_case2 = false;  // 标志变量用于替代 goto
    for (int N = 1; N <= max_n && !found_case2; ++N) {
        for (int k = 1; k <= N; ++k) {
            double p_value = 1.0 - binomcdf(N, k - 1, p0);
            if (p_value >= alpha2 && p_value - alpha2 < 0.005) {
                cout << "N = " << N << ", k = " << k << endl;
                found_case2 = true;  // 找到满足条件的 N 和 k 后退出循环
                break;
            }
        }
    }

    return 0;
}
