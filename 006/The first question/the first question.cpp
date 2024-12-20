#include <iostream>
#include <vector>  // ������� vector ͷ�ļ�
#include <cmath>
using namespace std;

// �������ֲ����ۻ��ֲ����� P(X <= k)
double binomcdf(int n, int k, double p) {
    double cdf = 0.0;
    vector<double> C(n + 1);
    C[0] = 1;
    for (int i = 0; i <= k; ++i) {
        if (i)
            C[i] = C[i - 1] * (n - i + 1) / i;  // ��������� C(n, i)
        cdf += C[i] * pow(p, i) * pow(1 - p, n - i);  // �ۻ�����
    }
    return cdf;
}

int main() {
    double p0 = 0.10;  // ���ֵ
    int max_n = 100;   // ���������

    cout << "����һ (�� = 0.05):" << endl;
    double alpha1 = 0.05;  // ������ˮƽ
    bool found_case1 = false;  // ��־����������� goto
    for (int n = 1; n <= max_n && !found_case1; ++n) {
        for (int k = 1; k <= n; ++k) {
            double p_value = 1.0 - binomcdf(n, k - 1, p0);
            if (p_value <= alpha1 && alpha1 - p_value < 0.005) {
                cout << "n = " << n << ", k = " << k << endl;
                found_case1 = true;  // �ҵ����������� n �� k ���˳�ѭ��
                break;
            }
        }
    }

    cout << "���ζ� (�� = 0.10):" << endl;
    double alpha2 = 0.10;  // ������ˮƽ
    bool found_case2 = false;  // ��־����������� goto
    for (int N = 1; N <= max_n && !found_case2; ++N) {
        for (int k = 1; k <= N; ++k) {
            double p_value = 1.0 - binomcdf(N, k - 1, p0);
            if (p_value >= alpha2 && p_value - alpha2 < 0.005) {
                cout << "N = " << N << ", k = " << k << endl;
                found_case2 = true;  // �ҵ����������� N �� k ���˳�ѭ��
                break;
            }
        }
    }

    return 0;
}
