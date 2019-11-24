from imports import *
import data


class Attention(tf.keras.layers.Layer):
    def __init__(self, units):
        super(Attention, self).__init__()
        self.dense_input = tf.keras.layers.Dense(units)
        self.dense_hidden = tf.keras.layers.Dense(units)
        self.dence_out = tf.keras.layers.Dense(1)

    def call(self, query, values):
        hidden_with_time_axis = tf.expand_dims(query, 1)
        score = self.dence_out(tf.nn.tanh(self.dense_input(values) + self.dense_hidden(hidden_with_time_axis)))
        attention_weights = tf.nn.softmax(score, axis=1)
        context_vector = tf.reduce_sum(attention_weights * values, axis=1)
        return context_vector, attention_weights


class Decoder(tf.keras.Model):
    def __init__(self, vocab_size, embedding_dim, dec_units, batch_size):
        super(Decoder, self).__init__()
        self.batch_size = batch_size
        self.dec_units = dec_units
        self.embedding = tf.keras.layers.Embedding(vocab_size, embedding_dim)
        self.gru = tf.keras.layers.GRU(self.dec_units, return_sequences=True, return_state=True)
        self.dence_out = tf.keras.layers.Dense(vocab_size)
        self.attention = Attention(self.dec_units)

    def call(self, input, hidden, enc_output):
        context_vector, attention_weights = self.attention(hidden, enc_output)
        input = self.embedding(input)
        input = tf.concat([tf.expand_dims(context_vector, 1), input], axis=-1)
        output, state = self.gru(input)
        output = tf.reshape(output, (-1, output.shape[2]))
        input = self.dence_out(output)
        return input, state, attention_weights
