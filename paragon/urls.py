from django.conf.urls import patterns, include, url
from django.contrib import admin
# url(r'^admin/doc/', include('django.contrib.admindocs.urls')),
admin.autodiscover()

urlpatterns = patterns('',
    #Generic web pages
    url(r'^$', 'paragon.views.home', name='home'),
    url(r'^user$', 'paragon.views.user_interface', name='user_interface'),
    url(r'^login$', 'paragon.views.login_process', name='login_process'),
    url(r'^logout$', 'paragon.views.logout_process', name='logout_process'),
    url(r'^register$', 'paragon.views.register_interface', name='register_interface'),
    url(r'^register/process$', 'paragon.views.register_process', name='register_process'),
    url(r'^new$', 'paragon.views.new_interface', name='new_interface'),
    url(r'^update$', 'paragon.views.update_interface', name='edit_profile'),
    url(r'^sample$', 'paragon.views.sample', name='sample'),

    #User data update processes
    url(r'^update-email/process$', 'paragon.views.update_email_process', name='update_email'),
    url(r'^update-password/process$', 'paragon.views.update_password_process', name='update_password'),

    #Book pages
    url(r'^tmp-book-contents/(?P<username>\w+)/(?P<id>[0-9]+)$', 'paragon.views.tmp_book_contents', name='tmp_book_contents'),
    url(r'^tmp-book-image/(?P<username>\w+)/(?P<type>\w+)/(?P<file>.+)$', 'paragon.views.tmp_book_image', name='tmp_book_image'),

    #Uploading interfaces
    url(r'^upload-pair$', 'paragon.views.upload_pair_interface', name='upload_pair_interface'),
    url(r'^upload-multi$', 'paragon.views.upload_multi_interface', name='upload_multi_interface'),
    url(r'^upload-book$', 'paragon.views.upload_book_interface', name='upload_book_interface'),

    #Uploading processes
    url(r'^upload-image/process$', 'paragon.views.upload_image_process', name='upload_image_process'),
    url(r'^upload-pair/process$', 'paragon.views.upload_pair_process', name='upload_pair_process'),
    url(r'^upload-multi/process$', 'paragon.views.upload_multi_process', name='upload_multi_process'),
    url(r'^upload-book/process$', 'paragon.views.upload_book_process', name='upload_book_process'),
    url(r'^upload-book-temp/process$', 'paragon.views.upload_book_temp_process', name='upload_book_temp_process'),

    #Content pages
    url(r'^contents-single/(?P<id>[0-9]+)$', 'paragon.views.contents_single', name='contents_single'),

    #Progress indicators
    url(r'^progress-single/(?P<id>[0-9]+)$', 'paragon.views.progress_single', name='progress_single'),
    url(r'^progress-multi/(?P<id>[0-9]+)$', 'paragon.views.progress_multi', name='progress_multi'),
    url(r'^progress-book/(?P<id>[0-9]+)$', 'paragon.views.progress_book', name='progress_book'),

    #Fetch Images
    url(r'^image/(?P<id>[0-9]+)$', 'paragon.views.image', name='image'),
    url(r'^temp-image/(?P<username>\w+)$', 'paragon.views.temp_image', name='temp_image'),
    url(r'^result-image/(?P<username>\w+)/(?P<id>[0-9]+)$', 'paragon.views.result_image', name='result_image'),

    #Is this user interface up to date?
    url(r'^user/uptodate/(?P<usercount>[0-9]+)$', 'paragon.views.to_date', name='to_date'),

    #Results
    url(r'^results-pair/(?P<id>[0-9]+)$', 'paragon.views.results_pair', name='results_pair'),
    url(r'^results-multi/(?P<id>[0-9]+)$', 'paragon.views.results_multi', name='results_multi'),

    #Delete process
    url(r'^delete-pair/process/(?P<id>[0-9]+)$', 'paragon.views.delete_pair_process', name='delete_pair_process'),
    url(r'^delete-multi/process/(?P<id>[0-9]+)$', 'paragon.views.delete_multi_process', name='delete_multi_process'),
    url(r'^delete-book/process/(?P<id>[0-9]+)$', 'paragon.views.delete_book_process', name='delete_book_process'),


    #Collation process
    url(r'^collate-pair/process/(?P<id>[0-9]+)$', 'collate.views.collate_pair_process', name='collate_pair_process'),
    url(r'^collate-multi/process/(?P<id>[0-9]+)$', 'collate.views.collate_multi_process', name='collate_multi_process'),
    url(r'^collate-book/process/(?P<id>[0-9]+)$', 'collate.views.collate_book_process', name='collate_book_process'),
    url(r'^admin/', include(admin.site.urls)),
)
