from __future__ import absolute_import

import os

#from celery import Celery
from celery import Celery
from django.conf import settings

# set the default Django settings module for the 'celery' program.
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'paragon.settings')
#' + os.environ["HOSTNAME"] + '
app = Celery('paragon', broker='amqp://guest:guest@localhost:5672//' ,include=['collate.tasks'])

# Using a string here means the worker will not have to
# pickle the object when using Windows.
app.config_from_object('django.conf:settings')
app.autodiscover_tasks(lambda: settings.INSTALLED_APPS)

#debug task
#@app.task(bind=True)
#def debug_task(self):
#    print('Request: {0!r}'.format(self.request))
