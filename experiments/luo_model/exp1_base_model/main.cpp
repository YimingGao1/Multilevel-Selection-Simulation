#include "luo_model/utils.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
    if (argc != 1 && argc != 2 && argc != 8) {
        cout << "Usage: ./main [output_path]" << endl;
        cout << "or: ./main [output_path] [m] [n] [r] [s] [full_output=0,1],[uniform_init=0,1]" << endl;
        return 1;
    }
    bool full_output = 0;
    bool uniform_init = 0;
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

    int m = 50; // number of groups
    int n = 50; // number of individuals in a group

    double r = 0.1; // proportionality constant
    double s = 0.05; // proportionality constant
    
    if (argc == 8) {
        m = std::stoi(argv[2]);
        n = std::stoi(argv[3]);
        r = std::stod(argv[4]);
        s = std::stod(argv[5]);
        full_output = std::stoi(argv[6]);
        uniform_init = std::stoi(argv[7]);
    }
    
    *output_stream << "t";
    for (int i = 0; i < n + 1; i++) {
        *output_stream << ",u" << i;
    }
    *output_stream << std::endl;
    //*output_stream << m << " " << n << " " << r << " " << s << endl;
    vector<double> u=vector<double>(n + 1, 0.0); // initial distribution of balls in groups
    // groups[i] is the proportion of groups that have i G type individuals, where i in [0..n]
    if(uniform_init){
        // uniform distribution
        u = uniform_initial_distribution(m, n); // initial distribution of balls in groups
    } else {
        // random distribution
        u = randomize_initial_distribution(m, n); // initial distribution of balls in groups
    }

    // // assume 2 groups with 1 G type individual, 1 group with 2 G type individuals, 1 group with 0 G type individuals
    // u[0] = 0.0/m;
    // u[1] = 2.0/m;
    // u[2] = 1.0/m;
    // u[3] = 1.0/m;

    double T = 0.0; // time

    while(fabs(u[0] - 1.0) > 1e-8 && fabs(u[n] - 1.0) > 1e-8) {
        // cout << "U: ";
        if (full_output) {
            *output_stream << T << " ";
            for (int i = 0; i < u.size(); i++) {
                *output_stream << "," << u[i];
            }
            *output_stream << endl;
        }

        // rate of balls leaving each group
        vector<double> L = get_leaving_rates(u, m, n, r, s);

        // draw time to next event from poisson distribution
        double tau = 1.0/draw_time_poisson(L);
        //cout << "Time to Next Event: " << tau << endl;
        // cout << "tau: " << tau << endl;

        // draw random number I1, group that the individual is drawn from
        int I1 = draw_random_number(L, "(L) = Drawing Group Prob: ");
        // cout << "Drawing Group Index: " << I1 << endl;

        // draw random number I2, group that the individual is placed in
        vector<double> G = get_incoming_rates(u, m, n, r, s, I1);
        int I2 = draw_random_number(G, "(G) = Placed Group Prob: ");
        // cout << "Placed Group Index: " << I2 << endl;

        // update u and t
        u[I1] -= 1.0/m;
        u[I2] += 1.0/m;
        T += tau;
        if(uniform_init==1 && T>10){
            break;
        }

    }

    // cout << "Final U: ";
    *output_stream << T << " ";
    for (int i = 0; i < u.size(); i++) {
        *output_stream << "," << u[i];
    }
    *output_stream << endl;

    // cout << "Total Time Taken: " << T << endl;

    // ------- OUTPUT FORMAT -------
    // First Line: m n r s
    // Other Lines: T u[0] u[1] ...

    return 0;
}
