import sys
import preprocmod
import dewarpmod
#from paragon.views import apply_dewarp, apply_preproc
#from collate.models import CollationImage

class CollationImage:
	def __init__(self):
		self.path = ''

sys.settrace
img = CollationImage()
img.path = '/var/www/html/paragon/modulepipe/example.jpg'
#apply_preproc(img)
preprocmod.pre_process(img.path)
dewarpmod.say_hello(img.path, img.path)
