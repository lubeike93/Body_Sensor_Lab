from keras.models import Sequential
from keras.layers import Dense
from keras.layers import Dropout
from keras.models import load_model
from keras.utils import to_categorical
import pandas as pd
import numpy as np


def read_data(filename):
    data_frame = pd.read_csv(filename, delimiter=',', header=0)
    x = np.array(data_frame.loc[:, 'meanSVMAcc':'meanPeak'])
    y = np.transpose(np.array(data_frame.loc[:, 'label']))

    return np.float32(x), y.astype(int)


def build_linear_model(input_dim):
    model = Sequential()
    model.add(Dense(50, input_dim=input_dim, activation='relu'))
    model.add(Dropout(0.25))
    model.add(Dense(40, activation='relu'))
    model.add(Dense(30, activation='relu'))
    model.add(Dense(30, activation='relu'))
    model.add(Dense(30, activation='relu'))
    model.add(Dense(3, activation='softmax'))
    model.compile(loss='categorical_crossentropy', optimizer='adagrad', metrics=['accuracy'])
    return model


def fit_linear_model(X_train, y_train, dimension, iteration, loadfile=None, savefile=None):
    if iteration == 0:
        return load_model(loadfile)
    if loadfile is None:
        model = build_linear_model(dimension)
    else:
        model = load_model(loadfile)
    model.fit(X_train, y_train, nb_epoch=iteration, verbose=1, batch_size=64)
    if savefile is not None:
        model.save(savefile)
    return model


def main():
    train_x, train_y = read_data('17feats_remix_training_data.csv')
    test_x, test_y = read_data('17feats_remix_training_data.csv')
    train_y = to_categorical(train_y, 3)
    model = fit_linear_model(X_train=train_x,
                             y_train=train_y,
                             dimension=17,
                             iteration=3500,
                             loadfile=None,
                             savefile='17feats_remix_model.h5')
    y_out = model.predict(test_x)
    prediction_labels = np.argmax(y_out, axis=1)
    #print(prediction_labels)


if __name__ == "__main__":
    main()
