#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <cmath>
#include <cstdlib>

using namespace std;
typedef pair<double,double> data;

//! Declarations
bool CheckRange(double &num, double &low, double &high);
double Efficiency(double &gamma);
void CoinFit(const double &gammaA, const double &gammaB);
void Coin(const double &gammaA, const double &gammaB);


//********** Main **********
int main(int argc, char* argv[]) {
   Coin(1,3);
   //Coin(atof(argv[1]), atof(argv[2]));
   //CoinFit(atof(argv[1]), atof(argv[2]));
}


//********** CheckRange **********
bool CheckRange(double &num, double &low, double &high) {
   return((num >= low && num <= high));
}


//********** Coin **********
void Coin(const double &gammaA, const double&gammaB) {
   typedef pair<double,double> GammaPair;
   typedef multimap<double,double> CoinMap;
   typedef pair<CoinMap::iterator, CoinMap::iterator> RangePair;

   CoinMap coinMap;
   double gamma, coinGamma;
   set<double> gammas;
   set<GammaPair> coinSet;
   vector<double> trunk, branch0, branch1;

   ifstream input("coin-test.dat");
   while(!input.eof()) {
      input >> gamma >> coinGamma;
      gammas.insert(gamma);
      coinSet.insert(make_pair(gamma,coinGamma));
   }
   input.close();
   
   //! Build the coincidence map. (PART A)
   cout << "Building Coincidence Map (PART A)" << endl;
   set<GammaPair> tempCoinSet = coinSet;
   for(set<GammaPair>::iterator it = tempCoinSet.begin(); 
       it != tempCoinSet.end(); it++) {
      GammaPair inv = make_pair(it->second, it->first);
      set<GammaPair>::iterator itInvPos = tempCoinSet.find(inv);
      
      cout << "The Current Pair: " << it->first << " " << it->second << endl;

      if(itInvPos != tempCoinSet.end()) {
	 coinMap.insert(make_pair(it->first,it->second));
	 tempCoinSet.erase(inv);
      }
   }

   cout << endl << "Here's what we've got in the coincidence map" << endl;
   for(CoinMap::iterator it = coinMap.begin(); it != coinMap.end(); it++)
      cout << "OH YeAH!!!!! " << (*it).first << " " << (*it).second << endl;
   
   cout << endl << "Begin working on building the trees...(PART B)" << endl;
   for(set<double>::iterator it = gammas.begin(); it != gammas.end(); it++) {
      cout << "Coincidence info for Gamma " << *it << endl;
      
      RangePair currentRange = coinMap.equal_range((*it));
      
      //Fills up the trunk.
      for(CoinMap::iterator itMap = currentRange.first; 
	  itMap != currentRange.second; itMap++)
	 trunk.push_back(itMap->second);

      //Outputs the trunk
      cout << "Trunk : ";
      for(vector<double>::iterator main = trunk.begin(); 
	  main != trunk.end(); main++) {
	 cout << *main << " " ;
      }
      cout << endl;
      //Loops over possible iterations of the trunk and creates branches.
      if(trunk.size() > 1) {
	 do {
	    GammaPair testPair = make_pair(trunk[0], trunk[1]);
	    if(coinSet.find(testPair) == coinSet.end()) {
	       branch0.insert(branch0.begin(), trunk.begin()+1, trunk.end());
	       trunk.erase(trunk.begin()+1);
	    }
	    
	    vector<double> tempTree = branch0;
	    sort(tempTree.begin(), tempTree.end());
	    tempTree.erase(unique(tempTree.begin(), tempTree.end()), tempTree.end());
	    
	    if(!tempTree.empty()) {
	       cout << "Branch0 :";
	       for(vector<double>::iterator itBranch0 = tempTree.begin(); 
		   itBranch0 != tempTree.end(); itBranch0++) {
		  cout << *itBranch0 << " ";
	       }
	       cout << endl;
	    }
	 } while (next_permutation(trunk.begin(), trunk.end()));
	 cout << endl;
      }
	 
      trunk.clear();
      branch0.clear();
      branch1.clear();
      cout << endl;
   }//for(set<double>
}


//********** CoinFit **********
void CoinFit(const double &gammaA, const double &gammaB) {
   
   bool rangeA, rangeB;
   data searchA = make_pair(gammaA, gammaB);
   data searchB = make_pair(gammaB, gammaA);
   double gamma, coinGamma, area, error;
   map<data , data> info;
   
   ifstream input("coinFit.temp");
   while(input) {
      input >> gamma >> coinGamma >> area >> error;
      data temp = make_pair(gamma, coinGamma);
      double low  = area - area * error;
      double high = area + area * error;
      data temp0 = make_pair(low, high);
      info.insert(make_pair(temp, temp0));
   }
   input.close();

   map<data, data>::iterator resultA = info.find(searchA);
   map<data, data>::iterator resultB = info.find(searchB);

   if(resultA == info.end()) {
      cout << "Cannot match up the pair: Gamma = " << gammaA
	   << "    CoinGamma = " << gammaB << endl;
   } else if (resultB == info.end()) {
      cout << "Cannot match up the pair: Gamma = " << gammaB 
	   << "    CoinGamma = " << gammaA << endl;
   } else {
      double lowA  = (*resultA).second.first;
      double highA = (*resultA).second.second;
      double lowB  = (*resultB).second.first;
      double highB = (*resultB).second.second;      

      cout << "Range A : " << lowA << " " << highA << endl
	   << "Range B : " << lowB << " " << highB << endl;

      if(CheckRange(highA, lowB, highB) || 
	 CheckRange(highB, lowA, highA))
	 cout << "All right!!!" << endl;
   }
}


//********** Efficiency **********
double Efficiency(double &gamma) { 
   double a = 3.32279414992658;
   double b = 1.54427141359085;
   double c = 0.0;
   double d = 1.72160285083389;
   double e = -0.917694438461626;
   double f = -0.023829901492745;
   double g = 4.87920961486282;
   double E1 = 50.;
   double E2 = 1000.;

   double eff = exp(pow(pow((a+b*log10(gamma/E1)+c*pow(log10(gamma/E1),2)),-g) + 
    			pow(d+e*log10(gamma/E2)+f*pow(log10(gamma/E2),2), -g), -1/g));
   return(eff);   
}
