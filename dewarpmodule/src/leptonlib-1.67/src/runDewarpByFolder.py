import dewarpmod
import os
template_src = '/home/kang/Desktop/paragon/FQ190_HRH_2_D2.jpg'
target_src   = '/home/kang/Desktop/paragon/FQ190_WU_1_D2.jpg'

out_template = '/home/kang/Desktop/paragon/template_new.jpg'
out_target   = '/home/kang/Desktop/paragon/target_new.jpg'

BIN = "/home/lingxi/CDH/"
EXE = BIN+"leptonlib-1.67/src/"
DATA = BIN+"/Data"
Dewarping = DATA+"/dewarping"
#print os.path.dirname(os.path.realpath(__file__))
#os.chdir(Dewarping)
#print os.path.dirname(os.path.realpath(__file__))
#os.chdir(os.path.dirname(os.path.realpath(__file__)))
print os.path.dirname(os.path.realpath(__file__))
files = os.listdir(EXE)

for file in files:
	fileSplit = file.split(".")
	#print file
	if(len(fileSplit)>=2 and fileSplit[1]=="jpeg"):
		print "OK" + file
	#else:
		#print file
		# #print os.path.dirname(os.path.realpath(__file__))
		# print (Dewarping+"/"+"Y1 C7v"+".jpeg",Dewarping+"/"+"Y1 C7v"+"Dewarped.jpeg")
		status  = dewarpmod.say_hello(fileSplit[0]+".jpeg",file[0]+"Dewarped.jpeg")
		# #status  = dewarpmod.say_hello(Dewarping+"/"+"Y1\ C7v"+".jpeg",Dewarping+"/"+"Y1\ C7v"+"Dewarped.jpeg")


#template_features_x,template_features_y,target_features_x,target_features_y = patchsingmod.say_hello(template_src,target_src,out_template,out_target)
#status  = dewarpmod.say_hello('1555-7.jpg','1555-7adewarped.jpg')
