INTRODUCTION
------------

This module provides utility functions to connect a Drupal site to a CARE CRM system.

Functions include a low-level way to call CARE API methods directly, as well as some
higher-level functions and utilities.


INSTALLATION & CONFIGURATION
----------------------------

1) Install module and enable it.
2) Configure the CARE WSDL, and documentation URLS, at:
   Configuration > Web services > CARE settings

USAGE
-----

This module can run test CARE calls, via the administration menu item:
  Configuration > Web services > CARE settings >  Test CARE call
  
Other than this the module does nothing on its own, but it may be required by other
modules that need to use connections to a CARE server.
 
CONTACT
-------

This module was written by, and is maintained by,
Anthony Cartmell <ajcartmell@fonant.com>