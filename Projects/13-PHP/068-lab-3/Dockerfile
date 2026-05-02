# Dockerfile
FROM php:8.2-apache

# Install PDO MySQL and GD (for image validation/processing)
RUN apt-get update && apt-get install -y \
    libpng-dev \
    libjpeg-dev \
    libwebp-dev \
    && docker-php-ext-configure gd --with-jpeg --with-webp \
    && docker-php-ext-install pdo pdo_mysql gd

# Enable Apache rewrite module
RUN a2enmod rewrite

# Allow .htaccess overrides
RUN echo '<Directory /var/www/html>\n\
    Options Indexes FollowSymLinks\n\
    AllowOverride All\n\
    Require all granted\n\
</Directory>' > /etc/apache2/conf-available/allow-override.conf \
    && a2enconf allow-override

# www-data needs write access for uploads (volume mount overrides build-time dirs)
RUN chown -R www-data:www-data /var/www/html