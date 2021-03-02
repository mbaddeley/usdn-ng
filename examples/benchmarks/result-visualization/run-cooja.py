#!/usr/bin/env python

import sys
import os
from subprocess import Popen, PIPE, STDOUT

# get the path of this example
SELF_PATH = os.path.dirname(os.path.abspath(__file__))
# move three levels up
CONTIKI_PATH = os.path.dirname(os.path.dirname(os.path.dirname(SELF_PATH)))

cooja_jar = os.path.normpath(os.path.join(CONTIKI_PATH, "tools", "cooja", "dist", "cooja.jar"))
cooja_input = 'cooja.csc'
cooja_output = 'COOJA.testlog'

#######################################################
# Run a child process and get its output

def run_subprocess(args, input_string):
    retcode = -1
    stdoutdata = ''
    try:
        proc = Popen(args, stdout = PIPE, stderr = STDOUT, stdin = PIPE, shell = True)
        (stdoutdata, stderrdata) = proc.communicate(input_string)
        if not stdoutdata:
            stdoutdata = ''
        if stderrdata:
            stdoutdata += stderrdata
        retcode = proc.returncode
    except OSError as e:
        sys.stderr.write("run_subprocess OSError:" + str(e))
    except CalledProcessError as e:
        sys.stderr.write("run_subprocess CalledProcessError:" + str(e))
        retcode = e.returncode
    except Exception as e:
        sys.stderr.write("run_subprocess exception:" + str(e))
    finally:
        return (retcode, stdoutdata)

#############################################################
# Run a single instance of Cooja on a given simulation script

def execute_test(cooja_file):
    # cleanup
    try:
        os.rm(cooja_output)
    except:
        pass

    filename = os.path.join(SELF_PATH, cooja_file)
    args = " ".join(["java -jar ", cooja_jar, "-nogui=" + filename, "-contiki=" + CONTIKI_PATH])
    sys.stdout.write("  Running Cooja, args={}\n".format(args))

    (retcode, output) = run_subprocess(args, '')
    if retcode != 0:
        sys.stderr.write("Failed, retcode=" + str(retcode) + ", output:")
        sys.stderr.write(output)
        return False

    sys.stdout.write("  Checking for output...")

    is_done = False
    with open(cooja_output, "r") as f:
        for line in f.readlines():
            line = line.strip()
            if line == "TEST OK":
                sys.stdout.write(" done.\n")
                is_done = True
                continue

    if not is_done:
        sys.stdout.write("  test failed.\n")
        return False

    sys.stdout.write(" test done\n")
    return True

#######################################################
# Run the application

def main():
    if not os.access(cooja_jar, os.R_OK):
        print('The file "{}" does not exist, did you build Cooja?'.format(cooja_jar))
        exit(-1)

    input_file = cooja_input
    if len(sys.argv) > 1:
        # change from the default
        input_file = sys.argv[1]

    if not os.access(input_file, os.R_OK):
        print('Simulation script "{}" does not exist'.format(input_file))
        exit(-1)

    print('Using simulation script "{}"'.format(input_file))
    if not execute_test(input_file):
        exit(-1)

#######################################################

if __name__ == '__main__':
    main()
