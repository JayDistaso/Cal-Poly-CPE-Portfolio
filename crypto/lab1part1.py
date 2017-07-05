import time
import random

#Source: https://en.wikipedia.org/wiki/Mersenne_Twister#Python_implementation
def _int32(x):
    # Get the 32 least significant bits.
    return int(0xFFFFFFFF & x)

class MT19937:

    def __init__(self, seed):
        # Initialize the index to 0
        self.index = 624
        self.mt = [0] * 624
        self.mt[0] = seed  # Initialize the initial state to the seed
        for i in range(1, 624):
            self.mt[i] = _int32(
                1812433253 * (self.mt[i - 1] ^ self.mt[i - 1] >> 30) + i)

    def extract_number(self):
        if self.index >= 624:
            self.twist()

        y = self.mt[self.index]

        # Right shift by 11 bits
        y = y ^ y >> 11
        # Shift y left by 7 and take the bitwise and of 2636928640
        y = y ^ ((y << 7) & 2636928640)
        # Shift y left by 15 and take the bitwise and of y and 4022730752
        y = y ^ ((y << 15) & 4022730752)
        # Right shift by 18 bits
        y = y ^ y >> 18

        self.index = self.index + 1

        return _int32(y)

    def twist(self):
        for i in range(624):
            # Get the most significant bit and add it to the less significant
            # bits of the next number
            y = _int32((self.mt[i] & 0x80000000) +
                       (self.mt[(i + 1) % 624] & 0x7fffffff))
            self.mt[i] = self.mt[(i + 397) % 624] ^ y >> 1

            if y % 2 != 0:
                self.mt[i] = self.mt[i] ^ 0x9908b0df
        self.index = 0
        #test

def oracle():
   random.seed()
   #Actual implmentation, for debugging lets use 1-3 seconds
   print("First time wait (5-60 seconds)")
   time.sleep(random.randrange(5, 60))
   print("Seeding")
   #time.sleep(random.randrange(1, 3))
   initInput = int(time.time())
   twister = MT19937(initInput)
   print("Second wait (5-60 seconds)")
   time.sleep(random.randrange(5, 60))
   #time.sleep(random.randrange(1, 3))
   realOutput = twister.extract_number()
   print("The output was ", realOutput, "with a key of ", initInput)
   print("Bruteforcing for key")
   guessTime = int(time.time())
   testOutput = 0
   while testOutput != realOutput:
      twister = MT19937(guessTime)
      testOutput = twister.extract_number()
      if testOutput == realOutput:
         print("The seed was ", guessTime)
      guessTime -= 1
   print("Done Bruteforcing")

#main
oracle()