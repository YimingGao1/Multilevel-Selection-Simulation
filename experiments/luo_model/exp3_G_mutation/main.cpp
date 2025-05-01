#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <cmath>
#include "luo_model/utils.hpp"
using namespace std;



int main(int argc, char** argv) {
    int m = 50, n = 49;
    double r = 0.2, s = 0.2;
    vector<double> nu_list = {1e-5,1e-4,1e-3,5e-3,1e-2,5e-2,1e-1};
    int rep_tatl=1000;

    ostream* output_stream = &cout;
    ofstream outfile;

    // check if output file is provided
    if (argc >= 2) {
        outfile.open(argv[1]);
        if (!outfile.is_open()) {
            cerr << "Error opening file: " << argv[1] << endl;
            return 1;
        }
        output_stream = &outfile;
    }
    // output header
    *output_stream << "v,p\n";


    for (double nu : nu_list) {
        double total_p = 0.0;

        for (int rep = 0; rep < rep_tatl; ++rep) {
            vector<double> u = randomize_initial_distribution(m, n);
            double T = 0.0;

            while (T < 50.0) {
                vector<double> L = get_leaving_rates_mutation(u, m, n, r, s, nu);
                double tau = 1.0/draw_time_poisson(L);
                if (T + tau > 50.0) break;
                int I1 = draw_random_number(L,"");
                vector<double> G = get_incoming_rates_mutation(u, m, n, r, s, I1, nu);
                int I2 = draw_random_number(G,"");
                u[I1] -= 1.0 / m;
                u[I2] += 1.0 / m;
                T += tau;
            }
            total_p += u[n];
        }

        double avg_p = total_p / rep_tatl*1.0;
        *output_stream << nu << "," << avg_p << "\n";
    }
    if (outfile.is_open()) {
        outfile.close();
    }

    return 0;
}
