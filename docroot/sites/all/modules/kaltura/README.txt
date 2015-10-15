INTRODUCTION
------------
Add full video capabilities to your site with the Kaltura Module.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/kaltura

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/kaltura


REQUIREMENTS
------------
This module requires the following modules:
 * Chaos tools (https://www.drupal.org/project/ctools)
 * Entity API (https://www.drupal.org/project/entity)
 * Libraries (https://www.drupal.org/project/libraries)
 * Views (https://www.drupal.org/project/views)

And a PHP library:
 * Kaltura Client library
   (http://cdnbakmi.kaltura.com/content/clientlibs/php5_16-12-2014.tar.gz),
   which can be downloaded and installed with the Drush command.


INSTALLATION
------------
 1. Install as you would normally install a contributed Drupal module. See:
    https://www.drupal.org/documentation/install/modules-themes/modules-7
    for further information.
 2. Move the crossdomain.xml file (supplied with the module) to the root
    directory of your domain. So if your domain is domain.com, then this file
    should be accessible in http://domain.com/crossdomain.xml .
 3. Place the Kaltura Client PHP library in the DRUPAL_ROOT/sites/all/libraries/
    directory. If you haven't installed the library manually yet use the Drush
    command:
    drush kaltura-client-install


CONFIGURATION
-------------
 * Go to Administration » Configuration » Media » Kaltura module settings:
   Register to the Kaltura Partner Program using the registration form. Your
   Kaltura partner details will be automatically saved into the module
   configuration, and will be sent to you in an email as well.

 * Configure user permissions in Administration » People » Permissions.

 * It is recommended to configure a cron job for your site, since some
   statistic data about the media in your site is updated by cron. Some of views
   provided with the module rely on those statistics.

According to the modules you enabled, you can now start working with the
Kaltura module to create nodes, and enable your site with full video and rich
media capabilities.

Further documentation about the Kaltura module for Drupal can be found here:
http://corp.kaltura.com/wiki/index.php/Kaltura-drupal-module


MAINTAINERS
-----------
The Kaltura Module is developed by: Kaltura, Inc. www.kaltura.com
