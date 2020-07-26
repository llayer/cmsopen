#ifndef __TOP_TREE_MTCollection_H__
#define __TOP_TREE_MTCollection_H__

#include <string>
#include <iostream>
#include <vector>

/** MTCollection: class MTCollection for NTuple. \\
Contains for a given object the list of possible collections.
(e. g. vector of shrinking cone taus and hps taus...)
*/
using namespace std;

namespace TopTree
{
  template < class T > class MTCollection
  {
  public:
    //methods
    MTCollection (){};
    ~MTCollection (){};
    
     void Reset() 
     {
        objects.clear ();
        algo = "";}
 
     void Dump(std::ostream & os = std::cout)
     {
       os << "MTCollection contains " << objects.size() << " for the algo: " << algo << endl;}
       
     void PrintListOfObjects(std::ostream & os = std::cout)
     {
       os << "List for algo "<< algo << endl;
       for (unsigned int i =0 ; i< objects.size() ; i++){
            os << " ---  " << i << endl;
            objects[i].Dump(os);}}
     
    //data members
    
    std::vector < T > objects; // object = TopTree::NTJet or TopTree::NTTau...
    std::string algo;

  };
}
#endif

