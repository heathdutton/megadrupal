MODULE
------
IP to Location

Contents of files:
------------------

  * Installation
  * Configuration

REQUIREMENTS
------------
Drupal 7.0


DESCRIPTION/FEATURES
--------------------
The IPtoLocation module uses a visitor’s IP address and enables you to identify
his/her geographical location. This module tracks visitor's geographical
location (longitude/latitude), country, region and city based on the IP address
of the visitor. This provides the developers functionalities to show location
of the Guest/Login users across the website or portal. 
 
The location information is stored at session variable ($_SESSION) with an
array key ‘iptolocation’. It also provides a feature to perform your own IP
lookup and admin spoofing of an arbitrary IP for testing purposes.


Installation:
-------------
1. Copy ip folder to modules (usually 'sites/all/modules') directory.
2. At the 'admin/modules' page, enable the IP address manager module.

Configuration:
--------------
At the 'admin/config/people/iptolocation' page enable Ip location test
configuration and cache.


CREDITS
--------

This module was created by OSSCube Solutions Pvt Ltd <www dot osscube dot com>
Guided by Bhupendra Singh <bhupendra at osscube dot com>
Developed by Radhey Shyam <radhey at osscube dot com>
