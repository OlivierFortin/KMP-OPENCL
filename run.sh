#!/bin/bash
export PATH=/usr/local/browndeer/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/browndeer/lib:/usr/local/lib:$LD_LIBRARY_PATH

---------------------
sudo -s <<EOF
./bin/main
EOF
