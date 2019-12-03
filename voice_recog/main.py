import os
import sys
import wave
from pprint import pprint

import numpy as np
import pyaudio
import speech_recognition as sr
import sounddevice as sd
from scipy.io.wavfile import write
import soundfile as sf
import matplotlib.pyplot as plt
from pathlib import Path

from recog_nn import recog_google

np.set_printoptions(threshold=sys.maxsize)

fs = 44100  # Sample rate
seconds = 2  # Duration of recording


def record(duration=seconds):
    print("Start")
    myrecording = sd.rec(int(duration * fs), samplerate=fs, channels=2)
    sd.wait()
    print("Stop")
    return myrecording


def read(filename, play=False):
    data, fs = sf.read(filename, dtype='float32')
    if play:
        sd.play(data, fs)
        status = sd.wait()
    return data


def save(record, filename, format=None):
    if format:
        sf.write(filename, record, fs, format)
    else:
        sf.write(filename, record, fs)


def listen_micro():
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print("Say something!")
        audio = r.listen(source)
        print(audio)

    return audio


def rec_to_np_array(record):
    res = []
    for i in range(0, len(record)):
        tmp = np.fromstring(record[i], dtype='>u4')
        res.append(tmp)
    return np.asarray(tmp)


def distance(record1, record2):
    len1 = record1.shape[0]
    len2 = record2.shape[0]
    dp = np.zeros((len1, 2), dtype=np.float32)
    dp += 1e8
    dp[0][0] = np.square(record1[0] - record2[0]).mean(axis=0)
    for i in range(1, len1):
        #  print(record1[i], record2[i])
        dp[i][0] = dp[i - 1][0] + np.square(record1[i] - record2[i]).mean(axis=0)
    # print(dp)
    for i2 in range(1, len2):
        # print(i2, len2)
        for i1 in range(max(0, i2 - 100), min(len1, i2 + 100)):
            dp[i1][i2 % 2] = dp[i1][(i2 + 1) % 2] + np.square(record1[i1] - record2[i2]).mean(axis=0)
            if i1 > 0:
                dp[i1][i2 % 2] = min(
                    dp[i1][i2 % 2],
                    min(dp[i1 - 1][i2 % 2], dp[i1 - 1][(i2 + 1) % 2])
                    + np.square(record1[i1] - record2[i2]).mean(axis=0))
        #    print(dp)
    return dp[len1 - 1][(len2 + 1) % 2]


def fft(y, show=False):
    N = y.shape[0]
    Y_k = np.fft.fft(y)[0:int(N / 2)] / N  # FFT function from numpy
    Y_k[1:] = 2 * Y_k[1:]  # need to take the single-sided spectrum only
    Pxx = np.abs(Y_k)  # be sure to get rid of imaginary part

    f = fs * np.arange((N / 2)) / N  # frequency vector

    # plotting
    if show:
        plt.plot(f, Pxx, linewidth=5)

    #    plt.show()

    return Pxx


def freq_err(freq1, freq2):
    freq1 = np.log(freq1)
    freq2 = np.log(freq2)

    mul = np.linspace(1, freq1.shape[0], freq1.shape[0])
    mul = np.log(freq1.shape[0]) - np.log(mul)
   # print(mul)
    return (mul * np.abs(freq1 - freq2)).mean()  # / np.max(freq1 + freq2))


class Processing:
    @staticmethod
    def load_all_user_data(data_dir="commands/", user_suf=""):

        data_dir_path = Path(data_dir)
        commands_pathes = [x for x in data_dir_path.iterdir() if x.is_dir()]

        user_files = dict()

        for path in commands_pathes:
            for file in path.iterdir():
                if file.is_file():
                    name = file.name
                    for j in range(20):
                        if name == str(j) + user_suf + ".wav":
                            if path.name not in user_files:
                                user_files[path.name] = []
                            print(file)
                            user_files[path.name].append(read(str(file), False))

        return user_files

    @staticmethod
    def init():
        sd.default.samplerate = fs

    @staticmethod
    def clear_record(record):
        pos_st = 0
        pos_end = record.shape[0] - 1
        while np.abs(record[pos_st]).mean() < 0.1:
            pos_st += 1
        while np.abs(record[pos_end]).mean() < 0.1:
            pos_end -= 1

        return record[pos_st:pos_end, :]

    @staticmethod
    def scale(record, max_val=1):
        cur_max = np.max(np.abs(record))
        record *= max_val / cur_max
        return record

    @staticmethod
    def get_rec(i):
        if (i < 10):
            path = "commands/left/{}.wav".format(i + 1)
        else:
            path = "commands/left/{}k.wav".format(i + 1 - 10)
        sound = read(path, play=False)
        return sound

    @staticmethod
    def test_simple(records_list):
        leng = len(records_list)
        res = np.zeros((leng, leng))
        fig, ax = plt.subplots()
        # ax.set_xscale('log')
        # ax.set_yscale('log')
        # plt.ylabel('Amplitude')
        # plt.xlabel('Frequency [Hz]')
        # for i in range(leng):
        #         #     fft(records_list[i][:, 0], True)
        #         # plt.show()

        for i in range(leng):
            for j in range(leng):
                res[i][j] = Processing.eval_by_freq(Processing.get_rec(i)[:, 0], Processing.get_rec(j)[:, 0])

        print(res)
        plt.imshow(res)
        plt.show()

    @staticmethod
    def eval_by_freq(v1, v2):
        freq1 = fft(v1[:, 0], False)
        freq2 = fft(v2[:, 0], False)
        freq1 = freq1.reshape((-1, 1))
        freq2 = freq2.reshape((-1, 1))

        if freq1.shape != freq2.shape:
            print("Freq error,scaling")
            freq2 = freq2[:freq1.shape[0], ...]
            freq1 = freq1[:freq2.shape[0], ...]

        return freq_err(freq1, freq2)

    @staticmethod
    def load_all_data():
        users = {"Anton": "", "Karina": "k"}
        data = dict()
        for user, user_pref in users.items():
            data[user] = Processing.load_all_user_data(user_suf=user_pref)
        return data

    @staticmethod
    def get_relative_error(example, data):
        err = 0
        for val in data:
            err += Processing.eval_by_freq(example, val)
        return err / len(data)

    @staticmethod
    def get_inner_set_error(data):
        if len(data) < 3:
            print("Size too small for correct inner set error")
            return 0
        res = 0
        for x in data:
            res += Processing.get_relative_error(x, data)
        return res / len(data)

    @staticmethod
    def test_on_data(data, record, command=None):
        best_user = None
        best_user_score = 1e9

        for usr in data:
            usr_best = 1e9
            usr_mean = 0
            usr_count = 0
            for command_key in data[usr]:
                if command_key == command or command is None:
                    for i in range(len(data[usr][command_key])):
                        eval = Processing.eval_by_freq(record, data[usr][command_key][i])
                        # print("User {}, command = {}, eval = {}".format(usr, command_key, eval))
                        usr_best = min(usr_best, eval)
                        usr_mean += eval
                        usr_count += 1

                    inner_conf = Processing.get_inner_set_error(data[usr][command_key])
                    usr_conf = Processing.get_relative_error(record, data[usr][command_key])
                    #  print("Incof = ", inner_conf, " usrconf = ", usr_conf)
                    print("Confidence for user {} on dataset with {} samples for command {} is {:3.2f}".format(
                        usr, len(data[usr][command_key]), command_key, min(1., (inner_conf / usr_conf) ** 3)
                    ))
            if usr_count <= 0:
                print("User {} never recorded '{}'".format(usr, command))
            else:
                print("For user {} best score {:3.2f}, mean {:3.2f}".format(usr, usr_best, usr_mean / usr_count))

            if usr_best > best_user_score:
                best_user_score = usr_best
                best_user = usr
        return best_user


if __name__ == "__main__":
    Processing.init()
    data = Processing.load_all_data()

    rec = record(2)

    save(rec, "tmp.wav", "PCM_24")
    r = sr.Recognizer()
    with sr.WavFile("tmp.wav") as source:
        audio = r.record(source)
    recogntion_res = recog_google(audio)

    if not Processing.test_on_data(data, rec, recogntion_res):
        Processing.test_on_data(data, rec)
