
import numpy
import pmt
import sys
import digimodes

from gnuradio import gr, blocks

class psk31_decoder(gr.sync_block):
    def __init__(self):
        gr.sync_block.__init__(
            self,
            name = "psk31_decoder",
            in_sig = [numpy.uint8],
            out_sig = None,
        )
        self.nz = 0
        self.curr = ""
        self.rev_varicodes = dict()
        for k in digimodes.varicodes.keys():
            self.rev_varicodes[digimodes.varicodes[k]] = k

    def work(self, input_items, output_items):

        for i in input_items[0]:

            #print i
            if i == 0:
                i = 1
            else:
                i = 0

            if i == 0:
                self.nz += 1

                if self.nz == 2:
                    if self.rev_varicodes.has_key(self.curr):
                        sys.stdout.write(self.rev_varicodes[self.curr])
                    else:
                        pass
                    self.nz = 0
                    self.curr = ""

            elif i == 1:
                if self.nz == 1 and self.curr != "":
                    self.curr += "0"
                self.curr += "1"
                self.nz = 0

        sys.stdout.flush()
        return len(input_items[0])

