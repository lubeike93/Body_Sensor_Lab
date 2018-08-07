import argparse
from freeze import freeze
import keras
import keras.backend as K
import os
import tensorflow as tf
from tensorflow.python.tools.freeze_graph import freeze_graph


if __name__ == '__main__':

    input_binary = True
    K.set_learning_phase(0)

    path_to_keras_model_file = 'themodel.h5'
    model_file_basename, file_extension = os.path.splitext(os.path.basename(path_to_keras_model_file))

    model = keras.models.load_model(path_to_keras_model_file)

    model_input = model.input.name.replace(':0', '')
    model_output = model.output.name.replace(':0', '')

    sess = K.get_session()

    width, height, channels = int(model.input.shape[2]), int(model.input.shape[1]), int(model.input.shape[3])
    # END OF keras specific code

    freeze(sess, model_file_basename, model_input, width, height, channels, model_output)