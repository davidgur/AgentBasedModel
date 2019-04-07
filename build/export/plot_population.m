population_arr = csvread("population_sizes.csv");
Gr9 = population_arr(
:, [1:5]);
Gr10 = population_arr(
:, [6:10]);
Gr11 = population_arr(
:, [11:15]);
Gr12 = population_arr(
:, [16:20]);

daysToModel = length(Gr9) / (24 * 60);
tTotal = linspace(0, daysToModel, length(Gr9));

% ----- PLOTTING -----
subplot(2, 2, 1)
h = plot(tTotal, Gr9, 'LineWidth', 2);
legend(h,
'Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered');
title('Grade 9');
xlabel 'Time (days)';
ylabel 'Population';

subplot(2, 2, 2)
h = plot(tTotal, Gr10, 'LineWidth', 2);
legend(h,
'Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered');
title('Grade 10');
xlabel 'Time (days)';
ylabel 'Population';

subplot(2, 2, 3)
h = plot(tTotal, Gr11, 'LineWidth', 2);
legend(h,
'Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered');
title('Grade 11');
xlabel 'Time (days)';
ylabel 'Population';

subplot(2, 2, 4)
h = plot(tTotal, Gr12, 'LineWidth', 2);
legend(h,
'Susceptible', 'Vaccinated', 'Exposed', 'Infected', 'Recovered');
title('Grade 12');
xlabel 'Time (days)';
ylabel 'Population';