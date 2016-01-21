INTRODUCTION
------------
This module provides a PCA Predict (formerly Postcode Anywhere) based address
field lookup service. The module relies on the API and PHP interface defined in
the Address Field Lookup module.

SUPPORTED SERVICES
------------------
This module currently supported the following Postcode Anywhere Services

POSTCODEANYWHERE INTERACTIVE FIND (V1.10)
-----------------------------------------
Documentation can be found here:

http://www.pcapredict.com/support/webservice/serviceslist/postcodeanywhere

The following formats are supported:

* JSON
* XMLA

REQUIREMENTS
------------
This module requires the following modules:

  * Address Field Lookup (https://drupal.org/project/addressfield_lookup)
  * Address Field (https://drupal.org/project/addressfield)
  * Chaos tool suite (https://drupal.org/project/ctools)

INSTALLATION
------------
* Enable the Address Field Lookup and Address Field Lookup Postcode Anywhere
  modules.
  - Install as you would normally install a contributed Drupal module. See:
    https://drupal.org/documentation/install/modules-themes/modules-7
    for further information.
* Clear cache.

CONFIGURATION
-------------
Configuration is provided by the Address Field Lookup module itself. Once this
sub-module is enabled you should see a new entry for Postcode Anywhere on the
overview page.

Firstly you need to ensure you have an API key for Postcode Anywhere, to do this
follow these steps:

* Sign up for an account with PCA Predict at this URL
  https://www.pcapredict.com/register/
* Once logged in to your account go to the control panel and click the button
  labelled 'New Service' (https://account.pcapredict.com/#/newservice/)
* Scroll down and choose the 'API Key' option by clicking the 'Create' button.
* You should now have an API key in your 'All Services' panel, copy it.

Now you can navigate to the module configiguration page at the following URL:

admin/config/regional/addressfield-lookup

From here you can click the 'configure' link next to 'Postcode Anywhere' which
will take you to a form where you can configure the Postcode Anywhere
integration. Paste the API key you copied earlier into the 'Licence' field.

The additional field for 'Login' is optional and only required for a
Royal Mail licence.

MAINTAINERS
-----------
Current maintainers:
  * Dan Richards - https://www.drupal.org/user/3157375

This project has been sponsored by:
  * LUSH Digital - https://www.drupal.org/node/2529922
    In order for us to become a company of the future and clear cosmetic leader
    we are putting the internet at the heart of our business. It's time for Lush
    to be at the forefront of digital and revolutionise the cosmetics industry.

    Lush Digital enables technology to connect people throughout our community
    and build relationships bringing customer to the heart of our business.
  * FFW - https://www.drupal.org/marketplace/FFW
    FFW is a digital agency built on technology, driven by data, and focused on
    user experience.

ACKNOWLEDGEMENTS
----------------
Huge thank you to the developers/sponsors of the 'Postcodeanywhere Addressfield'
module (https://www.drupal.org/project/postcodeanywhere_addressfield). This
module is based on the concepts outlined there and without it my life would
have been much harder!

Postcodeanywhere Addressfield developers and sponsors:

  * Yuriy Gerasimov (https://www.drupal.org/u/ygerasimov)
  * Commerce Guys (https://commerceguys.com/)
  * iKOS (http://www.i-kos.com/)
