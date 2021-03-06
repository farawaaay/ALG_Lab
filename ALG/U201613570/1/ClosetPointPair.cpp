#include <float.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#define PointPair pair<Point, Point>
#define PointPairs vector<PointPair>
#define Points vector<Point>
#define IndexList vector<size_t>

using namespace std;

struct Point {
  double x;
  double y;
  bool operator==(Point p) { return p.x == this->x && p.y == this->y; }
};

double Min(double a, double b) {
  return a < b ? a : b;
}

double Distance(Point a, Point b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double Distance(PointPair pp) {
  return Distance(pp.first, pp.second);
}

double MinDistance(Points points_x, Points points_y, PointPairs& record) {
  if (points_x.size() < 2) {
    return DBL_MAX;
  }

  if (points_x.size() == 2) {
    PointPair p(points_x[0], points_x[1]);
    record = {p};
    return Distance(p);
  }

  // split into 2
  Points lpx, rpx, lpy, rpy;

  for (int i = 0; i < points_x.size(); i++) {
    if (i < points_x.size() / 2)
      lpx.push_back(points_x[i]);
    else
      rpx.push_back(points_x[i]);
  }

  double mid_x = points_x[points_x.size() / 2].x;

  for (int i = 0; i < points_y.size(); i++) {
    if (points_y[i].x < mid_x)
      lpy.push_back(points_y[i]);
    else
      rpy.push_back(points_y[i]);
  }

  PointPairs lpp, rpp;
  double dL = MinDistance(lpx, lpy, lpp);
  double dR = MinDistance(rpx, rpy, rpp);
  double min = Min(dL, dR);

  if (dL < dR) {
    record = lpp;
  } else if (dL > dR) {
    record = rpp;
  } else {
    record.reserve(lpp.size() + rpp.size());
    record.insert(record.end(), lpp.begin(), lpp.end());
    record.insert(record.end(), rpp.begin(), rpp.end());
  }

  vector<Point> in_range;
  for (auto point_y : points_y) {
    if (fabs(point_y.x - mid_x) <= min)
      in_range.push_back(point_y);
  }

  for (size_t i = 0; i < in_range.size(); i++) {
    for (size_t j = i + 1; j < in_range.size() && j <= i + 7; j++) {
      PointPair p(in_range[i], in_range[j]);
      double dis = Distance(p);
      if (dis < min) {
        min = dis;
        record = {p};
      } else if (dis == min) {
        bool added = false;
        for (auto __p : record)
          if (__p.first == p.first && __p.second == p.second ||
              __p.first == p.second && __p.second == p.first)
            added = true;

        if (!added)
          record.push_back(p);
      }
    }
  }
  return min;
}

double BruteForceMinDistance(Points p) {
  double min_d = DBL_MAX;
  for (size_t i = 0; i < p.size(); i++) {
    for (size_t j = i + 1; j < p.size(); j++) {
      double dx = (p[i].x - p[j].x);
      double dy = (p[i].y - p[j].y);
      double d = dx * dx + dy * dy;
      min_d = Min(min_d, d);
    }
  }
  return sqrt(min_d);
}

int main() {
  FILE* pFileIn;
  FILE* pFileOut;
  if (pFileIn = fopen("in.dat", "r"))
    pFileOut = fopen("out.dat", "wb");
  else
    return -1;
  int n;
  fscanf(pFileIn, "%d", &n);
  for (int i = 0; i < n; i++) {
    int m;
    fscanf(pFileIn, "%d", &m);

    Points points_x;
    PointPairs rec;

    for (int i = 0; i < m; i++) {
      int x, y;
      fscanf(pFileIn, "%d %d", &x, &y);
      points_x.push_back({(double)x, (double)y});
    }

    Points points_y(points_x);

    std::sort(points_x.begin(), points_x.end(),
              [](Point a, Point b) -> bool { return a.x < b.x; });
    std::sort(points_y.begin(), points_y.end(),
              [](Point a, Point b) -> bool { return a.y < b.y; });

    MinDistance(points_x, points_y, rec);

    for (auto pp : rec) {
      fprintf(pFileOut, "%d %d %d %d ", (int)pp.first.x, (int)pp.first.y,
              (int)pp.second.x, (int)pp.second.y);
    }
    fprintf(pFileOut, "\n");
  }
  fclose(pFileOut);
  fclose(pFileIn);

  return 0;
}
