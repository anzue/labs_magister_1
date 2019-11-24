from imports import *

# coded lang model
max_length_out = 0
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
        # TODO is this works ?
        lines = io.open(path, encoding='UTF-8').read().strip().split('\n')[:num_examples]
        word_pairs = [[preprocess_sentence(w) for w in l.split('\t')[:2]] for l in lines]
        return zip(*word_pairs)

    def tokenize(lang):
        lang_tokenizer = tf.keras.preprocessing.text.Tokenizer(filters='')
        lang_tokenizer.fit_on_texts(lang)
        tensor = lang_tokenizer.texts_to_sequences(lang)
        tensor = tf.keras.preprocessing.sequence.pad_sequences(tensor, padding='post')
        return tensor, lang_tokenizer

    def load_dataset(path, num_examples=None):
        out_lang, inp_lang = create_dataset(path, num_examples)
        inp_tensor, inp_lang_tokenizer = tokenize(inp_lang)
        out_tensor, out_lang_tokenizer = tokenize(out_lang)
        return inp_tensor, out_tensor, inp_lang_tokenizer, out_lang_tokenizer

    def tensors_to_tfDataset(inp_tensor, out_tensor):
        dataset = tf.data.Dataset.from_tensor_slices((inp_tensor, out_tensor)).shuffle(buffer_size)
        dataset = dataset.batch(batch_size, drop_remainder=True)
        return dataset

    global buffer_size, batch_size, steps_per_epoch, embedding_dim, units, vocab_inp_size, vocab_tar_size, max_length_out, max_length_inp

    inp_tensor, out_tensor, inp_lang, out_lang = load_dataset(path, max_size)
    max_length_out, max_length_inp = max(len(x) for x in out_tensor), max(len(x) for x in inp_tensor)
    inp_tensor_train, inp_tensor_val, out_tensor_train, out_tensor_val = \
        train_test_split(inp_tensor, out_tensor, test_size=0.1)

    buffer_size = len(inp_tensor_train)
    steps_per_epoch = len(inp_tensor_train) // batch_size
    vocab_inp_size = len(inp_lang.word_index) + 1
    vocab_tar_size = len(out_lang.word_index) + 1

    return (tensors_to_tfDataset(inp_tensor_train, out_tensor_train),
            tensors_to_tfDataset(inp_tensor_val, out_tensor_val),
            inp_lang, out_lang)
