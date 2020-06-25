1	#include "../interface/TopKinFitter.h"
2	
3	/// default configuration is: max iterations = 200, max deltaS = 5e-5, maxF = 1e-4
4	TopKinFitter::TopKinFitter(const int maxNrIter, const double maxDeltaS, const double maxF,
5	                           const double mW, const double mTop): 
6	  maxNrIter_(maxNrIter), maxDeltaS_(maxDeltaS), maxF_(maxF), mW_(mW), mTop_(mTop)
7	{
8	  fitter_ = new TKinFitter("TopKinFitter", "TopKinFitter");
9	  fitter_->setMaxNbIter(maxNrIter_);
10	  fitter_->setMaxDeltaS(maxDeltaS_);
11	  fitter_->setMaxF(maxF_);
12	  fitter_->setVerbosity(0);
13	}
14	
15	/// default destructor
16	TopKinFitter::~TopKinFitter() 
17	{
18	  delete fitter_;
19	}
20	
21	/// convert Param to human readable form
22	std::string 
23	TopKinFitter::param(const Param& param) const
24	{
25	  std::string parName;
26	  switch(param){
27	  case kEMom       : parName="EMom";       break;
28	  case kEtEtaPhi   : parName="EtEtaPhi";   break;
29	  case kEtThetaPhi : parName="EtThetaPhi"; break;    
30	  }
31	  return parName;
32	}

