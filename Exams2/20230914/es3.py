#!/usr/bin/env python3

import subprocess as s
import sys


if __name__ == '__main__':
    result = s.run(['ls', '-lR', sys.argv[1]], capture_output=True, text=True)  
    dic = {}
    
    for i in result.stdout.split('\n'):
        # 15 e 17
        if "->" in i:
            entry = i.split(" ")
            index = entry.index("->")
            if entry[17] not in dic.keys():
                dic[entry[index+1]] = [entry[index-1]]
            else:
                dic[entry[index+1]].append(entry[index-1])
            
    # stampo ciò che ho trovato     
    for (key, value) in dic.items():
        print(f"{key} sono indiati dai seguenti link simbolici {value}")