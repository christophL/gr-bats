
import numpy
import pmt
import sys
import digimodes

from gnuradio import gr, blocks

mystring = "ich kaufe ein A und loese: gicht\n"

class psk31_encoder(gr.sync_block):
    def __init__(self):
        gr.sync_block.__init__(
            self,
            name = "psk31_encoder",
            in_sig = None,
            out_sig = [numpy.uint8],
        )

        self.tx_string = "00000000"
        for c in mystring:
            self.tx_string += digimodes.varicodes[c]
            self.tx_string += "00"
        self.tx_string += "11111111"

        while len(self.tx_string) % 8 != 0:
            self.tx_string += "1"

        self.index = 0


    def work(self, input_items, output_items):

        index = self.index

        for i in range(len(output_items[0])):
            u8 = ~int(self.tx_string[8*index:8*(index+1)], 2) % 2 **8
            index = (index + 1) % (len(self.tx_string) / 8)
            output_items[0][i] = u8

        self.index = index
        return len(output_items[0])

