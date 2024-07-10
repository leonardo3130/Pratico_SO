import psutil


def lscmd():
    d = {}
    pids = psutil.pids()
    for pid in pids:
        path = ""
        try:
            path = psutil.Process(pid).exe()
        except Exception as e:
            pass

        if path not in d.keys():
            d[path] = [pid]
        else:
            d[path].append(pid)

    for k, v in d.items():
        print(k, v)


if __name__ == "__main__":
    lscmd()
