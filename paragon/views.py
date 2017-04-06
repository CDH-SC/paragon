from django.http import HttpResponse
from django.shortcuts import render_to_response
from django.core.context_processors import csrf
from django.contrib.auth.models import User
from django.contrib.auth import hashers
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.core.files.base import ContentFile, File
from django.core.files.images import ImageFile
from django.core.files.storage import default_storage
from django.core.exceptions import ObjectDoesNotExist
from django.views.generic.edit import UpdateView
from django.template import RequestContext
from django.db import IntegrityError
from django.core.mail import send_mail
from collate.models import *
from django.conf import settings
from preprocess import preprocmod
import ghostscript as gs
import dewarpmod
import os
import json
import zipfile
from .forms import UpdateForm

def validateEmail(email):
    from django.core.validators import validate_email
    from django.core.exceptions import ValidationError
    try:
        validate_email(email)
        return True
    except ValidationError:
        return False

def new_interface(request):
    context = {'user':request.user}
    context['username'] = request.user.username
    return render_to_response('select-collation.html' , context)

def home(request):
    context = {'user':request.user}
    context.update(csrf(request))
    if request.user.is_authenticated():
        context['username'] = request.user.username
        context['single_collations'] = UserHasPair.objects.filter(user_id=request.user.id)
        return user_interface(request)
    return render_to_response('home.html' , context)

def register_interface(request):
    context = {}#'user': request.POST['username'] }
    context.update(csrf(request))
    return render_to_response('register.html' , context)

def update_interface(request):
    import preprocess.preprocmod
    context = {'user':request.user}
    context.update(csrf(request))
    if request.user.is_authenticated():
        context['username'] = request.user.username
        context['email'] = request.user.email
        try:
            profile = UserProfile.objects.get(user=request.user)
            context['affiliation'] = profile.affiliation
        except UserProfile.DoesNotExist:
            context['affiliation'] = 'NONE'
        return render_to_response('user-update.html' , context)
    return render_to_response('home.html' , context)

@login_required
def to_date(request, usercount):
    response = {}
    count = 0
    usercount = int(usercount)
    count += len(UserHasPair.objects.filter(user=request.user))
    count += len(UserHasMulti.objects.filter(user=request.user))
    response['count'] = count
    response['usercount'] = usercount
    if count == usercount:
        response['msg'] = 'True'
    else:
        response['msg'] = 'False'
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

@login_required
def update_email_process(request):
    response = {}
    new_email = request.POST['value']
    if validateEmail(new_email):
        request.user.email = new_email
        request.user.save()
        response = {'status': 'Success'}
    else:
        response = {'status' : 'Invalid email format'}
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

@login_required
def update_password_process(request):
    response = {}
    old_password = request.POST['old']
    new_password_conf = request.POST['new1']
    new_password = request.POST['new2']
    if new_password == new_password_conf:
        response = {'status' : 'Success'}
        request.user.set_password(new_password)
        request.user.save()
    else:
        response = {'status' : 'Passwords do not match'}
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

def tmp_book_contents(request, username, id):
    response = {'username':username, 'book_id':id}
    pages_path = "/usr/src/app/paragon/static/media/paragon/" + username + "/tmp_book/book" + id + "/"
    response['book_path'] = pages_path
    import os
    files = os.listdir(pages_path)
    response['contents'] = files
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

def register_process(request):
    response = {}
    user = request.POST['user']
    pwd = request.POST['pwd']
    email = request.POST['email']
    university = request.POST['affiliation']
    if not(validateEmail(email)):
        response['success'] = False
        response['msg'] = "Invalid email address."
        response = json.dumps(response)
        return HttpResponse(response, content_type='application/json')
    try:
        user = User.objects.create_user(user, password=pwd, email=email)
        profile = UserProfile(user=user, affiliation=university)
        response['success'] = True
        response['msg'] = "Successfully registered."
        profile.save()
        user.save()
    except IntegrityError:
        response['success'] = False
        response['msg'] = "Username already exists."
    #send_mail('Paragon Registration', 'Welcome to Paragon!\nVisit us at: http://tundra.csd.sc.edu/paragon', 'admin@tundra.csd.sc.edu',
    #[email])
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')


@login_required
def progress_single(request, id):
    response = {'msg': 'Success', 'id': id, 'type':'s'}
    try:
        collation_pair = CollationPair.objects.get(pair_id=id)
        if collation_pair.result == None:
            response['progress'] = 0
        elif collation_pair.result.data == 'FAILED':
            response['progress'] = -1
        else:
            response['progress'] = 100
    except CollationPair.DoesNotExist:
        response['msg'] = 'Collation pair does not exist'
    response = json.dumps(response)

    return HttpResponse(response, content_type='application/json')

@login_required
def progress_multi(request, id):
    response = {'msg': 'Success', 'id': id, 'type':'m'}
    try:
        collation_multi = CollationMulti.objects.get(multi_id=id)
        response['progress'] = collation_multi.progress
    except CollationMulti.DoesNotExist:
        response['msg'] = 'Multi-collation does not exist'
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

@login_required
def progress_book(request, id):
    response = {'msg': 'Success', 'id': id, 'type': 'b'}
    try:
        collation_book = CollationBook.objects.get(book_id=id)
        response['progress'] = collation_book.progress
    except CollationBook.DoesNotExist:
        response['msg'] = 'Book-collaiton does not exist'
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

@login_required
def contents_single(request, id):
    pair = CollationPair.objects.get(pair_id=id)
    return HttpResponse(pair_to_string(pair), content_type='application/json')

def pair_to_string(pair):
    output = "%s , %s" % (pair.image_template.name, pair.image_target.name)
    return output

@login_required
def user_interface(request):
    context = {'user':request.user}
    context['username'] = request.user.username
    context['single_collations'] = UserHasPair.objects.filter(user_id=request.user.id)
    context['multi_collations'] = UserHasMulti.objects.filter(user_id=request.user.id)
    context['book_collations'] = UserHasBook.objects.filter(user_id=request.user.id)
    context['count'] = len(context['single_collations']) + len(context['multi_collations'])
    return render_to_response('user.html' , context)

@login_required
def upload_pair_interface(request):
    context = {'user':request.user}
    context['username'] = request.user.username
    context['exists'] = False
    #context['pair_id'] = 0
    context.update(csrf(request))
    return render_to_response('upload-pair.html' , context)

@login_required
def upload_multi_interface(request):
    context = {}
    context['username'] = request.user.username
    context.update(csrf(request))
    return render_to_response('upload-multi.html' , context)

@login_required
def upload_book_interface(request):
    context = {'username': request.user.username}
    context.update(csrf(request))
    return render_to_response('upload-book.html' , context)

@login_required
def upload_book_process(request):
    response = {'msg':'success'}
    uname = request.user.username
    template_list = json.loads(request.POST['template_list'])
    template_path = '/usr/src/app/paragon/static/media/paragon/%s/tmp_book/template/' % (uname)
    target_list = json.loads(request.POST['target_list'])
    target_path = '/usr/src/app/paragon/static/media/paragon/%s/tmp_book/target/' % (uname)
    collation_book = CollationBook(progress=0)
    collation_book.save()
    book_id = collation_book.book_id
    new_path = '/usr/src/app/paragon/static/media/paragon/%s/book_%d/' % (uname, book_id)
    try:
        os.mkdir(new_path)
    except OSError:
        pass
    page_num = 1
    for temp, targ in zip(template_list, target_list):
        page_path = new_path + str(page_num) + '/'
        try:
            os.mkdir(page_path)
        except OSError:
            pass
        dest_temp = page_path + 'template_' + temp
        dest_targ = page_path + 'target_' + targ
        os.rename(template_path+temp, dest_temp)
        os.rename(target_path+targ, dest_targ)
        img_temp = CollationImage(path=dest_temp, size=0, name=temp)
        img_targ = CollationImage(path=dest_targ, size=0, name=targ)
        apply_dewarp(img_temp)
        apply_dewarp(img_targ)
        img_temp.save()
        img_targ.save()
        collation_pair = CollationPair(image_template=img_temp, image_target=img_targ)
        collation_pair.save()
        relationship = BookHasPair(book=collation_book, pair=collation_pair, page_num=page_num)
        relationship.save()
        page_num+=1
    book_len = len(UserHasBook.objects.filter(user=request.user))+1
    relationship = UserHasBook(user=request.user, book=collation_book, name="Book Collation " + str(book_len))
    relationship.save()
    response['id'] = book_id
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')


@login_required
def upload_book_temp_process(request):
    upload_type = request.POST['type']
    page_formats = ['.jpg', '.jpeg']
    tmp_files = request.FILES.keys()
    tmp_files.sort()
    response = {'files':tmp_files}
    response['is_zip'] = False
    response['is_pdf'] = False
    response['type'] = upload_type
    book_id = 0
    username = request.user.username
    tmp_path = '/usr/src/app/paragon/static/media/paragon/' + username + '/tmp'
    tmp_book_path = '/usr/src/app/paragon/static/media/paragon/' + username + '/tmp_book/'
    pages_path = "/usr/src/app/paragon/static/media/paragon/" + username + "/tmp_book/" + upload_type + "/"
    bookContent = ""
    extension = ""
    upload_file = File(request.FILES['book'])
    for chunk in upload_file.chunks():
        bookContent+=chunk
    if request.FILES['book'].name.endswith('.zip'):
        response['is_zip'] = True
        extension = ".zip"
    if request.FILES['book'].name.endswith('.pdf'):
        response['is_pdf'] = True
        extension = ".pdf"
    upload_file = ContentFile(bookContent)
    default_storage.save(tmp_path+extension, upload_file)
    if response['is_zip']:
        response['contents'] = []
        file = open(tmp_path+extension, 'rb')
        file = zipfile.ZipFile(file)
        for f in file.namelist():
            if not('__MACOSX' in f) and not(f.endswith('/')):
                response['contents'].append(f)
                file.extract(f, pages_path)
        response['contents'].sort()
    if response['is_pdf']:
        try:
            os.mkdir(tmp_book_path)
        except OSError:
            pass #whatever
        try:
            os.mkdir(pages_path)
        except OSError:
            pass #whatever
        gsArgs = ['-sOUTPUTFILE=a-%03d.jpg', '-sDEVICE=jpeg', '-o', pages_path + '/a-%03d.jpg', tmp_path+extension]
        gs.Ghostscript(*gsArgs)
        response['contents'] = os.listdir(pages_path)
    response = json.dumps(response)
    default_storage.delete(tmp_path+extension)
    return HttpResponse(response, content_type='application/json')

@login_required
def upload_image_process(request):
    response = {}
    response['type'] = request.POST['type']
    username = request.user.username
    image_path = "/usr/src/app/paragon/static/media/paragon/" + username + "/temp.jpg"
    image_content = ""
    image = request.FILES['img']
    for chunk in image.chunks():
        image_content+= chunk
    image = ContentFile(image_content)
    default_storage.save(image_path, image)
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

@login_required
def delete_pair_process(request, id):
    response = {'msg': 'Success'}
    try:
        collation_pair = CollationPair.objects.get(pair_id=id)
        transformedPath = collation_pair.image_template.path.split('/')[0:-1]
        pairPath = '/'.join(transformedPath)
        transformedPath.append('transformed.jpg')
        transformedPath = '/'.join(transformedPath)
        default_storage.delete(collation_pair.image_template.path)
        default_storage.delete(collation_pair.image_target.path)
        default_storage.delete(transformedPath)
        collation_pair.image_template.delete()
        collation_pair.image_target.delete()
        collation_pair.delete()
        import os
        os.rmdir(pairPath)
    except CollationPair.DoesNotExist:
        response['msg'] = "Error: Single pair does not exist"
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

#@login_required
def delete_multi_process(request, id):
    response = {'msg': 'Success'}
    try:
        collation_multi = CollationMulti.objects.get(multi_id=id)
        pairs = MultiHasPair.objects.filter(multi=collation_multi)
        if(len(pairs) > 0):
            transformedPath = pairs[0].pair.image_template.path.split('/')[0:-1]
            multiPath = '/'.join(transformedPath)
            transformedPath.append('transformed.jpg')
            transformedPath = '/'.join(transformedPath)
            for hasPair in pairs:
                try:
                    default_storage.delete(hasPair.pair.image_template.path)
                    default_storage.delete(hasPair.pair.image_target.path)
                    default_storage.delete(transformedPath)
                    hasPair.pair.image_template.delete()
                    hasPair.pair.image_target.delete()
                    hasPair.pair.delete()
                except ObjectDoesNotExist:
                    print "object does not exist"
        else:
            username = request.user.username
            multiPath = "/usr/src/app/paragon/static/media/paragon/" + username + "/multi_" + str(collation_multi.multi_id) + "/"
        import shutil
        shutil.rmtree(multiPath)
        collation_multi.delete()
    except CollationMulti.DoesNotExist:
        response['msg'] = "Error: Multi-collation does not exist"
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

@login_required
def delete_book_process(request, id):
    response = {'msg': 'Success'}
    try:
        collation_book = CollationBook.objects.get(book_id=id)
        pairs = BookHasPair.objects.filter(book=collation_book)
        if(len(pairs) > 0):
            transformedPath = pairs[0].pair.image_template.path.split('/')[0:-1]
            bookPath = '/'.join(transformedPath)
            transformedPath.append('transformed.jpg')
            transformedPath = '/'.join(transformedPath)
            for hasPair in pairs:
                default_storage.delete(hasPair.pair.image_template.path)
                default_storage.delete(hasPair.pair.image_target.path)
                default_storage.delete(transformedPath)
                hasPair.pair.image_template.delete()
                hasPair.pair.image_target.delete()
                hasPair.pair.delete()
        else:
            uname = request.user.username
            bookPath = '/usr/src/app/paragon/static/media/paragon/%s/tmp_book/' % (uname)
        import shutil
        shutil.rmtree(bookPath)
        collation_book.delete()
    except CollationBook.DoesNotExist:
        response['msg'] = "Error: Multi-collation does not exist"
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

@login_required
def upload_pair_process(request):
    print "In upload pair process."
    username = request.user.username
    collation_pair = CollationPair()
    collation_pair.save()
    pair_id = collation_pair.pair_id
    response = {'pair_id': pair_id, 'preproc': 1}
    template_path = "/usr/src/app/paragon/static/media/paragon/" + username + "/" + "single_" + str(pair_id) + "/template.jpg"
    target_path = "/usr/src/app/paragon/static/media/paragon/" + username + "/" + "single_" + str(pair_id) + "/target.jpg"
    template = request.FILES['template']
    target = request.FILES['target']
    template_name = template.name
    target_name = target.name
    template_content = ""
    target_content = ""
    for chunk in template.chunks():
        template_content+= chunk
    for chunk in target.chunks():
        target_content+= chunk
    template = ContentFile(template_content)
    target = ContentFile(target_content)
    default_storage.save(template_path, template)
    default_storage.save(target_path, target)
    image_template = CollationImage(path=template_path, size=template.size, name=template_name)
    image_target = CollationImage(path=target_path, size=target.size, name=target_name)
    image_template.save()
    image_target.save()
    ### PREPROC
    preproc = int(request.POST['preproc'])
    print "PREPROC +_+_+_+_+__++_+_: " + str(preproc) + " " + str(preproc == 1)
    if int(preproc) == 1:
        if preprocmod.can_process(template_path) == -1:
            print("CANT PREPROC TEMP")
            response['preproc'] = 0
        if preprocmod.can_process(target_path) == -1:
            print("CANT PREPROC TARG")
            response['preproc'] = 0
        if response['preproc'] == 1:
            print("APPLYING PREPROC!!")
            apply_preproc(image_template)
            apply_preproc(image_target)
    else:
        response['preproc'] = 2
    ### END PREPROC
    dewarp = int(request.POST['dewarp'])
    print "DEWARP+_+_+_+_+_+_: " + str(request.POST['dewarp'])
    if int(dewarp) == 1:
        apply_dewarp(image_template)
        apply_dewarp(image_target)
    name = "Single Collation " + str(len(UserHasPair.objects.filter(user=request.user))+1)
    collation_pair = CollationPair.objects.get(pair_id=pair_id)
    collation_pair.image_template = image_template
    collation_pair.image_target = image_target
    collation_pair.save()
    relationship = UserHasPair(user=request.user, pair = collation_pair, name=name)
    relationship.save()
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

@login_required
def upload_multi_process(request):
    collation_multi = CollationMulti(progress=0)
    collation_multi.save()
    multi_id = collation_multi.multi_id
    username = request.user.username
    dir_path = "/usr/src/app/paragon/static/media/paragon/" + username + "/multi_" + str(multi_id) + "/"
    file_contents = {}
    template = None
    targetList = request.FILES.keys()
    targetList.remove('template')
    print("TARGETLIST +_+_+_+_+_+_: " + str(targetList))
    targets = []
    pairs = []
    template_contents = ""
    for chunk in request.FILES['template']:
            template_contents+=chunk
    template_file = ContentFile(template_contents)
    template_path = dir_path + "template.jpg"
    default_storage.save(template_path, template_file)
    template = CollationImage(path=template_path, size=template_file.size, name=request.FILES['template'].name)
    template.save()
    apply_dewarp(template)
    for key in targetList:
        print(request.FILES[key].name)
        pair = CollationPair()
        pair.save()
        pair_id = pair.pair_id
        image_contents = ""
        for chunk in request.FILES[key]:
            image_contents+=chunk
        image_file = ContentFile(image_contents)
        image_path = dir_path + "/" + str(pair_id) + "/" + "target.jpg"
        default_storage.save(image_path, image_file)
        pair.image_template = template
        image_model = CollationImage(path=image_path,size=image_file.size, name=request.FILES[key].name)
        image_model.save()
        apply_dewarp(image_model)
        pair.image_target = image_model
        pair.save()
        relationship = MultiHasPair(multi=collation_multi, pair=pair)
        relationship.save()
    multi_len = len(UserHasMulti.objects.filter(user=request.user))+1
    relationship = UserHasMulti(user=request.user, multi=collation_multi, name="Multi-collation " + str(multi_len))
    relationship.save()
    response = {'id' : multi_id}
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

def login_process(request):
    response = {}
    user = request.POST['user']
    pwd = request.POST['pwd']
    auth = authenticate(username=user, password=pwd)
    if auth is not None:
        response['msg'] = "Login Success"
        response['auth'] = True
        login(request, auth)
    else:
        response['msg'] = "Invalid username or password"
        response['auth'] = False
    response = json.dumps(response)
    return HttpResponse(response, content_type='application/json')

@login_required
def sample(request):
    context = RequestContext(request)
    context_dict = {}
    context_dict['user'] = {'user':request.user}
    context_dict['username'] = request.user.username
    return render_to_response('sample.html', context_dict)

@login_required
def logout_process(request):
    logout(request)
    context = {'user':request.user}
    return render_to_response('home.html' , context)

def results_pair(request, id):
    context = {}
    try:
        collation_pair = CollationPair.objects.get(pair_id=id)
    except CollationPair.DoesNotExist:
        return HttpResponse("Pair does not exist")
    try:
        img = collation_pair.image_template
        f = open(img.path, "rb")
        f = File(f)
        img = ImageFile(f)
        context['width'] = img.width
        context['height'] = img.height
    except IOError:
        return HttpResponse("I/O Error " + str(img.path))
    except CollationImage.DoesNotExist:
        return HttpResponse("Error: Result does not exist")
    context['username'] = request.user.username
    context['template_id'] = collation_pair.image_template.image_id
    context['template_name'] = collation_pair.image_template.name
    context['target_id'] = collation_pair.image_target.image_id
    context['target_name'] = collation_pair.image_target.name
    context['pair_id'] = id
    context['from_multi'] = False
    context['from_book'] = False
    try:
        id = request.GET['from_multi']
        context['from_multi'] = True
        context['multi_id'] = id
    except KeyError:
        pass
    if not(collation_pair.result == None):
        context['result'] =  collation_pair.result.data
    else:
        context['exists'] = True
        return render_to_response('results-pair.html' , context)
    return render_to_response('results-pair.html' , context)

def results_multi(request, id):
    context = {}
    try:
        collation_multi = CollationMulti.objects.get(multi_id=id)
        pairs = MultiHasPair.objects.filter(multi=collation_multi)
        dummyPair = pairs[0].pair
        template = dummyPair.image_template
        relationship = UserHasMulti.objects.get(multi=collation_multi)
        context['name'] = relationship.name
        context['username'] = request.user.username
        context['multi_id'] = collation_multi.multi_id
        context['template_id'] = template.image_id
        targets = [x.pair for x in pairs]
        ids = [x.pair_id for x in targets]
        targets = [x.image_target for x in targets]
        targets = [x.image_id for x in targets]
        context['targets'] = []
        for pair_id,image_id in zip(ids,targets):
            context['targets'].append({'pair_id':pair_id,'image_id':image_id})
    except CollationMulti.DoesNotExist:
        return HttpResponse("Multi-collation does not exist")
    return render_to_response('results-multi.html' , context)

def apply_preproc(image):
    preprocmod.pre_process(image.path)

def apply_dewarp(image):
    dewarpmod.say_hello(image.path, image.path)

def results_pair_multi(request, id, pair):
    context = {}
    try:
        collation_multi = CollationMulti.objects.get(multi_id=id)
    except CollationPair.DoesNotExist:
        return HttpResponse("Multi-collation does not exist")
    return results_pair(request, pair)

def temp_image(request, username):
    image_path = "/usr/src/app/paragon/static/media/paragon/" + username + "/temp.jpg"
    try:
        f = open(image_path, "rb")
        content = f.read()
        f.close()
        default_storage.delete(image_path)
        return HttpResponse(content, content_type="image/jpeg")
    except IOError:
        return HttpResponse("I/O Error")
    except CollationImage.DoesNotExist:
        return HttpResponse("Image does not exist")

def result_image(request, username, id):
    try:
        collation_pair = CollationPair.objects.get(pair_id=id)
        transformed = collation_pair.image_transformed
        image_path = transformed.path
        f = open(image_path, "rb")
        return HttpResponse(f.read(), content_type="image/jpeg")
    except IOError:
        return HttpResponse("I/O Error")
    except CollationImage.DoesNotExist:
        return HttpResponse("Image does not exist")

def image(request, id):
    try:
        img = CollationImage.objects.get(image_id=id)
        f = open(img.path, "rb")
        return HttpResponse(f.read(), content_type="image/jpeg")
    except IOError:
        return HttpResponse("I/O Error")
    except CollationImage.DoesNotExist:
        return HttpResponse("Image does not exist")

def tmp_book_image(request, username, type, file):
    path = "/usr/src/app/paragon/static/media/paragon/" + username + "/tmp_book/" + type + "/" + file
    try:
        f = open(path, "rb")
        return HttpResponse(f.read(), content_type="image/jpeg")
    except IOError:
        return HttpResponse("I/O Error @ " + path)
