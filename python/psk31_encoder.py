
import numpy
import pmt
import sys
import digimodes
import struct

from gnuradio import gr, blocks

class psk31_encoder(gr.sync_block):
    def __init__(self):
        gr.sync_block.__init__(
            self,
            name = "psk31_encoder",
            in_sig = None,
            out_sig = [numpy.uint8],
        )

        self.message_port_register_in(pmt.intern('in'))
        self.set_msg_handler(pmt.intern('in'), self.handle_msg)
        self.tx_string = ""


    def encode_string(self, string):
        s = "00000000"
        for c in string:
            s += digimodes.varicodes[c]
            s += "00"
        #s += "111111111111111111111111111111111111111"

        while len(s) % 8 != 0:
            s += "1"

        self.tx_string += s

    def handle_msg(self, msg):
        if(pmt.is_pair(msg)):
             blob = pmt.cdr(msg)
             s = ""
             for i in pmt.u8vector_elements(blob):
                 s += chr(i)
             print "encoder sent:" + s
             self.encode_string(s) #"\n\n\n"


    def work(self, input_items, output_items):
        items = 0
        
        #if(len(self.tx_string) == 0):
            #return 0

        for i in range(len(output_items[0])):
            if(len(self.tx_string) == 0):
                break
            u8 = ~int(self.tx_string[0:8], 2) % 2 **8
            output_items[0][i] = u8
            self.tx_string = self.tx_string[8:]
            items += 1
            print "remaining string: " + self.tx_string
        return items

