IMCE Watermark
http://drupal.org/project/imce_watermark

SUMMARY:
IMCE Watermark is a module for adding watermark for IMCE uploaded images.

REQUIREMENTS:
1. http://drupal.org/project/imce
2. Recommend to use "Imagecache Canvas Actions", submodule of 
   http://drupal.org/project/imagecache_actions module. 
   It isn't required, but recommend for performance issue.

DEMO:
http://demo.nikiit.ru/node/3

INSTALLATION:
Install as usual, see http://drupal.org/documentation/install/modules-themes/modules-7 for further information.

UPGRADING:
No action necessary when upgrading from Drupal 6 to 7.

CONFIGURATION:
1. Visit module configuration page at /admin/config/media/imce/imce_watermark 
   (/admin/settings/imce/imce_watermark for Drupal 6);
2. Select "Manual script watermark" for adding watermark using slow php-script:
  2.1. Upload new file for watermark;
  2.2. Set parameters for this image: vertical/horizontal positions, margins and alpha level values.
3. If "Imagecache Canvas Actions" enabled:
  3.1. Add image style (imagecache preset for Drupal 6), that have effect "Overlay (watermark)";
  3.2. Setup it's settings. 
  3.2. IMCE Watermark settings will show only styles, that have "Overlay (watermark".
4. User with permission "imce watermark control" can control watermark adding at IMCE upload time.
5. Additionally to common settings, pointed to #1, you can set different setup for watermarking
   for each IMCE PROFILES!
   So you can assign different watermark rules for each profiles (e.g.user roles). 
   Make for these the same actions as for #2-#3.

NOTES:
1. Please be warn, this module overwrite existance image with watermarked one, and don't store
   original file.
2. imce plupload supported.
3. Also should apply watermark for thumb images

CONTACTS:
  Nikit - http://nikiit.ru/contact
