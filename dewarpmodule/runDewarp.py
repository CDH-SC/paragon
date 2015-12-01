#This is the script to run the dewarping pipeline
#Center of Digital Humanities
#06/25/2015

# To run this pipeline, you'll need to install following packages and libraries;
# opencv

import numpy
import os
import sys
#if(sys.a):
#	print "Parameter Error"
#	exit(0)
input_path = sys.argv[1]
output_path = sys.argv[2]
#print input_path, output_path
os.system("python ./src/ocropy-master/ocropus-gpageseg %s" % (input_path));
#os.system("python ./src/leptonlib-1.67/src/testCommand.py ./data/Y1C7v.pseg.png ./data/output.jpg");
