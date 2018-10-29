#! /bin/bash

for i in rho rhou rhov E; do
  ./catfunc.sh ${i}_ ${i} 
done
exit 0

#
# end of file
#
