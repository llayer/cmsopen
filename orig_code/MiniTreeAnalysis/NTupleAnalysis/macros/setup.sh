#!/bin/bash

echo "#################################" 

dir=`pwd`
export NTUPLEANA_PATH=$dir
export NTUPLEANA=$dir
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/lib:$dir/../../../MiniTreeFormat/NTFormat/src/.:$dir/../.lib/
export NTUPLEDATAFORMAT_PATH=$dir/../../../MiniTreeFormat/NTFormat/


echo NTUPLEANA_PATH=$NTUPLEANA_PATH
echo NTUPLEANA=$NTUPLEANA_PATH

if [ -d ../.lib ] ; then 
        echo "List of librairies"
        ls ../.lib
else 
        mkdir ../.lib
fi

echo " Setup is DONE"
echo "#################################" 
