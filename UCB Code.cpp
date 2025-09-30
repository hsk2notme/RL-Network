#include <iostream>  
#include <vector>   
#include <cstdlib>   
#include <ctime>    
#include <cmath>    
#include <limits>
using namespace std;

// Hàm tạo số ngẫu nhiên theo phân phối chuẩn - lấy từ phương pháp Box-Muller -> thuật toán chuẩn
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

vector<double> run_ucb_experiment(int k, int num_steps, int num_runs, double c) {
    vector<double> average_rewards(num_steps, 0.0);

    for (int run = 0; run < num_runs; ++run) {
        BanditTestbed testbed(k);
        vector<double> Q(k, 0.0);
        vector<double> N(k, 0.0);

        for (int step = 0; step < num_steps; ++step) {
            int action_to_take = -1;
            
            for (int i = 0; i < k; ++i) {
                if (N[i] == 0) {
                    action_to_take = i;
                    break;
                }
            }

            if (action_to_take == -1) {
                vector<double> ucb_values(k, 0.0);
                int best_action_ucb = 0;
                for (int i = 0; i < k; ++i) {
                    double bonus = c * sqrt(log(step + 1) / N[i]);
                    ucb_values[i] = Q[i] + bonus;
                    if (ucb_values[i] > ucb_values[best_action_ucb]) {
                        best_action_ucb = i;
                    }
                }
                action_to_take = best_action_ucb;
            }
            
            double reward = testbed.get_reward(action_to_take);
            
            N[action_to_take]++;
            Q[action_to_take] += (1.0 / N[action_to_take]) * (reward - Q[action_to_take]);
            
            average_rewards[step] += reward;
        }
    }

    for (int i = 0; i < num_steps; ++i) {
        average_rewards[i] /= num_runs;
    }
    
    return average_rewards;
}

int main() {
    srand(time(0)); 
    int k; cout << "Nhap so luong hanh dong: "; cin >> k >> "\n";
    const int NUM_STEPS = 1000;
    const int NUM_RUNS = 2000;
    
    while (true) {
        double c;
        cout << "Nhap tham so c (nhap so am de thoat): ";
        cin >> c;

        if (c < 0) {
            cout << "Thoat chuong trinh." << endl;
            break; 
        }

        cout << "Dang chay thi nghiem voi c = " << c << "..." << endl;
        vector<double> results = run_ucb_experiment(K, NUM_STEPS, NUM_RUNS, c);

        cout << "--- Ket qua cho UCB c = " << c << " ---\n";
        cout << "Step,AverageReward\n";
        for (int i = 0; i < NUM_STEPS; ++i) {
            cout << i + 1 << "," << results[i] << "\n";
        }
    }

    return 0;
}
                    
    
    
