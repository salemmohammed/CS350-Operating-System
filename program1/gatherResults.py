#!/usr/bin/env python
import subprocess

tests = ['./lab4 -pid 1 100 1 1 0 -n 100 -a 1 -m 3 -l 3 -p 0 > ',
         './lab4 -pid 1 100 1 1 0 -n 10 -a 1 -m 1 -l 3 -p 0 > ',
         './lab4 -pid 1 10 3 1 0 -n 100 -a 1 -m 3 -l 1 -p 0 > ',
         './lab4 -pid 1 10 3 1 0 -n 100 -a 1 -m 3 -l 3 -p 0 > ']

for test in tests:
    print test
    trials = 10
    avgGlobTotalRate = 0
    avgGlobProcRate = 0
    avgLocalTotalRate = 0
    avgLocalProcRate = 0
    for i in range(0,trials):
        j = 1
        out = 'test' + str(j) + 'v' + str(i) + '.txt'
        #print 'lab 4 ' + str(i)
        subprocess.call(test + out, shell=True)
        #print 'lab 5 ' + str(i)
        output = subprocess.Popen('./lab5 -p 1 -s 50 -r 0 <' + out, stdout=subprocess.PIPE, shell=True)
        outputStr = str(output.communicate())
        outputStr = outputStr.replace('\\n',' ')
        outputStr = outputStr.split(' ')
        avgGlobTotalRate += float(outputStr[12][0:7]) / trials
        avgGlobProcRate += float(outputStr[17][0:7]) / trials
        output = subprocess.Popen('./lab5 -p 1 -s 50 -r 1 <' + out, stdout=subprocess.PIPE, shell=True)
        outputStr = str(output.communicate())
        outputStr = outputStr.replace('\\n',' ')
        outputStr = outputStr.split(' ')
        avgLocalTotalRate += float(outputStr[12][0:7]) / trials
        avgLocalProcRate += float(outputStr[17][0:7]) / trials
        j += 1


    print 'Global total'
    print avgGlobTotalRate
    print 'Global process'
    print avgGlobProcRate
    print 'Local total'
    print avgLocalTotalRate
    print 'Local process'
    print avgLocalProcRate
