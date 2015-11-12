--------------------------------------------------------------------------------
                       Yandex.Disk (API & StreamWrapper)
--------------------------------------------------------------------------------

An implementation of a WebDAV API of the Yandex.Disk cloud storage service
and stream wrapper class to work with users Disks via filesystem functions.

--------------------------------------------------------------------------------
INTRODUCTION
--------------------------------------------------------------------------------

This module integrates your Drupal site with Yandex.Disk storage service
(http://disk.yandex.com). Other modules and site administrators or even usual
users may use uri like the following in place of any paths after authentication
with their Yandex accounts:
 * yandexdisk://yandex_username/path_to_the_file
 * yandexdisk://yandex_username/path_to_the_directory

For a full description of the module, visit the project page:
 * http://drupal.org/sandbox/pingwin4eg/2294871
To submit bug reports and feature suggestions, or to track changes:
 * http://drupal.org/project/issues/2294871

--------------------------------------------------------------------------------
INSTALLATION AND REQUIREMENTS
--------------------------------------------------------------------------------

Install the module as any other Drupal module: download and enable it.

The only requirement is another module which can authenticate site users or at
least a maintenance account (admin) with their Yandex.Disk accounts. You can use
one of the following modules or write your own one for this purpose:
 * HybridAuth Social Login [http://drupal.org/project/hybridauth]:
   Use the latest stable release if possible.
 * Yandex Services Authorization API
   [http://drupal.org/project/yandex_services_auth]:
   Only releases after 2013-10-29 of the module are supported, due to a non
   backward compatible issue in it.

--------------------------------------------------------------------------------
CONFIGURATION
--------------------------------------------------------------------------------

As a site administrator configure one of the modules mentioned above to use
Yandex open authentication. Configure your Yandex app at http://oauth.yandex.com
to use a permission 'Application access to Yandex.Disk' in 'Yandex.Disk WebDAV
API' section.

Authenticate yourself using that module with Yandex and/or let your users sign
in to your site via their Yandex accounts. This way all access tokens are stored
in database for later use with this module.

Then add some view/edit permissions to roles that need access to the service.

CAUTION: USE INTELLIGENTLY. BE CAREFUL OF THAT SOME FUNCTIONALITY MAY DELETE,
         OVERWRITE OR EXPOSE YOUR USERS DISKS DATA.

Now you may use authenticated yandexdisk streams instead of any local path or
any URL or work with them as you like.

--------------------------------------------------------------------------------
INFORMATION FOR DEVELOPERS
--------------------------------------------------------------------------------

If you're going to write your own module on the base of this one, you may find
these methods and functions useful:
 * YandexDiskApiWebdavHelper::forAccount():
   Creates an object to work with one user's storage (a Disk). The argument of
   this function is a Yandex username. Then you can use any public method of
   this object. Main of them I guess would be imagePreview(), publish(),
   unpublish() and quota(). Because other functionality exists in stream wrapper
   class.
 * yandexdisk_token_save():
   Use this if you want to add new disks to the system without any auth module
   that supported by default.
 * yandexdisk_access():
   Use this to check if some site user has a privilege to do some operation on
   some yandexdisk:// uri. You can also implement hook_yandexdisk_access() in
   your module to modify default behavior.
 * yandexdisk_mkdir_recursive() and yandexdisk_copy_recursive():
   Just other helper functions.

--------------------------------------------------------------------------------
PROJECT MAINTAINER
--------------------------------------------------------------------------------

 * Mike Shiyan [http://drupal.org/u/pingwin4eg]
