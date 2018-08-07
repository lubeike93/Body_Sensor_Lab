function [ sliced_data, remains, num_portion, num_remain ] = data_slicer( input_dat, portion, label )
%#codegen
%DATA_SLICER Summary of this function goes here
%   Detailed explanation goes here
dim = size(input_dat, 2);
num_data = size(input_dat, 1);
sliced_data = zeros(portion, dim + 1);
num_portion = floor(num_data/ portion);
labels = zeros(portion, 1);
labels(1:portion) = label;

for i = 1:num_portion
    start_ind = (i - 1) * portion + 1;
    end_ind = start_ind + portion - 1;
    sliced_data(:, 2:end, i) = input_dat(start_ind:end_ind, :);
    sliced_data(:, 1, i) = labels;
end

if mod(num_data, portion) == 0
    remains = 0;
    num_remain = 0;
else
    remains = input_dat(end_ind+1:end, :);
    num_remain = 1;
end

end

