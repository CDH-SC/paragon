FROM python:2.7-alpine
USER root

WORKDIR /usr/src/app
#
COPY ./requirements.txt requirements.txt

RUN apk --update add bash \
    && adduser -s /bin/bash -D celery \
    && mkdir -p /usr/src/app/ \
    && pip --no-cache-dir install -r requirements.txt
