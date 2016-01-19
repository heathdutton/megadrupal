ABOUT
-----

The Contact Attach module gives users the ability of attaching files to messages
sent using either the site-wide contact form or the personal contact forms of
users. Allowed extensions, maximum file size and number of attachments can be
defined on a per-role basis for both the site-wide contact form and personal
contact forms.

CREDITS
-------

Original author and developer of the Drupal 5 and 6 branches:
Jason Flatt
drupal@oadaeh.net
http://drupal.org/user/4649

Drupal 7 branch developed by:
Tor Arne Thune
tathune@gmail.com
http://drupal.org/user/290961

INSTALLATION AND CONFIGURATION
------------------------------

In order to use this module, you will need to do the following:
 - Install the module in the usual way.
 - Grant permissions to attach files on contact forms on the permissions page.
   These permissions will be listed in the section headlined by the module name.
 - Configure the settings on the Contact form attachments page
   (admin/config/media/contact_attach). Roles listed here are those that have
   permissions to attach files on contact forms.
 - If the Drupal core File module is enabled a managed file field will be used
   instead of a simple file field.
