#!/usr/bin/env python2
"""
NOTE: This file is included for test purposes only. You should not need it to
capture any flags.
"""
from Crypto.Cipher import AES

from secrets import *

if __name__=='__main__':
    cipher = AES.new(FIXED_KEY, AES.MODE_ECB)

    c1 = cipher.decrypt(SERIAL_FLAG).encode('hex')
    c2 = cipher.decrypt(DEBUG_FLAG).encode('hex')
    print 'Here is your serial ciphertext: %s' % c1
    print 'Here is your debug ciphertext: %s' % c2
    print '%s %s' % (cipher.encrypt(c1.decode('hex')), cipher.encrypt(c2.decode('hex')))
