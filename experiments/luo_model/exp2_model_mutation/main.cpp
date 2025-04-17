#include <iostream>
#include "luo_model/utils.hpp"
#include <fstream>

using namespace std;
int main(int argc, char** argv) {
    if (argc != 1 && argc != 2 && argc != 9) {
        cout << "Usage: ./main [output_path]" << endl;
        cout << "or: ./main [output_path] [m] [n] [r] [s] [nu] [full_output=0,1],[uniform_init=0,1]" << endl;
        return 1;
    }
    bool full_output = false;
    bool uniform_init = 0;
    ostream* output_stream = &cout;
    fstream file;
    if (argc >= 2) {
        file.open(argv[1], fstream::out);
        if (!file.is_open()) file = fstream(argv[1], fstream::out);
        output_stream = &file;
    }
    int m = 50, n = 49;
    double r = 0.1, s = 0.05, nu = 0.0005;

    if (argc == 9) {
        m = stoi(argv[2]);
        n = stoi(argv[3]);
        r = stod(argv[4]);
        s = stod(argv[5]);
        nu = stod(argv[6]);
        full_output = stoi(argv[7]);
        uniform_init = stoi(argv[8]);
    }
    *output_stream << "t";
    for (int i = 0; i < n + 1; i++) {
        *output_stream << ",u" << i;
    }
    *output_stream << std::endl;
    vector<double> u = randomize_initial_distribution(m, n);
    if (uniform_init) {
        u = uniform_initial_distribution(m, n);
    }
    double T = 0.0;

    while (T < 50.0) {
        if (full_output) {
            *output_stream << T;
            for (double val : u) *output_stream << "," << val;
            *output_stream << endl;
        }
        vector<double> L = get_leaving_rates_mutation(u, m, n, r, s, nu);
        double tau = 1.0/draw_time_poisson(L);
        if (T + tau > 50.0) break;
        int I1 = draw_random_number(L,"(L) = Drawing Group Prob: ");
        vector<double> G = get_incoming_rates_mutation(u, m, n, r, s, I1, nu);
        int I2 = draw_random_number(G,"(G) = Placed Group Prob: ");
        u[I1] -= 1.0 / m;
        u[I2] += 1.0 / m;
        T += tau;
        if(uniform_init==1 && T>10){
            break;
        }
    }

    *output_stream << T;
    for (double val : u) *output_stream << "," << val;
    *output_stream << endl;
    return 0;
}
