//
//  Created by OleksiiTarbeiev on 25/04/2016.
//  Copyright © 2016 OleksiiTarbeiev. All rights reserved.
//
/* HomeWork 6 | semester 2 | Scoreboard  */

#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
using namespace std;
#endif /* __PROGTEST__ */

class CGraph {
  int V;
  vector <vector<int>> adj;
  vector<int> indegree;
  vector <int> Result;
public:
  CGraph(int v) : V(v), adj(v), indegree(v, 0) {}
  void addEdge(int v, int w);
  bool MainCheck();
  int TSHelper(vector<int> &, vector<bool> &, int &);
  bool ICHelper(int v, vector<bool> &, vector<bool> &);
  vector<int> GetResult (void) { return Result; }
  void printG (void);
};

void CGraph::addEdge(int v, int w) {
  adj[v].push_back(w);
  indegree[w]++;
}


int CGraph::TSHelper(vector <int>  & res,
                     vector <bool> & visited, int & tmp) {
	bool flag = false;
  for (int i = 0; i < V; i++) {
  	if (indegree[i] == 0 && !visited[i]) {
   		vector<int>::iterator j;
			for (j = adj[i].begin(); j != adj[i].end(); j++)
	     indegree[*j]--;
 
 		  res.push_back(i);
 		  visited[i] = true;
			if ( !TSHelper(res, visited, tmp) )
       return false;
      
   		visited[i] = false;
   		res.erase(res.end() - 1);
   		for (j = adj[i].begin(); j != adj[i].end(); j++)
    		indegree[*j]++; 
   		flag = true;
    }
  }
  if (!flag) {
    if (tmp != 0) 
     return false;
  	for (int i = 0; i < (int)res.size(); i++)
 	 		Result.push_back(res[i]);
 	 	tmp++; //icrement tmp to determine more then 1 path to loser
  }
  return true;
}

bool CGraph::ICHelper(int v, vector <bool> & visited,
                             vector <bool> & recStack) {
	if(visited[v] == false) {
    visited[v] = true;
    recStack[v] = true;
    vector<int>::iterator i;
    for(i = adj[v].begin(); i != adj[v].end(); ++i) {
    	if ( !visited[*i] && ICHelper(*i, visited, recStack) )
      	return true;
      else if (recStack[*i])
      	return true;
    } 
  }
  recStack[v] = false;
  return false;
}

bool CGraph::MainCheck() {
  vector<bool> visitedC(V);
  vector<bool> visitedS(V);
  vector<bool> recStack(V);
  for (int i = 0; i < V; i++) {
  	visitedC[i] = false;//for checking for cyclicity 
  	visitedS[i] = false;//for checking for few ways in sorst
  	recStack[i] = false;
  }
  for(int i = 0; i < V; i++)
  	if (ICHelper(i, visitedC, recStack)) //true == is cyclic
      return false;
  vector<int> res;
  int tmp = 0;
  if ( !TSHelper(res, visitedS, tmp) )//check for qniue path fromwinner
   return false;                      //to loser;
  return true;
}

void CGraph::printG (void) {
 cout << "GRAPH:" << endl;
 for (int i=0; i<V ;i++) {
  cout << i << ": ";
  for (auto j:adj[i])
   cout << j << " ";
  cout << endl;
 }
} 




////////////////////////////////////////////////////////////////////
class OrderingDoesNotExistException {
 public:
  OrderingDoesNotExistException (const string & _a) : a(_a) {}
 private:  
  string a;
};






////////////////////////////////////////////////////////////////////
class DuplicateMatchException {
 public:
  DuplicateMatchException(const string & _a) : a(_a) {}
 private: 
  string a;
};





////////////////////////////////////////////////////////////////////

template <typename _M>
class CRecord {
 public:
  CRecord (string s1, string s2, _M m) : p1(s1), p2(s2), res(m) {}
  template <typename _X>
  friend bool operator< (const CRecord<_X> &, const CRecord<_X> &);
  string P1 (void) const { return p1;}
  string P2 (void) const { return p2;}
  _M Res (void) const { return res;}
 private:
  string p1;
  string p2;
  _M res;
};

template <typename _M>
bool operator< (const CRecord<_M> & l, const CRecord<_M> & r) {
 if (l.P1() != r.P1())
  return l.P1() < r.P1();
 return l.P2() < r.P2();
} 






////////////////////////////////////////////////////////////////////
template <typename _M>
class CContest
{ 
 public:
  CContest (void) : count(0) {}
  ~CContest (void) {}
  CContest<_M> & AddMatch (const string &, const string &, const _M &);
  template <typename _C>
   bool IsOrdered ( const _C & ) const;
  template <typename _C>
   list<string> Results ( const _C & ) const;
 private:
  set <CRecord<_M>> s;      //just store match as it is;
  map<string, int> nameM;   // string and its int;
  vector<string> nameV;     //index -> string;
  int count;                //counter for map;
};

template <typename _M>
CContest<_M> & CContest<_M>::AddMatch(const string & p1,
                                      const string & p2, 
                                      const _M & res) {
 if (s.count(CRecord<_M>(p1, p2, res)) == 1 ||
     s.count(CRecord<_M>(p2, p1, res)) == 1 ) 
  throw DuplicateMatchException("DUBLICATES MATCH");

 s.insert(CRecord<_M>(p1, p2, res));
 
 if (nameM.find(p1) == nameM.end()) {
  nameM[p1] = count;
  nameV.push_back(p1); 
  count++;
 }
 if (nameM.find(p2) == nameM.end()) {
  nameM[p2] = count;
  nameV.push_back(p2); 
  count++;
 }
 return *this;
}

template <typename _M>
template <typename _C>
bool CContest<_M>::IsOrdered ( const _C & comp) const {
 CGraph g(nameV.size());
 map<string, int>::const_iterator mi1, mi2;
 for (auto i:s) {
  if ( comp(i.Res()) == 0 )//drow
   return false;
  mi1 = nameM.find(i.P1());
  mi2 = nameM.find(i.P2());
  if ( comp(i.Res()) > 0 ) //p1 -- winner
   g.addEdge (mi1->second, mi2->second);
  else if ( comp(i.Res()) < 0 ) //p2 -- winner
   g.addEdge (mi2->second, mi1->second);
 }
// g.printG();
 if (!g.MainCheck())
	return false;
 return true;
}

template <typename _M>
template <typename _C>
list<string> CContest<_M>::Results ( const _C & comp) const {
 list<string> l;
 CGraph g(nameV.size());
 map<string, int>::const_iterator mi1, mi2;
 
  
 for (auto i:s) {
  if ( comp(i.Res()) == 0 )//drow
   throw OrderingDoesNotExistException("FAIL ORDERIN");
  mi1 = nameM.find(i.P1());
  mi2 = nameM.find(i.P2());
  if ( comp(i.Res()) > 0 ) //p1 -- winner
   g.addEdge (mi1->second, mi2->second);
  else if ( comp(i.Res()) < 0 ) //p2 -- winner
   g.addEdge (mi2->second, mi1->second);
 }
 if (!g.MainCheck())
  throw OrderingDoesNotExistException("FAIL ORDERIN");
 for (auto i:g.GetResult())
  l.push_back(nameV[i]); 
 return l;
}


#ifndef __PROGTEST__
struct CMatch {
 public:
  CMatch (int a, int b) : m_A ( a ), m_B ( b ) {}
  int m_A;
  int m_B;
};

class HigherScoreThreshold
{
 public:
 HigherScoreThreshold (int diffAtLeast) : m_DiffAtLeast (diffAtLeast){}
 int operator () (const CMatch & x) const {
  return (x.m_A > x.m_B + m_DiffAtLeast) -
         (x.m_B > x.m_A + m_DiffAtLeast);
 }
 private:
  int m_DiffAtLeast;    
};

int HigherScore (const CMatch & x) {
 return (x.m_A > x.m_B) - (x.m_B > x.m_A);
}

int main (void)
{
 CContest<CMatch>  x;
  
 x.AddMatch ("C++","Pascal", CMatch(10,3) )
  .AddMatch ("C++","Java", CMatch(8,1) )
  .AddMatch ("Pascal","Basic", CMatch(40,0) )
  .AddMatch ("Java","PHP",CMatch(6,2) )
  .AddMatch ("Java","Pascal",CMatch(7,3) )
  .AddMatch ("PHP","Basic",CMatch(10,0) );

  assert ( !x.IsOrdered (HigherScore) );
  try {
   list<string> res = x.Results (HigherScore);
   assert ("Exception missing!" == NULL);
  }
  catch (const OrderingDoesNotExistException & e) {}
  catch ( ... ) {
   assert ("Invalid exception thrown!" == NULL);
  }

  x.AddMatch("PHP","Pascal",CMatch(3,6)); 

//x.printNames(); x.printRecord();/////////
  assert(x.IsOrdered(HigherScore));
  try {
   list<string> res = x.Results ( HigherScore );
   assert ((res==list<string>{ "C++","Java","Pascal","PHP","Basic" }));
  }
  catch ( ... ) {
   assert ("Unexpected exception!" == NULL);
  }

  assert ( !x.IsOrdered (HigherScoreThreshold(3)) );
  try {
   list<string> res = x.Results (HigherScoreThreshold(3));
   assert ( "Exception missing!" == NULL );
  }
  catch (const OrderingDoesNotExistException & e) {}
  catch ( ... ) {
   assert ("Invalid exception thrown!" == NULL);
  }
  
  assert (x.IsOrdered([](const CMatch & x) {
   return (x.m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try {
   list<string> res = x.Results ( [] ( const CMatch & x ) {
    return (x.m_A < x.m_B) - (x.m_B < x.m_A); 
   } );
   assert ((res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" }));
  }
  catch ( ... ) {
   assert ("Unexpected exception!" == NULL);
  }
  
  CContest<bool>  y;
  
  y.AddMatch ("Python","PHP",true)
   .AddMatch ("PHP", "Perl", true)
   .AddMatch ("Perl", "Bash", true)
   .AddMatch ("Bash", "JavaScript", true)
   .AddMatch ("JavaScript", "VBScript", true);
  
  assert (y.IsOrdered ([](bool v) {
   return v ? 10 : -10;
  } ) );
  try {
   list<string> res = y . Results ( [] ( bool v ) {
    return v ? 10 : - 10;
   });
   assert ((res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" }));
  }
  catch ( ... ) {
   assert ("Unexpected exception!" == NULL);
  }
    
  y.AddMatch ("PHP", "JavaScript", false);
  assert ( !y . IsOrdered ( [] ( bool v ) {
   return v ? 10 : -10;
  } ) );
  try {
   list<string> res = y.Results([](bool v) {
    return v ? 10 : - 10;
   } );
   assert ( "Exception missing!" == NULL );
  }
  catch ( const OrderingDoesNotExistException & e ) {}
  catch ( ... ) {
   assert ( "Invalid exception thrown!" == NULL );
  }
  
  try {
   y.AddMatch ( "PHP", "JavaScript", false );
   assert ( "Exception missing!" == NULL );
  }
  catch ( const DuplicateMatchException & e ) {}
  catch ( ... ) {
   assert ( "Invalid exception thrown!" == NULL );
  }
  try {
   y.AddMatch ( "JavaScript", "PHP", true );
   assert ( "Exception missing!" == NULL );
  }
  catch ( const DuplicateMatchException & e ) {}
  catch ( ... ) {
   assert ( "Invalid exception thrown!" == NULL );

  }
  return 0;
}
#endif /* __PROGTEST__ */

/*
The task is to develop a generic class CContest that computes the overall scoreboard based on the results of individual matches.

Assume a group of contestants. The contestants compete in matches, there are two contestants in each match, the result of the match is recorded (a simple boolean flag, points, times, ...). Our class needs to be flexible, thus the result is represented using some generic data type. The matches (i.e. the names of contestants and the result) are stored in our CContest instance. Subsequently, the scoreboard needs to be computed based on the individual matches. The trick is that our scoreboard must be computed just based on the results of the known matches, moreover, we want to know whether the known matches are enough to positively compute the scoreboard.

A scoreboard is unambiguous if each contestant Xi on position i holds:

    Xi defeated all contestants Xj, i+1 ≤ j ≤ n. The contestants were defeated either directly in a match, or indirectly (Xi defeats Xa, Xa defeats Xb, ..., Xz defeats Xj, i+1 ≤ a < b < ... < j ≤ n),
    Xineither won nor drawn any match with any contestant Xk, 0 ≤ k ≤ i-1.

For example let A defeats B and B defeats C. Then the scoreboard is unambiguous A B C. On the other hand, if:

A defeats B
B defeats C
C defeats A

or:

A defeats B
A defeats C

or:

A defeats B
B draws with C

then the scoreboard cannot be established unambiguously (we would have to add further matches, or we would have to establish the scoreboard on other criteria; we are not going to do so in this homework).

It was noted that a match is described as a triplet (contestant1, contestant2, result) where result is some data type to describe the winner/score/times/... The generic result needs to be transformed into one of win/draw/lose. That conversion is accomplished by a comparator passed to the scoreboard computation method. The comparator is one of function/functor/lambda-expression that given an instance of result returns an integer:

    negative, if contestant1 lost,
    zero for draw,
    positive if contestant1 won.

The interface of class template CContest shall be:

the template is parametrized by a generic parameter _M
    this parameter describes the type used to describe the result of a match. Data type _M guarantees moving, copying, and destroying (CopyConstructibe, MoveConstructible, CopyAssignable, MoveAssignable and Destructible). No further interface is guaranteed, in particular, there is not default constructor guaranteed for _M (i.e. does not have to be DefaultConstructible).
default constructor
    initializes an empty instance of CContest,
AddMatch(contestant1, contestant2, result)
    the method adds a new match to the storage in the instance. The contestants (contestant1 and contestant2) are strings, the result is of type _M. Calls to AddMatch may be chained. The method must throw exception DuplicateMatchException if the same match has been already registered.
IsOrdered (comparator)
    the method decides whether the final scoreboard may be unambiguously established based on the so-far registered matches. The result is true if the scoreboard is unambiguous, false otherwise. The parameter is comparator - a function, functor, or a lambda-expression to call when a match result need to be converted into the win/draw/lost information (i.e. transforms _M into positive/zero/negative integer, see above). The method must not modify CContest instance and in must not throw any exceptions. 
Results (comparator)
    the method computes the scoreboard. Return value is STL list (std::list) of strings, each string represents the name of a contestant. The list is ordered from the absolute winner (first string in the list) to the last contestant. The method must throw OrderingDoesNotExistException if the list cannot be established unambiguously. Parameter comparator is similar to the IsOrdered method.

Submit a source file with the implementation of the CContest class template. Use the attached source code as a basis for your implementation. Please preserve the conditional compile directives - if present, the source may be locally tested and submitted to Progtest without manual modifications.

The assessment is divided into mandatory and bonus tests. The mandatory tests add only a few contestant/matches, thus naive algorithm working in time #contestants x #matches passes the time limits. An advanced algorithm is needed to pass the bonus test where the number of contestants/matches is high.

A naive approach may need to find all contestants directly or indirectly defeated by a particular contestant X. Use BFS (breadth first search) algorithm for this search. Further inspiration: PA1, proseminar #11, labyrinth problem.

The example source code may be found in the attached archive.



Submission deadline: 	2017-04-30 23:59:59 	 
Evaluation: 	6.5000
Max. assessment: 	5.0000 (Without bonus points)
Submissions: 	7 / 20 Free retries + 20 Penalized retries (-2 % penalty each retry)
Advices: 	0 / 2 Advices for free + 2 Advices with a penalty (-10 % penalty each advice)
*/
