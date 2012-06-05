#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

#include <cmath>
#include <cstdlib>

using namespace std;
typedef pair<double,double> data;

//! Declarations
bool ConicidenceCheck(set<double> &history);
void BranchBuilder(set<double> history, set<double> searchSet, double currentNode);
void Coin(void);

void OutputSet(const string &title, const set<double> &output);

typedef pair<double,double> GammaPair;
typedef multimap<double, set<double> > CoinMap;

CoinMap coinMap;
set<double> gammas;
set<GammaPair> coinSet;
set<set<double> > cascades;
set<set<double> > histories;

//********** Main **********
int main(int argc, char* argv[]) {
   Coin();
}


//********** CoincidenceCheck **********
bool CoincidenceCheck(set<double> &history) {
   for(set<double>::iterator it = history.begin(); it != history.end(); it++) {
      for(set<double>::iterator it1 = history.begin(); 
	  it1 != history.end(); it1++) {
	 if(*it == *it1)
	    continue;

	 bool noCoin = coinSet.find(make_pair(*it,*it1)) == coinSet.end();
	 bool noInv = coinSet.find(make_pair(*it1,*it)) == coinSet.end();
	 if(noCoin || noInv)
	    return(false);
      }
   }
   return(true);
}


//********** BranchBuilder **********
void BranchBuilder(set<double> history, set<double> searchSet, double currentNode) 
{
   if(histories.find(history) != histories.end())
      return;
   if(searchSet.size() == 0) {
      cascades.insert(history);
      return;
   }
   if(searchSet.size() == 1 && CoincidenceCheck(history)) {
      cascades.insert(history);
      return;
   }
   
   set<double> newSearchSet;
   set<double> nodeCoins = coinMap.find(currentNode)->second;
   set_intersection(searchSet.begin(), searchSet.end(),
		    nodeCoins.begin(), nodeCoins.end(),
		    inserter(newSearchSet, newSearchSet.begin()) );
   cout << endl;
   for(set<double>::iterator it = nodeCoins.begin(); it != nodeCoins.end();
       it++) {
      history.insert(*it);
      if(CoincidenceCheck(history)) {
	 BranchBuilder(history, newSearchSet, *it);
	 histories.insert(history);
      }
   }
}


//********** Coin **********
void Coin(void) {
   double gamma, coinGamma;
   ifstream input("testCoins01.dat");
   while(!input.eof()) {
      input >> gamma >> coinGamma;
      gammas.insert(gamma);
      coinSet.insert(make_pair(gamma,coinGamma));
   }
   input.close();
   
   //! Build the coincidence map. (PART A)
   set<GammaPair> tempCoinSet = coinSet;
   set<double> tempSet;
   double tempGamma = coinSet.begin()->first;
   for(set<GammaPair>::iterator it = tempCoinSet.begin(); 
       it != tempCoinSet.end(); it++) {
      gamma = it->first;
      if(tempGamma != gamma || *it == *tempCoinSet.rbegin()) {
	 coinMap.insert(make_pair(tempGamma,tempSet));
	 tempSet.clear();
      }
      
      GammaPair inv = make_pair(it->second, it->first);
      set<GammaPair>::iterator itInvPos = tempCoinSet.find(inv);
      
      if(itInvPos != tempCoinSet.end()) {
	 tempSet.insert(it->second);
      }
      tempGamma = gamma;
   }

   cout << "Here's what we've got in the coincidence map (PART A)" << endl;
   for(CoinMap::iterator it = coinMap.begin(); it != coinMap.end(); it++) {
      cout << "Gamma " << it->first << " is true with ";
      for(set<double>::iterator it1 = it->second.begin(); 
	  it1 != it->second.end(); it1++) {
	 cout << *it1 << " ";
      }
      cout << endl;
   }
      
   cout << endl << "Begin working on building the branches...(PART B)" << endl;
   for(set<double>::iterator it = gammas.begin(); it != gammas.end(); it++) {
      set<double> parent;
      parent.insert(*it);
      BranchBuilder(parent, gammas, *it);
   }//for(set<double>::iterator it

   cout << endl << "Here are the cascades: " << endl;
   for(set<set<double> >::iterator it = cascades.begin(); it != cascades.end();
       it++) {
      OutputSet("Branch", *it);
   }
}//void Coin


//********* OutputSet **********
void OutputSet(const string &title, const set<double> &output) {
   cout << title << " : ";
   for(set<double>::iterator it = output.begin(); it != output.end(); it++)
      cout << *it << " ";
   cout << endl;
}
