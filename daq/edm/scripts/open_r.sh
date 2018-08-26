!/bin/bash -l
export TERM=vt100
explocation=/home/helios/experiments/iss000/daq
datalocation=/media/DIGIOSDATA3/data/iss000

cd ${datalocation}
caput Online_CS_SaveData No Save
caput Online_CS_StartStop Stop
xterm -T savedata -fa "Monospace" -fs 12 -bg rgb:cd/20/2c -fg white -geometry 100x30+500+20 -e "${explocation}/edm/scripts/start_run.sh"
sleep 1
cd ${explocation}

echo Run is underway
echo Done - Terminal Window is Free to use 
