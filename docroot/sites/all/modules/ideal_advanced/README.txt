CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting


INTRODUCTION
------------

The iDEAL advanced package contains two modules:
* ideal_advanced. The common ideal API which can be used to connect to the
  the ideal issuer. The ideal_advanced module uses the ideal connector
  library which is supplied by your issuer. You can also download it from
  github: https://github.com/mvdve/ideal_connector. This library is extended
  with Drupal functionality for a easy implementation.

* ideal_advanced_commerce. The commerce ideal module implements the
  ideal_advanced API. The module defines the commerce commerce payment method
  and checkout form. It will also handle the synchronization of ideal
  transaction and commerce payment.


REQUIREMENTS
------------
This module requires the following modules and PHP extensions:
 * Entity API  (https://www.drupal.org/project/entity)
 * Libraries   (https://www.drupal.org/project/libraries)
 * OpenSSL     (http://php.net/manual/en/book.openssl.php)
 * cURL        (http://php.net/manual/en/book.curl.php)

The following modules are not mandatory but they will provide a nice
ideal transaction overview:
 * Views       (https://www.drupal.org/project/views)
 * VBO         (https://www.drupal.org/project/views_bulk_operations)


INSTALLATION
------------
* Download the ideal connector library from your issuer's ideal administration
  page or from github: https://github.com/mvdve/ideal_connector

* Copy the library folder to your drupal libraries folder. It is important that
  the folder is named "ideal_connector". The folder name is case sensitive. For
  example: /sites/all/libraries/ideal_connector

* Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

* Create an ideal icon from 25x25 pixels and place it in your theme or public
  folder.


CONFIGURATION
-------------

* Configure the user permissions in Administration » People » Permissions
  "Administer the iDEAL settings"

* Generate a private certificate and key for the ideal connection with openSSL
  and place them in the private section. For linux users:

  generate key:
  openssl genrsa -aes128 -out priv.key -passout pass:[PWD] 2048

  generate certificate:
  openssl req -x509 -sha256 -new -key priv.key -passin pass:[PWD]
  -days 1825 -out cert.cer

* Create a new iDEAL configuration and set the credentials and
  filepath at te module settings page: Configuration » web services » iDEAL

* Set the filepath from the ideal icon to your chosen location. For example:
  sites/default/files/ideal.gif or sites/all/themes/theme_name/ideal.gif

* Test the connection with the test form.

* One iDEAL payment method will automatically be created on installation.
  Select the iDEAL configuration in the payment method action. Be sure
  that the iDEAL validation condition is added when additional iDEAL payment
  methods are created.

* Run cron once per day or more. You can use the Elysia cron module for the
  most optimum cron performance. It is recommended to schedule cron with an
  external system, for example from the webserver. See:
  https://www.drupal.org/node/23714


TROUBLESHOOTING
---------------

* All errors are logged in to the drupal log. Check this first.
  When no sufficient information is supplied, you can use the debug function.
  This will stop the ideal process at the point of error.
