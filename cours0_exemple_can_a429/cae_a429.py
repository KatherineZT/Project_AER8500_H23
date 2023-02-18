from ctypes import CDLL, c_int, c_char, byref
from struct import pack_into, unpack_from
from time import sleep

class a429_channel(object):
  def __init__(self, channel):
    self.channel = channel
    # Retrieve the file descriptor and initialize the front-end driver
    self.a429Lib = CDLL("liba429.so")
    self.fd = c_int()
    self.a429Lib.a429_open("/dev/cae_a4290", byref(self.fd))
  
  def close(self):
    sleep(1)
    self.a429Lib.a429_basic_config_ch(self.fd, self.channel, 0, 1, 0) # CH_FREE=0, CH_FAST=1, CH_RB_OFF=0
    self.a429Lib.a429_close(self.fd)

class a429_channel_output(a429_channel):
  def __init__(self, channel):
    a429_channel.__init__(self, channel)
    self.a429Lib.a429_basic_config_ch(self.fd, self.channel, 2, 1, 0) # CH_OUTPUT=2, CH_FAST=1, CH_RB_OFF=0

  def send(self, word):
    # Create the TX buffer
    txSize = c_int(1) # We send 1 word
    txBuffer = bytearray(txSize.value * 4) # 1 int = 4 bytes
    txPacket = (c_char * len(txBuffer)).from_buffer(txBuffer)

    # Pack the word into the TX buffer
    pack_into("I", txBuffer, 0, word)

    # Send the word
    self.a429Lib.a429_outputw(self.fd, self.channel, txPacket, byref(txSize))

class a429_channel_input(a429_channel):
  def __init__(self, channel):
    a429_channel.__init__(self, channel)
    self.a429Lib.a429_basic_config_ch(self.fd, self.channel, 1, 1, 0) # CH_INPUT=1, CH_FAST=1, CH_RB_OFF=0
    self.a429Lib.a429_rx_setperiod(self.fd, 255)

  def recv(self):
    # Prepare the RX buffer
    txSize = c_int(1) # We send 1 word
    rxSize = c_int(txSize.value * 2) # The A429 driver adds a word with TagTime/ChannelIndex/ParityError so we expect the double of words
    rxBuffer = bytearray(rxSize.value * 4) # 1 integer = 4 bytes
    rxPacket = (c_char * len(rxBuffer)).from_buffer(rxBuffer)
    while True:
      # Receive the words
      rxSize = c_int(txSize.value * 2) # The A429 driver adds a word with TagTime/ChannelIndex/ParityError so we expect the double of words
      self.a429Lib.a429_channel_input(self.fd, self.channel, rxPacket, byref(rxSize))
      if rxSize.value >= 2:
        break

    # Decodage
    data = unpack_from("II", rxBuffer) # We received two integers
    return data[1]
