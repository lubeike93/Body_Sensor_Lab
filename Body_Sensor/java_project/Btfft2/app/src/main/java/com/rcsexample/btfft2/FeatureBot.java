package com.rcsexample.btfft2;

import java.lang.Math;
import java.util.ArrayList;

/**
 * Created by wang on 2018/1/9.
 */

public class FeatureBot {
    public FeatureBot(){
    }

    public int getSteps(double[] xSequence, double[] ySequence, double[] zSequence, int status) {
        if (status == 3) return 0;

        int sigma;
        int minDis;
        switch (status) {
            case 0:
                sigma = 8;
                minDis = 40;
                break;
            case 1:
                sigma = 8;
                minDis = 25;
                break;
            case 2:
                sigma = 10;
                minDis = 40;
                break;
            default:
                sigma = 4;
                minDis = 35;
                break;
        }

        // smoothing triaxial data
        double[] smoothedX = gaussianSmoothing(xSequence, 4);
        double[] smoothedY = gaussianSmoothing(ySequence, 4);
        double[] smoothedZ = gaussianSmoothing(zSequence, 4);

        // triaxial to single vector magnitude
        double[] svm = triaxialToSVM(smoothedX, smoothedY, smoothedZ);

        // smoothing svm
        double[] smoothedSVM = gaussianSmoothing(svm, sigma);

        double absMean = (max(smoothedSVM) + min(smoothedSVM)) / 2.0;
        double mph = Math.max(400, absMean + (max(smoothedSVM) - absMean) / 2.0);
        mph = Math.max(mph, 400);
        double[] peaks = findPeaks(smoothedSVM, mph, minDis);

        return peaks.length;
    }

    public double[] getFeatures(double[] xSequence, double[] ySequence, double[] zSequence, boolean useFullFeat) {
        int sigma = 4;

        // smoothing triaxial data
        double[] smoothedX = gaussianSmoothing(xSequence, sigma);
        double[] smoothedY = gaussianSmoothing(ySequence, sigma);
        double[] smoothedZ = gaussianSmoothing(zSequence, sigma);

        // triaxial to single vector magnitude
        double[] svm = triaxialToSVM(smoothedX, smoothedY, smoothedZ);

        // smoothing svm
        double[] smoothedSVM = gaussianSmoothing(svm, sigma);

        double absMean = (max(smoothedSVM) + min(smoothedSVM)) / 2.0;
        double mph = absMean + (max(smoothedSVM) - absMean) / 2.0;
        double[] peaks = findPeaks(smoothedSVM, mph, 35);
        double meanPeaks = mean(peaks);

        ArrayList<Double> featureList = generateFeatureVector(smoothedX, smoothedY, smoothedZ, smoothedSVM, meanPeaks, useFullFeat);
        return arrayList2Array(featureList);
    }

    private ArrayList<Double> generateFeatureVector(double[] x, double[] y, double[] z, double[] svm, double meanPeaks, boolean useFullFeat) {
        ArrayList<Double> featureList = new ArrayList<>();

        if (useFullFeat) {
            featureList.add(mean(svm));
            featureList.add(mean(x));
            featureList.add(mean(y));
            featureList.add(mean(z));
        }

        featureList.add(max(svm));
        featureList.add(max(x));

        if (useFullFeat) {
            featureList.add(max(y));
            featureList.add(max(z));
            featureList.add(min(svm));
        }

        featureList.add(min(x));

        if (useFullFeat) {
            featureList.add(min(y));
            featureList.add(min(z));
        }

        featureList.add(std(svm));
        featureList.add(std(x));

        if (useFullFeat) {
            featureList.add(std(y));
            featureList.add(std(z));
        }

        featureList.add(meanPeaks);

        return featureList;
    }

    private double std(double[] input) {
        double sum = 0.0, standardDeviation = 0.0;

        for(double num : input) {
            sum += num;
        }

        double mean = sum/input.length;

        for(double num: input) {
            standardDeviation += Math.pow(num - mean, 2);
        }

        return Math.sqrt(standardDeviation/input.length);
    }

    private double[] findPeaks(double[] sequence, double minPeakHeight, double minDistance) {
        int lastPeakIndex = 0;
        ArrayList<Double> peaksArrayList = new ArrayList<>();

        for (int i = 1; i < sequence.length - 1; i++) {
            if (sequence[i] >= sequence[i - 1]){
                if (sequence[i] > sequence[i + 1]) {
                    if(sequence[i] > minPeakHeight) {
                        int dis = i - lastPeakIndex;
                        if (lastPeakIndex == 0) {
                            peaksArrayList.add(sequence[i]);
                            lastPeakIndex = i;
                        } else if (dis >= minDistance) {
                            peaksArrayList.add(sequence[i]);
                            lastPeakIndex = i;
                        }
                    }
                }
            }
        }

        double[] peaksArray = new double[peaksArrayList.size()];
        for (int i = 0; i < peaksArrayList.size(); i++) {
            peaksArray[i] = peaksArrayList.get(i);
        }
        return peaksArray;
    }

    private double[] triaxialToSVM(double[] xSequence, double[] ySequence, double[] zSequence) {
        double[] svm = new double[xSequence.length];
        for (int i = 0; i < xSequence.length; i++) {
            svm[i] = Math.sqrt(xSequence[i] * xSequence[i] + ySequence[i] * ySequence[i] + zSequence[i] * zSequence[i]);
        }
        return svm;
    }

    private double[] gaussianSmoothing(double[] data, double sigma){
        int n = data.length;
        int[] t = new int[data.length];
        for (int i = 0; i < data.length; i++){
            t[i] = i+1;
        }
        int[] dt = diff(t);
        int dt_ = dt[0];
        double a = 1 / (Math.sqrt(2 * Math.PI) * sigma);
        double sigma2 = sigma * sigma;
        double mean = mean(t);
        double thresh = dt_ * a * 0.000001;

        // make filter
        ArrayList<Double> filter = new ArrayList<>();
        for (int i = 0; i < t.length; i++){
            double buffer = dt_ * a * Math.exp(-0.5 * Math.pow(t[i] - mean, 2) / sigma2);
            if (buffer >= thresh) filter.add(buffer);
        }
        double[] filterArray = new double[filter.size()];
        for (int i = 0; i < filter.size(); i++){
            filterArray[i] = filter.get(i);
        }
        int sizeAfterConv = filterArray.length + data.length - 1;

        // init smoothed data
        double[] smoothedData = new double[sizeAfterConv];
        for(int i = 0; i < sizeAfterConv; i++){
            smoothedData[i] = 0;
        }

        // convolution
        ArrayList<Double> smoothedList = new ArrayList<>();
        for(int i = 0; i < sizeAfterConv; i++)
        {
            for(int j = Math.max(0,i + 1 - filterArray.length); j <= Math.min(i, data.length-1); j++) {
                smoothedData[i] += data[j] * filterArray[i-j];
            }
            smoothedList.add(smoothedData[i]);
        }

        // trim to same size as data
        boolean front = true;
        do {
            if (front) {
                front = false;
                smoothedList.remove(0);
            } else {
                front = true;
                smoothedList.remove(smoothedList.size() - 1);
            }
        } while (smoothedList.size() != data.length);

        double[] results = new double[smoothedList.size()];
        for (int i = 0; i < smoothedList.size(); i++){
            results[i] = smoothedList.get(i);
        }
        return results;
    }

    private int[] diff(int[] input) {
        int[] result = new int[input.length-1];
        for (int i = 1; i < input.length; i++) {
            result[i-1] = input[i] - input[i-1];
        }
        return result;
    }

    private double mean(int[] input) {
        double mean = 0;
        for (int i = 0; i < input.length; i++){
            mean += input[i];
        }
        mean = mean / input.length;
        return mean;
    }

    private double mean(double[] input) {
        double mean = 0;
        for (int i = 0; i < input.length; i++){
            mean += input[i];
        }
        mean = mean / input.length;
        return mean;
    }

    private double max(double[] input) {
        double max = -99999.0;
        for (int i = 0; i < input.length; i++) {
            if (max < input[i]) max = input[i];
        }
        return max;
    }

    private double min(double[] input) {
        double min = 99999.0;
        for (int i = 0; i < input.length; i++) {
            if (min > input[i]) min = input[i];
        }
        return min;
    }

    private double[] arrayList2Array(ArrayList<Double> al) {
        double[] a = new double[al.size()];
        for (int i = 0; i < al.size(); i++) {
            a[i] = al.get(i);
        }
        return a;
    }

}
