1	#ifndef CommonTools_Utils_cutParset_h
2	#define CommonTools_Utils_cutParset_h
3	#include "../src/SelectorPtr.h"
4	//#include "Exception.h"
5	#include <Reflex/Type.h>
6	#include <string>
7	
8	namespace reco {
9	  namespace parser {
10	    bool cutParser(const Reflex::Type &t, const std::string & cut, SelectorPtr & sel, bool lazy) ;
11	
12	    template<typename T>
13	    inline bool cutParser(const std::string & cut, SelectorPtr & sel, bool lazy=false) {
14	        return reco::parser::cutParser(Reflex::Type::ByTypeInfo(typeid(T)), cut, sel, lazy);
15	    }
16	  }
17	}
18	
19	#endif
