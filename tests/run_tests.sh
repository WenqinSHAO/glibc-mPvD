#!/bin/bash

space=" "

for var in $@
do
arg=$arg$space$var
done

cd ../build
./testrun.sh ../tests/${arg:1}

exit 0
