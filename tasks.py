from django.conf import settings
import os, sys
import PIL
from PIL import Image
lib_path = os.path.abspath(settings.PROJECT_ROOT)
sys.path.append(lib_path)
sys.path.append('/usr/src/app/paragon/')
sys.path.append('/usr/local/lib')
import patchsingmod
import classmod
import numpy
import collatemod
from celery import *
from collate.models import *
from django.core.files.base import File

@task
def task_calculate(template_source,target_source):
    print("Calculate TEMPLATE Source ::::+++:::___::: " + template_source)
    print("Calculate Target Source ::::+++:::___::: " + target_source)
    template_features_x_one,template_features_y_one,target_features_x_one,target_features_y_one=patchsingmod.say_hello(template_source,target_source,template_source,target_source)
    print(template_features_x_one)
    response_data= {"temp_feat_x":template_features_x_one,"temp_feat_y":template_features_y_one,  "targ_feat_x":target_features_x_one,"targ_feat_y":target_features_y_one,"template_src": template_source,"target_src": target_source,}
    return response_data

@task
def task_transform(data):
    template_source= data['template_src']
    target_source= data['target_src']
    image_template = Image.open(template_source)
    image_target = Image.open(target_source)
    template_x = data['temp_feat_x']
    template_y = data['temp_feat_y']
    target_x = data['targ_feat_x']
    target_y = data['targ_feat_y']
    template_x=numpy.array(template_x,numpy.int)
    template_y=numpy.array(template_y,numpy.int)
    target_x=numpy.array(target_x,numpy.int)
    target_y=numpy.array(target_y,numpy.int)
    landmarks1=numpy.column_stack((template_y,template_x))
    landmarks2=numpy.column_stack((target_y,target_x))
    landmarks1=tuple(landmarks1)
    landmarks2=tuple(landmarks2)
    landmarks1=numpy.fliplr(landmarks1)
    landmarks2=numpy.fliplr(landmarks2)
    output = '/'.join(target_source.split('/')[0:-1]) + '/transformed.jpg'
    collatemod.say_hello(template_source,target_source,output,landmarks1,landmarks2)
    response_data = { "transformed_src": output,"template_src": template_source,"target_src": target_source,}
    return response_data

@task
def task_results(id, template_src, transformed_src):
    print("TEMPLATE PATH::::" + " " + str(template_src))
    print("Transformed PATH::::" + " " + str(transformed_src))
    response_data = classmod.say_hello(template_src, transformed_src)
    return response_data
