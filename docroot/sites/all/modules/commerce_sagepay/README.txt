CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Known issues
 * Support
 * Sponsorship


INTRODUCTION
------------

This module allows Drupal Commerce customers to pay using the SagePay Form,
Server or Direct implementation.

It supercedes the modules:
Commerce SagePay Form
Commerce SagePay Direct
Commerce SagePay Server
Commerce SagePay Token
Commerce 3D Secure

If you have any of these modules installed, you should fully uninstall them
before installing this module.

INSTALLATION
------------

 1. Copy the 'commerce_sagepay' folder into the modules directory
    usually: '/sites/all/modules/'.

 2. In your Drupal site, enable the module under Administration -> Modules
    The module will be in the group Commerce - Payment.

 3. Enter your SagePay API details under Administration -> Store ->
    SagePay Settings.

    You will need your SagePay vendor name and if you are using Form integration
    you will also need the encryption key that was sent to you when your SagePay
    account was set up.


KNOWN ISSUES
------------

 * SagePay allows the basket to be sent as XML - this feature is not currently
   working.
 * The SagePay simulator does not currently support the V3.0 protocol that this
   module uses, so this option is disabled.


SUPPORT
-------

If the encounter any issues, please file a support request
at http://drupal.org/project/issues/commerce_sagepay


SPONSORSHIP
-----------

This module is sponsored by Sage Pay (http://www.sagepay.com) and developed
by i-KOS (http://www.i-kos.com), a SagePay partner.
