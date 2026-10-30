clc
clear
close all

%% generate dataset
red_green_datapoints = [
    1.0, 3.0;
    1.7, 4.8;
    1.5, 3.2;
    3.3, 4.3;
    1.19, 1.75;
    2.79, 2.40;
    1.2, 1.2;
    3.2, 2.8;
    2.85, 2.0;
    4.5, 1.0;
    4.0, 4.6;
    4.0, 2.7;
    4.3, 2.65;
    3.8, 2.53;
];

red_green_labels = [
    "green";
    "green";
    "green";
    "green";
    "green";
    "green";
    "red";
    "red";
    "red";
    "red";
    "red";
    "red";
    "red";
    "red";
];

% define the grid for plotting the entire space.
[x,y] = meshgrid(0:.2:5,0:.2:5);
x = x(:);
y = y(:);

%% Train the linear discriminator to be used as a classifier

% train the classifier
lda = fitcdiscr(red_green_datapoints(:,1:2), red_green_labels);

%% classification
% perform the classification task
[j,err,P,logp,coeff] = classify([x y], red_green_datapoints(:,1:2), red_green_labels);

% extract the coefficients for the constant and linear parameters
K = coeff(1,2).const;
L = coeff(1,2).linear;

% define the two locations on the x-axis to solve for the y-axis value
x_disseminator = [0 5]

% Function to compute K + [x,y]*L =0 for multiple vectors (x and y).
A = transpose(L)
y_disseminator = (-K-A(1,1)*x_disseminator)/A(1,2)

%% predict red or green

% For two potential data points, see if they fall in the red or green
% space.

test_location_1 = [1, 2];
test_location_2 = [4, 3];
test_locations = [test_location_1;test_location_2];
classify(test_location_1, red_green_datapoints(:,1:2),red_green_labels)
classify(test_location_2, red_green_datapoints(:,1:2),red_green_labels)

% find the misclassified items
lda_class = resubPredict(lda);
bad = ~strcmp(lda_class,red_green_labels);

%Plot dataset
f = figure('Name', 'Training Data');
hold on;
gscatter(red_green_datapoints(:,1), red_green_datapoints(:,2), red_green_labels,'grb');
plot(x_disseminator, y_disseminator, 'LineWidth',2)
plot(test_locations(:,1), test_locations(:,2), 'bd', 'MarkerSize',8)
plot(red_green_datapoints(bad,1), red_green_datapoints(bad,2), 'kx', 'MarkerSize',15);
xlabel('Voltage1');
ylabel('Voltage2');
axis([0 5 0 5])
legend('green', 'red', 'linear discriminator', 'test points', 'misclassified data points')
hold off;
saveas(gcf, 'Training Data.png')

%% visualize region
g = figure('Name','Predicted Outcomes');
hold on
gscatter(x,y,j,'grb','sod')
plot(x_disseminator,y_disseminator,'LineWidth',2)
xlabel('Voltage 1');
ylabel('Voltage 2');
axis([0 5 0 5])
legend('green','red','linear discriminator')
hold off;
saveas(gcf, 'Predicted Outcomes.png')

