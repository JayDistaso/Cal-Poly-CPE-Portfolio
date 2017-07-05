#!/usr/bin/env python3

#import cryptolibhere

#This module is used to convert between ASCII and hex
import binascii as hexer
import string as mod
import codecs as decoder

def asciiToHex(text):
   return hexer.hexlify(text.encode('utf8'))

def hexToAscii(hex):
   return hexer.unhexlify(hex)

def hexToBase64(hex):
   return hexer.b2a_base64(hex.encode('utf8'))
   
def base64ToHex(text):
   return hexer.a2b_base64(text.decode('utf8'))

#Source
#http://stackoverflow.com/questions/2612720/how-to-do-bitwise-exclusive-or-of-two-strings-in-python
def sxor(s1, s2):    
    # convert strings to a list of character pair tuples
    # go through each tuple, converting them to ASCII code (ord)
    # perform exclusive or on the ASCII code
    # then convert the result back to ASCII (chr)
    # merge the resulting array of characters as a string
    return ''.join(chr(ord(a) ^ ord(b)) for a,b in zip(s1,s2))

def sizeKey(plaintext, key):
   #Key is too short
   while (len(plaintext) > len(key)):
      diff = (len(plaintext) - len(key))
      if (diff > len(key)):
         diff = len(key)
      key = key + key[:diff]
   
   #Key is too long or just right
   key = key[:len(plaintext)]    
   
   return key

def xorTwoStrings(plaintext, key):
   
   key = sizeKey(plaintext, key)
   #Slightly convoluted way to xor two strings found at
   #http://stackoverflow.com/questions/2612720/how-to-do-bitwise-exclusive-or-of-two-strings-in-python
   list = [ord(a) ^ ord(b) for a,b in zip(plaintext, key)]   
   
   #Turn list back into a number (base unknown, prob 10)
   joined = int(''.join(map(str,list)))
   
   return joined

#Following two functions inspired by 
#http://stackoverflow.com/questions/32124176/why-am-i-wrong-with-my-index-of-coincidence-program-in-python-3-4

#Count number of times each letter appears, in array
def appearCounter(text):
   appears = [0] * 26
   #Promote all chars to uppercase for easier comparison
   for c in text.upper():
      #Compare with all letters of alphabet
      if c in mod.ascii_uppercase:
         appears[ord(c) - ord('A')] += 1
   return appears

#English has an expected of 0.065
def calcCoincidence(text):
   #Get number of times each letter appears
   appears = appearCounter(text)
   
   #Calculate using index of coincidence formula
   s = sum (n * (n - 1) for n in appears)
   total = sum(appears)
   #Divide by normalizing coefficent
   if (total == 1 or total == 0):
      return 0
   return s / (total * (total - 1))

#MAIN
NUM_KEYS = 256

file = open('Lab0.TaskII.B.txt')

#Go through each string
for cipher in range (0, 1000):
   if (cipher % 100 == 0):
      print('processed cipher #')
      print(cipher) 
   #XOR against all posisble keys
   line = file.read(218)
   file.read(1)
   for key in range (0, NUM_KEYS - 1):
      #Read string, one byte at a time
      totalString = ''
      for hexchar in range (0, 109):
         readchars = line[2 * hexchar:( 2 * hexchar) + 2]
         number = int(readchars, 16)
         xord = number ^ key
         if (xord > 150):
            break
         totalString += chr(xord)
         
      #If we have a close index of coincidence then record it, hopefully there is only one
      #@@@THIS COULD BE IMPROVED@@@
      #For a sufficently long english message 0.0625 coincidence
      #Print all close strings
      if (totalString != '' and hexchar > 100):
         coinc = calcCoincidence(totalString)
         if ((coinc > 0.0625) and (coinc < 0.066)):
            goodString = totalString
            print('-----')
            print('Possible string is')
            print(goodString)
            print('key for this result is')
            print(key)
            print('Line for this result is')
            print(cipher)
            print('-----')
            print('Score for this line is', coinc)
