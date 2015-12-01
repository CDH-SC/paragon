from pylab import *
import argparse,glob,os,os.path
import traceback
from scipy.ndimage import measurements
from scipy.misc import imsave
from scipy.ndimage.filters import gaussian_filter,uniform_filter,maximum_filter
from multiprocessing import Pool
#import ocrolib
#from ocrolib import psegutils,morph,sl
#from ocrolib.toplevel import *

