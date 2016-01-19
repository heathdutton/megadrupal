
IMPORTANT
---------

PHP5 only - at least 5.1.6 - 5.2.x recommended

DESCRIPTION
-----------

This module adds a security layer to Drupal based on PHPIDS (www.php-ids.org).
With a defined set or rules, it tries to detect malicious input from the (anonymous)
user - it does not strip, filter or sanitize the input. It logs directly to watchdog 
or syslog (if enabled), so you have a clear view on who's trying to break your site. 
It can send out a mail after a certain level of impact has been reached or redirect the 
user to another page thus making his action completely worthless. Although the 
functionality is there to redirect users after a certain impact.

INSTALLATION
------------

1) Download the latest PHPIDS package from http://www.php-ids.org.
2) Their are 3 ways to install PHPIDS package on your webserver
2a) Shared Webhosting users: Unpack the tar/zip to the module directory
    of phpids module (example: sites/all/modules/phpids/phpids-0.x).
2b) Shared webhosting users (more secure): Unpack the tar/zip to the root
    directory of your shared webspace (example: /var/www/u3485258/phpids-0.x).
2c) For owner of own webservers: Unpack the tar/zip to a searchable php-library
    directory on your webserver (example: /usr/share/php/phpids-0.x).
3) Create a writable temp folder for phpids for caching the filters (2 ways)
3a) Make sure the phpids-0.x/lib/IDS/tmp folder is writable
    chmod 770 or 777 the phpids-0.x/lib/IDS/tmp folder.
3b) Create a phpids folder in your configured php-tmp-upload directory and
    make sure the created phpids folder is writable (chmod 770 or 777).
4) Add following two lines to your robots.txt file:
    Disallow: /warning.html
    Disallow: /?q=warning.html
5) Activate and configure drupal phpids module. See CONFIGURATION AND TESTING
   
There is a Config.ini in the IDS/Config folder, do not worry, it's harmless, just keep
it like it is.

UPDATE
----------

Please note before and check after update your drupal phpids settings!
Following update paths exists within the new branch of drupal phpids module:

old D5 version
5.x-2.0 -> 5.x-3.x
5.x-2.0 -> 6.x-2.x
5.x-2.0 -> 7.x-1.x

old D6 version
6.x-1.10 -> 6.x-2.x
6.x-1.10 -> 7.x-1.x

CONFIGURATION AND TESTING
-------------------------

After that, enable the module and surf to the settings page on
http://yourdrupal/?q=admin/settings/phpids and change the default
settings to your needs.

You have to set the correct PHP-IDS Path and PHP-IDS Temp Path.
The drupal status report informs you that drupal phpids module is correctly
running or not.

In addition, it is possible to set html-, json-included or exclude fields and variables
from scanning by PHPIDS. So you have the opportunity to finetune PHPIDS to your
website and you could reduce false positives.

Test if PHPIDS starts logging (not as user 1)

* normal log level
  http://yourdrupal/?q=admin/reports/dblog&test=">XXX
* mail level - if you filled in an email
  http://yourdrupal/?q=admin/reports/dblog&test=">XXX<
* warning level - redirects the (anonymous) user
  http://yourdrupal/?q=admin/reports/dblog&test=">XXX<"><script>

You should see the attacks logged in your dblog or in your syslog file.

BUGS, REQUESTS 
--------------

http://drupal.org/project/phpids

TODO / FEATURES
---------------

* Build in white lists (more users/roles etc)
