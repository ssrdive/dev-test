import os 
import re

from io import TextIOWrapper

NEW_BUILD_VERSION = os.environ["BuildNum"]
SOURCE_PATH = os.path.join(os.environ["SourcePath"], "develop","global","src")
COPY_SUFFIX = "1"

def openFileReaderAndReturnCopy(base_path : str, file_name : str) \
                                -> tuple[TextIOWrapper, TextIOWrapper]:
    "Return TextIOWrapper to given file_name and a new file_name + COPY_SUFFIX file"
    os.chmod(os.path.join(base_path, file_name), 755)
    fin = open(os.path.join(base_path, file_name), 'r') 
    fout = open(os.path.join(base_path, file_name + COPY_SUFFIX), 'w') 
    return [fin, fout]

def replaceVersion(fin : TextIOWrapper, fout : TextIOWrapper, pattern : str, \
                    replacement : str) -> None:
    "Reaplce the version matching the pattern to replacement in fin to fout"
    for line in fin:
        line = re.sub(pattern, replacement, line) 
        fout.write(line)
    fin.close()
    fout.close()

def promotCopyToOriginal(file_name : str) -> None:
    "Remove the given file_name and rename the file_name + COPY_SUFFIX to file_name"
    os.remove(os.path.join(SOURCE_PATH, file_name)) 
    os.rename(os.path.join(SOURCE_PATH, file_name + COPY_SUFFIX), 
              os.path.join(SOURCE_PATH, file_name))

def update_file(file_name : str, pattern : str, replacement : str) -> None:
    "Update the given file_name by matching pattern regex with replacement"
    fin, fout = openFileReaderAndReturnCopy(SOURCE_PATH, file_name)
    replaceVersion(fin, fout, pattern, replacement)
    promotCopyToOriginal(file_name)

def main(): 
    "Replace versions in the files"
    update_file("SConstruct", r"point=\d+(\.\d+)*", "point=" + NEW_BUILD_VERSION)
    update_file("VERSION", r"ADLMSDK_VERSION_POINT=\d+(\.\d+)*","ADLMSDK_VERSION_POINT=" + NEW_BUILD_VERSION)

if __name__ == '__main__':
    main()