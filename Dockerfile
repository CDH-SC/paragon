# Dockerfile for Paragon
FROM python:2.7
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app
RUN apt-get update && apt-get install -y libopencv-dev python-opencv
#
COPY ./requirements2 requirements.txt
RUN pip --no-cache-dir install -r requirements.txt
WORKDIR /usr/src/app/paragon
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
