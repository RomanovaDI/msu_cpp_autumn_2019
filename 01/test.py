import subprocess

def check(expr, result):
    out = subprocess.run(['./calc', expr], stdout=subprocess.PIPE)
    if out.stdout.decode('ascii') != result:
        print('error {0} != {1}, got {2}'.format(expr, result, out.stdout))

check('2 + 3 * 4 - -2', '16')
check('2 + 2', '4')
check('2*2*2*2*2*2*2*2*2*2', '1024')