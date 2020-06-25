1	/*
2	Allowing lazy parsing of expressions. See description on hypernews:
3	  https://hypernews.cern.ch/HyperNews/CMS/get/physTools/1224.html
4	Also adding support for "_" in the method names.
5	*/
6	
7	#ifndef CommonTools_Utils_StringCutObjectSelector_h
8	#define CommonTools_Utils_StringCutObjectSelector_h
9	/* \class StringCutObjectSelector
10	 *
11	 * \author Luca Lista, INFN
12	 *
13	 * $Id: StringCutObjectSelector.h,v 1.1 2011/10/07 13:22:38 alebihan Exp $
14	 */
15	//AC#include "FWCore/Utilities/interface/EDMException.h"
16	#include "../src/SelectorPtr.h"
17	#include "../src/SelectorBase.h"
18	#include "cutParser.h"
19	
20	template<typename T, bool DefaultLazyness=false>
21	struct StringCutObjectSelector {
22	  StringCutObjectSelector(const std::string & cut, bool lazy=DefaultLazyness) : 
23	    type_(Reflex::Type::ByTypeInfo(typeid(T))) {
24	    if(! reco::parser::cutParser<T>(cut, select_, lazy)) {
25	      std::cout << "failed to parse "<< cut << std::endl;
26	    }
27	  }
28	  StringCutObjectSelector(const reco::parser::SelectorPtr & select) : 
29	    select_(select),
30	    type_(Reflex::Type::ByTypeInfo(typeid(T))) {
31	  }
32	  bool operator()(const T & t) const {
33	    using namespace Reflex;
34	    Object o(type_, const_cast<T *>(& t));
35	    return (*select_)(o);  
36	  }
37	
38	private:
39	  reco::parser::SelectorPtr select_;
40	  Reflex::Type type_;
41	};
42	
43	#endif
44	
45	
