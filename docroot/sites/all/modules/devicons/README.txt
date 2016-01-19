********************************************************************
                      D R U P A L    M O D U L E
********************************************************************
Name: Devicons
Author: medienverbinder
Contact: http://drupal.org/user/1293712
Drupal: 7
********************************************************************

INTRODUCTION
------------

This Module makes the Devicons Font available to Drupal as a library.


INSTALLATION
------------

Libraries API 2

Download and install the Libraries API 2 module.
Move the downloaded module package to sites/all/modules/.
  => sites/all/modules/libraries

In administration backend on the module configuration (admin/modules)
page you have now to install/activate the module and save the module
configuration.


Download Devicons
-----------------

=> https://github.com/vorillaz/devicons/

Move the font to the directory "devicons" and place it inside 
  => "sites/all/libraries"

Make sure the path to the Devicons folder becomes
  => "sites/all/libraries/devicons/"

CSS file example:
Example: "sites/all/libraries/devicons/css/devicons.css"


Drush - Download
----------------

If you have Drush installed:

Download Devicons from project page:
(git required)
drush dl-devicons

List library information by running:
drush libraries-list
Module Development

The global Integration of devicons can be enabled/disabled in the backend.
To load the Devicons manually from inside your own module, simply do:

libraries_load('devicons');


Devicons Drupal Module
------------------------

Download and install the Devicons module.
 Move the downloaded module package to:
=> sites/all/modules/devicons

In administration back-end on the module configuration (admin/modules)
page you have now to install/activate the module and save the module
configuration.


Notice
------

The Drupal status report provides information on the correct installation
of the Devicons. (/admin/reports/status)


Activating
----------

After activation of the module you can Enable Devicons for the
whole page (hook_init()).

=> /admin/config/devicons/settings

 Now you can visit the demo page to take a look at the Devicons.

=> /admin/config/devicons/demo


CONTACT
-------

Making amazing Drupal based web applications for radio stations and media companies is my passion.
For further information or if you have any questions please do not hesitate to contact me:
Visit my website: http://medienverbinder.de
Follow me on Twitter: http://twitter.com/medienverbinder
Visit my Drupal profile: https://www.drupal.org/u/medienverbinder
