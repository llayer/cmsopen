
1	#!/bin/tcsh
2	
3	setenv ROOTSYS /afs/cern.ch/sw/lcg/app/releases/ROOT/5.27.04/x86_64-slc5-gcc43-opt/root
4	setenv PATH ${ROOTSYS}/bin:$PATH
5	setenv LD_LIBRARY_PATH ${ROOTSYS}/lib:${LD_LIBRARY_PATH}
6	setenv ROOT_INCLUDE ${ROOTSYS}/include
