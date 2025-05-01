#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <random>
#include <string>

using namespace std;

void print_vector(vector<double> v, string title);
vector<double> get_coop_payoff(vector< vector<double> > payoff, int n);
vector<double> get_def_payoff(vector< vector<double> > payoff, int n);
vector<double> get_group_payoff(vector< vector<double> > payoff, int n);
vector<double> get_leaving_rates(const vector<double>& u, int m, int n, const vector<double>& G, const vector<double>& pi_c, const vector<double>& pi_d, double lambda, double w_i, double w_g);
vector<double> get_incoming_rates(const vector<double>& u, int m, int n, const vector<double>& G_payoff, const vector<double>& pi_c, const vector<double>& pi_d, double lambda, double I1, double w_i, double w_g);
mt19937& get_random_generator();
vector<double> randomize_initial_distribution(int m, int n);
vector<double> uniform_initial_distribution(int m, int n);
double draw_time_poisson(const vector<double>& L);
int draw_random_number(const vector<double>& draw_prob);
int draw_random_number(const vector<double>& draw_prob, string type);



#endif