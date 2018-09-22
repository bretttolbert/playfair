'''
Allows scoring of text using n-gram probabilities
Adapted from http://practicalcryptography.com/cryptanalysis/text-characterisation/quadgrams/
'''
from math import log10
import string

class ngram_score(object):
    def __init__(self,ngramfile,sep=' '):
        ''' load a file containing ngrams and counts, calculate log probabilities '''
        self.ngrams = {}
        for line in file(ngramfile):
            key,count = line.split(sep) 
            self.ngrams[key] = int(count)
        self.L = len(key)
        self.N = sum(self.ngrams.itervalues())
        #calculate log probabilities
        for key in self.ngrams.keys():
            self.ngrams[key] = log10(float(self.ngrams[key])/self.N)
        self.floor = log10(0.01/self.N)

    def score(self,text):
        ''' compute the score of text '''
        score = 0
        ngrams = self.ngrams.__getitem__
        for i in xrange(len(text)-self.L+1):
            if text[i:i+self.L] in self.ngrams: score += ngrams(text[i:i+self.L])
            else: score += self.floor          
        return score
       
def get_qgram_stats_array_idx(qgram):
    ic = [ord(c)-ord('A') for c in qgram]
    return 17576*ic[0] + 676*ic[1] + 26*ic[2] + ic[3]

"""
To sanity check, index of quadgram 'TION' (very common in English) should be one of the lowest values
>>> get_qgram_stats_array_idx('TION')
339729
So line number 339731 should be a high value, e.g. -2.85387119802
while the surrounding values are lower, e.g. -7.02886084843
"""

def gen_c_array(lang):
    with open('qgram_data_{}.h'.format(lang), 'w') as f:
        f.write('float qgram_stats_array_{}[] = {{\n'.format(lang))
        n = ngram_score('quadgrams_{}.txt'.format(lang))
        for l1 in string.ascii_uppercase:
            for l2 in string.ascii_uppercase:
                for l3 in string.ascii_uppercase:
                    for l4 in string.ascii_uppercase:
                        quadgram = l1 + l2 + l3 + l4
                        score = 0.0
                        if quadgram in n.ngrams:
                            score = n.ngrams[quadgram]
                        else:
                            score = n.floor
                        f.write('{},\n'.format(score))
        f.write('};\n')


if __name__ == '__main__':
    langs = ['en', 'fr']
    for lang in langs:
        gen_c_array(lang)
