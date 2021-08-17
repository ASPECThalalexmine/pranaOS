#!/usr/bin/env python
#


from Parser.parser import Parser
from Generator.generator import Generator
from type_file import Type
from token import Token
import argparse

def run(input_f, output_f):

    code = []

    file = open(input_f, "r") 
    for line in file: 
        line = line.replace('\n', '')
        if len(line) > 0:
            code.append(line)

    parser = Parser()
    gen = Generator()
    parser.set_code_lines(code)
    gen.generate(output_f, parser.parse())
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('inf', type=str, help='Input file')
    parser.add_argument('outf', type=str, help='Output file')

    args = parser.parse_args()
    run(args.inf, args.outf)
