from distutils.core import setup, Extension
import numpy
import os.path as path
module1 = Extension('dewarpmod',
	include_dirs=['/home/lingxi/CDH/leptonlib-1.67/src/'],
	library_dirs=['/usr/local/lib/','/usr/lib/','/usr/lib/i386-linux-gnu/'],
	#libraries=['libgd'],
	libraries=['tiff','jpeg','png','z','m','c'],
	#libraries=['libtiff.so.4','libjpeg.so.8','libpng12.so.0','libz.so.1','libm.so.6','libc.so.6'],
	sources=['adaptmap.c','affine.c','affinecompose.c','arithlow.c','arrayaccess.c','bardecode.c','baseline.c','bbuffer.c','bilinear.c','binarize.c','binexpand.c','binexpandlow.c','binreduce.c','binreducelow.c','blend.c','bmf.c','bmpio.c','bmpiostub.c','boxbasic.c','boxfunc1.c','boxfunc2.c','boxfunc3.c','ccbord.c','ccthin.c','classapp.c','colorcontent.c','colormap.c','colormorph.c','colorquant1.c','colorquant2.c','colorseg.c','compare.c','conncomp.c','convertfiles.c','convolve.c','convolvelow.c','correlscore.c','dewarp.c','dwacomb.2.c','dwacomblow.2.c','edge.c','endiantest.c','enhance.c','fhmtauto.c','fhmtgen.1.c','fhmtgenlow.1.c','finditalic.c','flipdetect.c','fliphmtgen.c','fmorphauto.c','fmorphgen.1.c','fmorphgenlow.1.c','fpix1.c','fpix2.c','gifio.c','gifiostub.c','gplot.c','graphics.c','graymorph.c','graymorphlow.c','grayquant.c','grayquantlow.c','heap.c','jbclass.c','jpegio.c','jpegiostub.c','kernel.c','leptwin.c','list.c','maze.c','morph.c','morphapp.c','morphdwa.c','morphseq.c','numabasic.c','numafunc1.c','numafunc2.c','pageseg.c','paintcmap.c','parseprotos.c','partition.c','pix1.c','pix2.c','pix3.c','pix4.c','pix5.c','pixabasic.c','pixacc.c','pixafunc1.c','pixafunc2.c','pixalloc.c','pixarith.c','pixcomp.c','pixconv.c','pixtiling.c','pngio.c','pngiostub.c','pnmio.c','pnmiostub.c','projective.c','psio1.c','psio1stub.c','psio2.c','psio2stub.c','ptabasic.c','ptafunc1.c','ptra.c','queue.c','rank.c','readbarcode.c','readfile.c','regutils.c','rop.c','ropiplow.c','roplow.c','rotate.c','rotateam.c','rotateamlow.c','rotateorth.c','rotateorthlow.c','rotateshear.c','runlength.c','sarray.c','scale.c','scalelow.c','seedfill.c','seedfilllow.c','sel1.c','sel2.c','selgen.c','shear.c','skew.c','spixio.c','stack.c','sudoku.c','textops.c','tiffio.c','tiffiostub.c','utils.c','viewfiles.c','warper.c','watershed.c','webpio.c','webpiostub.c','writefile.c','zlibmem.c','zlibmemstub.c','my_dewarp.c','dewarpModule.c'],
	language='c',
	extra_compile_args=['-O3'])
		  
setup(name='dewarpmod',
	version='1.0',
	description='blah',
	author='blah',
	url='blah',
	ext_modules=[module1])
