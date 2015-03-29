#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import glob
import datetime
import argparse

class Compiler:
    def __init__(self, input_folder, output_folder):
        if os.path.exists(input_folder) and os.path.isdir(input_folder):
            tmp = os.path.join('/tmp', 'pocketvox.txt')
            self.writeFormatedTemporaryFiles(input_folder, tmp)
        
            if not os.path.exists(output_folder) or not os.path.isdir(output_folder):
                os.mkdir(output_folder)
            
            self.buildLanguageModel(tmp, output_folder)
        else:
            print 'Input folder not valid !'
    
    def writeFormatedTemporaryFiles(self, folder, tmpfile):
        files = glob.glob(os.path.join(folder, '*.dic'))
        if os.path.exists(tmpfile):
            os.remove(tmpfile);

        out = open(tmpfile, 'w+')
            
        for f in files:
            lines = [line.split('=')[0] for line in open(f)]
                
            for line in lines:
                out.write('<s>' + line + '</s>\n')

    def buildLanguageModel(self, tmpfile, output_folder):
        now = datetime.datetime.now()
        base = 'pocketvox_' + str(now.year) + str(now.month) + str(now.day) + str(now.hour)
        vocab = base + '.vocab'
        idngram = base + '.idngram'
        arpa = base + '.arpa'
        lm = base + '.lm.dmp'
        
        vocab = os.path.join(output_folder, vocab)
        idngram = os.path.join(output_folder, idngram)
        arpa = os.path.join(output_folder, arpa)
        lm = os.path.join(output_folder, lm)
        
        os.system('text2wfreq < ' + tmpfile + ' | wfreq2vocab > ' + vocab)
        os.system('text2idngram -vocab ' + vocab + ' -idngram ' + idngram  + ' < ' + tmpfile)
        os.system('idngram2lm -vocab_type 0 -idngram ' + idngram + ' -vocab ' + vocab + ' -arpa ' + arpa)
        os.system('sphinx_lm_convert -i ' + arpa + ' -o ' + lm)

        os.remove(vocab)
        os.remove(idngram)
        os.remove(arpa)

        print 'Language model has been written in ' + lm

if __name__ == '__main__':
    #parse args
    parser = argparse.ArgumentParser()
    parser.add_argument('-input', action='store', help='Give the input folder')
    parser.add_argument('-output', action='store', help='Give the output folder')
    args = parser.parse_args()
    
    #use the compiler
    print args.input
    print args.output
    compiler = Compiler(args.input, args.output)
