********************
Htaccess
********************

Htaccess is a simple module for Drupal 7.x which autogenerates 
a Drupal root htaccess file based on your settings.

************
Requirements
************

Drupal 7.x

****************************
Installation & Configuration
****************************

1. Copy the entire htaccess directory into
your Drupal site at sites/all/modules directory
and enable the module in the "Administer" -> "Modules",

2. On the settings page, set the options
you want for your htaccess,

3. On the generate page, set a name and
a short description of what you htaccess do.
Then click on generate. A htaccess profile will
be created, based on the settings you set,

4. The "deploy" page list all the htaccess profile you have created.
You can view, delete, deploy or download the htaccess.

******
Notice
******

When deploying a htaccess, the module will try to access into the .htacess
in your root installation. It appears that often, the permissions of the
root .htaccess don't allow the apache user to write in the file. In this
case, you can either update the permissions of the .htaccess in order that
the apache user can write in it or download the htaccess and manually put it
in your root installation or deploy with Drush.

****************************
Drush commands
****************************
drush htaccess-deploy PROFILE_NAME

example: drush htaccess-deploy default
(will deploy the default htaccess shipped with Drupal)

Alias: ht

*******************
Author / Maintainer
*******************
giorgio79
Jean-Baptiste L. (aka, Jibus)
