make

HEADER="#Points\tNewSOR\tNewResidual\tOldSOR\tOldResidual"

#echo -e "$HEADER\n" > timePlot
#echo -e "$HEADER\n" > memPlot
#echo -e "$HEADER\n" > cachePlot
#echo -e "$HEADER\n" > flopsPlot

for i in 2000 2047 2048
do
    echo "$i points"
    PI=$(echo "scale=20; 355/113" | bc)
    INTERVAL=$(echo "scale=8; $PI/($i-1)" | bc)
    echo "TIME"
    ./oldSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o timeOld
    ./pdeSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o time
    TIME=$(cat time timeOld | grep Tempo | cut -d ':' -f 2 | tr -d ' ' | tr '\n' '\t')
    echo -e "$i\t$TIME" >> timePlot
    
    echo "MEM"
    likwid-perfctr -m -f -C 2 -g MEM ./pdeSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o mem > MEM
    likwid-perfctr -m -f -C 5 -g MEM ./oldSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o mem > MEMOLD
    TEXT=$(cat MEM MEMOLD | grep bandwidth | cut -d '|' -f 3 | tr -d ' ' | tr '\n' '\t')
    echo -e "$i\t$TEXT" >> memPlot
    rm MEM MEMOLD
    
    echo "CACHE"
    likwid-perfctr -m -f -C 3 -g CACHE ./pdeSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o cache > CACHE
    likwid-perfctr -m -f -C 0 -g CACHE ./oldSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o cache > CACHEOLD
    TEXT=$(cat CACHE CACHEOLD | grep "data cache miss ratio" | cut -d '|' -f 3 | tr -d ' ' | tr '\n' '\t')
    echo -e "$i\t$TEXT" >> cachePlot
    rm CACHE CACHEOLD

    echo "FLOPS"
    likwid-perfctr -m -f -C 5 -g FLOPS_DP ./pdeSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o flops >> FLOPS
    likwid-perfctr -m -f -C 1 -g FLOPS_DP ./oldSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o flops >> FLOPSOLD
    TEXT=$(cat FLOPS FLOPSOLD | grep "DP MFLOP/s" | cut -d '|' -f 3 | tr -d ' ' | tr '\n' '\t')
    echo -e "$i\t$TEXT" >> flopsPlot
    rm FLOPS FLOPSOLD
done

rm time timeOld mem cache flops
