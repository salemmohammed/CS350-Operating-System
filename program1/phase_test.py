from subprocess import check_output

def getRefs(string):
    refs = []
    lines = [l.split(' ') for l in string.split('\n')]
    for l in lines:
        if l[0] == 'REFERENCE' and l[1] == '1':
            refs.append(l[2].strip())
    return refs

def countSwitches(arr):
    switches = 0
    last = arr[0]
    for i in arr:
        if i != last:
            i = last
            switches = switches + 1
    return switches

def main():
    runFor = 10

    switchCounts = {1: [], 2:[], 3:[]}

    for p in switchCounts.keys():
        print('running "./lab4 -pid 1 10 2 2 %i -n 10" %i times' % (p, runFor))
        for n in range(runFor):
            out = check_output(['./lab4', '-pid', '1', '10', '2', '2', str(p), '-n', '10'])
            switchCounts[p].append(countSwitches(getRefs(out)))

    for p, counts in switchCounts.iteritems():
        print('average number of page switches with phases set to %i on process 1: %i' %(p, sum(counts) / len(counts)))

main()