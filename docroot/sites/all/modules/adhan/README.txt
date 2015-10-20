-- SUMMARY --

The adhan module provides a theme-able Drupal block which displaying Islamic
prayer times. It includes (or will in a future release) a compass showing the
Qibla, sun and moon directions. The visitor's location will be determined by
using the Smart IP module.

-- REQUIREMENTS --

 * Device Geolocation (part of Smart IP)

-- INSTALLATION --

 * Download and install Smart IP, by uploading it to sites/all/modules
 * Enable Device Geolocation (Smart IP itself is optional)
 * Install the module by placing it in sites/all/modules
 * Download the DukhoolWaqt class library from http://www.dukhoolwaqt.org
 * Upload it in the module's root folder (sites/all/modules/adhan)
 * Enable the block in Drupal's block administration

-- CONFIGURATION --

Navigate to admin/settings/adhan to change the following settings.

 1. Optimize compass.
This will generate the compass in gif format instead of png, which means the
file size of the compass will be around 9 KB as opposed to 40 KB. Note that the
compass looks slightly better in png format.
