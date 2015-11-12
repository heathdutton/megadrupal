INTRODUCTION
------------
Autocompleting addresses is one of the small steps to create a better web
experience. The API of postcode.nl offers autocompletion functionality for
addresses located in The Netherlands. This module integrates this service into
Webform by creating a component that automatically fills in the streetname and
city when the user provides his/her postal code and house number. The service
that responds the address information is offered free of charge to webshops by
postcode.nl.

 * Visit the project page for a full description of this module:
   https://www.drupal.org/project/webformautoaddress

 * Bug reports and feature suggestions can be submitted via:
   https://www.drupal.org/project/issues/webformautoaddress


REQUIREMENTS
------------
This module requires the following modules:
 * Webform: https://www.drupal.org/project/webform
 * Chaos tool suite (ctools): https://www.drupal.org/project/ctools
 * View: https://www.drupal.org/project/views

Due to the dependency to Webform, PHP 5.3 is required.


INSTALLATION
------------
  * Downloading and enabling is like any other module. If you are unfamiliar, a
    complete description on installing modules can be found at
    https://www.drupal.org/documentation/install/modules-themes


CONFIGURATION
-------------
 * On each 'webform node' component(s) can be added of the type 'Address
   (autocomplete)'. More information on how to use Webform can be found at
   https://www.drupal.org/documentation/modules/webform

 * In order for the autocompletion to work, you need to obtain a valid key and
   secret by creating an account at https://api.postcode.nl/. Insert the key and
   secret on the component's edit page.

 * Visit the 'webform node' to see the Webform Auto Address component in action.


MAINTAINERS
-----------
Current maintainer(s):
 * dr. Niels Sluijs (MrWatergate) - https://www.drupal.org/user/1096864

This project has been sponsored by:
 * Sicse
   Specialized in creating and managing dynamic web-solutions. In this way our
   clients are able to offer advanced services such as: e-commerce, social
   networking, video publishing, etc. Visit http://www.sicse.nl for more
   information (in Dutch).
