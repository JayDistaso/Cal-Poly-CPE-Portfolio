import time
import random
import requests
import MT19937
import base64

twister = MT19937.MT19937(0)


def TaskI():
  oracle()

def TaskII():
  #Register a User so we can get tokens
  rObject= requests.post('http://localhost:3030/register', data = {'user' : 'j', 'password': 'q'})

  #Variable that represents internal state of twister
  internalState = [0] * 624
  for i in range(78): #should be 78
    #Generate token
    rObject= requests.post('http://localhost:3030/forgot', data = {'user' : 'j'})
    #Parse this token from http
    token = parseToken(rObject.text)
    #Turn Base64 token output into uint32 that comes from twister
    twisterOutput = tokenToInt(token)
    #Reverse twister operation
    for z in range(8): #should be 8
      internalState[8*i+z] = myunTwist(twisterOutput[z])
      #print internalState[8*i+z]

  #Done recreating initial state, now to finish recreating twister
  twister.mt = internalState


  #We can now get a token from our twister once more and it will match servers twister
  adminToken = generateToken()

  #Request admin token
  rObject= requests.post('http://localhost:3030/forgot', data = {'user' : 'admin'})
  #Go to generated link
  url = 'http://localhost:3030/reset?token=' + adminToken
  rObject = requests.post(url, data = {'user' : 'admin', 'password' : 'CRACKED'})
  #The admin's password will now be set to "Cracked"


def oracle():
   random.seed()
   #Actual implmentation, for debugging lets use 1-3 seconds
   print("First time wait (5-60 seconds)")
   time.sleep(random.randrange(5, 60))
   print("Seeding")
   #time.sleep(random.randrange(1, 3))
   initInput = int(time.time())
   twister = MT19937.MT19937(initInput)
   print("Second wait (5-60 seconds)")
   time.sleep(random.randrange(5, 60))
   #time.sleep(random.randrange(1, 3))
   realOutput = twister.extract_number()
   print("The output was ", realOutput, "with a key of ", initInput)
   print("Bruteforcing for key")
   guessTime = int(time.time())
   testOutput = 0
   while testOutput != realOutput:
      twister = MT19937.MT19937(guessTime)
      testOutput = twister.extract_number()
      if testOutput == realOutput:
         print("The seed was ", guessTime)
      guessTime -= 1
   print("Done Bruteforcing")

#This function turns a BAse64 token to an uint32
def tokenToInt(token):
  myNums = token.decode("base64").encode("utf8")
  print myNums
  arr = [''] * 8
  d = 0
  for c in myNums:
    if (c == ':'):
      arr[d] = int(arr[d])
      d += 1
    else:
      arr[d] += c
  arr[d] = int(arr[d])
  return arr

#This function turns a uint32 to a Base64 token
def generateToken():
  #Generate a 256-bit random number as our reset tokwn
  #by concatentating 8, 32-bit integers with colons
  token = str(twister.extract_number())
  for i in range(7):
    token += ":" + str(twister.extract_number())
  return base64.b64encode(token)


def parseToken(text):
  #Start of token is [623]
  token = ''
  index = 623
  char = ''
  char = text[index]
  while char != '<':
    token += char
    index += 1
    char = text[index]
  #print("The token was parsed to: ")
  return token


def myTwist(x):
  y = int(0xFFFFFFFF & x)
  # Right shift by 11 bits
  y = y ^ y >> 11
  # Shift y left by 7 and take the bitwise and of 2636928640
  y = y ^ y << 7 & 2636928640
  # Shift y left by 15 and take the bitwise and of y and 4022730752
  y = y ^ y << 15 & 4022730752
  # Right shift by 18 bits
  y = y ^ y >> 18
  return y


#http://krypt05.blogspot.com/2015/10/reversing-shift-xor-operation.html 
#was invaluable resource for helping us wrap our heads around the xor magic
def myunTwist(x):
  #print("UNTWISTING")
  y = int(0xFFFFFFFF & x)
  #print("starting with number y =", y)
  y = undoShiftRightXor(y, 18)
  #print("After 18shift y=", y)
  y = reverse_left_shiftAND(y, 15, 4022730752) 
  #print("After 15shift y=", y)
  y = reverse_left_shiftAND(y, 7, 2636928640) 
  #print("after 7 shift y =", y)
  #print("Changing value to correct")
  #y = 1026699
  y = reverse_right_shift(y, 11)
  #print("After 11 shift y = ", y)
  return y

def reverse_left_shiftAND(value, shift, magicNum):
    output, i = 0, 0
    while i * shift < 32:
        compartment = int(bin((0xffffffff >> (32- shift)) << (shift * i))[-32:], 2)
        part_output = value & compartment
        value ^= (part_output << shift) & magicNum
        output |= part_output
        i += 1
    return output

def reverse_right_shift(value, shift, mult=0xffffffff):
    output, i = 0, 0
    while i * shift < 32:
        compartment = int(bin(0xffffffff << (32 - shift))[-32:], 2) >> (shift * i)
        part_output = value & compartment
        value ^= part_output >> shift & mult
        output |= part_output
        i += 1
    return output

def undoShiftRightXor(value, shift):
  #What bit we have worked magic on
  i = 0
  #Return value will add up operations
  result = 0
  #Go through all 32 bits
  while i * shift < 32:
    #Create a mask shift wide, since only some bits are recoverable at a time
    partMask = (-1 << (32 - shift)) >> (shift * i)
    #Get recoverable bits
    part = value & partMask
    #Undo the xor from the next part of the integer
    value ^= part >> shift
    #Add the correct bits to return value
    result |= part
    i += 1
  return result



#main

TaskII()
#oracle()
