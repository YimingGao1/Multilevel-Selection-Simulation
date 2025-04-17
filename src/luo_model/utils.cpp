#include "luo_model/utils.hpp"
#include <iostream>
#include <cmath>
#include <random>

using namespace std;

vector<double> get_leaving_rates(vector<double> u, int m, int n, double r, double s) {
    vector<double> L(n + 1, 0.0);

    double sum = 0.0;
    for(int i = 0; i < n + 1; i++) {
        sum += u[i] * (1 + r * i/n);
    }

    for(int i = 0; i < n + 1; i++) {
        double term_sum = sum - u[i] * (1 + r * i/n);

        if (i == 0 || i == n) {
            // m * u[i] to the term to ensure positive probabilities
            // fixed typo from the paper
            L[i] = m * u[i] * term_sum;
        } else {
            L[i] = m * u[i] * (n - i) * i/n * (2 + s) + m * u[i] * term_sum;
        }
    }

    return L;
}


vector<double> get_incoming_rates(vector<double> u, int m, int n, double r, double s, double I1) {
    vector<double> G(n + 1, 0.0);

    for(int i = 0; i < n + 1; i++) {
        if(i == I1 - 1 && I1 < n) {
            G[i] = (n - I1) * (1 + s) * I1/n + u[I1 - 1] * (1 + r * (I1 - 1)/n);
        } else if(i == I1 + 1 && I1 > 0) {
            G[i] = I1 * (1 - I1/n) + u[I1 + 1] * (1 + r * (I1 + 1)/n);
        } else if(abs(i - I1) > 1) {
            G[i] = u[i] * (1 + r * i/n);
        }
    }

    return G;
}

vector<double> get_leaving_rates_mutation(vector<double> u, int m, int n, double r, double s, double nu) {
    vector<double> L(n + 1, 0.0);
    double sum = 0.0;
    for (int i = 0; i < n + 1; i++) {
        sum += u[i] * (1 + r * i / n);
    }
    for (int i = 0; i < n + 1; i++) {
        double term_sum = sum - u[i] * (1 + r * i / n);
        if (i == 0 || i == n) {
            L[i] = m * u[i] * term_sum;
        } else {
            L[i] = m * u[i] * (n - i) * i / n * (2 + s) + m * u[i] * term_sum;
        }
        L[i] += nu * u[i] * n;  // add mutation outflow
    }
    return L;
}

vector<double> get_incoming_rates_mutation(vector<double> u, int m, int n, double r, double s, int I1, double nu) {
    vector<double> G(n + 1, 0.0);
    for (int i = 0; i < n + 1; i++) {
        if (i == I1 - 1 && I1 < n) {
            G[i] += (n - I1) * (1 + s) * I1 / n + u[I1 - 1] * (1 + r * (I1 - 1) / n);
        } else if (i == I1 + 1 && I1 > 0) {
            G[i] += I1 * (1 - I1 / (double)n) + u[I1 + 1] * (1 + r * (I1 + 1) / n);
        } else if (abs(i - I1) > 1) {
            G[i] += u[i] * (1 + r * i / n);
        }
    }

    // Add mutation inflow rates
    for (int i = 0; i < n + 1; i++) {
        if (i == 0) {
            G[i] += nu * u[1];
        } else if (i == n) {
            G[i] += nu * u[n - 1];
        } else {
            G[i] += nu * u[i - 1] * (n - i + 1);
            G[i] += nu * u[i + 1] * (i + 1);
        }
    }
    return G;
}


double draw_time_poisson(vector<double> L) {
    double lambda = 0.0;
    for(int i = 0; i < L.size(); i++) {
        lambda += L[i];
    }
    random_device rd;
    mt19937 gen(rd());
    poisson_distribution<int> pd(lambda);
    return pd(gen);
}

int draw_random_number(vector<double> draw_prob, string type) {
    random_device rd;
    mt19937 gen(rd());

    double sum = 0.0;
    for(int i = 0; i < draw_prob.size(); i++) {
        sum += draw_prob[i];
    }

    vector<double> prob(draw_prob.size(), 0.0);
    for(int i = 0; i < draw_prob.size(); i++) {
        prob[i] = draw_prob[i] / sum;
    }
    // cout << type;
    // for(int t = 0; t < prob.size(); t++) {
    //     cout << prob[t] << " ";
    // }
    // cout << "\n";

    discrete_distribution<int> dd(prob.begin(), prob.end());
    return dd(gen);
}

vector<double> randomize_initial_distribution(int m, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, n);

    vector<double> G(m, 0.0);
    for(int i = 0; i < G.size(); i++) {
        G[i] = dis(gen);
    }

    vector<double> u(n + 1, 0.0);
    for(int i = 0; i < G.size(); i++) {
        u[G[i]] += 1.0/m;
    }

    return u;
}

vector<double> uniform_initial_distribution(int m, int n) {
    vector<double> u(n + 1, 0.0);

    // Assign uniform probability to each state
    double uniform_value = 1.0 / (n + 1);
    for (int i = 0; i <= n; i++) {
        u[i] = uniform_value;
    }

    return u;
}
