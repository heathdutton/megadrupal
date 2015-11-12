README.txt

This module collects site-wide contact information and displays it in a block
that you can place anywhere on your site.

Contact information that you provide will be displayed on the site in the hCard
microformat (http://microformats.org/wiki/hcard). An hCard is a small bundle of
code that you want to put on your web site so that Google Maps (and other
mapping services) can more easily index the site's location information.


INSTALLATION
============
- Once downloaded, enable the module at admin/modules
- Enter your contact information at admin/config/system/contactinfo
- Place the 'Contact information' block into one of your theme's regions at
  admin/structure/block


RECOMMENDED MODULES
===================

- Invisimail (http://drupal.org/project/invisimail)
  Email addresses within the hCard will be obfuscated if the Invisimail module
  is installed.


THANKS
======
Thanks to Shawn DeArmond, author of the hCard module on drupal.org which
inspired this module.


AUTHOR/MAINTAINER
======================
- dboulet (Daniel Boulet)
