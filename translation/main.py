from imports import *
import data
from model import Model


def prepeare_model(path, max_size):
    model = Model(path, max_size)
    return model


if __name__ == '__main__':
    max_size = 80000
    model = prepeare_model("data/eng-deu.txt", max_size)
    #print(data.vocab_inp_size)
   # model.load()
    model.train(200)
    res, _ = model.test(u'Habe ich recht?')
    print(res)
