#!/usr/bin/env python3.11

import os
import sys
import subprocess
from collections import defaultdict

def get_file_type(filepath):
    """ Get the file type using the 'file' command """
    result = subprocess.run(['file', '-b', filepath], stdout=subprocess.PIPE)
    return result.stdout.decode('utf-8').strip()

def catalog_files(directory='.'):
    """ Create a catalog of files in the directory """
    file_catalog = defaultdict(list)

    for root, dirs, files in os.walk(directory):
        for name in files:
            filepath = os.path.join(root, name)
            file_type = get_file_type(filepath)
            file_catalog[file_type].append(filepath)
        for name in dirs:
            filepath = os.path.join(root, name)
            file_type = 'directory'
            file_catalog[file_type].append(filepath)

    return file_catalog

def print_catalog(file_catalog):
    """ Print the catalog in the required format """
    for file_type, files in sorted(file_catalog.items()):
        print(f"{file_type}:")
        for filepath in files:
            print(f"  {filepath}")

if __name__ == "__main__":
    directory = sys.argv[1] if len(sys.argv) > 1 else '.'
    file_catalog = catalog_files(directory)
    print_catalog(file_catalog)
