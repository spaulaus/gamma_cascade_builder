#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

#include <cmath>
#include <cstdlib>

using namespace std;

//! Declarations
bool ConicidenceCheck(set<double> &history);
void BranchBuilder(set<double> history, set<double> searchSet, double currentNode);
void Coin(void);
void OutputSet(const string &title, const set<double> &output, const int &number);

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
    // cout << endl << "BEGINNING BRANCH BUILDER: " << endl
    // 	 << "Current Node: " << currentNode << endl;
    // OutputSet("Search Set", searchSet, 0);
    // OutputSet("History", history, 0);
    if(histories.find(history) != histories.end()) {
    	cout << "Inside history check " << endl; 
    	return;
    }
    if(searchSet.size() == 1 && CoincidenceCheck(history)) {
	cout << "Inside Cascade Insertion" << endl;
	OutputSet("History", history, 1);
	cascades.insert(history);
	return;
    }
    
    set<double> newSearchSet;
    set<double> nodeCoins = coinMap.find(currentNode)->second;
    set_intersection(searchSet.begin(), searchSet.end(),
		     nodeCoins.begin(), nodeCoins.end(),
		     inserter(newSearchSet, newSearchSet.begin()) );
    
    if(newSearchSet.size() == 0) {
	cascades.insert(history);
	return;
    }

    //OutputSet("NodeCoins", nodeCoins,0);
    //OutputSet("New Search Set", newSearchSet, 0);
    //cout << endl;
    
    for(set<double>::iterator it = newSearchSet.begin(); it != newSearchSet.end();
	it++) {
	//cout << "Inside newSearchSet Loop " << *it << " " << currentNode << endl;
	history.insert(*it);
	OutputSet("History In Loop",history,4);
	if(CoincidenceCheck(history)) {
	    BranchBuilder(history, newSearchSet, *it);
	    histories.insert(history);
	}
    }
    //cout << "Immediately following the newSearchSet loop " << currentNode << endl;
}


//********** Coin **********
void Coin(void) {
    double gamma, coinGamma;
    ifstream input("testCoins00.dat");
    //ifstream input("coin.dat");
    while(!input.eof()) {
	input >> gamma >> coinGamma;
	gammas.insert(gamma);
	coinSet.insert(make_pair(gamma,coinGamma));
    }
    input.close();
    
    //! Build the coincidence map. (PART A)
    set<double> tempSet;
    for(set<GammaPair>::iterator it = coinSet.begin(); 
	it != coinSet.end(); it++) {
	double curGamma = it->first;
	double nextGamma = coinSet.upper_bound(*it)->first;
	GammaPair invCoin = make_pair(it->second, it->first);
	bool hasInverse = coinSet.find(invCoin) != coinSet.end();

	if(hasInverse)
	    tempSet.insert(it->second);

	if(nextGamma != curGamma || *it == *coinSet.rbegin()) {
	    coinMap.insert(make_pair(curGamma, tempSet));
	    tempSet.clear();
	}
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
    } //for(set<double>::iterator it
    
    // set<double> parent;
    // parent.insert(706);
    // BranchBuilder(parent, gammas, 706);

    cout << endl << "Here are the cascades: " << endl;
    int counter = 0;
    for(set<set<double> >::iterator it = cascades.begin(); it != cascades.end();
	it++, counter++)
	OutputSet("Cascade", *it, counter);
    // cout << endl;
    // for(set<set<double> >::iterator it = histories.begin(); it != histories.end();
    // 	it++, counter++)
    // 	OutputSet("History",*it, counter);
}//void Coin


//********* OutputSet **********
void OutputSet(const string &title, const set<double> &output, const int &number) {
    cout << title << " " << number << " : ";
    for(set<double>::iterator it = output.begin(); it != output.end(); it++)
	cout << *it << " ";
    cout << endl;
}
