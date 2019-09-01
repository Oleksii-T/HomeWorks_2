//
//  Created by OleksiiTarbeiev on 04/04/2016.
//  Copyright © 2016 OleksiiTarbeiev. All rights reserved.
//
/* HomeWork 3 | semester 2 | Polynomials  */

#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <complex>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
string NameG="x";


ios_base & dummy_polynomial_manipulator ( ios_base & x )
{ 
  return x;
}

ios_base & ( *(polynomial_variable(const string & varName)) ) (ios_base & x)
{
  NameG = varName;
  return dummy_polynomial_manipulator;
}
class CPolynomial
{
  public:
    CPolynomial(void) {}
    ~CPolynomial(void) {}
    CPolynomial (const CPolynomial &);

    CPolynomial operator+ (const CPolynomial & rhs) const;
    CPolynomial operator- (const CPolynomial & rhs) const;
    bool operator== (const CPolynomial & rhs) const;
    bool operator!= (const CPolynomial & rhs) const;
    CPolynomial operator* (const CPolynomial & rhs) const;
    CPolynomial operator* (const double & rhs) const;
    double & operator[] (const int);
    const double & operator[] (const int) const;
    double operator() (const double) const;
    friend ostream & operator<< (ostream& os, const CPolynomial& rhs);

    unsigned int Degree() const;
    friend bool smallDiff ( double, double);
    friend string MakePolin (const vector<double> & a);
    friend bool dumpMatch (const CPolynomial &, const vector<double>&);
  private:
    vector <double> v;
};

CPolynomial::CPolynomial (const CPolynomial & obj) {
 for (int i=0;i<(int)obj.v.size();i++)
  v.push_back(obj.v[i]);
}

CPolynomial CPolynomial::operator+ (const CPolynomial & rhs) const {
 CPolynomial res;
 for (int i=0;i<(int)v.size();i++) {
  res.v.push_back(v[i] + rhs.v[i]);
  if (i == (int)rhs.v.size()-1)
   for (i+=1;i<(int)v.size();i++)
    res.v.push_back(v[i]);
  if (i == (int)v.size()-1)
   for (i+=1;i<(int)rhs.v.size();i++)
    res.v.push_back(rhs.v[i]);
 }
 return res;
}

CPolynomial CPolynomial::operator- (const CPolynomial & rhs) const{
 CPolynomial res;
 for (int i=0;i<(int)v.size();i++) {
  res.v.push_back(v[i] - rhs.v[i]);
  if (i == (int)rhs.v.size()-1)
   for (i+=1;i<(int)v.size();i++)
    res.v.push_back(v[i]);
  if (i == (int)v.size()-1)
   for (i+=1;i<(int)rhs.v.size();i++)
    res.v.push_back(rhs.v[i]*-1);
 }
 return res;
}

bool CPolynomial::operator== (const CPolynomial & rhs) const{
 for (int i=0; i<max((int)v.size(), (int)rhs.v.size()); i++) {
  if (i >= (int)v.size())
   if (rhs.v[i] == 0)
    continue;
   else
    return false;
  else if (i >= (int)rhs.v.size())
   if (v[i] == 0)
    continue;
   else
    return false;
  else if ( v[i] != rhs.v[i] ) 
    return false;
 }
 return true;
}


bool CPolynomial::operator!= (const CPolynomial & rhs) const{
 for (int i=0; i<max((int)v.size(), (int)rhs.v.size()); i++) {
  if (i >= (int)v.size())
   if (rhs.v[i] == 0)
    continue;
   else
    return true;
  else if (i >= (int)rhs.v.size())
   if (v[i] == 0)
    continue;
   else
    return true;
  else if ( v[i] != rhs.v[i] ) 
    return true;
 }
 return false;
}

CPolynomial CPolynomial::operator* (const CPolynomial & rhs) const{
 CPolynomial res;
 int Sres = Degree(), i;
 for (i=(int)rhs.v.size()-1;i>=0;i--)
  if (rhs.v[i] == 0)
   continue;
  else 
   break;
 Sres += i;
 res.v.resize(Sres+1, 0);
 for (i=0;i<(int)v.size();i++) 
  if (v[i]==0)
   continue;
  else
   for (int j=0;j<(int)rhs.v.size();j++) 
    if (rhs.v[j]==0)
     continue;
    else
     res.v[i+j] += v[i] * rhs.v[j];
 return res;
}

CPolynomial CPolynomial::operator* (const double & rhs) const{
 CPolynomial res;
 if (rhs == 0) {
  res.v.push_back(0);
  return res;
 }
 for (int i=0;i<(int)v.size();i++)
  res.v.push_back(v[i]*rhs);
 return res;
}

double & CPolynomial::operator[] (const int i) {
 if (i >= (int)v.size())
  v.resize (i+1, 0);
 return v[i];
}

const double & CPolynomial::operator[] (const int i) const{
 return v[i];
}

double CPolynomial::operator() (double x) const{
 double res=0;
 if ((int)v.size() == 0) 
  return res;
 for (int i=0;i<(int)v.size();i++) {
  if (v[i]==0)
   continue;
  res += v[i]*pow(x, i);
 }
 return res;
}

string MakePolin (const vector<double> & a) {
 stringstream s;
 int j=0;
 if ((int)a.size()==0) {
  s << "0";
  return s.str();
 }
 for (int i=(int)a.size()-1;i>=0;i--) {
  if (a[i] == 0)
   continue;
  if (j==0) {
    if (a[i] == -1)
     s << "- ";
    else if (a[i] < 0)
     s << "- " << a[i]*-1;
    else if (a[i] > 1)
     s << a[i];
  }
  else if (a[i] == 1)
   s << " + ";
  else if (a[i] == -1)
   s << " - ";
  else if (a[i] < 0)
    s << " - " << a[i]*-1;
  else if (a[i] > 0)
    s << " + " << a[i];  
//printf variable part
  if (i == 0 || a[i] == 0) {
   j++;
   continue;
  }
  else if (a[i] == 1 || a[i] == -1)
   s << NameG << "^" << i;
  else
   s << "*" << NameG << "^" << i;
  j++;
 }
 return s.str(); 
} 

ostream & operator<< (ostream & os, const CPolynomial & rhs) {
 string s=MakePolin (rhs.v);
 os << s;
 return os;
}

unsigned int CPolynomial::Degree(void) const{
 unsigned int i;
 for (i=(int)v.size()-1;i>=0;i--) {
  if (v[i] == 0) {
   if (i==0) {
    i=0;
    return i;   
   }
   continue;
  }
  else 
   return i; 
 }
}

#ifndef __PROGTEST__

bool smallDiff ( double a, double b ) {
   return fabs(a - b) < pow(1, -10);
} 

bool dumpMatch ( const CPolynomial & x, const vector<double> & ref ) {
 for (int i=0; i<max((int)x.v.size(), (int)ref.size()); i++) {
  if (i >= (int)x.v.size())
   if (ref[i] == 0)
    continue;	
   else
    return false;
  else if (i >= (int)ref.size())
   if (x.v[i] == 0)
    continue;
   else
    return false;
  else if ( x.v[i] != ref[i] ) 
    return false;
 }
 return true;
}

int  main ( void )
{
  CPolynomial a, b, c;
  ostringstream out;

  a[0] = -10; 
  a[1] = 3.5;
  a[3] = 1;
  assert ( smallDiff ( a ( 2 ), 5 ) );
  out.str ("");
  out << a;
  assert ( out.str () == "x^3 + 3.5*x^1 - 10" );
  a = a * -2;
  assert (a.Degree () == 3 && 
  			  dumpMatch (a, vector<double>{ 20.0, -7.0, -0.0, -2.0 }));
  out.str ("");
  out << a;
  assert ( out.str () == "- 2*x^3 - 7*x^1 + 20" );
  out.str ("");
  out << b;
  assert ( out.str () == "0" );
  b[5] = -1;
  out.str ("");
  out << b;
  assert ( out.str () == "- x^5" );
  c = a + b;
  assert (c.Degree () == 5 && 
       dumpMatch (c,vector<double>{20.0, -7.0, 0.0, -2.0, 0.0, -1.0}));

  out.str ("");
  out << c;
  assert ( out.str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a - b;
  assert (c.Degree () == 5 && 
     dumpMatch (c, vector<double>{20.0, -7.0, -0.0, -2.0, -0.0, 1.0}));

  out.str ("");
  out << c;
  assert ( out.str () == "x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a * b;
  assert ( c.Degree () == 8 && 
           dumpMatch (c, vector<double>{ 0.0, -0.0, 0.0, -0.0, -0.0, -20.0, 7.0, 0.0, 2.0 }));

  out.str ("");
  out << c;
  assert ( out.str () == "2*x^8 + 7*x^6 - 20*x^5" );
  assert ( a != b );
  b[5] = 0;
  assert ( !(a == b) );
  a = a * 0;
  assert ( a.Degree () == 0
           && dumpMatch ( a, vector<double>{ 0.0 } ) );
  assert ( a == b );


  // bonus
  a[2] = 4;
  a[1] = -3;
  b[3] = 7;
  out . str ("");
  out << polynomial_variable ( "y" ) << "a=" << a << ", b=" << b;
  assert ( out . str () == "a=4*y^2 - 3*y^1, b=7*y^3" );

  out . str ("");
  out << polynomial_variable ( "test" ) << c;
  assert ( out . str () == "2*test^8 + 7*test^6 - 20*test^5" );

  return 0;
}
#endif /* __PROGTEST__ */


/*

The task is to develop a C++ class CPolynomial representing polynomials of arbitrary degree.

The class will store individual coefficients (floating point numbers - double precision), moreover, public interface will offer overloaded operators and methods to manipulate polynomials:

default constructor
    the constructor will prepare an object representing polynomial 0,
copy constructor
    the constructor will be implemented if your internal representation requires it,
destructor
    destructor will be present if your internal representation requires it,
overloaded operator=
    the operator will copy the contents from one instance to another (if the automatically generated op= cannot be used with your implementation).
operator <<
    will output a textual form of the polynomial into an output stream. The output formatting must follow these rules:

        the polynomial will be displayed in a decreasing order of powers, i.e. from the highest power of x,terms with zero coefficient are not displayed in the output,
        terms with coefficient +1 and -1 are displayed without the coefficient (just a power of x),
        there are not unnecessary - in the output (i.e. x^1 - 9 or - x^2 + 4 is OK, whereas x^1 + (-9) is not),
        zero polynomial will be displayed as 0.

    The variable displayed in the output shall be x by default. There is manipulator, the manipulator may be used to change the default x to any other variable (bonus test only).

operator +
    adds two polynomials,
operator -
    subtracts two polynomials,
operator *
    multiplies given polynomial either with a double, or with another polynomial,
operators == and !=
    compare two polynomials,
operator []
    is used to access (read / write) individual coefficients, the term is determined by the index (0 = absolute, 1 = x, 2 = x^2, ... ). The read form must be available even on const instances,
operator ()
    evaluates the value of the polynomial for the given value x (x is a double),
Degree()
    the method returns the degree of the polynomial (e.g. x^3+4 has degree of 3, 5 has degree 0, specifically 0 has degree 0).
manipulator polynomial_variable ( name )
    This manipulator changes the name of the variable use to display the polynomial. The default is to display the polynomial with variable x. The manipulator may be used, to change the variable name to any string (the parameter). The implementation of the manipulator is an extension - the manipulator is tested only in the bonus tests. If you do not intent to pass the bonus tests, keep the sample implementation of the manipulator. The delivered implementation of polynomial_variable does not do anything useful, it is just present, thus programs that use the manipulator actually compile.

Submit a source file with your implementation of CPolynomial class. The submitted file shall not contain any #include directives nor main function. If your main function or #include remains in the file, please place them into a conditional compile block.

This task does not provide the required class interface. Instead, you are expected to develop the interface yourself. Use the description above, the attached examples, and your knowledge of overloaded operators.
Advice

    The testing environment uses output operator (<<) and indexing operator [] to examine your instance. If your overloaded operators << and [] do not work properly, the tests will be negative.
    Implement the output operator properly -- do not blindly sent the data to cout. Instead, send the data to the output stream passed as the parameter. Do not add any extra whitespace/newline characters.
    If your program does not compile (and especially if it compiles locally, however it does not compile in Progtest), there might be some problem in your interface design. Check your operator overloads, pay special attention to const qualifiers.
    You have to either implement your own functioning manipulator polynomial_variable, or the default (delivered) implementation must be preserved. If the default implementation is removed and your implementation is not supplied, the program will not compile.
    The bonus test requires very fast polynomial multiplication. The test multiplies polynomials of degree approx. 100000. Naive solution cannot pass the test, an efficient algorithms is required.
    CPolynomial class is tested for copying. Your implementation must include copy cons/op= if the implementation cannot use automatically generated copy cons/op=.
    STL classes std::vector and std::string are available. However, the rest of STL is not.







Submission deadline: 	2017-04-09 23:59:59 	 
Evaluation: 	5.5000
Max. assessment: 	5.0000 (Without bonus points)
Submissions: 	7 / 25 Free retries + 20 Penalized retries (-2 % penalty each retry)
Advices: 	2 / 2 Advices for free + 2 Advices with a penalty (-10 % penalty each advice)
*/

