import subprocess
import sys
import time

def main():
    graph = sys.argv[1]
    kamis_k = int(sys.argv[2])

    proc = subprocess.Popen(["./gramm-ecc-reductions/ecc", "-v"],
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            stdin=subprocess.PIPE,
                            encoding="UTF-8")
    proc.stdin.write(open(graph).read())
    proc.stdin.close()
    start_time = time.time()

    time_wasted = 0

    started = False
    passed_bound = False
    while proc.poll() is None:
        current_output = proc.stderr.readline()
        if current_output.startswith("*"):
            k = int("".join(filter(lambda c: c.isdigit(), current_output)))
            if not started:
                print(f"{k},", end="")
                started=True
                if k >= kamis_k:
                    passed_bound = True
                    time_wasted = 0

            if not passed_bound and k >= kamis_k:
                time_wasted = (time.time() - start_time) * 1000
                passed_bound = True

    total_time = (time.time() - start_time) * 1000

    # If we get here, Gramm outpaced our polling, so the number probably isn't great.
    while current_output := proc.stderr.readline():
        print("ALERT: THIS VALUE IS PROBABLY WRONG")
        if current_output.startswith("*"):
            k = int("".join(filter(lambda c: c.isdigit(), current_output)))
            if k >= kamis_k:
                time_wasted = (time.time() - start_time) * 1000

    print(f"{total_time},{time_wasted}")

if __name__ == "__main__":
    main()