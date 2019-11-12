

from imports import *

# coded lang model
max_length_targ = 0
max_length_inp = 0
buffer_size = 0
steps_per_epoch = 0
vocab_inp_size = 0
vocab_tar_size = 0
# train
batch_size = 64
embedding_dim = 256
units = 1024
# lang model
BEG_TOKEN = '<s>'
END_TOKEN = '<e>'


def unicode_to_ascii(s):
    return ''.join(c for c in unicodedata.normalize('NFD', s)
                   if unicodedata.category(c) != 'Mn')


def preprocess_sentence(w):
    w = unicode_to_ascii(w.lower().strip())
    w = re.sub(r"([?.!,¿])", r" \1 ", w)
    w = re.sub(r'[" "]+', " ", w)
    w = re.sub(r"[^a-zA-Z?.!,¿]+", " ", w)
    w = w.rstrip().strip()
    w = BEG_TOKEN + ' ' + w + ' ' + END_TOKEN
    return w


def load_data(path, max_size):  # http://www.manythings.org/anki/

    def create_dataset(path, num_examples):
        lines = io.open(path, encoding='UTF-8').read().strip().split('\n')
        word_pairs = [[preprocess_sentence(w) for w in l.split('\t')[:2]] for l in lines[:num_examples]]
        return zip(*word_pairs)

    def max_length(tensor):
        return max(len(t) for t in tensor)

    def tokenize(lang):
        lang_tokenizer = tf.keras.preprocessing.text.Tokenizer(filters='')
        lang_tokenizer.fit_on_texts(lang)
        tensor = lang_tokenizer.texts_to_sequences(lang)
        tensor = tf.keras.preprocessing.sequence.pad_sequences(tensor, padding='post')
        return tensor, lang_tokenizer

    def load_dataset(path, num_examples=None):
        targ_lang, inp_lang = create_dataset(path, num_examples)
        input_tensor, inp_lang_tokenizer = tokenize(inp_lang)
        target_tensor, targ_lang_tokenizer = tokenize(targ_lang)
        return input_tensor, target_tensor, inp_lang_tokenizer, targ_lang_tokenizer

    global buffer_size, batch_size, steps_per_epoch, embedding_dim, units, vocab_inp_size, vocab_tar_size, max_length_targ, max_length_inp

    input_tensor, target_tensor, inp_lang, targ_lang = load_dataset(path, max_size)
    max_length_targ, max_length_inp = max_length(target_tensor), max_length(input_tensor)
    input_tensor_train, input_tensor_val, target_tensor_train, target_tensor_val = \
        train_test_split(input_tensor, target_tensor, test_size=0.1)

    buffer_size = len(input_tensor_train)
    steps_per_epoch = len(input_tensor_train) // batch_size
    vocab_inp_size = len(inp_lang.word_index) + 1
    vocab_tar_size = len(targ_lang.word_index) + 1

    def tensors_to_tfDataset(inp_tensor, out_tensor):
        dataset = tf.data.Dataset.from_tensor_slices((inp_tensor, out_tensor)).shuffle(buffer_size)
        dataset = dataset.batch(batch_size, drop_remainder=True)
        return dataset

    return tensors_to_tfDataset(input_tensor_train, target_tensor_train), tensors_to_tfDataset(input_tensor_val,
                                                                                               target_tensor_val), inp_lang, targ_lang


if __name__ == '__main__':
    load_data("zzz")
