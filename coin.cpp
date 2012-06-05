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
	 bool hasCoin = coinSet.find(make_pair(*it,*it1)) != coinSet.end();
	 bool hasInv = coinSet.find(make_pair(*it1,*it)) != coinSet.end();
	 if(!hasCoin || !hasInv)
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
   set<double> newSearchSet;
   set<double> nodeCoins = coinMap.find(currentNode)->second;
   set_intersection(searchSet.begin(), searchSet.end(),
		    nodeCoins.begin(), nodeCoins.end(),
		    inserter(newSearchSet, newSearchSet.begin()) );
   for(set<double>::iterator it = newSearchSet.begin(); it != newSearchSet.end();
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
   ifstream input("testCoins00.dat");
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
	 //tempCoinSet.erase(inv);
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
}//void Coin
