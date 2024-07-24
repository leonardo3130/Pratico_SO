import os
import sys
import subprocess


def removeSameSha1(dir1, dir2):
    d1 = {}
    d2 = {}
    for entry in os.listdir(dir1):
        try:
            result = subprocess.run(
                ["sha1sum", os.path.join(dir1, entry)],
                capture_output=True,
                text=True,
                check=True,
            )
            out = result.stdout.split("  ")
            d1[out[1]] = out[0]
        except subprocess.CalledProcessError as e:
            print("Error:", e)
    for entry in os.listdir(dir2):
        try:
            result = subprocess.run(
                ["sha1sum", os.path.join(dir2, entry)],
                capture_output=True,
                text=True,
                check=True,
            )
            out = result.stdout.split("  ")
            d2[out[1]] = out[0]
        except subprocess.CalledProcessError as e:
            print("Error:", e)

    for k1, v1 in d1.items():
        for k2, v2 in d2.items():
            if v1 == v2:
                p1 = os.path.join(dir1, k1)
                p2 = os.path.join(dir2, k2)
                if os.path.exist(p1):
                    os.remove(p1)
                if os.path.exist(p2):
                    os.remove(p2)


if __name__ == "__main__":
    dir1 = sys.argv[1]
    dir2 = sys.argv[2]
    removeSameSha1(dir1, dir2)
