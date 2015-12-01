import os
import sys
from django.core.wsgi import get_wsgi_application

#import django.core.handlers.wsgi
#application = django.core.handlers.wsgi.WSGIHandler()


path = '/var/www/html/paragon'
if path not in sys.path:
    sys.path.append(path)

path = '/var/www/html/paragon/preproc'
if path not in sys.path:
    sys.path.append(path)

os.environ['DJANGO_SETTINGS_MODULE'] = 'paragon.settings' 

application = get_wsgi_application()