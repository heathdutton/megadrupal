-- SUMMARY --

Mobile AR Connector is primarily for use with the iOS Augmented Reality 
Framework "PRAugmentedReality" (see project page for more info) or any
other external caller who wishes to get/access geolocated data.

It provides a custom resource for Drupal Services and serves 3 purposes:

1. Find relevant data for the calling device:
   --> All content types with field(s) of type "geofield"
   --> All nodes of those content types updated since the last check
  
2. Filter the data leaving only a few specified fields
   --> 4 fields are specified by default: nid, title, coordinates, address

3. Send back the data to the calling device in a mobile-friendly format
   --> Without extra nesting of arrays

The module only contains one file: mobile_ar_connector.module


-- API HOOKS --

This module implements the drupal_alter hook. It allows for the addition of
fields to what is sent back to the device.
It only parameter, "data" is an array of field names. To add your own fields
to the list, append them to the array as such:

array_push($data, array('your_first_field', 'your_second_field'));


-- DEPENDENCIES --

This modules requires the following dependencies:
- services: http://drupal.org/project/services
- geofield: http://drupal.org/project/geofield
- geophp: http://drupal.org/project/geophp
- addressfield: http://drupal.org/project/addressfield


-- CONTACT --

Current maintainer: 
* Geoffroy Lesage (glesage) - http://drupal.org/user/54136

This project has been sponsored by:
* Promet Solutions Inc. - http://drupal.org/node/1130798
