from distutils.core import setup, Extension


module1 = Extension('classmod',
          include_dirs=['/usr/include/opencv','/usr/lib/x86_64-linux-gnu/','/usr/local/include/opencv2','/usr/src/app/paragon/modulepipe/eigen-eigen-b23437e61a07',
                        '/usr/local/lib/python2.7/site-packages/numpy',
                        '/usr/src/app/paragon/modulepipe'],
          library_dirs=['lib/x86_64-linux-gnu/','/usr/local/lib/python2.7/site-packages/numpy', '/usr/src/app/paragon/modulepipe'],
                  libraries=['opencv_calib3d','opencv_contrib','opencv_core','opencv_features2d','opencv_flann','opencv_gpu',
	                     'opencv_highgui','opencv_imgproc','opencv_legacy','opencv_ml','opencv_nonfree','opencv_objdetect',
	                     'opencv_photo','opencv_stitching','opencv_superres','opencv_ts','opencv_video','opencv_videostab'],
		  sources=['/usr/src/app/paragon/modulepipe/classmodule.cpp','/usr/src/app/paragon/modulepipe/patch_match.cpp',
                    '/usr/src/app/paragon/modulepipe/compute_jaccard.cpp','/usr/src/app/paragon/modulepipe/find_diff.cpp',
                    '/usr/src/app/paragon/modulepipe/refine_match.cpp',
                    '/usr/src/app/paragon/modulepipe/tps.cpp',
                    '/usr/src/app/paragon/modulepipe/book_pipeline_single.cpp',
                    '/usr/src/app/paragon/modulepipe/imgfeatures.c',
                    '/usr/src/app/paragon/modulepipe/kdtree.c',
                    '/usr/src/app/paragon/modulepipe/minpq.c',
                    '/usr/src/app/paragon/modulepipe/sift.c',
                    '/usr/src/app/paragon/modulepipe/utils.c'],
	          language='c++',
                  extra_compile_args=['-O3'])

setup(name='classmod',
      version='1.0',
	  description='blah',
	  author='blah',
	  url='blah',
	  ext_modules=[module1])
