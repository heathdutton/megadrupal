#! /bin/bash

# Get the FQDN

fqdn=`hostname -f`

# Install Drush
apt-get --purge remove drush
cd /usr/share/
git clone --recursive --branch 8.x-6.x http://git.drupal.org/project/drush.git
ln -s /usr/share/drush/drush.php /usr/bin/drush

# Download Drupal
cd /var/www
cp /vagrant/drupal.git ./drupal -r
#git clone --recursive --branch 8.x http://git.drupal.org/project/drupal.git
chown www-data:www-data drupal -R
cd drupal
git checkout 8.x
git pull


# Install Drupal
drush -y site-install standard --db-url=mysql://root:@localhost/drupal-up --site-name=Drupal-up --sites-subdir=$fqdn
cd sites/$fqdn
drush user-password admin --password="admin"

# Fix permissions
sudo /bin/bash /vagrant/files/fix-permissions.sh /var/www/drupal/ www-data

# Configure Apache
a2dissite default
a2enmod rewrite
cp /vagrant/files/drupal.conf /etc/apache2/conf.d/
cp /vagrant/files/vhost /etc/apache2/sites-available/$fqdn
sed -i "s/www.example.com/$fqdn/g" /etc/apache2/sites-available/$fqdn
a2ensite $fqdn
apache2ctl graceful

echo '/////////////////////////////////////////////////'
echo '   You can visit your site at:'
echo "       http://$fqdn"
echo '       Username: admin'
echo '       Password: admin'
echo '/////////////////////////////////////////////////'
