% This script calculates the time evolution of the temperature distribution
% in the metal plate

%% Clear previous work

clc; % clear command window
clear; % clear the workspace
close all; % close all previously opened up figures and images

%% calculate the coordinates of the points

% creates a vector that starts at 0, increments 0.25, and ends at 10
x_cm = 0:0.25:10;
y_cm = 0:0.25:10;
t = 0;
% creates nPts and nPts_center in case the matrix changes
nPts = 41; 
nPts_center = (nPts + 1) / 2;

% creates a matrix of 1s (41 x 41) and makes all the values 23
Tnew = 23 * ones(41,41);
% makes the first row 500
Tnew(1, :) = 500; 
% makes the last row 500
Tnew(41, :) = 500;
% row 2-40 and the 1st column are 0
Tnew(2:40, 1) = 0; 
% row 2-40 and the last column are 0 
Tnew(2:40, 41) = 0;

%% for loop

% print time t=0 first because it will not reach it in the for-loop
fprintf("At time %4d, center temp. = %8.4f " + char(176) + "C.\n", t, Tnew(nPts_center, nPts_center))
prob14_plotData(x_cm, y_cm, Tnew, t, 2) % plot t=0
pause(0.25)

% in case we want to change the increment, we make a variable
print_increment = 50;

% for loop from 1-2500 for time
for t=1:2500
    % initalize Told as Tnew
    Told = Tnew;
    % only changes the interior points with the equation below (neighboring
    % points) - uses the equation from the problem 14 directions and matrix
    % operations
    Tnew(2:nPts-1, 2:nPts-1) = (Told(1:nPts-2, 2:nPts-1) + Told(3:nPts, 2:nPts-1) + Told(2:nPts-1, 1:nPts-2) + Told(2:nPts-1, 3:nPts)) / 4;
    if mod(t, print_increment) == 0
        % only prints, plots, and pauses if t % 50 == 0
        fprintf("At time %4d, center temp. = %8.4f " + char(176)+ "C.\n", t, Tnew(nPts_center, nPts_center))
        prob14_plotData(x_cm, y_cm, Tnew, t, 2) % calls the function from prob14_plotData
        pause(0.25) % pauses for 0.25 before printing and plotting again
    end
end




