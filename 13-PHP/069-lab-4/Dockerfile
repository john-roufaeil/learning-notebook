FROM php:8.2-apache

RUN apt-get update && apt-get install -y libpng-dev libjpeg-dev \
    && docker-php-ext-install mysqli gd

RUN a2enmod rewrite

RUN echo "upload_max_filesize = 10M\npost_max_size = 12M" > /usr/local/etc/php/conf.d/uploads.ini

RUN chown -R www-data:www-data /var/www/html