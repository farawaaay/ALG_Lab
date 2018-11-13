#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

void print_bst_helper(vector<vector<int>> root, int i, int j, int parent_index, string child, int& d_index);

vector<vector<int>> optimal_bst(vector<float> p, vector<float> q, int n) {
  vector<vector<float>> e(n + 1, vector<float>(n + 1));
  vector<vector<float>> w(n + 1, vector<float>(n + 1));
  vector<vector<int>> root(n, vector<int>(n));
  for (int i = 1; i <= n + 1; i++) {
    e[i - 1][i - 1] = q[i - 1];
    w[i - 1][i - 1] = q[i - 1];
  }
  for (int l = 1; l <= n; l++) {
    for (int i = 1; i <= n - l + 1; i++) {
      int j = i + l - 1;
      e[i - 1][j] = numeric_limits<float>::max();
      w[i - 1][j] = w[i - 1][j - 1] + p[j - 1] + q[j];
      for (int r = i; r <= j; r++) {
        float t = e[i - 1][r - 1] + e[r][j] + w[i - 1][j];
        if (t < e[i - 1][j]) {
          e[i - 1][j] = t;
          root[i - 1][j - 1] = r;
        }
      }
    }
  }
  return root;
}

void print_bst(vector<vector<int>> root) {
  int i = 1, j = root.size();
  int d_index = 0;
  int mid = root[i - 1][j - 1];
  cout << "k" << mid << "为根" << endl;
  print_bst_helper(root, i, mid - 1, mid, "左", d_index);
  print_bst_helper(root, mid + 1, j, mid, "右", d_index);
}

void print_bst_helper(vector<vector<int>> root, int i, int j, int parent_index, string child, int& d_index) {
  if (i > j) {
    cout << "d" << (d_index++) << "为k" << parent_index << "的" << child << "孩子" << endl;
    return;
  }
  int mid = root[i - 1][j - 1];
  cout << "k" << mid << "为k" << parent_index << "的" << child << "孩子" << endl;
  print_bst_helper(root, i, mid - 1, mid, "左", d_index);
  print_bst_helper(root, mid + 1, j, mid, "右", d_index);
}

int main() {
  vector<float> p = {0.15, 0.1, 0.05, 0.1, 0.2};
  vector<float> q = {0.05, 0.1, 0.05, 0.05, 0.05, 0.1};
  vector<vector<int>> root = optimal_bst(p, q, p.size());
  print_bst(root);
}
