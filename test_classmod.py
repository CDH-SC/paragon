import sys

import numpy
import classmod
sys.path.append("/var/www/html/paragon")

template_src = 'template_src.jpg'
target_src   = 'registered.jpg'

output_boxes = classmod.say_hello(template_src, target_src)

