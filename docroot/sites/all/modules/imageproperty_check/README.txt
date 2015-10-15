Imagesize check module

About
-----
This module notifies administrator of all the images which exceed the maximum
memory size(in Kbs) each image preset could have.

Requirement
-----------
Performance and Scalability module(pasc) in sites/all/module directory.


Installation instructions
-------------------------
1.Download and extract the Performance and Scalability module(pasc) into the
modules directory (usually "sites/all/modules").
   Link: http://drupal.org/project/pasc
   Drush users can use the command "drush dl pasc".

2.Download and extract the Imagesize check module(imagesize_check) into the
modules directory (usually "sites/all/modules").
   Link: https://drupal.org/sandbox/neetumorwani/2086453
   Drush users can use the command "drush dl imagesize_check".

3. Go to "Administer" -> "Modules" and enable the module.


Configuring the module
----------------------
- For module configuration settings, go to
"Administer" -> "Reports" -> "Image Glitches" -> "Enter the values for the
maximum memory size(in Kbs) an image preset could have".
Url is - /admin/reports/imagesize-check-errors/imagesize-check-settings
- List of all the images which exceed the maximum memory size limit of each
image preset could have can be viewed on the page
"Administer" -> "Reports" -> "Image Glitches".
Url for this is - /admin/reports/imagesize-check-errors
- For HTML mail module configuration settings, go to
"Administer" -> "Configuration" -> "System" -> "Mail System" ,Under 'New
Settings section' add the name of the module as 'imagesize_check' and key as
'imagesize_check_mail_key'. Save the settings.
- Again on the same page,Choose "HTML Mail System" for 'imagesize_check module
key'.
- Page - admin/reports/imagesize-check-errors/aspect-ratio
List of all the images which are not suitable for the specific image style
applied to them.
- This page -> admin/reports/imagesize-check-errors/pager-cron-config lists
has the Pager and Cron configuration for the module.
Cron run time can be configured in hours.
Pager Configuration allows us to control the number of images reported on the
page "Image size Gliches" and "Image Glitches Aspect Ratio". Accordingly pager
will be introduced.
