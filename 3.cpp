//
//  Created by OleksiiTarbeiev on 19/04/2016.
//  Copyright © 2016 OleksiiTarbeiev. All rights reserved.
//
/* HomeWork 5 | semester 2 | VAT register II  */

#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;

class CDate																													 //
{																																		 //
 public:																														 //
  CDate ( int y, int m, int d ) : m_Year(y), m_Month(m), m_Day (d) {}//
  int Compare ( const CDate & x ) const {														 //
   if (m_Year != x.m_Year)																					 //
    return m_Year - x.m_Year;																				 //
   if (m_Month != x.m_Month)																				 //
    return m_Month - x.m_Month;																			 //
   return m_Day - x.m_Day;																					 //
  }																																	 //
  int Year (void) const { return m_Year; }													 //
  int Month (void) const { return m_Month; }												 //
  int Day (void) const { return m_Day; }														 //
  friend ostream & operator << (ostream & os, const CDate & x ) {		 //
   char oldFill = os.fill ();																			 	 //
   return os << setfill ('0') << setw (4) << x.m_Year << "-" 				 //
             << setw (2) << (int) x.m_Month << "-" 									 //
             << setw (2) << (int) x.m_Day << setfill (oldFill);			 //
  }																																	 //
private:																														 //
 int16_t m_Year;																										 //
 int8_t m_Month;																										 //
 int8_t m_Day;																											 //
};																																	 //
#endif /* __PROGTEST__ */


//global function which return simplified string
string rd(const string &input) {
 string res;
 unique_copy (input.begin(), input.end(), back_insert_iterator<string>(res), [](char a,char b){ return isspace(a) && isspace(b);});
 transform(res.begin(), res.end(), res.begin(), ::tolower);
 if (res[0]==' ')
  res.erase(res.begin());
 if (res[res.size()-1]==' ')
  res.pop_back();
 return res;
}


//////////////////////////////////////////////////////////////////////
class CInvoice
{
 public:
  CInvoice (const CDate &,const string &,const string &,unsigned int, double);
  CInvoice() {}
  ~CInvoice() {}
  CDate Date (void) const 
   { return *inv.Sdate.begin(); }
  string Buyer (void) const
   { return inv.Sbuyer; }
  string Seller (void) const
   { return inv.Sseller; }
  int Amount (void) const
   { return inv.Samount; }
  double VAT (void) const
   { return inv.SVAT; }
  void Change (string, string);
  friend bool operator< (const CInvoice &left, const CInvoice &right);
  bool operator== (const CInvoice &) const;
  void printIn (void) const;
 private:
  struct S_inv {
   list<CDate> Sdate;
   string Sseller;
   string Sbuyer;
   unsigned int Samount;
   double SVAT; 
  } inv;
};


CInvoice::CInvoice (const CDate & date, const string & seller, const string & buyer, unsigned int amount, double VAT) {

 inv.Sdate.push_back(date);
 inv.Sseller = seller;
 inv.Sbuyer = buyer;
 inv.Samount = amount;
 inv.SVAT = VAT;
}

bool operator< (const CInvoice &l, const CInvoice &r) {
 if ( l.Date().Compare(r.Date()) != 0) {
  if (l.Date().Compare(r.Date()) == -1) return false;
  else return true;
 }
 if ( l.Seller() != r.Seller() )
  return l.Seller() < r.Seller();
 if ( l.Buyer() != r.Buyer() )
  return l.Buyer() < r.Buyer();
 if ( l.Amount() != r.Amount() ) 
  return l.Amount() < r.Amount();
 return l.VAT() < r.VAT();
}

void CInvoice::Change (string s, string b) {
 inv.Sseller = s;
 inv.Sbuyer = b;
}
 
bool CInvoice::operator== (const CInvoice & rhs) const {
 if (Date().Compare(rhs.Date()) == 0 && Seller() == rhs.Seller() &&
     Buyer() == rhs.Buyer() && Amount() == rhs.Amount() &&
     VAT() == rhs.VAT() )
  return true;
 return false;
}

void CInvoice::printIn (void) const {
 cout << Date() << "|" << Seller() << "|" << Buyer() << "|" << Amount() << "|" << VAT() << endl;
}

 
 
 
//////////////////////////////////////////////////////////////////////
map <CInvoice, int> Order;
//////////////////////////////////////////////////////////////////////
class CSortOpt
{
 public:
  static const int BY_DATE = 0;
  static const int BY_BUYER = 1;
  static const int BY_SELLER = 2;
  static const int BY_AMOUNT = 3;
  static const int BY_VAT = 4;
  CSortOpt (void) {}
  CSortOpt & AddKey (int key, bool ascending = true );
  vector<pair<int, bool>> get(void) const;
 private:
  vector<pair<int,bool>> keys;
};
 
CSortOpt & CSortOpt::AddKey (int key, bool ascending) {
 vector<pair<int,bool>>::iterator it;
 for (it = keys.begin() ; it != keys.end(); it++)
  if (it->first == key)
   return *this;
 keys.push_back(make_pair(key, ascending));
 return *this;
}

vector<pair<int, bool>>  CSortOpt::get(void) const{
 vector<pair<int, bool>> trg=keys;
 return trg;
}
 
 
 
 
 
//////////////////////////////////////////////////////////////////////
class CComp {
 public:
  CComp (void) {}
  CComp (const vector<pair<int,bool>> & trg) { k=trg; }
  ~CComp () {}
  bool operator() (const CInvoice &,const CInvoice &);
 private: 
  vector <pair<int,bool>> k;
};

bool CComp::operator() (const CInvoice & f, const CInvoice & s) {
map <CInvoice, int>::iterator f_it, s_it;
 for (int i=0;i<(int)k.size();i++) 
  switch (k[i].first) {
   case 0:
     if (f.Date().Compare(s.Date()) == 0) continue; 
     if (k[i].second==true) {
       if( f.Date().Compare(s.Date()) == -1) return true;
       else if( f.Date().Compare(s.Date()) == 1) return false; }
     else if (k[i].second==false) {
       if( f.Date().Compare(s.Date()) == -1) return false;
       else if( f.Date().Compare(s.Date()) == 1) return true; }
     break;
   case 1:
     if (f.Buyer()==s.Buyer()) continue;
     else if (k[i].second==true) return f.Buyer()<s.Buyer();
     else if(k[i].second==false) return f.Buyer()>s.Buyer();
     break;
   case 2:
     if (f.Seller()==s.Seller()) continue;
     else if (k[i].second==true) return f.Seller()<s.Seller();
     else if (k[i].second==false) return f.Seller()>s.Seller();
     break;
   case 3:
     if (f.Amount()==s.Amount()) continue;
     else if (k[i].second==true) return f.Amount()<s.Amount();
     else if (k[i].second==false) return f.Amount()>s.Amount();
     break;
   case 4:
     if (f.VAT()==s.VAT()) continue;
     else if (k[i].second==true) return f.VAT()<s.VAT();
     else if (k[i].second==false) return f.VAT()>s.VAT();
     break;
  }
 f_it = Order.find(f); //possition of 'f' in Order map
 s_it = Order.find(s); //possition of 's' in Order map
 return f_it->second < s_it->second;//return difference of such indexes
}

 
 
 
//////////////////////////////////////////////////////////////////////
class CVATRegister
{
 public:
  CVATRegister (void) : count(0){}
  bool RegisterCompany (const string &);
  bool AddIssued (const CInvoice &); // seller
  bool AddAccepted (const CInvoice &); // buyer
  bool DelIssued (const CInvoice &);
  bool DelAccepted (const CInvoice &);
  list<CInvoice> Unmatched (const string &, const CSortOpt &) const;
  void print (void);
  void SortingProsses (list<CInvoice> &,
  										 const vector<pair<int,bool>>) const;
  list<CInvoice> FillWithOffNames(list<CInvoice> &) const;
 private:
  map<string, string> offname; //first-simplified name;second-official;
  map<string, set<CInvoice>> recI; //list of all payments with 
                                   //"Seller" as a key;
  map<string, set<CInvoice>> recA; //list of all payments with
  																 //"Buyer" as a key;
  set<CInvoice> uniqueIn; //holds only unique invoices
  int count;  // count for global Order map
};

bool CVATRegister::RegisterCompany (const string & name ) { 
 string NewName=(rd(name));
 if (!recI.empty())
  if (recI.find(NewName) != recI.end())
   return false; // in "I" list already exist such Comp;
 if (!recA.empty()) 
  if (recA.find(NewName) != recA.end()) 
   return false; // in "A" list already exist such Comp;
 recI[NewName];
 recA[NewName];
 offname[NewName] = name;
 return true;
}

bool CVATRegister::AddIssued (const CInvoice & x ) {   // seller
 //Make tmp invoice with simplified seller and buyer to store
 CInvoice tmp = CInvoice( CDate(x.Date().Year(), x.Date().Month(), x.Date().Day()), rd(x.Seller()), rd(x.Buyer()), x.Amount(), x.VAT() );
 set<CInvoice>::iterator j;
 map<string,set<CInvoice>>::iterator i = recI.find(tmp.Seller()), ii = recI.find(tmp.Buyer());

 if (tmp.Seller() == tmp.Buyer()) return false;//same comp;
 if (i == recI.end() ) return false; //comp-S was not registered(recA doesnt checked);
 if (ii == recI.end() ) return false; //comp-B was not registered(recA doesnt checked); 
 if (i->second.count(tmp) == 1) return false; //invoice already added;
 i->second.insert(tmp);
 //filling Order of Invoices map
 if (Order.find(tmp) == Order.end())
  Order[tmp] = count;
 count++;
 //filing unique set of invoices
 if (uniqueIn.empty() || uniqueIn.count(tmp) == 0) {
  uniqueIn.insert(tmp);
  return true;
  }
 if (uniqueIn.count(tmp) == 1) { 
  j = uniqueIn.find(tmp);
  uniqueIn.erase(j);
 }
 return true;
}

bool CVATRegister::AddAccepted (const CInvoice & x ) { // buyer
 //Make tmp invoice with simplified seller and buyer to store 
 CInvoice tmp = CInvoice( CDate(x.Date().Year(), x.Date().Month(), x.Date().Day()), rd(x.Seller()), rd(x.Buyer()), x.Amount(), x.VAT() );
 set<CInvoice>::iterator j;
 map<string,set<CInvoice>>::iterator i = recA.find(tmp.Buyer()), ii = recA.find(tmp.Seller()) ;

 if (tmp.Seller() == tmp.Buyer()) return false;//same comp;
 if (i == recA.end() ) return false; //comp-B was not registered(recI doesnt checked);
 if (ii == recA.end() ) return false; //comp-S was not registered(recI doesnt checked);
 if (i->second.count(tmp) == 1) return false; //invoice already added;
 i->second.insert(tmp);
 //filling Order of Invoices
 if (Order.find(tmp) == Order.end())
  Order[tmp] = count;
 count++;
 //filing unique set of invoices 
 if (uniqueIn.empty() || uniqueIn.count(tmp) == 0) {
  uniqueIn.insert(tmp);
  return true;
 }
 if (uniqueIn.count(tmp) == 1) { 
  j = uniqueIn.find(tmp);
  uniqueIn.erase(j);
 } 
 return true;
}

bool CVATRegister::DelIssued (const CInvoice & x ) {   // seller
 //Make tmp invoice with simplified seller and buyer to store
  CInvoice tmp = CInvoice( CDate(x.Date().Year(), x.Date().Month(), x.Date().Day()), rd(x.Seller()), rd(x.Buyer()), x.Amount(), x.VAT() );
 set<CInvoice>::iterator j;
 map<string,set<CInvoice>>::iterator i = recI.find(tmp.Seller());

 if (i == recI.end()) return false; // no such comp was not registered;
 if (i->second.count(tmp) == 0) return false;//invoice wasnt being added;
 i->second.erase(tmp);
 //filing unique set of invoices 
 if (uniqueIn.empty() || uniqueIn.count(tmp) == 0) {
  uniqueIn.insert(tmp);
  return true;
 }
 if (uniqueIn.count(tmp) == 1) { 
  j = uniqueIn.find(tmp);
  uniqueIn.erase(j);
 }
 return true;
}

bool CVATRegister::DelAccepted (const CInvoice & x ) { // buyer
 //Make tmp invoice with simplified seller and buyer to store
 CInvoice tmp = CInvoice( CDate(x.Date().Year(), x.Date().Month(), x.Date().Day()), rd(x.Seller()), rd(x.Buyer()), x.Amount(), x.VAT() );
 set<CInvoice>::iterator j;
 map<string,set<CInvoice>>::iterator i = recA.find(tmp.Buyer());

 if (i == recA.end()) return false;// no such comp was not registered;
 if (i->second.count(tmp)==0) return false;//invoice wasnt being added;
 i->second.erase(tmp);
 //filing unique set of invoices
 if (uniqueIn.empty() || uniqueIn.count(tmp) == 0) {
  uniqueIn.insert(tmp);
  return true;
 }
 if (uniqueIn.count(tmp) == 1) { 
  j = uniqueIn.find(tmp);
  uniqueIn.erase(j);
 }
 return true;
}

list<CInvoice> CVATRegister::Unmatched(const string & company,
                                       const CSortOpt & sortBy) const {
 list<CInvoice> l;
 set <CInvoice>::iterator i;
 string New=rd(company);
 
 for (i=uniqueIn.begin();i!=uniqueIn.end();i++) 
  if (i->Seller() == New || i->Buyer() == New)
   l.push_back(*i); //choosing invoices in which we are interest from 
                    //unique set, which was formed previously
 l.sort( CComp(sortBy.get()) );
 return FillWithOffNames(l);
}

list <CInvoice> CVATRegister::FillWithOffNames(list<CInvoice> & l) const{
 list <CInvoice> tmp;

 for (auto i:l) {
  auto is=offname.find(i.Seller());  
  auto ib=offname.find(i.Buyer());
  i.Change(is->second, ib->second);
  tmp.push_back(i);
 }
 return tmp;
}

void CVATRegister::print (void) { //just printing of two main maps
 cout << " $List of Issed:" << endl;
 for (auto i:recI) {
  cout << "   *" << i.first << ": " << endl;;
  for (auto j:i.second) {
   cout << "    -";
   j.printIn();
  }
 }
 cout << " $List of Accepted:" << endl;
 for (auto i:recA) {
  cout << "   *" << i.first << ": " << endl;;
  for (auto j:i.second) {
   cout << "    -";
   j.printIn();
  }
 }
 cout << endl;
}




#ifndef __PROGTEST__

bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{
 auto j=b.begin();
 int count=0;

 if (a.size() != b.size()) {
  cout << " BAD SIZE " << a.size() << "!=" << b.size() << endl;
  return false;
 }
 for (auto i:a)
   if (i == *j) { 
    j++;
    count++;
    continue;
   }
   else {
    cout << "CAUSE on position " << count << ":\n      is:"; 
    i.printIn();
    cout << "  should:";
    j->printIn();
    return false;
   }
 return true;
}

int main ( void )
{
  CVATRegister r;
  assert ( r.RegisterCompany ( "first Company" ) );
  assert ( r.RegisterCompany ( "Second     Company" ) );
  assert ( r.RegisterCompany ( "ThirdCompany, Ltd." ) );
  assert ( r.RegisterCompany ( "Third Company, Ltd." ) );
  assert ( !r.RegisterCompany ( "Third Company, Ltd." ) );
  assert ( !r.RegisterCompany ( " Third  Company,  Ltd.  " ) );
  
  assert ( r.AddIssued (CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 20)) );
  assert ( r.AddIssued (CInvoice(CDate(2000, 1, 2), "FirSt Company", "Second Company ", 200, 30)) );
  assert ( r.AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
  assert ( r.AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( r.AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r.AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
  assert ( r.AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
  
  assert ( !r.AddIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) ); 
  assert ( !r.AddIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
  assert ( !r.AddIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );

  assert ( equalLists (r.Unmatched ("First Company", CSortOpt ().AddKey (CSortOpt::BY_SELLER, true).AddKey (CSortOpt::BY_BUYER, false ).AddKey (CSortOpt::BY_DATE, false) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_DATE, true ) . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
           } ) );
  assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_VAT, true ) . AddKey ( CSortOpt::BY_AMOUNT, true ) . AddKey ( CSortOpt::BY_DATE, true ) . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
/////////////////////////////////////////////////////////

assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () ),
           list<CInvoice>
           {
CInvoice (CDate(2000,1,1),"first Company","Second     Company",100,20.000000),
CInvoice(CDate(2000,1,2),"first Company","Second     Company",200,30.000000),
CInvoice(CDate(2000,1,1),"first Company","Second     Company",100,30.000000),
CInvoice(CDate(2000,1,1),"first Company","Second     Company",300,30.000000),
CInvoice(CDate(2000,1,1),"first Company","Third Company, Ltd.",200,30.000000),
CInvoice(CDate(2000,1,1),"Second     Company","first Company",300,30.000000)
           } ) );
/////////////////////////////////////////////////////////
  assert ( equalLists ( r . Unmatched ( "second company", CSortOpt () . AddKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
           } ) );
  assert ( equalLists ( r . Unmatched ( "last company", CSortOpt () . AddKey ( CSortOpt::BY_VAT, true ) ),
           list<CInvoice>
           {
           } ) );   
       
  assert ( r . AddAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( r . AddAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . AddAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
  
  
  assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) . AddKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),//
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),//
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),//
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),//
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )//
           } ) );       

  assert ( !r . DelIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
  assert ( !r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
  assert ( !r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
  assert ( r . DelIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );

  assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) . AddKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
           
  assert ( r.DelAccepted(CInvoice(CDate(2000,1,1),"First Company"," Third  Company,  Ltd.   ",200,30)) );
  assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) . AddKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . DelIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . Unmatched ( "First Company", CSortOpt () . AddKey ( CSortOpt::BY_SELLER, true ) . AddKey ( CSortOpt::BY_BUYER, true ) . AddKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) ); 

  return 0;
}



#endif /* __PROGTEST__ */



/*
The task is to develop classes to implement VAT (value added tax) register.

Assume companies that issue / accept invoices from other companies. A company that issues an invoice is obligated to pay VAT (based on the amount). Similarly, a company that accepts (pays) an invoice may (under some circumstances) apply for the VAT refund. The refunds are the bottlenecks of the system, in general, refunds may be abused to commit tax evasions. The implemented register shall help to avoid the evasions.

Both issued and accepted invoices are registered in the CVATRegister class. Companies are obligated to register their invoices, both issued and accepted. The register then matches the corresponding pairs, thus is able to find the unmatched invoices. The invoices may be both registered in the system, or deleted from the system (e.g. if an erroneous invoice is registered). The interface of the register is:

default constructor
    initializes an empty register,
RegisterCompany ( name )
    the method registers a new company to the system. The name passed to this call is the "official" name of the company, and will be used in the exports from the register. Return value is a success (true) / fail (false) indicator. The call fails if the same company is already registered. The register is, indeed, a bit tolerant in the company names:

        the comparison does not distinguish lower/upper case,
        abundant spaces are ignored.

    These rules are used when registering the company, as well as in the adding/removing of invoices. For example names "My Company", "mY CoMpAnY", " My Company ", and " mY CoMPAnY " are considered identical (one company), however, names "My Company" and "MyCompany" denote two different companies.
AddIssued ( invoice )
    the method adds an invoice to the register. The method is called by the company that actually issues the invoice. Return value is a success (true) / fail (false) indicator. An error is reported a company is not registered (either seller or buyer), the seller and the buyer is the same company, or an identical invoice has already been registered via AddIssued (we assume two identical invoices cannot exists, there must be different at least one of date/buyer/seller/amount/VAT).
AddAccepted ( invoice )
    the method adds an invoice to the register, this method is called by the company that accepted the invoice (buyer). The behavior is the same as AddIssued.
DelIssued ( invoice )
    the method removes an invoice from the register. The method is called by a selling company that previously registered the invoice via AddIssued. Return value is a success (true) / fail (false) indicator. Error is returned if the invoice has not been previously registered with AddIssued.
DelAccepted ( invoice )
    the method is analogous to DelIssued except for the buyer company.
Unmatched ( company, sortOpt )
    the method finds all invoices where company company is either the seller or the buyer, and where the invoices had not been matched (i.e. had been registered either by AddIssued, or by AddAccepted, but not both). The method returns a list of such invoices, the list will be sorted by the sort keys listed in sortOpt (see below). The invoices returned in the list will use the "official" company names, as they were registered via RegisterCompany. The official names will also be used for the sorting.

Class CInvoice represent a single invoice. The interface requirements are:

constructor ( date, seller, buyer, amount, vat )
    initializes the object using the given date, seller and buyer companies, the amount, and the VAT.
Date, Seller, Buyer, Amount, VAT
    access methods are used to read the individual fields.
more
    you may add further public/private methods and fields that are needed for the implementation.

Class CSortOpt defines the sort criteria. In general, all invoice fields may be used to sort the lists. For example, an instance:

CSortOpt () . AddKey ( CSortOpt::BY_AMOUNT, true ) . AddKey ( CSortOpt::BY_SELLER, false )

means to sort the list in an ascending order using the amount (the first sort key). If there are two invoices with the same amount, the the second sort key (seller company name) will be used, the sort order is descending in this case. If the second key is not enough to establish the sort order, the order of invoice registering will be used as the last sort key. The interface of CSortOpt is:

default constructor
    initializes the empty object,
AddKey ( sortBy, ascending )
    adds next sort key sortBy, sort order is defined by the second parameter ascending (true = ascending, false = descending). Sort keys are:

        BY_DATE - by invoice date,
        BY_SELLER - by the name of the seller, the official name is used, sorting is case insensitive,
        BY_BUYER - by the name of the buyer, the official name is used, sorting is case insensitive,
        BY_AMOUNT - by the amount billed,
        BY_VAT - by VAT.

Class CDate implements a simple date wrapper, the implementation is included in the testing environment, there is a copy of the implementation included in the attached source. The implementation cannot be changed, moreover, the implementation must be kept in the conditional compile block.

Submit a source of the classes CVATRegister, CSortOpt, and CInvoice. Use the attached file as a basis of your implementation. Moreover, the attached file contains some basic tests. You are free to add further classes to simplify your implementation.

The problem is a bit similar to the previous homework. We assume you use adequate STL containers to implement this program. Moreover, we expect both time and memory efficient implementation. All Add / Del methods must be faster than linear, sorting must be n log n. There is a significant malus if the solution does not pass the optional speed test. Be cautious when using C++11 unordered_set / unordered_map. Do not implement hash function as a specialization of std::hash. Use your own function/functor instead, use the type explicitly in declarations. (The specialization of std::hash assumes that namespace std is re-opened. This is difficult to accomplish when your code is already enclosed in a namespace. The solutions on the Internet (e.g. stack overflow, cpp reference) implicitly assume you do not use namespaces. Thus the solutions are not portable, in particular, they are not compatible with namespaces and this are not compatible with Progtest.)





Submission deadline: 	2017-04-23 23:59:59 	 
Evaluation: 	0.2647
Max. assessment: 	5.0000 (Without bonus points)
Submissions: 	9 / 20 Free retries + 20 Penalized retries (-2 % penalty each retry)
Advices: 	1 / 2 Advices for free + 2 Advices with a penalty (-10 % penalty each advice)
*/
