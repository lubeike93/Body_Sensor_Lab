package com.rcsexample.btfft2;

import android.content.res.AssetManager;
import org.tensorflow.contrib.android.TensorFlowInferenceInterface;
import org.tensorflow.Operation;

/**
 * Created by wang on 2017/12/22.
 */

public class MotionClassifier {
    private String inputName;
    private String outputName;
    private int inputSize;
    private boolean logStats = false;

    private TensorFlowInferenceInterface inferenceInterface;

    private String[] outputNames;
    private float[] outputs;

    private MotionClassifier(){}

    public static MotionClassifier create(
            AssetManager am,
            String modelFilename,
            String inputName,
            String outputName,
            int inputSize
    ){
        MotionClassifier c = new MotionClassifier();

        // 'model/dense_1_input:0'
        c.inputName = inputName;
        // 'model/output_node0:0'
        c.outputName = outputName;
        c.inputSize = inputSize;
        c.inferenceInterface = new TensorFlowInferenceInterface(am, modelFilename);
        final Operation operation = c.inferenceInterface.graphOperation(outputName);
        final int numClasses = (int) operation.output(0).shape().size(1);

        c.outputNames = new String[] {outputName};
        c.outputs = new float[numClasses];
        return c;
    }

    public int recognizeMotion(final float[] featureArray){
        inferenceInterface.feed(inputName, featureArray, 1, inputSize);
        inferenceInterface.run(outputNames, logStats);
        inferenceInterface.fetch(outputName, outputs);

        int finalPrediction = -1;
        float largestValue = 0;
        int index = 0;
        for(float pred : outputs){
            if (largestValue < pred){
                largestValue = pred;
                finalPrediction = index;
            }
            index++;
        }
        return finalPrediction;
    }

    public void close(){
        inferenceInterface.close();
    }
}
