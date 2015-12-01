from django.http import HttpResponse
from django.core.files.base import File
from django.conf import settings
import os,sys
import PIL
from collate.models import *
from collate.tasks import *
from PIL import Image
lib_path = os.path.abspath(settings.PROJECT_ROOT)
sys.path.append(lib_path)
import patchmod
import patchsingmod
import classmod
import collatemod
import numpy
import json
import time
#from upload.tasks import celery_add_task

def collate_book_process(request, id):
    try:
        progress = 0
        collation_book = CollationBook.objects.get(book_id=id)
        pairs = BookHasPair.objects.filter(book=collation_book)
        numberOfpairs = len(pairs)
        increment = 100/numberOfpairs
        response = {'book_id':collation_book.book_id}
        for pair in pairs:
            response[id] = collate_pair_loop(pair.pair_id)
            progress+=increment
            collation_book.progress = progress
            collation_book.save()
    except CollationBook.DoesNotExist:
        return HttpResponse("book does not exist")
    collation_book.progress = 100
    collation_book.save()
    response = json.dumps(response)
    return HttpResponse(response, mimetype='application/json')

def collate_multi_process(request, id):
    try:
        progress = 0
        collation_multi = CollationMulti.objects.get(multi_id=id)
        pairs = MultiHasPair.objects.filter(multi=collation_multi)
        numberOfpairs = len(pairs)
        increment = 100/numberOfpairs
        response = {}
        response['multi_id'] = collation_multi.multi_id
        for pair in pairs:
            response[id] = collate_pair_loop(pair.pair_id)
            progress+=increment
            collation_multi.progress = progress
            collation_multi.save()
    except CollationMulti.DoesNotExist:
        return HttpResponse("multi does not exist")
    collation_multi.progress = 100
    collation_multi.save()
    response = json.dumps(response)
    return HttpResponse(response, mimetype='application/json')

def collate_pair_loop(id):
    #to revert to 7/1/15 version get rid of conditional statement & return 'ERROR'
    try:
        collation_pair = CollationPair.objects.get(pair_id=id)
    except CollationPair.DoesNotExist:
        return "DNE"
    template_src = collation_pair.image_template.path
    target_src = collation_pair.image_target.path
    response = calculate_template(template_src, target_src)
    if not(response == 'ERROR'):
        response = transform(response)
        response = result_list(response, id)
    #7/9/2015
    else:
        image_transformed = CollationImage(size=0, path=template_src)
        image_transformed.save()
        result = Result(data="FAILED")
        result.save()
        collation_pair.result = result
        collation_pair.image_transformed = image_transformed
        collation_pair.save()
    #END 7/9/2015
    return response

def collate_pair_process(request, id):
    try:
        collation_pair = CollationPair.objects.get(pair_id=id)
    except CollationPair.DoesNotExist:
        return HttpResponse("Pair does not exist")
    template_src = collation_pair.image_template.path
    target_src = collation_pair.image_target.path
    response = calculate_template(template_src, target_src)
    print "RESPONSEE:+_+_+_+_+_ ::: " + str(response)
    if not(response == 'ERROR'):
        response = transform(response)
        response = result_list(response, id)
    #7/9/2015
    else:
        print "template_source:+_+_+_+_:: " + str(template_src)
        image_transformed = CollationImage(size=0, path=template_src)
        image_transformed.save()
        result = Result(data="FAILED")
        result.save()
        collation_pair.result = result
        collation_pair.image_transformed = image_transformed
        collation_pair.save()
    #END 7/9/2015
    return HttpResponse(response, mimetype='application/json')

def calculate_template(template_src, target_src):
    temp_path = settings.PROJECT_ROOT
    template_source= template_src
    target_source= target_src
    result=task_calculate.delay(template_source,target_source)
    return result.get()

def transform(data):
    result = task_transform.delay(data)
    return result.get()

def result_list(data, id):
    try:
        collation_pair = CollationPair.objects.get(pair_id=id)
    except CollationPair.DoesNotExist:
        return HttpResponse("Pair does not exist")
    result_boxes = task_results.delay(id, data['template_src'], data['transformed_src'])
    response_data = {}
    response_data['outputs'] = result_boxes.get()
    result = Result(data=json.dumps(result_boxes.get()))
    result.save()
    image_transformed = CollationImage(size=0, path=data['transformed_src'])
    image_transformed.save()
    collation_pair.result = result
    collation_pair.image_transformed = image_transformed
    collation_pair.save()
    response_data = json.dumps(response_data['outputs'])
    return response_data
