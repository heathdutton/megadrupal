Make File
=============

File entity representation for a Drush make file. It adds the property information when uploading a .make file to Drupal and adds special treatment of that file (similarly to how images have special upload properties).

This does not allow you to export files or define files with Drush nor does it generate a Drush make file. It only exposes the data within an uploaded Drush make file to Drupal through Entity properties.

See http://drush.ws/docs/make.txt for a full description of the make file.

Usage
-----

Enable the module

- Go do admin/content/file
- Click on "Add File"
- Upload a drush make file
- Enter a human name and machine name for the file
    * The properties such as "core" and "api" should be prefilled with the settings from the make file
    * Note that unlike Drush make itself, external make files are not downloaded and agregated automatically
- Save the file

When you load the file entity, you'll find the full list of projects and libraries defined in the make file. When using EntityMetadataWrappers the data for those properties is also available.