#!user/bin/env python
import sys
import matplotlib as plt
from matplotlib import pyplot as pllt

def graph(f):

    list = []
    file_handle = open(f , "r")
    for line in file_handle:
        if('EXIT' in line):
           print(line) 



def main():
    file_in = input()
    graph(file_in)


if __name__ == "__main__":
    main()
