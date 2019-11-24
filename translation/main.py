from imports import *
import data
from model import Model


def prepeare_model(path, max_size):
    model = Model(path, max_size)
    return model


if __name__ == '__main__':
    max_size = 50000
    model = prepeare_model("data/eng-fra.txt", max_size)
    model.train(100)
    res, _ = model.test(u'Habe ich recht?')
    print(res)
