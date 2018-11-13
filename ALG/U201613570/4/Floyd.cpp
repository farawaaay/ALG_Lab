#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
using namespace std;
using IntMatrix = vector<vector<int>>;
using IntTensor = vector<IntMatrix>;
const int INFINITE = 100;

tuple<IntTensor, IntTensor> floyd_3(IntMatrix w);
tuple<IntMatrix, IntMatrix> floyd_2(IntMatrix w);

tuple<IntTensor, IntTensor> floyd_3(IntMatrix w) {
    int n = w.size();
    IntTensor d(n + 1, IntMatrix(n, vector<int>(n)));
    IntTensor pi(n + 1, IntMatrix(n, vector<int>(n)));
    d[0] = w;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j || w[i][j] == INFINITE)
                pi[0][i][j] = -1;
            else
                pi[0][i][j] = i + 1;
        }
    }
    for (int k = 1; k <= n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int d1 = d[k - 1][i][j];
                int d2 = d[k - 1][i][k - 1] + d[k - 1][k - 1][j];
                if (d1 <= d2) {
                    d[k][i][j] = d1;
                    pi[k][i][j] = pi[k - 1][i][j];
                } else {
                    d[k][i][j] = d2;
                    pi[k][i][j] = pi[k - 1][k - 1][j];
                }
            }
        }
    }
    return make_tuple(d, pi);
}


tuple<IntMatrix, IntMatrix> floyd_2(IntMatrix w) {
    int n = w.size();
    IntMatrix d(w);
    IntMatrix pi(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j || w[i][j] == INFINITE)
                pi[i][j] = -1;
            else
                pi[i][j] = i + 1;
        }
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int d1 = d[i][j];
                int d2 = (d[i][k] < INFINITE && d[k][j] < INFINITE) ? (d[i][k] + d[k][j]) : INFINITE;
                if (d1 <= d2) {
                    d[i][j] = d1;
                    pi[i][j] = pi[i][j];
                } else {
                    d[i][j] = d2;
                    pi[i][j] = pi[k][j];
                }
            }
        }
    }
    return make_tuple(d, pi);
}

vector<int> get_reverse_path(IntMatrix pi, int i, int j) {
    vector<int> r_path;
    int j_ = j;
    while (pi[i][j] != -1) {
        r_path.push_back(j + 1);
        j = pi[i][j] - 1;
    }
    if (pi[i][j_] != -1 || i == j_)
        r_path.push_back(i + 1);
    return r_path;
}

ostream &print_path(ostream &out, vector<int> r_path) {
    for (auto it = r_path.crbegin(); it != r_path.crend() - 1; it++)
        out << *it << ' ';
    out << r_path.front() << endl;
    return out;
}

int main() {
    ifstream infile("in.dat");
    ofstream outfile("out.dat");
    int n;
    infile >> n;
    IntMatrix w(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            infile >> w[i][j];
    }
    infile.close();
    IntMatrix d, pi;
    tie(d, pi) = floyd_2(w);
    outfile << n << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (d[i][j] == INFINITE)
                outfile << 32767;
            else
                outfile << d[i][j];
            if (j == n - 1)
                outfile << endl;
            else
                outfile << ' ';
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            vector<int> r_path = get_reverse_path(pi, i, j);
            if (r_path.size() > 0)
                print_path(outfile, r_path);
            else
                outfile << "NULL" << endl;
        }
    }
    outfile.close();
    return 0;
}
