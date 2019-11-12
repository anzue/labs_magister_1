import data

from data import load_data, preprocess_sentence
from decoder import Decoder
from encoder import Encoder
from imports import *


class Model:
    def __init__(self, dataset_path, max_size):

        self.lang_code = dataset_path.split('/')[-1].split('.')[0]

        self.dataset, self.dataset_validation, self.inp_tokenizer, self.out_tokenizer = load_data(dataset_path,
                                                                                                  max_size)

        self.encoder = Encoder(data.vocab_inp_size, data.embedding_dim, data.units, data.batch_size)
        self.decoder = Decoder(data.vocab_tar_size, data.embedding_dim, data.units, data.batch_size)
        self.optimizer = tf.keras.optimizers.Adam()

        self.checkpoint_dir = './training_checkpoints_' + self.lang_code
        self.checkpoint_prefix = os.path.join(self.checkpoint_dir, "ckpt")
        checkpoint = tf.train.Checkpoint(optimizer=self.optimizer,
                                         encoder=self.encoder,
                                         decoder=self.decoder)
        self.checkpoint = checkpoint

    def load(self):
        self.checkpoint.restore(tf.train.latest_checkpoint(self.checkpoint_dir))

    def train(self, epochs=1):

        if self.dataset is None:
            print("call load dataset first")
            return -1

        encoder = self.encoder
        decoder = self.decoder
        optimizer = self.optimizer

        targ_lang = self.inp_tokenizer

        def loss_function(real, pred):
            loss_object = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True, reduction='none')
            mask = tf.math.logical_not(tf.math.equal(real, 0))
            loss_ = loss_object(real, pred)
            mask = tf.cast(mask, dtype=loss_.dtype)
            loss_ *= mask
            return tf.reduce_mean(loss_)

        @tf.function
        def train_step(inp, targ, enc_hidden):
            loss = 0
            with tf.GradientTape() as tape:
                enc_output, enc_hidden = encoder(inp, enc_hidden)
                dec_hidden = enc_hidden
                dec_input = tf.expand_dims([targ_lang.word_index[data.BEG_TOKEN]] * data.batch_size, 1)
                for t in range(1, targ.shape[1]):
                    predictions, dec_hidden, _ = decoder(dec_input, dec_hidden, enc_output)
                    loss += loss_function(targ[:, t], predictions)
                    dec_input = tf.expand_dims(targ[:, t], 1)

            batch_loss = (loss / int(targ.shape[1]))
            variables = encoder.trainable_variables + decoder.trainable_variables
            gradients = tape.gradient(loss, variables)
            optimizer.apply_gradients(zip(gradients, variables))
            return batch_loss

        for epoch in range(epochs):
            run_time = time.time()
            enc_hidden = encoder.initialize_hidden_state()
            total_loss = 0
            for (batch, (inp, targ)) in enumerate(self.dataset.take(data.steps_per_epoch)):
                loss = train_step(inp, targ, enc_hidden)
                total_loss += loss
            print('for epoch {}  loss is {:.3f} time = {:.3f}'.format(epoch, total_loss.numpy() / data.steps_per_epoch,
                                                                      time.time() - run_time))

            if epoch % 50 == 0:
                self.checkpoint.save(file_prefix=self.checkpoint_prefix + "_" + epoch)

            if epoch % 10 == 0:
                validation_loss = 0
                for (batch, (inp, targ)) in enumerate(self.dataset_validation.take(data.steps_per_epoch)):
                    loss = train_step(inp, targ, enc_hidden)
                    validation_loss += loss

                print("for epoch {} validation loss is {:.3f}".format(epoch,
                                                                      validation_loss.numpy() / data.steps_per_epoch))

        self.checkpoint.save(file_prefix=self.checkpoint_prefix)
        print('train finished')

    def test(self, sentence):
        print("Translating - ", sentence)
        inp_lang = self.inp_tokenizer
        targ_lang = self.out_tokenizer
        sentence = preprocess_sentence(sentence)
        sentence_cleared = " ".join(i for i in sentence.split(' ') if i in inp_lang.word_index)
        inputs = [inp_lang.word_index[i] for i in sentence_cleared.split(' ')]
        # print(inputs)
        inputs = tf.keras.preprocessing.sequence.pad_sequences([inputs],
                                                               maxlen=data.max_length_inp, padding='post')
        inputs = tf.convert_to_tensor(inputs)
        result = ''
        hidden = [tf.zeros((1, data.units))]
        enc_out, enc_hidden = self.encoder(inputs, hidden)

        dec_hidden = enc_hidden
        dec_input = tf.expand_dims([targ_lang.word_index[data.BEG_TOKEN]], 0)

        for t in range(data.max_length_targ):
            predictions, dec_hidden, attention_weights = \
                self.decoder(dec_input, dec_hidden, enc_out)

            predicted_id = tf.argmax(predictions[0]).numpy()
            result += targ_lang.index_word[predicted_id] + ' '

            if targ_lang.index_word[predicted_id] == data.END_TOKEN:
                print("Translated  - ", result[:-5])
                return result[:-5], sentence

            dec_input = tf.expand_dims([predicted_id], 0)

        print("Translated  - ", result)

        return result, sentence
