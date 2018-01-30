#!/usr/bin/env python2
"""
NOTE: This file is included for test purposes only. You should not need it to
capture any flags.
"""
import os
from Crypto.Cipher import AES
from SocketServer import TCPServer, ForkingMixIn, BaseRequestHandler

from secrets import *

class FlagRequestHandler(BaseRequestHandler):
    cipher = AES.new(FIXED_KEY, AES.MODE_ECB)

    def handle(self):
        token = os.urandom(16)
        challenge = FlagRequestHandler.cipher.decrypt(token)
        self.request.sendall(challenge.encode('hex'))
        self.request.settimeout(1.0)
        response_swapped = self.request.recv(32)
        print(response_swapped)
        response_swapped = response_swapped.strip().decode('hex')
        response = ''.join([response_swapped[i+1]+response_swapped[i] for i in range(0, len(response_swapped), 2)])
        if response == token:
            self.request.sendall(SERVER_FLAG+' '*(64-len(SERVER_FLAG)))
        else:
            err = "Invalid Response"
            self.request.sendall(err+'!'*(64-len(err)))

class FlagServer(TCPServer, ForkingMixIn):
    pass

if __name__ == '__main__':
    flag_server = FlagServer(('0.0.0.0', 1337), FlagRequestHandler)
    flag_server.timeout = 0.5
    flag_server.serve_forever()
