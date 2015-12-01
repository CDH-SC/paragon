
import numpy
import patchsingmod

template_src = '2_1_a.jpg'
target_src   = '2_1_b.jpg'

template_new = '2_1_a_new.jpg'
target_new	 = '2_1_b_new.jpg'

template_features_x,template_features_y,target_features_x,target_features_y=patchsingmod.say_hello(template_src, target_src, template_new, target_new)

print "template_features_x"
print template_features_x