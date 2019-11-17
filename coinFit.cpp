bool CheckRange(double &num, double &low, double &high);
double Efficiency(double &gamma);
void CoinFit(const double &gammaA, const double &gammaB);

//********** CheckRange **********
bool CheckRange(double &num, double &low, double &high) {
   return((num >= low && num <= high));
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
