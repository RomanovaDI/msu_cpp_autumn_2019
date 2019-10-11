import subprocess

def check(expr, result, ret):
    out = subprocess.run(['./calc', expr], stdout=subprocess.PIPE)
    if out.returncode != ret:
        print('test err: return value is {0} but must be >= {1}'.format(out.returncode, ret))
    if out.returncode == 0 and out.stdout.decode('ascii') != result:
        print('test err: error {0} != {1}, got {2}'.format(expr, result, out.stdout))

check('2 + 3 * 4 - -2', '16', 0)
check('2 + 2', '4', 0)
check('2*2*2*2*2*2*2*2*2*2', '1024', 0)
check('23+3+4/0', '0', 1)
check('2werf3+3+4/0', '0', 1)