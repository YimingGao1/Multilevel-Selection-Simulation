#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <random>
#include <string>

std::vector<double> get_leaving_rates(std::vector<double> u, int m, int n, double r, double s);

std::vector<double> get_incoming_rates(std::vector<double> u, int m, int n, double r, double s, double I1);

std::vector<double> get_leaving_rates_mutation(std::vector<double> u, int m, int n, double r, double s, double nu);

std::vector<double> get_incoming_rates_mutation(std::vector<double> u, int m, int n, double r, double s, int I1, double nu);

double draw_time_poisson(std::vector<double> L);

int draw_random_number(std::vector<double> draw_prob, std::string type);

std::vector<double> randomize_initial_distribution(int m, int n);

std::vector<double> uniform_initial_distribution(int m, int n);
#endif