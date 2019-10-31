#!/usr/bin/python3

import sys

#print('Number of arguments: ', len(sys.argv), 'arguments.')
#print('Argument List: ', str(sys.argv))

sys.argv.pop(0)
strings = sys.argv

for str in strings:
    file = open("./"+str+".h", "a")
    split = str.split('/')
    file.write("#ifndef " + split[len(split)-1].upper() + "_H\n")
    file.write("#define " + split[len(split)-1].upper() + "_H\n")
    file.write("\n\n\n\n\n#endif\n")
    
    file = open("./"+str+".cpp", "a")
    file.write("#ifndef " + split[len(split)-1].upper() + "_CPP\n")
    file.write("#define " + split[len(split)-1].upper() + "_CPP\n")
    file.write("\n#include \""+ str +".h\"")
    file.write("\n\n\n\n\n#endif\n")

    file.close()

