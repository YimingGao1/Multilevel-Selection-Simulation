#include "game_theory_model/utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>

using namespace std;



int main(int argc, char** argv) {
    if (argc != 1 && argc != 2 && argc != 8) {
        cout << "Usage: ./main [output_path]" << endl;
        cout << "or: ./main [output_path] [m] [n] [lambda] [w_i] [w_g] [full_output=0,1]" << endl;
        return 1;
    }

    // default to std::cout
    std::ostream* output_stream = &std::cout;

    // if given output file path change stream to filestream
    std::fstream file;
    if (argc >= 2) {
        file.open(argv[1], std::fstream::out);
        if (!file.is_open()) {
            file = fstream(argv[1], std::fstream::out);
        }
        output_stream = &file;
    }
    bool full_output = 0;

    int m = 100; // number of groups
    int n = 100; // number of individuals in a group

    double lambda = 0.1; // group level events rate

    double w_i = 0.01; // individual level events
    double w_g = 0.01; // group level events
    
    if (argc == 8) {
        m = std::stoi(argv[2]);
        n = std::stoi(argv[3]);
        lambda = std::stod(argv[4]);
        w_i = std::stod(argv[5]);
        w_g = std::stod(argv[6]);
        full_output = std::stoi(argv[7]);
    }
    
    // payoff matrix
    double reward = 2;
    double sucker = -1;
    double temptation = 1;
    double punishment = 0;
    vector< vector<double> > payoff(2, vector<double>(2, 0.0));

    payoff[0][0] = reward;
    payoff[0][1] = sucker;
    payoff[1][0] = temptation;
    payoff[1][1] = punishment;

    // get cooperator payoff values
    vector<double> pi_c = get_coop_payoff(payoff, n);
    // print_vector(pi_c, "Cooperator Payoff: ");

    // get defector payoff values
    vector<double> pi_d = get_def_payoff(payoff, n);
    // print_vector(pi_d, "Defector Payoff: ");

    // get group payoff values
    vector<double> G_payoff = get_group_payoff(payoff, n);
    // print_vector(G_payoff, "Group Payoff: ");

    // cout << "\nStarting Simulation\n" << endl;


    *output_stream << m << " " << n << " " << lambda << " " << w_i << " " << w_g << endl;
    

    // groups[i] is the proportion of groups that have i G type individuals, where i in [0..n]
    vector<double> u = randomize_initial_distribution(m, n); // initial distribution of balls in groups

    double T = 0.0; // time
    
    double inv_m = 1.0 / m;

    while(fabs(u[0] - 1.0) > 1e-8 && fabs(u[n] - 1.0) > 1e-8) {
        // print_vector(u, "U: ");
        if (full_output) {
            *output_stream << T << " ";
            for (int i = 0; i < u.size(); i++) {
                *output_stream << " " << u[i];
            }
            *output_stream << endl;
        }

        // rate of balls leaving each group
        vector<double> L = get_leaving_rates(u, m, n, G_payoff, pi_c, pi_d, lambda, w_i, w_g);
        // print_vector(L, "Leaving Rates: ");

        // draw time to next event from poisson distribution
        double tau = 1.0/draw_time_poisson(L);
        // cout << "tau: " << tau << endl;

        // draw random number I1, group that the individual is drawn from
        // int I1 = draw_random_number(L, "(L) = Drawing Group Prob: ");
        int I1 = draw_random_number(L);
        // cout << "Drawing Group Index: " << I1 << endl;

        // draw random number I2, group that the individual is placed in
        vector<double> G = get_incoming_rates(u, m, n, G_payoff, pi_c, pi_d, lambda, I1, w_i, w_g);
        // print_vector(G, "Incoming Rates: ");

        int I2 = draw_random_number(G);
        // cout << "Placed Group Index: " << I2 << endl;

        // update u and t
        u[I1] -= inv_m;
        u[I2] += inv_m;
        T += tau;

        // cout << "Time Taken for Current Event: " << tau << endl;
        // cout << "Total Time: " << T << endl;

        // cout << "\n";
    }

    // cout << "Final U: ";
    *output_stream << T << " " << endl;
    for (int i = 0; i < u.size(); i++) {
        *output_stream << " " << u[i];
    }
    *output_stream << endl;
    // cout << "Total Time Taken: " << T << endl;

    return 0;
}