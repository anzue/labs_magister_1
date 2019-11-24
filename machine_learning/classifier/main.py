# dataset - https://www.kaggle.com/uciml/mushroom-classification/download
# mushrooms

from keras import Sequential
from keras.layers import Dense

import csv
import numpy as np
from numpy import argmax

from sklearn.model_selection import train_test_split, cross_val_score
from tensorflow import keras, metrics
import tensorflow as tf
from keras import metrics


def create_model(input_shape, output_shape):
    classifier = Sequential()
    classifier.add(Dense(input_shape , activation='relu', kernel_initializer='random_normal'))
    classifier.add(Dense(30 , activation='relu', kernel_initializer='random_normal'))
    classifier.add(Dense(1, activation='sigmoid', kernel_initializer='random_normal'))
    classifier.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
    return classifier


def read_transform_data(path: str):
    items = []
    headers = []
    with open(path, newline='') as csvfile:
        i = 0
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in spamreader:
            if (i > 0):
                # print(row,len(row))
                row = [ord(x[0]) for x in row]
                items.append(np.asarray(row))
            else:
                headers.append(row)

            # print(', '.join(row))
            i += 1
    items = np.asarray(items)
    converted = []
    for i in range(items.shape[0]):
        converted.append([])

    for i in range(items.shape[1]):
        unique = np.unique(items[:, i])
        for j in range(items.shape[0]):
            for g in range(unique.shape[0]):
                converted[j].append(1 if (unique[g] == items[j][i]) else 0)
    converted = np.asarray(converted)

    x_items = converted[:, 2:]
    y_items = converted[:, :2]

    y_conv = np.ndarray(shape=(y_items.shape[0], 1), dtype=np.float32)
    for i in range(y_items.shape[0]):
        y_conv[i] = np.where(y_items[i] == 1)[0]

    y_items = np.asarray(y_conv)

    x_train, x_test, y_train, y_test = train_test_split(x_items, y_items, test_size=0.33, random_state=42)
    return x_train, x_test, y_train, y_test


if __name__ == "__main__":

    
    x_train, x_test, y_train, y_test = read_transform_data("mushrooms.csv")

    model = create_model(x_train.shape[0], 1)
    # model.compile(loss='mean_squared_error',
    #               optimizer='sgd',
    #               metrics=[metrics.binary_accuracy])
    model.fit(x_train, y_train, epochs=1)
    print(model.summary())

    y_pred_train = model.predict(x_train)

    # acc, acc_op = tf.metrics.accuracy(labels=np.argmax(y_train, 1),
    #                                   predictions=np.argmax(y_pred_train, 1))


    print("train classification = ", model.evaluate(x_train, y_train))
    print("test  classification = ", model.evaluate(x_test, y_test))

    print(model.predict(x_train[:1]), y_train[0])

    #print(list(zip(model.predict(x_test),y_test)))




