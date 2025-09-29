#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <numeric>

using namespace std;

// Hàm tạo số ngẫu nhiên theo phân phối chuẩn - lấy từ phương pháp Box-Muller
double random(double mi, double sigma) {
    static bool call = false;
    static double spare;

    if (call) {
        call = false;
        return mi + sigma * spare;
    }

    call = true;
    double x, y, A;
    do {
        x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        // SỬA LỖI: Dùng x, y đã khai báo thay vì u, v
        A = x * x + y * y; 
    } while (A >= 1.0 || A == 0.0);
    
    A = sqrt(-2.0 * log(A) / A);
    spare = y * A;
    return mi + sigma * x * A;
}

class BanditTestbed {
public:
    int k;
    vector<double> true_values;

    BanditTestbed(int num_actions) {
        k = num_actions;
        true_values.resize(k);
        for (int i = 0; i < k; ++i) {
            true_values[i] = random(0.0, 1.0);
        }
    }

    double get_reward(int action) {
        return random(true_values[action], 1.0);
    }
};

vector<double> GBA_pointing(int k, int num_steps, int num_runs, double alpha) {
    vector<double> average_rewards(num_steps, 0.0);
    for (int run = 1; run <= num_runs; run++) {
        BanditTestbed testbed(k);
        vector<double> H(k, 0.0);
        double baseline = 0.0;
        
        for (int step = 1; step <= num_steps; step++) {
            double sum_believe_point = 0; 
            for (int i = 0; i < k; i++) {
                sum_believe_point += exp(H[i]); 
            }

            vector<double> pi(k, 0.0);
            for (int j = 0; j < k; j++) {
                pi[j] = exp(H[j]) / sum_believe_point;
            }

            int action_to_take;
            double rand_num = (double)rand() / RAND_MAX;
            double cumulative_prob = 0.0;
            for (int i = 0; i < k; ++i) {
                cumulative_prob += pi[i];
                if (rand_num < cumulative_prob) {
                    action_to_take = i;
                    break;
                }
            }

            double reward = testbed.get_reward(action_to_take);
            average_rewards[step] += reward;
            baseline += (1 / step) * (reward - baseline);
            for (int i = 0; i < k; i++) {
                if (i == action_to_take) {
                    H[i] += alpha * (reward - baseline) * (1 - pi[i]); 
                } else {
                    H[i] -= alpha * (reward - baseline) * pi[i]; 
                }
            }
        }
    }

    // Tính trung bình cuối cùng
    for (int i = 0; i < num_steps; ++i) {
        average_rewards[i] /= num_runs;
    }
    
    return average_rewards;
}

int main() {
    srand(time(0)); 
    const int NUM_STEPS = 1000;
    const int NUM_RUNS = 2000;
    
    int k; 
    cout << "Nhap so luong hanh dong: "; 
    cin >> k; 

    while (true) {
        double alpha;
        cout << "\n>>> Nhap toc do hoc alpha (hoac nhap so am de thoat): ";
        cin >> alpha;

        if (alpha < 0) {
            cout << "Thoat chuong trinh." << endl;
            break; 
        }

        cout << "Dang chay thi nghiem voi alpha = " << alpha << "..." << endl;
        vector<double> results = GBA_pointing(k, NUM_STEPS, NUM_RUNS, alpha);

        cout << "--- Ket qua cho Alpha = " << alpha << " ---\n";
        cout << "Step,AverageReward\n";
        for (int i = 0; i < NUM_STEPS; ++i) {
            cout << i + 1 << "," << results[i] << "\n";
        }
    }

    return 0;
}
