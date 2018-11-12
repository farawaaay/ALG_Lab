#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Bigint {
 private:
  std::vector<int> number;
  bool positive;
  unsigned int skip;
  static const int base = 1 << 16;

 public:
  Bigint();                                                       // Constructors
  Bigint(std::string);                                            // Constructors
  Bigint(const Bigint& b);                                        // Constructors
  Bigint operator+(Bigint const&) const;                          // Adding
  Bigint& operator+=(long long);                                  // Adding
  Bigint operator-(Bigint const&) const;                          // Subtraction
  Bigint operator*(Bigint const&);                                // Multiplication
  friend std::istream& operator>>(std::istream&, Bigint&);        // Input&Output
  friend std::ostream& operator<<(std::ostream&, Bigint const&);  // Input&Output
  int SegmentLength(int) const;
};

// Constructor
Bigint::Bigint() {
  positive = true;
  skip = 0;
}
Bigint::Bigint(const Bigint& b)
    : number(b.number),
      positive(b.positive),
      skip(b.skip) {}

Bigint::Bigint(std::string stringInteger) {
  int size = stringInteger.length();
  skip = 0;
  positive = (stringInteger[0] != '-');
  while (true) {
    if (size <= 0)
      break;
    if (!positive && size <= 1)
      break;
    int length = 0;
    int num = 0;
    int prefix = 1;
    for (int i = size - 1; i >= 0 && i >= size - 9; --i) {
      if (stringInteger[i] < '0' || stringInteger[i] > '9')
        break;
      num += (stringInteger[i] - '0') * prefix;
      prefix *= 10;
      ++length;
    }
    number.push_back(num);
    size -= length;
  }
}

// Adding
Bigint Bigint::operator+(Bigint const& b) const {
  Bigint c = *this;
  if (!b.positive)
    return c - b;
  std::vector<int>::iterator it1 = c.number.begin();
  std::vector<int>::const_iterator it2 = b.number.begin();
  int sum = 0;
  while (it1 != c.number.end() || it2 != b.number.end()) {
    if (it1 != c.number.end())
      sum += *it1;
    else {
      c.number.push_back(0);
      it1 = c.number.end() - 1;
    }
    if (it2 != b.number.end()) {
      sum += *it2;
      ++it2;
    }
    *it1 = sum % base;
    ++it1;
    sum /= base;
  }
  if (sum)
    c.number.push_back(1);
  return c;
}

Bigint& Bigint::operator+=(long long b) {
  std::vector<int>::iterator it = number.begin();
  if (skip > number.size())
    number.insert(number.end(), skip - number.size(), 0);
  it += skip;
  bool initial_flag = true;
  while (b || initial_flag) {
    initial_flag = false;
    if (it != number.end()) {
      *it += b % base;
      b /= base;
      b += *it / base;
      *it %= base;
      ++it;
    } else {
      number.push_back(0);
      it = number.end() - 1;
    }
  }
  return *this;
}

// Subtraction
Bigint Bigint::operator-(Bigint const& b) const {
  Bigint c = *this;
  std::vector<int>::iterator it1 = c.number.begin();
  std::vector<int>::const_iterator it2 = b.number.begin();
  int dif = 0;
  while (it1 != c.number.end() || it2 != b.number.end()) {
    if (it1 != c.number.end()) {
      dif += *it1;
      ++it1;
    }
    if (it2 != b.number.end()) {
      dif -= *it2;
      ++it2;
    }
    if (dif < 0) {
      *(it1 - 1) = dif + base;
      dif = -1;
    } else {
      *(it1 - 1) = dif % base;
      dif /= base;
    }
  }
  if (dif < 0)
    c.positive = false;
  if (number.size() > 1) {
    do {
      it1 = c.number.end() - 1;
      if (*it1 == 0)
        c.number.pop_back();
      else
        break;
    } while (number.size() > 1);
  }
  return c;
}

// Multiplication
Bigint Bigint::operator*(Bigint const& b) {
  std::vector<int>::iterator it1;
  std::vector<int>::const_iterator it2;
  Bigint c;
  for (it1 = number.begin(); it1 != number.end(); ++it1) {
    for (it2 = b.number.begin(); it2 != b.number.end(); ++it2) {
      c.skip = (unsigned int)(it1 - number.begin()) + (it2 - b.number.begin());
      c += (long long)(*it1) * (*it2);
    }
  }
  c.skip = 0;
  return c;
}

// Input & Output
std::ostream& operator<<(std::ostream& out, Bigint const& a) {
  if (!a.number.size())
    return out << 0;
  int i = a.number.size() - 1;
  for (; i >= 0 && a.number[i] == 0; --i)
    ;
  if (i == -1)
    return out << 0;
  if (!a.positive)
    out << '-';
  std::vector<int>::const_reverse_iterator it = a.number.rbegin() + (a.number.size() - i - 1);
  out << *it++;
  for (; it != a.number.rend(); ++it) {
    for (int i = 0, len = a.SegmentLength(*it); i < 9 - len; ++i)
      out << '0';
    if (*it)
      out << *it;
  }
  return out;
}

std::istream& operator>>(std::istream& in, Bigint& a) {
  std::string str;
  in >> str;
  a = str;
  return in;
}

int Bigint::SegmentLength(int segment) const {
  int length = 0;
  while (segment) {
    segment /= 10;
    ++length;
  }
  return length;
}

int main() {
  ifstream infile("in.dat");
  ofstream outfile("out.dat");
  int n;
  infile >> n;
  for (int i = 0; i < n; i++) {
    int flag;
    string x, y;
    infile >> x >> y >> flag;
    Bigint a(x);
    Bigint b(y);
    if (flag == 1)
      outfile << a + b << endl;
    else if (flag == 2)
      outfile << a - b << endl;
    else if (flag == 3)
      outfile << a * b << endl;
  }
  infile.close();
  outfile.close();
  return 0;
}
