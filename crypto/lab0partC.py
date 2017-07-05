#Lab0
#Authors: Daniel Carlson & Justin Distaso 
#CSC 323
#Delivery Date: 1/20/17

import binascii as hexer
import string as mod
import itertools
import base64


def HtoA(h):
    chars_in_reverse = []
    while h != 0x0:
        chars_in_reverse.append(chr(h & 0xFF))
        h = h >> 8

    chars_in_reverse.reverse()
    return ''.join(chars_in_reverse)

def hexToAscii(hex):
   return hexer.unhexlify(hex.encode('utf-8'))

def AsciiToHex(Ascii):
    return hexer.hexlify(Ascii)

def b64toAscii(b64):
    return b64.decode("base64").encode("hex")

def xor_two_str(a,b):
    xored = []
    for i in range(max(len(a), len(b))):
        xored_value = ord(a[i%len(a)]) ^ ord(b[i%len(b)])
        xored.append(hex(xored_value)[2:])
    return ''.join(xored)

#XORs a string with a key (after making them the same length)
def myXOR(string1, key):
    if (len(key) >= len(string1)):
        key = key[0:len(string1)]
    else:
    	while (len(string1) > len(key)):
        	diff = len(string1) - len(key) % (len(key) + 1)
        	key = key + key[:diff]
    
    #list = [ord(a) ^ ord(b) for a,b in zip(string1, key)]
    #joined = int(''.join(map(str, list)))
    #return joined

    #return (xor_two_str(key, string1))
    a = int(key, base=16)
    b = int(string1, base=16)
    return hex(a ^ b)


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
    else:
        minus1 = total - 1
        denom = total*minus1
        return float(s) / denom

#XOR's a line of HEX characters with a single byte, k
def singleByteXOR(line, k):
    printable = True
    theString = ''
    for hexchar in range(0,len(line)/2):
        readchars = line[2*hexchar: (2 * hexchar) + 2]
        number = int(readchars, 16)
        xord = number ^ k
        if ((xord >= 10) and (xord <= 127)):
            theString += chr(xord)
        else:
            printable = False
    if (printable == True):
        return theString
    else:
        theString = ''
    return theString

#encodes a message (text) with a key of any length. returns the ciphertext
def vigenereEncode(text, key):
    keyLen = len(key)
    iterations = len(text) / keyLen
    leftover = len(text) % keyLen
    shifts = [0] * keyLen
    newString = ''
    #this for loop sets the shift values of the key
    i = 0
    for c in key:
        shifts[i] = ord(c) - 65
        i = i + 1
    for x in range(0, iterations):
        p = 0
        for cc in text[(x*keyLen):((x*keyLen)+keyLen)]:
            tempVal = ord(cc) + shifts[p]
            if (tempVal > 90):
                tempVal = tempVal - 26
            newString += chr(tempVal)
            p = p + 1
    if (leftover != 0):
        d = 0
        num1 = iterations * keyLen
        num2 = num1 + leftover
        for tempC in text[num1:num2]:
            tempV = ord(tempC) + shifts[d]
            if (tempV > 90):
                tempV = tempV - 26
            newString += chr(tempV)
            d = d + 1
    return newString

#tests the ciphertext of a vigenere cipher with a particular key
def vigenereDecode(text, key):
    keyLen = len(key)
    iterations = len(text) / keyLen
    leftover = len(text) % keyLen
    shifts = [0] * keyLen
    newString = ''
    #the first for loop sets the shift values of the key
    i = 0
    for c in key:
        shifts[i] = ord(c) - 65
        i = i + 1
    for x in range(0, iterations):
        p = 0
        for cc in text[(x*keyLen):((x*keyLen)+keyLen)]:
            tempVal = ord(cc) - shifts[p]
            if (tempVal < 65):
                tempVal = tempVal + 26
            newString += chr(tempVal)
            p = p + 1
    if (leftover != 0):
        d = 0
        num1 = iterations * keyLen
        num2 = num1 + leftover
        for tempC in text[num1:num2]:
            tempV = ord(tempC) - shifts[d]
            if (tempV < 65):
                tempV = tempV + 26
            newString += chr(tempV)
            d = d + 1
    return newString


#The following 4 functions are used for frequency analysis and were taken from https://inventwithpython.com/hacking/chapter20.html
#--------------------------------------------------------------------

englishLetterFreq = {'E': 12.70, 'T': 9.06, 'A': 8.17, 'O': 7.51, 'I': 6.97, 'N': 6.75, 'S': 6.33, 'H': 6.09, 'R': 5.99, 'D': 4.25, 'L': 4.03, 'C': 2.78, 'U': 2.76, 'M': 2.41, 'W': 2.36, 'F': 2.23, 'G': 2.02, 'Y': 1.97, 'P': 1.93, 'B': 1.29, 'V': 0.98, 'K': 0.77, 'J': 0.15, 'X': 0.15, 'Q': 0.10, 'Z': 0.07}
ETAOIN = 'ETAOINSHRDLCUMWFGYPBVKJXQZ'
LETTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
 
def getLetterCount(message):
      # Returns a dictionary with keys of single letters and values of the
      # count of how many times they appear in the message parameter.
    letterCount = {'A': 0, 'B': 0, 'C': 0, 'D': 0, 'E': 0, 'F': 0, 'G': 0, 'H': 0, 'I': 0, 'J': 0, 'K': 0, 'L': 0, 'M': 0, 'N': 0, 'O': 0, 'P': 0, 'Q': 0, 'R': 0, 'S': 0, 'T': 0, 'U': 0, 'V': 0, 'W': 0, 'X': 0, 'Y': 0, 'Z': 0}
    for letter in message.upper():
        if letter in LETTERS:
            letterCount[letter] += 1
    return letterCount
 
def getItemAtIndexZero(x):
    return x[0]
 
def getFrequencyOrder(message):
    # Returns a string of the alphabet letters arranged in order of most
    # frequently occurring in the message parameter.

    # first, get a dictionary of each letter and its frequency count
    letterToFreq = getLetterCount(message)
 
     # second, make a dictionary of each frequency count to each letter(s)
     # with that frequency
    freqToLetter = {}
    for letter in LETTERS:
        if letterToFreq[letter] not in freqToLetter:
            freqToLetter[letterToFreq[letter]] = [letter]
        else:
            freqToLetter[letterToFreq[letter]].append(letter)
 
    # third, put each list of letters in reverse "ETAOIN" order, and then
    # convert it to a string
    for freq in freqToLetter:
        freqToLetter[freq].sort(key=ETAOIN.find, reverse=True)
        freqToLetter[freq] = ''.join(freqToLetter[freq])
 
    # fourth, convert the freqToLetter dictionary to a list of tuple
    # pairs (key, value), then sort them
    freqPairs = list(freqToLetter.items())
    freqPairs.sort(key=getItemAtIndexZero, reverse=True)

    # fifth, now that the letters are ordered by frequency, extract all
    # the letters for the final string
    freqOrder = []
    for freqPair in freqPairs:
        freqOrder.append(freqPair[1])

    return ''.join(freqOrder)
 
def englishFreqMatchScore(message):
    # Return the number of matches that the string in the message
    # parameter has when its letter frequency is compared to English
    # letter frequency. A "match" is how many of its six most frequent
    # and six least frequent letters is among the six most frequent and
    # six least frequent letters for English.
    freqOrder = getFrequencyOrder(message)
    matchScore = 0
    # Find how many matches for the six most common letters there are.
    for commonLetter in ETAOIN[:6]:
        if commonLetter in freqOrder[:6]:
            matchScore += 1
    # Find how many matches for the six least common letters there are.
    for uncommonLetter in ETAOIN[-6:]:
        if uncommonLetter in freqOrder[-6:]:
            matchScore += 1     
    return matchScore

#--------------------------------------------------------------------

#groups Hex characters into an array of size: keyLen. Each element contains
#all the hex characters that were XOR'ed with the same byte
def Cgroup(text, keyLen):
    num = len(text) / keyLen
    theArr = [''] * keyLen
    for i in range(0, len(text)/2):
        theArr[i % keyLen] += text[(2*i): (2*i)+ 2]
    return theArr

#XOR's hex text with a key of variable length. returns the decoded message in plaintext
def myHexDecode(cText, keyArr):
    kLen = len(keyArr)
    iterations = len(cText) / (kLen*2)
    leftover = len(cText) % (kLen*2)
    message = ''
    for x in range(0, iterations):
        for p in range(0, kLen):
            spot = ((x*2*kLen) + (2*p))
            readchars = cText[spot: (spot+2)]
            number = int(readchars, 16)
            tempVal = number ^ keyArr[p]
            message += chr(tempVal)
    if (leftover != 0):
        num1 = iterations * 2 * kLen
        for d in range(0, (leftover/2)):
            readchars = cText[(num1 + 2*d): (num1 + 2*d + 2)]
            number = int(readchars, 16)
            tempV = number ^ keyArr[d]
            message += chr(tempV)
    return message

#groups plaintext characters into an array of size: keyLen. Each element contains
#all the characters that were shifted by the same value
def group(text, keyLen):
    num = len(text) / keyLen
    theArr = [''] * keyLen
    for x in range(0, keyLen):
        for i in range(0, num):
            theArr[x] += text[(i*keyLen)+x]
    return theArr

#This function tests groups of text by shifting them by every possible value in the alphabet (0-26)
#The result is given a score based on the similarity of the frequency of its letters with the English language
#The highest score will result from the shift that is most likely. This is done for every "spot" in the key,
#so the key is found one letter at a time
def checkGroups(arr):
    length = len(arr)
    possibleKey = ''
    for x in range(0, length):
        highScore = 0
        tempLocation = 0
        for letter in range(0,26):
            result = vigenereDecode(arr[x], chr(letter+65))
            score = englishFreqMatchScore(result)
            if (score > highScore):
                highScore = score 
                tempLocation = letter
        #print highScore
        possibleKey += chr(tempLocation + 65)
    return possibleKey

#Has the same functionality as the above function (checkGroups()), except this is for Hex chars instead of plaintext
#Every byte value is texted from 0 to 255, and the segments of the array are XOR'ed with this byte value to find the 
#key, byte by byte
def checkGroupsHEX(arr):
    length = len(arr)
    possibleKey = [0] * length
    for x in range(0, length):
        highScore = 0
        tempLocation = 0
        for byte in range(0,256):
            result = singleByteXOR(arr[x], byte) 
            if (result == ''):
                score = 0
            else:
                score = englishFreqMatchScore(result)
            if (score > highScore):
                highScore = score 
                tempLocation = byte
        possibleKey[x] = tempLocation
    return possibleKey

#This function opens a file and returns all the text in the file as a string
def getStringfromFile(fName):
    inFile2 = open(fName)
    theString = ''
    for line in inFile2:
        heyyy = line.strip()
        theString += heyyy
    inFile2.close()
    return theString

#This function uses the index of coincidence to find the keylength 
def getKeyLen(fileName):
    file = open(fileName)
    readchars = file.read()
    bestC = 0
    bestL = 0
    for theKeyLength in range (2, 20):
        chars = [''] * 20
        file.seek(0)
        for subset in range (0, len(readchars)):
            chars[subset % theKeyLength] += readchars[subset: subset + 1]
        totalCoinc = 0
        for subset in range (0, theKeyLength):
            coinc = calcCoincidence(chars[subset])
            totalCoinc += coinc
        totalCoinc /= theKeyLength
        if (totalCoinc > bestC):
            bestC = totalCoinc
            bestL = theKeyLength
    file.close()
    return bestL

def partB():
    print("Part B: ")
    print('\n')
    inFile = open('Lab0.TaskII.B.txt')
    for line in inFile:
        for theKey in range(0 , 255):
            stuff = singleByteXOR(line, theKey)
            if (stuff != ''):
                print (stuff)
    inFile.close()
    print('\n')

def partC():
    print("Part C:")
    print('\n')
    fileName = 'Lab0.TaskII.C.txt'
    line = getStringfromFile(fileName)
    keyLen = getKeyLen(fileName)
    myHex = b64toAscii(line)
    groupArr = Cgroup(myHex, keyLen/2)
    possibleKey = checkGroupsHEX(groupArr)
    print("The Key is: ")
    print (possibleKey)
    print('\n')
    plainText = myHexDecode(myHex, possibleKey)
    print("Decrypted Message: ")
    print (plainText)
    print('\n\n')

def partD(bruteForce):
    #opens the appropriate file and gets the ciphertext from it
    fileName = 'Lab0.TaskII.D.txt'
    line = getStringfromFile(fileName)

    betterKeyLength = getKeyLen(fileName)
    #groups the ciphertext into an array of  'keylength' elements
    #each element contains all the letters shifted by the same key
    groupArr = group(line, betterKeyLength)

    #This function tests every element of the array with every letter and 
    #does a frequency analysis on the result to determine the (potentially) correct key letter by letter
    possibleKey = checkGroups(groupArr)
    print("Part D:")
    print('\n')
    print("The Key is: ")
    print (possibleKey)
    print("Decrypted Message: ")
    print (vigenereDecode(line, possibleKey))

    #This next part brute forces the whole thing. change the "repeat" value to the key length, and 
    #the for loop will generate all possible keys of that key length. Anything above 4 will take a 
    #very long time, so don't do that
    if (bruteForce == True):
        for tempKey in itertools.imap(''.join, itertools.product('ABCDEFGHIJKLMNOPQRSTUVWXYZ', repeat=3)):
            result = vigenereDecode(line, tempKey)
            score = englishFreqMatchScore(result)
            if (score > 8):
                print('-----')
                print('Possible string is')
                print(result)
                print('-----')


#MAIN

#Part B takes about 10 seconds to run, so be patient
#partB()

#partC()

partD(False)
