************
*  README  *
************

DESCRIPTION
------------

This module provides a form to register to addemar via REST method.

INSTALLATION
-------------

1. Place the entire addemar_subscription directory into your Drupal
  "sites/all/modules/" directory.
2. Enable the addemar_subscription module by navigating to:
  "administer > modules"

FEATURES
---------

  * a block to place into a region
  * a page located under "newsleter/subscribe"
  * a configuration page is located under
        "admin/config/services/addemar_subscription"
    where you can define:
    - REST settings
      - REST Url
      - REST Token
      - REST Profile
      - REST Method
      - REST Message
    - General settings
      - Group ID
      - Source
      - Type of subscription
    - Form settings
      - Addemar fields
      - Page url
      - Submit value
    - Message settings
      - Success
      - Error


AUTHOR
-------
Gigot Frédéric
http://drupal.org/user/1269974
