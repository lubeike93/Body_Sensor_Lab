function [ h_a, v_a ] = acc_3dto2d( xyz_acc_data )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
mx = sum(xyz_acc_data(:, 1)) / size(xyz_acc_data, 1);
my = sum(xyz_acc_data(:, 2)) / size(xyz_acc_data, 1);
mz = sum(xyz_acc_data(:, 3)) / size(xyz_acc_data, 1);

vertical_a = zeros(size(xyz_acc_data, 1), 3);
vertical_a(:, 1) = mx ./ sqrt(mx.^2 + my.^2 + mz.^2) .* xyz_acc_data(:, 1);
vertical_a(:, 2) = my ./ sqrt(mx.^2 + my.^2 + mz.^2) .* xyz_acc_data(:, 2);
vertical_a(:, 3) = mz ./ sqrt(mx.^2 + my.^2 + mz.^2) .* xyz_acc_data(:, 3);
horizontal_a = xyz_acc_data - vertical_a;

h_a = zeros(size(xyz_acc_data, 1), 1);
v_a = zeros(size(xyz_acc_data, 1), 1);

for index = 1:size(xyz_acc_data, 1)
    h_a(index) = norm(horizontal_a(index, :));
    v_a(index) = norm(vertical_a(index, :));
end
end