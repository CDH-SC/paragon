from distutils.core import setup, Extension
import os

def getSubDirs(dir = '/usr/local/OpenCV/modules/'):

	dirs =	[name for name in os.listdir(dir) if os.path.isdir(os.path.join(dir, name))]
	return dirs


print(str(getSubDirs))


module1 = Extension('classmod',
          include_dirs=['/usr/local/include/',
		  				'/usr/local/include/opencv/',
		  				'/usr/local/OpenCV/include/opencv/','/usr/local/OpenCV/include/','/usr/local/OpenCV/modules/','/usr/src/app/paragon/modulepipe/eigen-eigen-b23437e61a07',
                        '/usr/local/lib/python2.7/site-packages/numpy',
                        '/usr/local/OpenCV/modules/core',
			'/usr/local/OpenCV/modules/',
			'/usr/local/OpenCV/modules/highgui/include/',
            '/usr/local/OpenCV/modules/ml/include/',
			'/usr/local/OpenCV/modules/legacy/include/',
			'/usr/local/OpenCV/modules/objdetect/include/',
			'/usr/local/OpenCV/modules/calib3d/include',
			'/usr/local/OpenCV/modules/nonfree/include/',
			'/usr/local/OpenCV/modules/features2d/include/',
			'/usr/local/OpenCV/modules/imgproc/include/',
			'/usr/local/OpenCV/modules/flann/include/',
			'/usr/local/OpenCV/modules/video/include/',
			'/usr/local/OpenCV/modules/core/include/',
                        '/usr/local/OpenCV/modules/core/include/opencv2/',
                        '/usr/local/OpenCV/modules/core/include/opencv2/core/',
                        '/usr/src/app/paragon/modulepipe'],
          library_dirs=['/usr/src/app/OpenCV/include/opencv/','/usr/src/app/OpenCV/include/','/usr/src/app/OpenCV/modules/','/usr/src/app/paragon/modulepipe/eigen-eigen-b23437e61a07',
                        '/usr/local/lib/python2.7/site-packages/numpy',
                        '/usr/src/app/OpenCV/modules/core',
						'/usr/local/lib',
						'/usr/lib',
						'/usr/lib/x86_64-linux-gnu/',
                        '/usr/src/app/OpenCV/modules/core/include/opencv2/',
                        '/usr/src/app/OpenCV/modules/core/include/opencv2/core/',
                        '/usr/src/app/paragon/modulepipe','/usr/include/opencv','/usr/local/include','/usr/include/opencv2','lib/x86_64-linux-gnu/','/usr/local/lib/python2.7/site-packages/numpy', '/usr/src/app/paragon/modulepipe'],
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
