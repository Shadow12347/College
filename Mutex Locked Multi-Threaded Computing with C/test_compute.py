#!/usr/bin/python

from subprocess import call
import sys

#problemName = 'mutex_compute'
problemName = 'pthread_compute'

if len(sys.argv) > 1:
    inputScript = sys.argv[1]
else:
    inputScript = 'integers.dat'

call("make -f Makefile_%s" % problemName, shell=True)
call("./%s < %s > output" % (problemName, inputScript), shell=True)

with open("output") as f:
    answers = f.readlines()
    theMax = float(answers[0].split(":")[1])
    theMin = float(answers[1].split(":")[1])
    theAvg = float(answers[2].split(":")[1])
    if theMax == 100 and theMin == 10 and theAvg == 55:
        print "Your output should parse okay!"
    else:
        print "There was a problem parsing your output. We expected 100, 10, and 55 but got %f, %f, and %f" % (theMax, theMin, theAvg)
