from math import floor
from random import random

code = ""
for i in range(26):
    code += f"if (c == '{chr(ord('A') + i)}') c = '{chr(ord('a') + i)}'; \n"
    if i != 25:
        code += "else "
print(code)

code = "switch(c) {\n"
for i in range(26):
    code += f"\tcase '{chr(ord('A') + i)}':  c = '{chr(ord('a') + i)}'; break;\n"
code += '}'
print(code)

code = "const string input = \""
for i in range(4096):
    n = floor(random() * 52)
    if n < 26:
        code += chr(ord('A') + n)
    else:
        code += chr(ord('a') + n - 26)
code += "\";"
print(code)