#!/bin/bash

ipcs -m | cut -d' ' -f2 | grep '^[0-9]' | while read x; 
do ipcrm -m {$x}; 
done
