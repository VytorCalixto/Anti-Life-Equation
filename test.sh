make

HEADER="#Points SOR Residual"

echo -e "$HEADER\n" > timePlot
echo -e "$HEADER\n" > memPlot
echo -e "$HEADER\n" > cachePlot
echo -e "$HEADER\n" > flopsPlot

for i in 10 100 127 128 200 255 256 500 511 512 1000 1023 1024 2000 2047 2048
do
    echo "$i points"
    PI=$(echo "scale=20; 355/113" | bc)
    INTERVAL=$(echo "scale=20; $PI/($i-1)" | bc)
    echo "TIME"
    ./pdeSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o time
    TIME=$(cat time | grep Tempo | cut -d ':' -f 2 | tr -d ' ' | tr '\n' '\t')
    echo -e "$i\t$TIME" >> timePlot
    
    echo "MEM"
    likwid-perfctr -m -f -C 2 -g MEM ./pdeSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o mem > MEM
    TEXT=$(cat MEM | grep bandwidth | cut -d '|' -f 3 | tr -d ' ' | tr '\n' '\t')
    echo -e "$i\t$TEXT" >> memPlot
    rm MEM
    
    echo "CACHE"
    likwid-perfctr -m -f -C 3 -g CACHE ./pdeSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o cache > CACHE
    TEXT=$(cat CACHE | grep "data cache miss ratio" | cut -d '|' -f 3 | tr -d ' ' | tr '\n' '\t')
    echo -e "$i\t$TEXT" >> cachePlot
    rm CACHE

    echo "FLOPS"
    likwid-perfctr -m -f -C 5 -g FLOPS_DP ./pdeSolver -hx $INTERVAL -hy $INTERVAL -i 20 -o flops >> FLOPS
    TEXT=$(cat FLOPS | grep "DP MFLOP/s" | cut -d '|' -f 3 | tr -d ' ' | tr '\n' '\t')
    echo -e "$i\t$TEXT" >> flopsPlot
    rm FLOPS
done

rm time mem cache flops
