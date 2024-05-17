''' 
import subprocess
import os

dic = {}

for (root, dirs, files) in os.walk("./"):
    for file in files:
        try:
            file_type = subprocess.run(["file", "-b", f"{file}"], capture_output=True, text=True, check=True)
            if file_type.stdout.strip() not in dic.keys():
                dic[file_type.stdout.strip()] = [file]
            else:
                dic[file_type.stdout.strip()].append(file)
        except subprocess.CalledProcessError as e:
            print(f"Error: {e}")

    for dir in dirs:
        if "directory" not in dic.keys():
            dic["directory"] = [dir]
        else:
            dic["directory"].append(dir)



for (key, value) in dic.items():
    print(f"Type: {key}:\n")
    print("------------\n")
    for file in value:
        print(f"\t{file}\n")
    print("------------\n")

'''

import subprocess
import os

dic = {}

entries = os.listdir('.')

dirs = [entry for entry in entries if os.path.isdir(os.path.join('.', entry))]
files = [entry for entry in entries if os.path.isfile(os.path.join('.', entry))]

for file in files:
    try:
        file_type = subprocess.run(["file", "-b", f"{file}"], capture_output=True, text=True, check=True)
        if file_type.stdout.strip() not in dic.keys():
            dic[file_type.stdout.strip()] = [file]
        else:
            dic[file_type.stdout.strip()].append(file)
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")

dic["directory"] = dirs


for (key, value) in dic.items():
    print(f"Type: {key}:\n")
    print("------------\n")
    for file in value:
        print(f"\t{file}\n")
    print("------------\n")
