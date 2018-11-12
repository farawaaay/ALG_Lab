#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <ctime>

#define L 100
#define DBL_MAX 1.7976931348623158e+308

using namespace std;

struct Point {
  double x;
  double y;
};

double BruteForceMinDistance(Point p[], int left, int right);
bool CmpX(Point a, Point b) {
  return a.x < b.x;
}
bool CmpY(Point a, Point b) {
  return a.y < b.y;
}
inline double Min(double a, double b) {
  return b < a ? b : a;
}
inline double Min(double a, double b, double c) {
  return Min(a, Min(b, c));
}
inline double Distance(Point p1, Point p2) {
  return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

Point* sorted_x = new Point[L];
Point* sorted_y = new Point[L];

bool sorted = false;

double MinDistance(Point* p, int left, int right) {
  if (left == right) {
    return DBL_MAX;
  } else if (right - left <= 2) {
    return BruteForceMinDistance(p, left, right);
  } else {
    if (!sorted) {
      for (int i = left; i < right; i++) {
        // sorted_x[i] = p[i];
        sorted_y[i] = p[i];
      }
      // std::sort(sorted_x + left, sorted_x + right, CmpX);
      std::sort(sorted_y + left, sorted_y + right, CmpY);
      sorted = true;
    }

    int mid = (right + left) >> 1;
    double min = Min(MinDistance(p, left, mid), MinDistance(p, mid + 1, right));
    double mid_x = p[mid].x;

    double cross = DBL_MAX;
    int* temp = new int[L];
    Point* inner = new Point[L];
    int k = 0;
    for (int i = left; i <= right; i++)
      if (fabs(mid_x - sorted_y[i].x) <= min) {
        temp[k++] = i;
        inner[k - 1] = sorted_y[i];
        printf("(%d,%d): P[%d]: (%f,%f);\n", left, right, i, sorted_y[i].x,
               sorted_y[i].y);
      }

    for (int i = 0; i < k; i++)
      for (int j = i + 1; j < k; j++)
        if (fabs(sorted_y[temp[j]].y - sorted_y[temp[i]].y) < min) {
          double dx = sorted_y[temp[i]].x - sorted_y[temp[j]].x;
          double dy = sorted_y[temp[i]].y - sorted_y[temp[j]].y;
          double d = dx * dx + dy * dy;
          cross = Min(cross, d);
        }

    return Min(min, sqrt(cross));
  }
}

double BruteForceMinDistance(Point p[], int left, int right) {
  double min_d = DBL_MAX;
  for (int i = left; i <= right; i++) {
    for (int j = i + 1; j <= right; j++) {
      double dx = (p[i].x - p[j].x);
      double dy = (p[i].y - p[j].y);
      double d = dx * dx + dy * dy;
      // printf("Distance({%f, %f}, {%f, %f}) = %f\n",
      //        p[i].x,
      //        p[i].y,
      //        p[j].x,
      //        p[j].y,
      //        sqrt(d));
      min_d = Min(min_d, d);
    }
  }
  return sqrt(min_d);
}

// int main() {
//  srand(unsigned(time(NULL)));
//  Point* points = new Point[L];
//
//  for (int i = 0; i < L; i++) {
//    points[i].x = (rand() % 20000 / 10.0) - 1000;
//    points[i].y = (rand() % 20000 / 10.0) - 1000;
//  }
//
//  std::sort(points + 0, points + L - 1, CmpX);
//
//  printf("Result: %f\n", MinDistance(points, 0, L - 1));
//  printf("Result: %f\n", BruteForceMinDistance(points, 0, L - 1));
//}
