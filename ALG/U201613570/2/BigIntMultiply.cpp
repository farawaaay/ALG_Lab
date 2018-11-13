#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define BASE 100000000
#define BASE_LOG_10 8
#define IntList vector<int>

class BigInt {
 private:
  IntList digits;
  bool sign;

 public:
  BigInt(string);
  BigInt(IntList, bool);
  ~BigInt();
  BigInt operator-() const;
  BigInt operator-(BigInt const&) const;
  BigInt operator+(BigInt const&) const;
  BigInt operator*(BigInt const&)const;
  BigInt operator<<(size_t s) const;
  bool operator>(BigInt const&) const;
  bool operator<(BigInt const&) const;
  bool operator==(BigInt const&) const;
  friend std::istream& operator>>(std::istream&, BigInt&);        // Input&Output
  friend std::ostream& operator<<(std::ostream&, BigInt const&);  // Input&Output
};

BigInt::~BigInt() {}
BigInt::BigInt(string s) {
  digits = {};
  sign = s.substr(0, 1) != "-";  // true for positive
  if (!sign) {
    s = s.substr(1);
  }
  size_t l = s.length();
  size_t i = l % BASE_LOG_10;
  if (i != 0)
    digits = {stoi(s.substr(0, i))};
  for (; i < l; i += BASE_LOG_10) {
    digits.insert(digits.begin(), stoi(s.substr(i, BASE_LOG_10)));
  }

  size_t ll = digits.size();
}

BigInt::BigInt(IntList digits, bool sign)
    : digits(digits), sign(sign) {}

BigInt BigInt::operator-() const {
  return BigInt(this->digits, !this->sign);
}

bool BigInt::operator>(BigInt const& b) const {
  BigInt a = *this;
  size_t a_l = a.digits.size();
  size_t b_l = b.digits.size();
  if (a_l > b_l) {
    return true;
  } else if (a_l < b_l) {
    return false;
  }
  for (size_t i = a_l - 1; i >= 0; i++) {
    if (a.digits[i] > b.digits[i]) {
      return true;
    } else if (a.digits[i] < b.digits[i]) {
      return false;
    }
  }

  return false;
}

bool BigInt::operator==(BigInt const& b) const {
  BigInt a = *this;
  size_t a_l = a.digits.size();
  size_t b_l = b.digits.size();
  if (a_l == b_l) {
    for (size_t i = 0; i < a_l; i++) {
      if (a.digits[i] != b.digits[i]) {
        return false;
      }
    }
    return true;
  }
  return false;
}

bool BigInt::operator<(BigInt const& b) const {
  BigInt a = *this;
  if (a > b) {
    return false;
  }
  if (a == b) {
    return false;
  }

  return true;
}

BigInt BigInt::operator+(BigInt const& b) const {
  BigInt a = *this;
  BigInt _b(b.digits, b.sign);

  while (_b.digits.size() > 0 && _b.digits.back() == 0)
    _b.digits.pop_back();

  if (a.digits.size() == 0) {
    return b;
  }

  if (b.digits.size() == 0) {
    return a;
  }

  if (a.sign != b.sign) {
    return a - (-b);
  }

  BigInt c({}, a.sign);
  int carry = 0;
  for (int i = 0; i < max(a.digits.size(), b.digits.size()); i++) {
    int a_d = i < a.digits.size() ? a.digits[i] : 0;
    int b_d = i < b.digits.size() ? b.digits[i] : 0;
    c.digits.push_back(a_d + b_d + carry);
    if (c.digits[i] >= BASE) {
      carry = 1;
      c.digits[i] -= BASE;
    } else {
      carry = 0;
    }
  }

  if (carry) {
    c.digits.push_back(carry);
  }

  while (c.digits.size() > 0 && c.digits.back() == 0) {
    c.digits.pop_back();
  }

  return c;
}

BigInt BigInt::operator-(BigInt const& b) const {
  BigInt a = *this;
  if (a.digits.size() == 0) {
    return b;
  }

  if (b.digits.size() == 0) {
    return a;
  }

  if (a.sign != b.sign) {
    return a + (-b);
  }

  if (a < b) {
    return -(b - a);
  }

  BigInt c({}, a.sign);
  int borrow = 0;
  for (int i = 0; i < max(a.digits.size(), b.digits.size()); i++) {
    int a_d = i < a.digits.size() ? a.digits[i] : 0;
    int b_d = i < b.digits.size() ? b.digits[i] : 0;
    c.digits.push_back(a_d - b_d - borrow);
    if (c.digits[i] < 0) {
      borrow = 1;
      c.digits[i] += BASE;
    } else {
      borrow = 0;
    }
  }

  while (c.digits.size() > 0 && c.digits.back() == 0)
    c.digits.pop_back();

  return c;
};

BigInt BigInt::operator*(BigInt const& b) const {
  BigInt a = *this;
  BigInt c({}, !(a.sign ^ b.sign));
  size_t a_l = a.digits.size();
  size_t b_l = b.digits.size();

  if (a_l == 0 || b_l == 0) {
    return c;
  }

  if (/* a_l <= 1 || b_l <= 1 */ true) {
    if (a_l < b_l) {
      return b * a;
    }
    long long carry = 0;
    for (int i = 0; i < a_l; i++) {
      for (int j = 0; j < b_l; j++) {
        long long ret = (long long)a.digits[i] * (long long)b.digits[j];
        BigInt tmp({(int)(ret % BASE), int(ret / BASE)}, c.sign);
        if (tmp.digits.back() == 0) {
          tmp.digits.pop_back();
        }
        c = c + (tmp << (i + j));
      }
    }

    while (c.digits.size() > 0 && c.digits.back() == 0)
      c.digits.pop_back();

    return c;
  }

  // div and conq
  size_t max_l = max(a_l, b_l);
  size_t split_pos = max_l % 2 == 0 ? max_l >> 1 : (max_l + 1) >> 1;
  // split a, b
  BigInt a_low({}, c.sign);
  BigInt a_high({}, c.sign);
  BigInt b_low({}, c.sign);
  BigInt b_high({}, c.sign);

  for (int i = 0; i < max_l; i++) {
    if (i < split_pos) {
      if (i < a.digits.size())
        a_low.digits.push_back(a.digits[i]);
      if (i < b.digits.size())
        b_low.digits.push_back(b.digits[i]);
    } else {
      if (i < a.digits.size())
        a_high.digits.push_back(a.digits[i]);
      if (i < b.digits.size())
        b_high.digits.push_back(b.digits[i]);
    }
  }
  while (a_low.digits.size() > 0 && a_low.digits.back() == 0)
    a_low.digits.pop_back();
  while (b_low.digits.size() > 0 && b_low.digits.back() == 0)
    b_low.digits.pop_back();
  while (a_high.digits.size() > 0 && a_high.digits.back() == 0)
    a_high.digits.pop_back();
  while (b_high.digits.size() > 0 && b_high.digits.back() == 0)
    b_high.digits.pop_back();

  BigInt z0 = a_low * b_low;
  BigInt z1 = (a_low + a_high) * (b_low + b_high);
  BigInt z2 = a_high * b_high;
  BigInt z3 = z1 - z2 - z0;

  for (int i = 0; i < 2 * split_pos; i++) {
    if (i % 2 == 0) {
      z3.digits.insert(z3.digits.begin(), 0);
    }
    z2.digits.insert(z2.digits.begin(), 0);
  }

  BigInt z8 = z2 + z3 + z0;
  z8.sign = c.sign;
  return z8;
}

BigInt BigInt::operator<<(size_t s) const {
  BigInt a = *this;
  for (int i = 0; i < s; i++) {
    a.digits.insert(a.digits.begin(), 0);
  }

  return a;
}

std::ostream& operator<<(std::ostream& out, BigInt const& a) {
  if (!a.sign) {
    out << "-";
  }
  if (a.digits.size() == 0) {
    out << "0";
    return out;
  }

  for (int i = a.digits.size() - 1; i >= 0; i--) {
    string s = to_string(a.digits[i]);
    if (s.length() == BASE_LOG_10 || i == a.digits.size() - 1) {
      out << s;
    } else {
      if (BASE_LOG_10 > 1) {
        for (int i = 0; i < BASE_LOG_10 - s.length(); i++) {
          out << "0";
        }
      }
      out << s;
    }
  }

  return out;
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
    BigInt a(x);
    BigInt b(y);
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