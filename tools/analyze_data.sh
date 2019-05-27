#!/bin/bash

# First, make a directory for all the export data
rm -rf analyzed_data
mkdir analyzed_data

# Iterate through all the directories in the folder (all simulations)
for d in * ; do
    if [[ -d "$d" ]] && [[ "$d" == S* ]] ; then

        #(1) at_home_plot.py
        mkdir analyzed_data/${d:6}
        python at_home_plot.py $d analyzed_data/${d:6}/A

        #(2) plot_population.py
        python plot_population.py $d analyzed_data/${d:6}/

        #(3) duration_of_outbreak.py
        echo "${d:6}: $(python duration_of_outbreak.py $d)" >> analyzed_data/duration_of_outbreak.txt

        #(4) infected_portion.py
        echo "${d:6}: $(python infected_portion.py $d)" >> analyzed_data/infected_portion.txt

        #(5) peak_times.py
        echo "${d:6}: $(python peak_times.py $d)" >> analyzed_data/peak_times.txt

        #(6) secondary_infections.py
        echo "${d:6}: $(python secondary_infections.py $d)" >> analyzed_data/secondary_infections.txt

    fi
done

# Also do probability of outbreak
python probability_of_outbreak.py >> analyzed_data/probability_of_outbreak.txt

# Done!
echo "Done!"