/*
Allowing lazy parsing of expressions. See description on hypernews:
  https://hypernews.cern.ch/HyperNews/CMS/get/physTools/1224.html
Also adding support for "_" in the method names.
*/

#ifndef CommonTools_Utils_StringCutObjectSelector_h
#define CommonTools_Utils_StringCutObjectSelector_h
/* \class StringCutObjectSelector
 *
 * \author Luca Lista, INFN
 *
 * $Id: StringCutObjectSelector.h,v 1.1 2011/10/07 13:22:38 alebihan Exp $
 */
//AC#include "FWCore/Utilities/interface/EDMException.h"
#include "../src/SelectorPtr.h"
#include "../src/SelectorBase.h"
#include "cutParser.h"

template<typename T, bool DefaultLazyness=false>
struct StringCutObjectSelector {
  StringCutObjectSelector(const std::string & cut, bool lazy=DefaultLazyness) : 
    type_(Reflex::Type::ByTypeInfo(typeid(T))) {
    if(! reco::parser::cutParser<T>(cut, select_, lazy)) {
      std::cout << "failed to parse "<< cut << std::endl;
    }
  }
  StringCutObjectSelector(const reco::parser::SelectorPtr & select) : 
    select_(select),
    type_(Reflex::Type::ByTypeInfo(typeid(T))) {
  }
  bool operator()(const T & t) const {
    using namespace Reflex;
    Object o(type_, const_cast<T *>(& t));
    return (*select_)(o);  
  }

private:
  reco::parser::SelectorPtr select_;
  Reflex::Type type_;
};

#endif


