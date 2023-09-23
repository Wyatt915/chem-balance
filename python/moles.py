#!/usr/bin/env python3

import json

with open("elements.json", 'r') as elements:
    ELEMENTS=json.load(elements)

def csv2json():
    element_list = []
    element_dict = {}
    with open("elements.csv", 'r') as csv:
        for line in csv:
            element_list.append(line.strip().split(','))

    for e in element_list:
        element_dict[e[2].strip('"')] = {"Z": int(e[0]), "name": e[1].strip('"'), "mass": float(e[3])}

    print(json.dumps(element_dict))

def count(chem):
    l = len(chem)
    s = 0
    e = 1

    elements = {}

    def num(sub, s, e):
        n = 1 # if no number is specified, we assume 1
        while chem[s:e].isdigit():
            e+=1
        e -=1 # overshoot
        if e > s:
            n = int(chem[s:e])
            s = e
        return n, s, e

    coef, s, e = num(chem, s, e)
    while e < len(chem):
        pass





def molar_mass(chem):
    pass

if __name__ == '__main__':
    counts = {}
    while True:
        s = input("Symbol: ")
        if s == 'q':
            break
        q = input("Qty: ")
        counts[s] = int(q)
    mass = 0
    for e in counts:
        mass += ELEMENTS[e]["mass"] * counts[e]
    print(mass)
    for e in counts:
        print(f"{e}:\t {(ELEMENTS[e]['mass'] * counts[e])/mass}")

