from distutils.core import setup, Extension
import numpy
import os.path as path
module1 = Extension('dewarp',
	include_dirs=['/home/lingxi/CDH/leptonlib-1.67/src/'],
	library_dirs=['/usr/local/lib/','usr/lib'],
	#libraries=['libjpeg.so','libtiff','libpng','libz'],
	sources=['dewarptest.c'],
	language='c',
	extra_compile_args=['-O3'])
		  
setup(name='dewarp',
	version='1.0',
	description='blah',
	author='blah',
	url='blah',
	ext_modules=[module1])
