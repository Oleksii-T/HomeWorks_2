//
//  Created by OleksiiTarbeiev on 28/03/2017.
//  Copyright © 2016 OleksiiTarbeiev. All rights reserved.
//
/* HomeWork 2 | semester 2 | VAT reports  */

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;


class CVATRegister
{
  public:
   CVATRegister (void); //Constructor
   ~CVATRegister (void); //Destructor
   
   bool NewCompany (const string & name, const string & addr, const string & taxID);
    bool CancelCompany (const string & name, const string & addr);
   bool CancelCompany  (const string & taxID );
    bool Invoice (const string & taxID, unsigned int amount );
   bool Invoice (const string & name, const string & addr, unsigned int amount );
    bool Audit (const string & name, const string & addr, unsigned int & sumIncome) const;
   bool Audit (const string & taxID, unsigned int & sumIncome) const;
	  unsigned int MedianInvoice (void) const;   
   void pv (const vector<int> & vec);
  private:
   vector<pair<int,string>> id; //TaxID
   vector<pair<int,string>> n_a; //name and address
   vector<int> sum; // total revenue
   vector<int> invoicelist; //list of all invoices for Median invoice
};

CVATRegister::CVATRegister (void) //Constructor
 { cout << "@Constructor->" << endl; }


CVATRegister::~CVATRegister (void) //Destructor
 { cout << "@Destructor->" << endl; }


bool CVATRegister::NewCompany(const string & name, const string & addr, const string & taxID) {
 cout << "@NewComp->|j=" << id.size() << endl;

 string NameAddress = name+"~%$"+addr;
 transform(NameAddress.begin(), NameAddress.end(),
 					 NameAddress.begin(), ::tolower);
 cout << " '" << NameAddress << "|" << taxID << "'" << endl;
 if (id.size()==0) {
  id.push_back(make_pair(0,taxID));
  n_a.push_back(make_pair(0,NameAddress));
  sum.push_back(0);
  return true;
 }

 auto it1 = lower_bound(id.begin(), id.end(), taxID,
          [](pair<int, string> const & x, string d)
          { return x.second < d; });
 if (it1!=id.end()) // if it1==end() then element isnt existing. and 
  									// should be enserted at the and
  if ( id[it1-id.begin()].second == taxID ) 
   return false;
 
 auto it2 = lower_bound(n_a.begin(), n_a.end(), NameAddress,
          [](pair<int, string> const & x, string d)
          { return x.second < d; });
 if (it2!=n_a.end())
  if (n_a[it2-n_a.begin()].second == NameAddress) 
   return false;
 
 id.insert ( it1, {id.size(), taxID} );
 n_a.insert ( it2, {n_a.size(), NameAddress} );
 sum.push_back(0);

 return true;
}


bool CVATRegister::CancelCompany (const string & name,const string & addr) {
 cout << "@Cancel Company1->" << endl; 

 string NameAddress = name+"~%$"+addr;
 transform(NameAddress.begin(), NameAddress.end(), 
           NameAddress.begin(), ::tolower);
           
 auto it = lower_bound(n_a.begin(), n_a.end(), NameAddress,
          [](pair<int, string> const & x, string d)
          { return x.second < d; });
 if (it==n_a.end())
  return false;
 if (n_a[it-n_a.begin()].second != NameAddress)
  return false;

 int target = n_a[it-n_a.begin()].first, i;
 for (i=0;i<(int)id.size();i++)
  if (id[i].first==target)
   break;

 n_a.erase(it);
 id.erase(id.begin()+i);
 sum.erase(sum.begin()+target);

 for (i=0;i<(int)id.size();i++) {
  if (id[i].first>target)
   id[i].first -= 1;
  if (n_a[i].first>target)
   n_a[i].first -= 1;
 }
 return true;
}


bool CVATRegister::CancelCompany (const string & taxID ) {
 cout << "@Cancel Company2->" << endl; 

 
 auto it = lower_bound(id.begin(), id.end(), taxID,
          [](pair<int, string> const & x, string d)
          { return x.second < d; });
 if (it==id.end())
  return false;
 if (id[it-id.begin()].second != taxID)
  return false;
  
 int target = id[it-id.begin()].first, i;
 for (i=0;i<(int)n_a.size();i++)
  if (n_a[i].first==target)
   break;
  
 id.erase(it);
 n_a.erase(n_a.begin()+i);
 sum.erase(sum.begin()+target);

 for (i=0;i<(int)id.size();i++) {
  if (id[i].first>target)
   id[i].first -= 1;
  if (n_a[i].first>target)
   n_a[i].first -= 1;
 }
  
 return true;
}


bool CVATRegister::Invoice (const string & taxID, unsigned int amount) {
 cout << "@Invoice1->" << endl; 

 auto it = lower_bound(id.begin(), id.end(), taxID,
          [](pair<int, string> const & x, string d)
          { return x.second < d; });
 if (it==id.end())
  return false; 
 if (id[it-id.begin()].second != taxID)
  return false;


 int index = id[it-id.begin()].first;
 sum[index] += amount;
 
 invoicelist.push_back(amount);
 
 return true;
}


bool CVATRegister::Invoice (const string & name, const string & addr, unsigned int amount ) {
 cout << "@Invoice2->" << endl;  
 string NameAddress = name+"~%$"+addr;
 transform(NameAddress.begin(), NameAddress.end(),
           NameAddress.begin(), ::tolower);

 auto it = lower_bound(n_a.begin(), n_a.end(), NameAddress,
          [](pair<int, string> const & x, string d)
          { return x.second < d; });
 if (it==n_a.end())
  return false;
 if (n_a[it-n_a.begin()].second != NameAddress)
  return false;
 
 int index = n_a[it-n_a.begin()].first;
 sum[index] += amount;
 
 invoicelist.push_back(amount);
 
 return true;
}


bool CVATRegister::Audit (const string & name, const string & addr, unsigned int & sumIncome) const {
 cout << "@Audit1->" << endl; 
 string NameAddress = name+"~%$"+addr;
 transform(NameAddress.begin(), NameAddress.end(), 
           NameAddress.begin(), ::tolower);

 auto it = lower_bound(n_a.begin(), n_a.end(), NameAddress,
          [](pair<int, string> const & x, string d)
          { return x.second < d; });
 if (it==n_a.end())
  return false;
 if (n_a[it-n_a.begin()].second != NameAddress)
  return false;

 int index = n_a[it-n_a.begin()].first;
 sumIncome = sum[index];

 return true;
}


bool CVATRegister::Audit (const string & taxID, unsigned int & sumIncome) const {
 cout << "@Audit2->" << endl;

 auto it = lower_bound(id.begin(), id.end(), taxID,
          [](pair<int, string> const & x, string d)
          { return x.second < d; });
 if (it==id.end())
  return false;
 if (id[it-id.begin()].second != taxID)
  return false;

 int index = id[it-id.begin()].first;
 sumIncome = sum[index];

 return true;
}


unsigned int CVATRegister::MedianInvoice (void) const {
 cout << "@MedianInvoice->" << endl;
 
 unsigned int medsum=0;
 vector<int> buf;

 for (int i=0;i<(int)invoicelist.size();i++)
   buf.push_back(invoicelist[i]);
   
 if (buf.size()== 0)
  return medsum;

 sort( buf.begin(), buf.end() );
 
 if (buf.size() % 2) { //not multipler of 2
  medsum = buf[ ( (buf.size()+1) /2)-1 ];
  return medsum;
 }
 medsum = buf[ ( (buf.size()+2) /2)-1 ];
 return medsum;
}

void CVATRegister::pv (const vector<int> & vec)
{
 cout << "Your vector-->";
 for (auto x: vec)
  cout << "  " << x;
 cout << "\n\n";
}

int main ( void )
{
  unsigned int sumIncome;

  CVATRegister b1;
  assert ( b1 . NewCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . NewCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . NewCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . Invoice ( "666/666", 2000 ) );
  assert ( b1 . MedianInvoice () == 2000 );
  assert ( b1 . Invoice ( "666/666/666", 3000 ) );
  assert ( b1 . MedianInvoice () == 3000 );
  assert ( b1 . Invoice ( "123456", 4000 ) );
  assert ( b1 . MedianInvoice () == 3000 );
  assert ( b1 . Invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . MedianInvoice () == 4000 );
  assert ( b1 . Audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . Audit ( "123456", sumIncome ) && sumIncome == 4000 );
  assert ( b1 . CancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . MedianInvoice () == 4000 );
  assert ( b1 . CancelCompany ( "666/666" ) );
  assert ( b1 . MedianInvoice () == 4000 );
  assert ( b1 . Invoice ( "123456", 100 ) );
  assert ( b1 . MedianInvoice () == 3000 );
  assert ( b1 . Invoice ( "123456", 300 ) );
  assert ( b1 . MedianInvoice () == 3000 );
  assert ( b1 . Invoice ( "123456", 200 ) );
  assert ( b1 . MedianInvoice () == 2000 );
  assert ( b1 . Invoice ( "123456", 230 ) );
  assert ( b1 . MedianInvoice () == 2000 );
  assert ( b1 . Invoice ( "123456", 830 ) );
  assert ( b1 . MedianInvoice () == 830 );
  assert ( b1 . Invoice ( "123456", 1830 ) );
  assert ( b1 . MedianInvoice () == 1830 );
  assert ( b1 . Invoice ( "123456", 2830 ) );
  assert ( b1 . MedianInvoice () == 1830 );
  assert ( b1 . Invoice ( "123456", 2830 ) );
  assert ( b1 . MedianInvoice () == 2000 );
  assert ( b1 . Invoice ( "123456", 3200 ) );
  assert ( b1 . MedianInvoice () == 2000 );

  CVATRegister b2;
  assert ( b2 . NewCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . NewCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . NewCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . NewCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . MedianInvoice () == 0 );
  assert ( b2 . Invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . MedianInvoice () == 1000 );
  assert ( b2 . Invoice ( "abcdef", 2000 ) );
  assert ( b2 . MedianInvoice () == 2000 );
  assert ( b2 . Invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . MedianInvoice () == 2000 );
  assert ( ! b2 . Invoice ( "1234567", 100 ) );
  assert ( ! b2 . Invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . Invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . Audit ( "1234567", sumIncome ) );
  assert ( ! b2 . Audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . Audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . CancelCompany ( "1234567" ) );
  assert ( ! b2 . CancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . CancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . CancelCompany ( "abcdef" ) );
  assert ( b2 . MedianInvoice () == 2000 );
  assert ( ! b2 . CancelCompany ( "abcdef" ) );
  assert ( b2 . NewCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . CancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . CancelCompany ( "ACME", "Kolejni" ) );

  return 0;
}

/*
Your task is to implement class CVATRegister, which implements a simple tax revenue database.

We assume the database holds information about all companies that are obligated to pay the VAT. To reduce tax evasion, a company is obligated to report all invoices in the database. Each company is identified by its tax ID. The IDs are strings and are unique in the database. Second, companies may be positively identified by their names and addresses. We assume the pair (name, address) is unique in the database. That is, there may exist several companies with the same name (e.g. "ACME"), there may be many companies registered with the same address (e.g. "Prague"), however, there may be at most one company "ACME" registered in "Prague". Moreover, tax IDs are compared case sensitive, whereas names and addresses are compared case insensitive.

The class shall implement public interface shown below. The interface consists of:

    A constructor (without parameters). This constructor initializes a new empty database.
    Destructor -- it shall free all resources used by the instance.
    Method NewCompany (name, addr, id ) which adds a new record to the database. The parameters are name, address and tax id of the company being registered. The method returns true if it succeeds, or false if it fails (the pair name, addr is already present in the database, or the id is not unique in the database).
    Method CancelCompany (name, addr) / CancelCompany (id) removes the corresponding record from the database, the record is identified either by the name and address, or by its tax id. The method returns true if it succeeds, or false if it fails (the corresponding record was not present).
    Methods Invoice (name, addr, amount) / Invoice ( id, amount ) add amount to the income of some company. There are two variants of the method: the company is either identified directly by tax id, or by company name and address. The methods return true if they succeed, or false if they fail (the corresponding record was not found).
    Method Audit ( name, addr, sum ) / Audit ( id, sum ) searches the database for the record identified by name and address, or by tax id. The method fills the output parameter (sum) with the total revenue of the particular company. The method returns true if it succeeds, or false if it fails (the corresponding record was not found). Moreover, if the method fails, the output parameters shall be left unmodified.
    Method MedianInvoice () searches for the median amount in the registered invoices. Only positively registered invoices are included in the computation, i.e. failed invocations of Invoice do not participate in the median. On the other hand, all so-far registered invoices participate in the median computation (including invoices from erased companies). If the number of invoices is even, the result will be the greater of the two middle values. If there are no invoices registered yet, the result is zero.

Submit a source file with your CVATRegister implementation. The class must follow the public interface below. If there is a mismatch in the interface, the compilation will fail. You may extend the interface and add you auxiliary methods and member variables (both public and private, although private are preferred). The submitted file must include both declarations as well as implementation of the class (the methods may be implemented inline but do not have to). The submitted file shall not contain anything except the class CVATRegister. In particular, if the file contains main, your tests, or any #include definitions, please, keep them in a conditional compile block. Use the attached source as a basis for your implementation. If the preprocessor definitions are preserved, the file maybe submitted to Progtest.

The class is tested in a limited environment -- both memory and running time is limited. The available memory is big enough to store the records. The class does not have to implement a copy constructor or an overloaded operator =. This functionality is not tested in this homework.

Your implementation must be both time and space efficient. A simple linear-time solution will not succeed (it takes more than 5 minutes for the test data). You may assume NewCompany and CancelCompany calls are rare compared to the other methods. Thus these two methods may run in linear time. Methods Invoice and Audit are called very often, thus they must be very efficient (e.g. logarithmic time or amortized constant time). Finally, MedianInvoice is not called often in the mandatory tests. Thus the implementation does not have to be very efficient (linear or n log n is fine to pass the mandatory tests), however, much better time complexity is required to pass the bonus test.

Either STL container or dynamic array allocation is required to implement the database. If implemented using the dynamically allocated array, set the initial size of the array to some small value (e.g. one hundred elements). When the array is full, do not increase the size by just one element. The overhead of the resizing would be enormous. Instead, increase the size by e.g. a thousand elements or use a quotient ranging from 1.5 to 2 (better solution).

If STL is used, your implementation does not have to care about the allocation. Caution: only some STL containers are available (see the headers included in the attached source). In particular, map / unordered_map / set / unordered_set / ... are disabled in this homework.

The attached source file lists some basic tests. The tests are quite short, you have to add your own tests. Please note that the tests included in the submitted files are recognized as an integral part of your solution. Thus someone else's tests included in your solution are just another form of cribbing.



Submission deadline: 	2017-04-02 23:59:59 	 
Evaluation: 	5.5000
Max. assessment: 	5.0000 (Without bonus points)
Submissions: 	9 / 20 Free retries + 20 Penalized retries (-2 % penalty each retry)
Advices: 	0 / 2 Advices for free + 2 Advices with a penalty (-10 % penalty each advice)
*/
