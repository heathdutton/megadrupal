Ooyala Uploader
===============

This module integrates the Ooyala Web Uploader with the Ooyala Drupal module.
With this module, videos can be uploaded directly to the Ooyala backlot.
Currently, only modern HTML5 browsers are supported by the Web Uploader,
including Chrome, Firefox, and Safari.

Requirements include:
 * jQuery Update: http://drupal.org/project/jquery_update
 * Libraries: https://drupal.org/project/libraries
 * The Ooyala Backlot Ingestion Library (extract in sites/all/libraries):
   https://github.com/lullabot/backlot-ingestion-library/tarball/master

   * You may have to rename the extracted directory to
     "backlot-ingestion-library"

Once this module is enabled, an additional "Ooyala uploader" widget will be
available for all Ooyala fields. Select that widget when creating new fields or
when updating existing fields.
