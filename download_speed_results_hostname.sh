#!/bin/bash

echo "Using {$1} as the hostname"
echo "Using {$2} as the remote-system"
echo "Using {$3} as the savefile"

scp $1@$2:/p/jmz9sadprojects/nsp-kyber/speed.csv $3_speed.csv 
