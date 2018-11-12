#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

struct Point {
  int x, y;
  Point(int x, int y) : x(x), y(y) {}
  Point() = default;
  friend ostream &operator<<(ostream &out, const Point point) {
    out << point.x << " " << point.y;
    return out;
  }
  bool operator==(Point p) { return x == p.x && y == p.y; }
};

struct PointPair {
  Point a, b;
  PointPair(Point a, Point b) : a(a), b(b) {}
  PointPair() = default;
  friend ostream &operator<<(ostream &out, const PointPair pp) {
    out << pp.a << " " << pp.b;
    return out;
  }
  bool operator==(PointPair pp) {
    return (a == pp.a && b == pp.b) || (a == pp.b && b == pp.a);
  }
};

bool x_cmp(Point a, Point b) { return a.x < b.x; }

bool y_cmp(Point a, Point b) { return a.y < b.y; }

// Returns the Euclidean distance of two point
float distance(Point a, Point b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Resturns the min-distance of points, assign pairs as the point pairs with
// min-distance
float min_distance(vector<Point> xs, vector<Point> ys,
                   vector<PointPair> &pairs) {
  if (xs.size() < 2)
    return numeric_limits<float>::max();
  else if (xs.size() == 2) {
    pairs = {PointPair(xs[0], xs[1])};
    return distance(xs[0], xs[1]);
  } else {
    size_t l;
    vector<PointPair> p1, p2;
    float min;
    // partion x
    if (xs.size() % 2 == 0)
      l = xs.size() / 2;
    else
      l = (xs.size() + 1) / 2;
    int mid_x = xs[l].x;
    vector<Point> x_l(xs.begin(), xs.begin() + l);
    vector<Point> x_r(xs.begin() + l, xs.end());
    // partion y
    vector<Point> y_l, y_r;
    for (auto point : ys) {
      if (point.x <= mid_x)
        y_l.push_back(point);
      else
        y_r.push_back(point);
    }
    // calculace distance of two point sets (left, right)
    float min_l = min_distance(x_l, y_l, p1);
    float min_r = min_distance(x_r, y_r, p2);
    if (min_l < min_r) {
      min = min_l;
      pairs = p1;
    } else if (min_l > min_r) {
      min = min_r;
      pairs = p2;
    } else {
      min = min_l;
      pairs = p1;
      pairs.insert(pairs.end(), p2.begin(), p2.end());
    }
    // get points in 2 * min area
    vector<Point> y_tmp;
    for (auto point : ys) {
      if (mid_x - min <= point.x && point.x <= mid_x + min)
        y_tmp.push_back(point);
    }
    for (size_t i = 0; i < y_tmp.size(); i++) {
      for (size_t j = i + 1; j < y_tmp.size() && j <= i + 7; j++) {
        float dis = distance(y_tmp[i], y_tmp[j]);
        if (dis < min) {
          min = dis;
          pairs = {PointPair(y_tmp[i], y_tmp[j])};
        } else if (dis == min) {
          PointPair p(y_tmp[i], y_tmp[j]);
          if (find(pairs.begin(), pairs.end(), p) == pairs.end())
            pairs.push_back(p);
        }
      }
    }
    return min;
  }
}

vector<PointPair> nearest_pairs(vector<Point> xs) {
  sort(xs.begin(), xs.end(), x_cmp);
  vector<Point> ys(xs);
  sort(ys.begin(), ys.end(), y_cmp);
  vector<PointPair> pairs;
  min_distance(xs, ys, pairs);
  return pairs;
}

double Min(double a, double b, bool &eq) {
  eq = a == b;
  return a < b ? a : b;
}

double BruteForceMinDistance(vector<Point> p) {
  double min_d = DBL_MAX;
  for (size_t i = 0; i < p.size(); i++) {
    for (size_t j = i + 1; j < p.size(); j++) {
      double dx = (p[i].x - p[j].x);
      double dy = (p[i].y - p[j].y);
      double d = dx * dx + dy * dy;
      // printf("Distance({%f, %f}, {%f, %f}) = %f\n", p[i].x, p[i].y, p[j].x,
      //        p[j].y, sqrt(d));

      bool _;
      min_d = Min(min_d, d, _);
    }
  }
  return sqrt(min_d);
}

int main(size_t argc, char **argv) {
  /*
  ifstream infile("in.dat");
  ofstream outfile("out.dat");
  int n;
  infile >> n;
  for (int i = 0; i < n; i++) {
    int m;
    infile >> m;
    vector<Point> xs;
    for (int j = 0; j < m; j++) {
      int x, y;
      infile >> x >> y;
      xs.push_back(Point(x, y));
    }
    vector<PointPair> pairs = nearest_pairs(xs);
    outfile << pairs[0];
    for (size_t i = 1; i < pairs.size(); i++)
      outfile << " " << pairs[i];
    outfile << endl;
  }
  infile.close();
  outfile.close();
  */

  srand(unsigned(time(NULL)));
  size_t L = atoi(argv[1]);

  vector<Point> points(L);

  for (int i = 0; i < L; i++) {
    points[i].x = (rand() % 20000 / 10.0) - 1000;
    points[i].y = (rand() % 20000 / 10.0) - 1000;
  }

  for (auto pp : nearest_pairs(points)) {
    cout << pp << endl;

    cout << distance(pp.a, pp.b) << endl;
  }

  double d = BruteForceMinDistance(points);

  printf("MinDistance: %f\n", d);

  return 0;
}
