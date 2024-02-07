#ifndef CCOMPLEX
#define CCOMPLEX

#define PI 3.141592653589793238462643383
#define SmallNo 1.e-14
#define DEG 0.01745329251994329576923690768

class CComplex {
  public:
  // data members
  double re, im;

  // member functions
  CComplex();
  CComplex(double x);
  CComplex(int x);
  CComplex(__int64 x);
  CComplex(double x, double y);
  CComplex Sqrt();
  CComplex Conj();
  CComplex Inv();
  void Set(double x, double y);
  double Abs();
  double Arg();
  double Re();
  double Im();
  char* ToString(char* s);
  char* ToStringAlt(char* s);

  // operator redefinition
  // Addition
  CComplex operator+(const CComplex& z);
  CComplex operator+(double z);
  CComplex operator+(int z);
  CComplex operator+(__int64 z);
  friend CComplex operator+(int x, const CComplex& y);
  friend CComplex operator+(__int64 x, const CComplex& y);
  friend CComplex operator+(double x, const CComplex& y);
  friend CComplex operator+(const CComplex& x, const CComplex& y);
  void operator+=(const CComplex& z);
  void operator+=(double z);
  void operator+=(int z);
  void operator+=(__int64 z);

  // Subtraction
  CComplex operator-();
  CComplex operator-(const CComplex& z);
  CComplex operator-(double z);
  CComplex operator-(int z);
  CComplex operator-(__int64 z);
  friend CComplex operator-(int x, const CComplex& y);
  friend CComplex operator-(__int64 x, const CComplex& y);
  friend CComplex operator-(double x, const CComplex& y);
  friend CComplex operator-(const CComplex& x, const CComplex& y);
  friend CComplex operator-(const CComplex& x);
  void operator-=(const CComplex& z);
  void operator-=(double z);
  void operator-=(int z);
  void operator-=(__int64 z);

  // Multiplication
  CComplex operator*(const CComplex& z);
  CComplex operator*(double z);
  CComplex operator*(int z);
  CComplex operator*(__int64 z);
  friend CComplex operator*(int x, const CComplex& y);
  friend CComplex operator*(__int64 x, const CComplex& y);
  friend CComplex operator*(double x, const CComplex& y);
  friend CComplex operator*(const CComplex& x, const CComplex& y);
  void operator*=(const CComplex& z);
  void operator*=(double z);
  void operator*=(int z);
  void operator*=(__int64 z);

  // Division
  CComplex operator/(const CComplex& z);
  CComplex operator/(double z);
  CComplex operator/(int z);
  CComplex operator/(__int64 z);
  friend CComplex operator/(int x, const CComplex& y);
  friend CComplex operator/(__int64 x, const CComplex& y);
  friend CComplex operator/(double x, const CComplex& y);
  friend CComplex operator/(const CComplex& x, const CComplex& y);
  void operator/=(const CComplex& z);
  void operator/=(double z);
  void operator/=(int z);
  void operator/=(__int64 z);

  // Equals
  void operator=(double z);
  void operator=(int z);
  void operator=(__int64 z);

  // Tests
  bool operator==(const CComplex& z);
  bool operator==(double z);
  bool operator==(int z);
  bool operator==(__int64 z);

  bool operator!=(const CComplex& z);
  bool operator!=(double z);
  bool operator!=(int z);
  bool operator!=(__int64 z);

  bool operator<(const CComplex& z);
  bool operator<(double z);
  bool operator<(int z);
  bool operator<(__int64 z);

  bool operator<=(const CComplex& z);
  bool operator<=(double z);
  bool operator<=(int z);
  bool operator<=(__int64 z);

  bool operator>(const CComplex& z);
  bool operator>(double z);
  bool operator>(int z);
  bool operator>(__int64 z);

  bool operator>=(const CComplex& z);
  bool operator>=(double z);
  bool operator>=(int z);
  bool operator>=(__int64 z);

  private:
};

// useful functions...
#define I CComplex(0, 1)
double Re(const CComplex& a);
double Im(const CComplex& a);
double abs(const CComplex& x);
double absq(const CComplex& x);
double arg(const CComplex& x);
CComplex conj(const CComplex& x);
CComplex exp(const CComplex& x);
CComplex sqrt(const CComplex& x);
CComplex tanh(const CComplex& x);
CComplex sinh(const CComplex& x);
CComplex cosh(const CComplex& x);
CComplex cos(const CComplex& x);
CComplex acos(const CComplex& x);
CComplex sin(const CComplex& x);
CComplex asin(const CComplex& x);
CComplex tan(const CComplex& x);
CComplex atan(const CComplex& x);
CComplex atan2(const CComplex& y, const CComplex& x);
CComplex log(const CComplex& x);
CComplex pow(const CComplex& x, int y);
CComplex pow(const CComplex& x, double y);
CComplex pow(const CComplex& x, const CComplex& y);
CComplex Chop(const CComplex& a, double tol = 1.e-12);

#endif // CCOMPLEX check