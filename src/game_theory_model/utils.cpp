#include "game_theory_model/utils.hpp"
#include <iostream>
#include <cmath>
#include <random>

using namespace std;

void print_vector(vector<double> v, string title) {
    cout << title;
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << "\n";
}

vector<double> get_coop_payoff(vector< vector<double> > payoff, int n) {
    double R = payoff[0][0];
    double S = payoff[0][1];
    double T = payoff[1][0];
    double P = payoff[1][1];

    // cooperator payoffs for every group
    vector<double> pi_c(n + 1, 0.0);
    for (int i = 0; i < n + 1; i++) {
        pi_c[i] = R * i / n + S * (n - i) / n;
    }

    return pi_c;
}

vector<double> get_def_payoff(vector< vector<double> > payoff, int n) {
    double R = payoff[0][0];
    double S = payoff[0][1];
    double T = payoff[1][0];
    double P = payoff[1][1];

    // defector payoffs for every group
    vector<double> pi_d(n + 1, 0.0);
    for (int i = 0; i < n + 1; i++) {
        pi_d[i] = T * i / n + P * (n - i) / n;
    }

    return pi_d;
}

vector<double> get_group_payoff(vector< vector<double> > payoff, int n) {
    double R = payoff[0][0];
    double S = payoff[0][1];
    double T = payoff[1][0];
    double P = payoff[1][1];

    // cooperator payoffs for every group
    vector<double> pi_c(n + 1, 0.0);
    for (int i = 0; i < n + 1; i++) {
        pi_c[i] = R * i / n + S * (n - i) / n;
    }

    // defector payoffs for every group
    vector<double> pi_d(n + 1, 0.0);
    for (int i = 0; i < n + 1; i++) {
        pi_d[i] = T * i / n + P * (n - i) / n;
    }

    // average payoff values of groups with different number of cooperators
    vector<double> G(n + 1, 0.0);
    for (int i = 0; i < n + 1; i++) {
        G[i] = (i * pi_c[i] + (n - i) * pi_d[i]) / n;
    }

    return G;
}

vector<double> get_leaving_rates(const vector<double>& u, int m, int n, const vector<double>& G, const vector<double>& pi_c, const vector<double>& pi_d, double lambda, double w_i, double w_g) {
    vector<double> L;
    L.reserve(n + 1);
    
    vector<double> lambda_u(n + 1, 0.0);
    double sum = 0.0;
    for (int i = 0; i < n + 1; i++) {
        lambda_u[i] = lambda * u[i] * (1 + w_g * G[i]);
        sum += lambda_u[i];
    }

    double inv_n = 1.0 / n;

    L.push_back(m * u[0] * (sum - lambda_u[0]));

    for (int i = 1; i < n; i++) {
        double term_sum = sum - lambda_u[i];
        double common_sum = m * u[i] * term_sum;
        double coop_def_sum = m * u[i] * (n - i) * i * inv_n * (2 + w_i * (pi_d[i] + pi_c[i]));
        L.push_back(coop_def_sum + common_sum);
    }

    L.push_back(m * u[n] * (sum - lambda_u[n]));

    return L;
}

vector<double> get_incoming_rates(const vector<double>& u, int m, int n, const vector<double>& G_payoff, const vector<double>& pi_c, const vector<double>& pi_d, double lambda, double I1, double w_i, double w_g) {
    vector<double> G(n + 1, 0.0);

    double inv_n = 1.0 / n;
    double I1_inv_n = I1 * inv_n;
    double one_minus_I1_inv_n = 1 - I1_inv_n;

    if (I1 > 0) {
        G[I1 - 1] = (n - I1) * (1 + w_i * pi_d[I1]) * I1_inv_n + u[I1 - 1] * lambda * (1 + w_g * G_payoff[I1 - 1]);
    }

    if (I1 < n) {
        G[I1 + 1] = I1 * one_minus_I1_inv_n * (1 + w_i * pi_c[I1]) + u[I1 + 1] * lambda * (1 + w_g * G_payoff[I1 + 1]);
    }

    for (int i = 0; i < n + 1; i++) {
        if (abs(i - I1) > 1) {
            G[i] = u[i] * lambda * (1 + w_g * G_payoff[i]);
        }
    }

    return G;
}

mt19937& get_random_generator() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen;
}

double draw_time_poisson(const vector<double>& L) {
    double lambda = accumulate(L.begin(), L.end(), 0.0);

    poisson_distribution<int> pd(lambda);
    return pd(get_random_generator());
}

int draw_random_number(const vector<double>& draw_prob) {
    double sum = accumulate(draw_prob.begin(), draw_prob.end(), 0.0);

    vector<double> prob(draw_prob.size());
    for (size_t i = 0; i < draw_prob.size(); i++) {
        prob[i] = draw_prob[i] / sum;
    }

    discrete_distribution<int> dd(prob.begin(), prob.end());
    return dd(get_random_generator());
}

vector<double> randomize_initial_distribution(int m, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, n);

    vector<double> G(m, 0.0);
    for (int i = 0; i < G.size(); i++) {
        G[i] = dis(gen);
    }

    double inv_m = 1.0 / m;
    vector<double> u(n + 1, 0.0);
    for (int i = 0; i < G.size(); i++) {
        u[G[i]] += inv_m;
    }

    return u;
}