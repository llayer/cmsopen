#ifndef CommonTools_Utils_cutParset_h
#define CommonTools_Utils_cutParset_h
#include "../src/SelectorPtr.h"
//#include "Exception.h"
#include <Reflex/Type.h>
#include <string>

namespace reco {
  namespace parser {
    bool cutParser(const Reflex::Type &t, const std::string & cut, SelectorPtr & sel, bool lazy) ;

    template<typename T>
    inline bool cutParser(const std::string & cut, SelectorPtr & sel, bool lazy=false) {
        return reco::parser::cutParser(Reflex::Type::ByTypeInfo(typeid(T)), cut, sel, lazy);
    }
  }
}

#endif
