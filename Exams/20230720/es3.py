'''
Scrivere un programma python o uno script bash che data una directory produca un elenco dei file e
delle directory che non potrebbero essere copiati in file system che supportino solo caratteri ascii nei
nomi.
'''
''' 
import subprocess 

def list_ASCII_entries(path):
    try:
        entries = subprocess.run(['ls', path], capture_output = True, text = True, check = True)
    except Exception as e:
        print(e)


path = input()
list_ASCII_entries(path)'''
import subprocess

def is_ascii(s):
    for c in s:
        if ord(c) > 127:
            return False
    return True

def list_files_and_directories(directory):
    try:
        result = subprocess.run(['ls', '-a', directory], capture_output=True, text=True, check=True)
        entries = result.stdout.split("\n")
        entries = list(filter(lambda entry: is_ascii(entry), entries))
        return entries
    except subprocess.CalledProcessError as e:
        print("Error:", e)

directory_path = '.'
result = list_files_and_directories(directory_path)

for el in result:
    print(el)
