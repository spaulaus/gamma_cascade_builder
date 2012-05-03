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
   typedef multimap<double, set<double> > CoinMap;

   CoinMap coinMap;
   double gamma, coinGamma;
   set<double> gammas;
   set<GammaPair> coinSet;
   
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
   set<double> tempSet;
   double tempGamma = coinSet.begin()->first;
   for(set<GammaPair>::iterator it = tempCoinSet.begin(); 
       it != tempCoinSet.end(); it++) {
      cout << "The Current Pair: " << it->first << " " << it->second << endl;
      gamma = it->first;
      
      if(tempGamma != gamma) {
	 coinMap.insert(make_pair(tempGamma,tempSet));
	 tempSet.clear();
      }
      
      GammaPair inv = make_pair(it->second, it->first);
      set<GammaPair>::iterator itInvPos = tempCoinSet.find(inv);
      
      if(itInvPos != tempCoinSet.end()) {
	 tempSet.insert(it->second);
	 tempCoinSet.erase(inv);
      }
         
      tempGamma = gamma;
   }

   cout << endl << "Here's what we've got in the coincidence map" << endl;
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
      cout << "Coincidence info for Gamma " << *it << endl;
      CoinMap::iterator itMap = coinMap.find(*it);
      if(itMap == coinMap.end())
	 continue;
      
      set<double> trunk = itMap->second;      
      set<double> branch0, branch1, branch2;

      cout << "Trunk: ";
      for(set<double>::iterator it0 = trunk.begin(); it0 != trunk.end(); it0++)
	 cout << *it0 << " ";
      cout << endl;

      branch0.insert(*trunk.begin());
      trunk.erase(*trunk.begin());

      CoinMap::iterator itFound = coinMap.find(*branch0.begin());
      if(itFound == coinMap.end()) {
	 cout << "Branch0 : " << *branch0.begin() << endl;
	 continue;
      }
      
      set<double> branchCoin = itFound->second;
      for(set<double>::iterator it0 = trunk.begin(); it0 != trunk.end(); it0++) {
	 set<double>::iterator found = branchCoin.find(*it0);
	 if(found != branchCoin.end()) {
	    branch0.insert(*it0);
	 }else {
	    branch1.insert(*it0);
	    trunk.erase(*it0);
	 }
      }
      cout << "Branch 0 : ";
      for(set<double>::iterator it1 = branch0.begin(); 
	  it1 != branch0.end(); it1++)
	 cout << *it1 << " ";
      cout << endl;
      
      if(!branch1.empty()) {
	 itFound = coinMap.find(*branch1.begin());
	 branchCoin = itFound->second;
	 
	 for(set<double>::iterator it0 = trunk.begin(); 
	     it0 != trunk.end(); it0++) {
	    set<double>::iterator found = branchCoin.find(*it0);
	    if(found != branchCoin.end()) {
	       branch1.insert(*it0);
	    }else {
	       branch2.insert(*it0);
	       trunk.erase(*it0);
	    }
	 }
	 cout << "Branch 1 : ";
	 for(set<double>::iterator it1 = branch1.begin(); 
	     it1 != branch1.end(); it1++)
	    cout << *it1 << " ";
	 cout << endl;
      }
      cout << endl;
   }//for(set<double>
}//void Coin


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
