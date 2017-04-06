# Dockerfile for Paragon
FROM python:2.7
USER root
RUN apt-get update && apt-get install -y sudo
RUN mkdir -p /usr/src/app
RUN mkdir -p /usr/local/
WORKDIR /usr/local/
COPY ./install-opencv.sh /usr/local/
#COPY ./opencv_contrib/ /usr/src/opencv_contrib/
RUN sh /usr/local/install-opencv.sh
#RUN apt-get update && apt-get install -y libopencv-dev python-opencv
#
WORKDIR /usr/src/app/
COPY ./requirements2 requirements.txt
RUN pip --no-cache-dir install -r requirements.txt
WORKDIR /usr/src/app/paragon
#COPY modulepipe modulepipe
#RUN  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu/

COPY ./celery/conf/celeryd /etc/init.d/celeryd
COPY ./celery/default/celeryd /etc/default/celeryd
RUN adduser celery --ingroup sudo
#RUN python ./modulepipe/Setup_class.py build_ext --inplace
#RUN python ./modulepipe/Setup_collate.py build_ext --inplace
#RUN python ./modulepipe/Setup_patch.py build_ext --inplace
#RUN python ./modulepipe/Setup_patchsing.py build_ext --inplace
#
# #ADD repositories /etc/apk/repositories
# RUN apk add --update py-pip \
#     && pip install numpy
#
# RUN apk add --no-cache --virtual .build-deps \
#   build-base postgresql-dev libffi-dev libxml2-dev libxslt-dev py-numpy@testing\
#     && pip install -r requirements.txt \
#     && find /usr/local \
#         \( -type d -a -name test -o -name tests \) \
#         -o \( -type f -a -name '*.pyc' -o -name '*.pyo' \) \
#         -exec rm -rf '{}' + \
#     && runDeps="$( \
#         scanelf --needed --nobanner --recursive /usr/local \
#                 | awk '{ gsub(/,/, "\nso:", $2); print "so:" $2 }' \
#                 | sort -u \
#                 | xargs -r apk info --installed \
#                 | sort -u \
#     )" \
#     && apk add --virtual .rundeps $runDeps \
#     && apk del .build-deps
