Installation

- Download and install the Libraries (7.x-2.x branch) module
  http://drupal.org/project/libraries

- Install the php-opencloud library (http://php-opencloud.com/) using Composer
  into sites/all/libraries/php-opencloud/.  If you have Composer installed
  (http://getcomposer.org/), run "composer require rackspace/php-opencloud:dev-master"
  from inside sites/all/libraries/php-opencloud/

- Configure your setttings at /admin/config/media/cloud-files

- Visit admin/config/media/file-system and set the Default download method to
  Rackspace Cloud Files.

- Add a field of type File or Image etc and set the Upload destination to
  Rackspace Cloud Files in the Field Settings tab.