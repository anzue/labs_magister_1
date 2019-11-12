import os

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
import tensorflow as tf
import numpy as np
import json
import io
import re
from flask import Flask, jsonify, request
from flask import abort

import time
import unicodedata
from sklearn.model_selection import train_test_split
from tqdm import tqdm
