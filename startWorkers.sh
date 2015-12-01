cd /var/www/html/paragon
celery -A paragon worker -P eventlet -c 1000 &