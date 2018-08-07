import tensorflow as tf
import numpy as np
import pandas as pd


def read_data(filename):
    data_frame = pd.read_csv(filename, delimiter=',', header=0)
    x = np.array(data_frame.loc[:, 'meanSVMAcc':'meanPeak'])
    y = np.transpose(np.array(data_frame.loc[:, 'label']))
    return np.float32(x), y.astype(int)


# read csv data
test_x, test_y = read_data('testing_data.csv')

# open .pb model
with open('model.pb', 'rb') as f:
    # init a buffer
    graph_def = tf.GraphDef()
    # read the .pb model into the buffer
    graph_def.ParseFromString(f.read())

# init a new graph
with tf.Graph().as_default() as graph:
    # import the .pb model from buffer into graph, add 'model' as prefix to nodes names
    tf.import_graph_def(graph_def, name=None)

    # get the input node as a tensor
    x = graph.get_tensor_by_name('import/dense_1_input:0')
    # get the output node as a tensor
    y = graph.get_tensor_by_name('import/output_node0:0')

    # open session for the graph
    with tf.Session(graph=graph) as sess:
        # feed the graph with test_x
        y_out = sess.run(y, feed_dict={x: test_x})
        # make predictions
        predictions = np.argmax(y_out, axis=1)
        print(predictions)
