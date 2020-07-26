
#!/bin/tcsh

setenv ROOTSYS /afs/cern.ch/sw/lcg/app/releases/ROOT/5.27.04/x86_64-slc5-gcc43-opt/root
setenv PATH ${ROOTSYS}/bin:$PATH
setenv LD_LIBRARY_PATH ${ROOTSYS}/lib:${LD_LIBRARY_PATH}
setenv ROOT_INCLUDE ${ROOTSYS}/include
