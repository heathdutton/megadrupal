eWAY Integration Direct Connection Module 7.x-1.x
=================================================

Drupal Core: 7.x

Maintainers:
* Joseph Z (joseph.zhao@xing.net.au)
* Jason G (jason.guo@xing.net.au)

CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation
* Dependencies


Introduction
------------

This module provides eWAY Direct Connection for use with Drupal Commerce.

The Direct Connection implementation allows for purchases to be submitted directly to eWAY Rapid 3.1.

Please note that Direct Connection is not compatible with 3DSecure.

Installation
------------

Enable the module from admin/modules or use Drush:

drush pm-enable eway_integration_commerce_dc

Dependencies
------------

This module is dependent on following modules:
* eway_integration_commerce
* eway_integration

You will be prompted to enable these modules if they are not already enabled.
