#include <vector>
#define FloatTable vector<vector<float>>
#define FloatList vector<float>
#define IntTable vector<vector<int>>

constexpr float FLOAT_MAX = numeric_limits<float>::max();
using namespace std;

IntTable OptimalBST(FloatList p, FloatList q, size_t n) {
  FloatTable e(n + 1, FloatList(n + 1));
  FloatTable w(n + 1, FloatList(n + 1));
  IntTable root(n, vector<int>(n));

  // just from textbook
  for (int i = 1; i <= n + 1; i++) {
    e[i - 1][i - 1] = q[i - 1];
    w[i - 1][i - 1] = q[i - 1];
  }
  for (int l = 1; l <= n; l++) {
    for (int i = 1; i <= n - l + 1; i++) {
      int j = i + l - 1;
      e[i - 1][j] = FLOAT_MAX;
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