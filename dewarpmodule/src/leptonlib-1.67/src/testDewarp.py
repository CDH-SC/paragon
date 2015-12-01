import dewarpmod

template_src = '/home/kang/Desktop/paragon/FQ190_HRH_2_D2.jpg'
target_src   = '/home/kang/Desktop/paragon/FQ190_WU_1_D2.jpg'

out_template = '/home/kang/Desktop/paragon/template_new.jpg'
out_target   = '/home/kang/Desktop/paragon/target_new.jpg'

#template_features_x,template_features_y,target_features_x,target_features_y = patchsingmod.say_hello(template_src,target_src,out_template,out_target)
#status  = dewarpmod.say_hello('1555-7.jpg','1555-7adewarped.jpg')
status  = dewarpmod.say_hello('1555-7.jpg','TESTDewarped.jpg')
