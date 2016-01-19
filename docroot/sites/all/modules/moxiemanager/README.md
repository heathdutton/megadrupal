Moxiemanager
============

The Moxiemanager module integrates the [Moxiemanager library](http://www.moxiemanager.com) with Drupal. The Moxiemanager library can be used to:

* Manage your media files;
* Browse uploaded files on your webserver in file-, image- and WYSIWYG-fields;
* Supports both CKEditor and TinyMCE;
* Edit images files directly with adjustments like cropping, scaling, resizing, etc;
* And much more.


Installation Instructions
-------------------------

1. Buy a **PHP** licence at [http://www.moxiemanager.com/getit/](http://www.moxiemanager.com/getit/).
2. Follow the instructions to download the library.
3. Extract the library in the libraries folder: *sites/all/libraries/moxiemanager*.
4. Copy the folder *DrupalMMAuthenticator*, which is part of this module, to *sites/all/libraries/moxiemanager/plugins*.
5. Download and install the [Libraries module](https://drupal.org/project/libraries). Use these contributed module installation instructions if you're new to this kind of thing: https://drupal.org/documentation/install/modules-themes/modules-7
6. Enable the Moxiemanager module at the Modules list, at admin/modules. This will install all required modules.
7. Set user permissions for the Moxiemanager module by navigation to *admin/people/permissions* in your Drupal installation.
8. Configure the Moxiemanager module by navigating to *admin/config/media/moxiemanager* in your Drupal installation.
9. Check if the installation was successful by navigating to *admin/reports/status* in your Drupal installation.

