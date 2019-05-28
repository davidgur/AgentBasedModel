#!/bin/bash
# Iterate through all the directories in the folder (all simulations)
for d in * ; do
    if [[ -d "$d" ]] && [[ "$d" == S* ]] ; then
        #(2) plot_population.py
        python plot_population.py $d analyzed_data/${d:6}/
    fi
done
# Done!
echo "Done!"