-- SUMMARY --

The WURFL module is an easy wrapper for the WURFL OnSite PHP APIs.
It can be used as an API for module developers, or themers in order to make adaptive websites.
The module doesn't expose end-user functionality.

See http://scientiamobile.com/onsite for a more detailed explanation.
See the official documentation for the full list of available:
- capabilities (http://scientiamobile.com/wurflCapability)
- APIs (http://wurfl.sourceforge.net/php_index.php)

-- REQUIREMENTS --

* WURFL OnSite API >= 1.5
* Drupal Libraries API module

-- INSTALLATION --

* Install as usual, see https://www.drupal.org/documentation/install/modules-themes/modules-7 for further information.
  Note that installing external libraries is separate from installing this
  module and should happen in the sites/all/libraries directory. See
  http://drupal.org/node/1440066 for more information.

* Download the WURFL PHP API package >= 1.5 (http://scientiamobile.com/downloads)
* Extract the archive and copy the contents to the following location: sites/all/libraries/wurfl-php
* Make sure the main file, Application.php, is located at: sites/all/libraries/wurfl-php/WURFL/Application.php
* Download the WURFL Repository file (wurfl.zip) from http://scientiamobile.com/downloads and copy the contents
  to the following location: sites/all/libraries/wurfl-php/resources
* Check the Drupal Status report page for any errors (/admin/reports/status)
* Go to the WURFL Settings page for more configuration options (/admin/config/system/wurfl)
* Go to the WURFL Test Page to see the module in action (/admin/config/system/wurfl/test)


-- USAGE --

The module exposes an API for developers to use device information within their applications.

The API can be used in two ways:

* Object oriented and specific to WURFL
  calling  wurfl_get_requestingDevice(), returns the $requestingDevice object.
  This object can be queried for device capabilities:
  e.g.: $requestingDevice->getCapability("brand_name") // returns the brandname

* Functional approach, generic (Recommended)
  In order to allow for other frameworks to take over device capability detection the module allows a more generic
   approach. Getting a capability can be done by calling wurfl_devicecapability($capability).
  e.g.: wurfl_devicecapability("brand_name") // returns the brandname


-- CONTACT --

Current maintainers:
* Luca Corbo (lucor) - http://drupal.org/u/lucor
  luca.corbo@scientiamobile.com

This project has been sponsored by:
* ScientiaMobile, Inc
  ScientiaMobile provides the industry’s most accurate and flexible device detection solution.
  We help customers deliver great web experiences and manage the increasingly fragmented mobile device ecosystem.
  ScientiaMobile sells WURFL (Wireless Universal Resource File), a constantly-updated repository that catalogues
  thousands of devices and their capabilities and provides access to them via range of API languages.
  Visit http://scientiamobile.com/ for more information.
