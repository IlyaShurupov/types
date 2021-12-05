
import os
import sys

cmd = "xcopy " + sys.argv[1] + "example\\Font.ttf " + sys.argv[2] + " /Y"
print(cmd)
os.system(cmd)
