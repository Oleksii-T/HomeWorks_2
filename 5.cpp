//
//  Created by OleksiiTarbeiev on 02/05/2016.
//  Copyright © 2016 OleksiiTarbeiev. All rights reserved.
//
/* HomeWork 7 | semester 2 | Holidays I  */

#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST_ */

class CItem {
	public:
		virtual ~CItem (void) {}
		CItem (int _w) : w(_w) {}
		virtual bool Dang(void) const =0;
		virtual CItem* clone(void) const =0;
		friend ostream & operator<< (ostream & os,const CItem & x) {
			x.print(os);
			return os; }
		int Weight (void) {
			return w; }
		virtual void print(ostream & os) const {}
	protected:
		int w;
};


///////////////////////////////////////////////////////////////////////
class CKnife : public CItem { 
  public:
    CKnife ( int bladeLength ) : CItem::CItem(100),l(bladeLength) {}
 	  bool Dang(void) const override{
     	return (l>7); }
	  CItem* clone(void) const override {
	  	return new CKnife(l); }
  protected: 
    void print(ostream & os) const { 
    	os << "Knife, blade: " << l << " cm"; }
    int l;
};
///////////////////////////////////////////////////////////////////////
class CClothes : public CItem { 
  public:
   	CClothes ( string desc ) : CItem::CItem(500), d(desc) {}
   	bool Dang(void) const override{
     	return false; }
    CItem* clone(void) const override {
    	return new CClothes(d); }
  protected:   
   	void print (ostream & os) const { 
   		os << "Clothes (" << d << ")"; }
   	string d;
};
///////////////////////////////////////////////////////////////////////
class CShoes : public CItem { 
  public:
  	CShoes (void) : CItem::CItem(750) {}
   	bool Dang(void) const override {
     	return false; }
    CItem* clone(void) const override {
    	return new CShoes(); }
	protected:
    void print (ostream & os) const { 
    	os << "Shoes"; }
};
///////////////////////////////////////////////////////////////////////
class CMobile : public CItem { 
  public:
   	CMobile ( string _man, string _mod ) 
   		: CItem::CItem(150), man(_man), mod(_mod) {}
   	bool Dang(void) const override {
 			return (man=="Samsung" && mod=="Galaxy Note S7"); }
 		CItem* clone(void) const override {
 			return new CMobile(man, mod); }
  protected:
   	void print (ostream & os) const {
  		os << "Mobile " << mod << " by: " << man; }
   	string man;
   	string mod;
};



///////////////////////////////////////////////////////////////////////
class CThing {
 public:
//  virtual ~CThing (void) {
//		for (int i=0;i<(int)items.size();i++) 
//  		delete items[i]; }
  CThing & Add (const CItem & i) {
  	shared_ptr<CItem> ptr( i.clone() );
    items.push_back( ptr ); 
    return *this; }
  virtual int Weight (void) const { 
    int res=0;
    for (auto i:items)
    	res += i->Weight();
    return res; }
  bool Danger(void) const {
    for ( auto i:items )  //const shared_ptr<CItem> &
      if (i->Dang()) return true;
    return false; }
  int Count (void) const { 
  	return items.size(); }
  string Content (void) const;
  bool IdenticalContents (const CThing &) const;
 protected:
  vector<shared_ptr<CItem>> items;
};

string CThing::Content (void) const {
 	stringstream s;
 	int size = (int)items.size()-1;
	for (int i=0;i<(int)items.size();i++) {
		if (i == size) s << "\\-";
		else s << "+-";
		s << *items[i] << "\n";
	}	
 	return s.str();
}

bool CThing::IdenticalContents (const CThing & trg) const {
	if ( items.size() != trg.items.size() ) return false;
	bool flag;
	stringstream x;
	stringstream y;
	for (int i=0;i<(int)items.size();i++) {
		flag=false;
		x.str("");
		x << *items[i];
		for (int j=0;j<(int)trg.items.size();j++) {
			y.str("");
			y << *trg.items[j];
			if ( x.str() == y.str() ) {
				flag=true;
				break;
			}
		}
			if ( flag==false ) { 
				return false; }
	}
 	return true;
}

///////////////////////////////////////////////////////////////////////
class CSuitcase : public CThing {
  public:
   	CSuitcase ( int _w, int _h, int _d ) : w(_w), h(_h), d(_d) {}
   	string GetDims (void) const;
   	int Weight (void) const override {
   		return CThing::Weight()+2000; }
  	string PrintHelper (void) const { 
  		return CThing::Content(); }
   	friend ostream & operator<< (ostream & os, const CSuitcase & x) {
   		os << "Suitcase " << x.w << "x" << x.h << "x" << x.d << "\n";
   		os << x.PrintHelper(); 
   		return os; }   	
  protected:
   	int w;
   	int h;
   	int d;
};


///////////////////////////////////////////////////////////////////////
class CBackpack : public CThing {
  public:
   	CBackpack (void) {}
  	int Weight (void) const override {
  		return CThing::Weight()+1000; }
   	string PrintHelper (void) const { 
   		return CThing::Content(); }
   	friend ostream & operator<< (ostream & os, const CBackpack & x) {
			os << "Backpack\n";
   		os << x.PrintHelper();
   		return os; }
};


 #ifndef __PROGTEST__
int main ( void )
{
  CSuitcase x ( 1, 2, 3 );
  CBackpack y;
  ostringstream os;
  x.Add ( CKnife(7) ); //+100
  x.Add ( CClothes("red T-shirt") ); //+500
  x.Add ( CClothes ("black hat") );  //+500
  x.Add ( CShoes () ); //+750
  x.Add ( CClothes("green pants") );//+500
  x.Add ( CClothes("blue jeans") ); //+500
  x.Add ( CMobile("Samsung","J3") );//+150
  x.Add ( CMobile("Tamtung","Galaxy Note S7") );//+150
  os.str ( "" );
  os << x;
  assert ( os.str () == "Suitcase 1x2x3\n"
           "+-Knife, blade: 7 cm\n"
           "+-Clothes (red T-shirt)\n"
           "+-Clothes (black hat)\n"
           "+-Shoes\n"
           "+-Clothes (green pants)\n"
           "+-Clothes (blue jeans)\n"
           "+-Mobile J3 by: Samsung\n"
           "\\-Mobile Galaxy Note S7 by: Tamtung\n" ); 
  assert ( x.Count () == 8 );
  assert ( x.Weight () == 5150 );
  x.Add ( CKnife(8) );
  os.str ( "" );
  os << x;
  assert ( os.str () == "Suitcase 1x2x3\n"
           "+-Knife, blade: 7 cm\n"
           "+-Clothes (red T-shirt)\n"
           "+-Clothes (black hat)\n"
           "+-Shoes\n"
           "+-Clothes (green pants)\n"
           "+-Clothes (blue jeans)\n"
           "+-Mobile J3 by: Samsung\n"
           "+-Mobile Galaxy Note S7 by: Tamtung\n"
           "\\-Knife, blade: 8 cm\n" ); 
  assert ( x.Count () == 9 );
  assert ( x.Weight () == 5250 );
  assert ( x.Danger () );
  y.Add ( CKnife (7) )
   .Add ( CClothes("red T-shirt") )
   .Add ( CShoes () );
  y.Add ( CMobile("Samsung", "Galaxy Note S7") );
  y.Add ( CShoes () );
  y.Add ( CClothes ("blue jeans") );
  y.Add ( CClothes("black hat") );
  y.Add ( CClothes("green pants") );
  os.str ( "" );
  os << y;
  assert ( os . str () == "Backpack\n"
           "+-Knife, blade: 7 cm\n"
           "+-Clothes (red T-shirt)\n"
           "+-Shoes\n"
           "+-Mobile Galaxy Note S7 by: Samsung\n"
           "+-Shoes\n"
           "+-Clothes (blue jeans)\n"
           "+-Clothes (black hat)\n"
           "\\-Clothes (green pants)\n" );
  assert ( y . Count () == 8 );
  assert ( y . Weight () == 4750 );
  assert ( y . Danger () );
  y . Add ( CMobile ( "Samsung", "J3" ) );
  y . Add ( CMobile ( "Tamtung", "Galaxy Note S7" ) );
  y . Add ( CKnife ( 8 ) );
  os . str ( "" );
  os << y;
  assert ( os . str () == "Backpack\n"
           "+-Knife, blade: 7 cm\n"
           "+-Clothes (red T-shirt)\n"
           "+-Shoes\n"
           "+-Mobile Galaxy Note S7 by: Samsung\n"
           "+-Shoes\n"
           "+-Clothes (blue jeans)\n"
           "+-Clothes (black hat)\n"
           "+-Clothes (green pants)\n"
           "+-Mobile J3 by: Samsung\n"
           "+-Mobile Galaxy Note S7 by: Tamtung\n"
           "\\-Knife, blade: 8 cm\n" );
  assert ( y . Count () == 11 );
  assert ( y . Weight () == 5150 );
  assert ( y . Danger () );
  assert ( !x . IdenticalContents ( y ) );
  assert ( !y . IdenticalContents ( x ) );
  x . Add ( CMobile ( "Samsung", "Galaxy Note S7" ) );
  assert ( !x . IdenticalContents ( y ) );
  assert ( !y . IdenticalContents ( x ) );
  x . Add ( CShoes () );
  assert ( x . IdenticalContents ( y ) );
  assert ( y . IdenticalContents ( x ) );
  assert ( y . IdenticalContents ( y ) );
  assert ( x . IdenticalContents ( x ) );
  return 0;
}
#endif /* __PROGTEST__ */


/*
Having spent two semesters with Progtest programming and considering the upcoming holidays, one needs a change. It is
however crucial to properly pack all belongings before leaving for holidays. Therefore, the last Progtest homework is
focused on a program, which may be helpful when packing your suitcases.

We assume that we pack the following things into our luggage:

    CClothes - clothes, each 500g. There is a short textual description of the clothes.
    CShoes - shoes, 750g in weight.
    CKnife - a knife, 100g. We store the blade length with the knife, the length is in centimeters.
    CMobile - 150g in weight, both manufacturer and mode name is archived.

All things are packed into a luggage - either a CSuitcase or a CBackpack. The programmed luggage is a wonderful
thing - its capacity is infinite (anything may be stored in the luggage) while the weight of the empty suitcase is
only 2000g (suitcase) and 1000g (backpack). Moreover, the suitcase provides us with a very helpful interface which
simplifies certain common tasks:

    Constructor CSuitcase(w,h,d) initializes an empty suitcase with dimensions w x h x d. The dimensions must be stored
    and displayed (see sample runs). Backpack interface CBackpack is similar, except it does not take any parameters.
    Method Add adds any of the above things (clothes, mobile, knife, shoes) to the suitcase/backpack.
    Method Weight solves the total weight of the packed luggage (including all things inside). The total weight is the
    return value (an integer).
    Method Danger checks whether the packed suitcase/backpack contains a dangerous thing. If it does, the method returns
    true. We consider knives with blades exceeding 7cm dangerous, and of course, mobiles Samsung Galaxy Note S7 are
    dangerous too.
    Method Count counts all things in the luggage (the luggage itself is not included). The count is returned as an
    integer number.
    Method IdenticalContents tests whether the contents of two luggages is identical, or not (returns a bool value).
    The comparison only checks the contents, it does not care about the order of things in the luggage and it does
    not compare the luggage (suitcase/backpack) itself.
    Operator << displays the content of the suitcase/backpack in a stream. The format of the output is given below.
    Both CSuitcase and CBackpack class must correctly implement copying and deleting (releasing allocated resources). 

Your task is to design and implement the classes CKnife, CMobile, CClothes, CShoes, CSuitcase, and CBackpack. Pay
special attention to the design of the classes. The testing environment requires that inheritance and polymorphism
is used. Moreover, place the attributes (member variables) only in the classes which really need them. If you place
all attributes in the base class, the solution will not work.

Submit a source file, the file must contain declaration and implementation of the required classes. Do not place
#include / main function into the submitted file. If #include / main is left in the submitted file, it must be placed
in a conditional compile block (#ifndef __PROGTEST__ / #endif).
Notes:

    The output generated by the operator starts with the line Suitcase (with suitcase dimensions), or Backpack
    (without anything).
    The last line starts with \- and ends with a newline.
    The things in the output shall be listed in the order they were Add()ed.
    The output is compared with the reference output in a textual way. It is important to keep the formatting,
    including whitespaces.
    Do not use typeid operator. The use of typeid in this problem indicates deficient class design. The testing
    environment intentionally does not include typeinfo header file, thus, any use of typeid will lead to compile error.
    Get rid of long if / else if sequences when copying the suitcase. Use polymorphism to correctly copy the things
    in your suitcase.




Submission deadline: 	2017-05-07 23:59:59 	 
Evaluation: 	5.0000
Max. assessment: 	5.0000 (Without bonus points)
Submissions: 	10 / 20 Free retries + 20 Penalized retries (-2 % penalty each retry)
Advices: 	1 / 2 Advices for free + 2 Advices with a penalty (-10 % penalty each advice)
*/
