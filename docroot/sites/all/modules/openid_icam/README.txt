OpenID ICAM Extension for Drupal
================================

Description:
------------
This module extends Drupal's core OpenID support to meet the requirements of
the GSA/ICAM OpenID 2.0 Profile.

US Government agencies wishing to accept OpenID logins MUST

#1: only accept credentials from Certified Identity Providers (must be
implemented using an identity chooser, list of allowed OpenID URLs, etc)
#2: and web sites and applications accepting these credentials must be
configured to use the GSA/ICAM OpenID profile. 

This module provides both of these enhancements.

In its default configuration, it will ensure that the OpenID transaction
meets the requirements specified in the profile and will do the following:

- Validate the user enters an IdP URL that is from the ICAM approved list
  of providers (the list itself in set in the module configuration page)
- Alter OpenID forms to use a new return_to URL (openid/authenticate_icam
   instead of openid/authenticate)
- Publish the new OpenID return_to URL in an XRDS document as
   http://specs.openid.net/auth/2.0/return_to (required for RP discovery), 
   using XRDS_Simple module
- Adds PAPE parameters to the OpenID request for preferred_auth_policies 
   and/or max_auth_age.   
- Validates that the OpenID response also meets the ICAM requirements:  
    - The OpenID Provider (OP) response was using a secure transport 
    - All of the PAPE required_auth_policies in the request were returned 
       by the OP.
- The module is fully configurable, including the ability to disable or 
   enable each of the above features, and to log OpenID Request/Response 
   for troubleshooting.


Some caveats:
-------------

-The module is currently using a hard-coded list of ICAM approved providers,
  which was current at the time of the last stable release.  You may
  wish to periodically check the provider list for changtes at:
   
    http://openidentityexchange.org/certified-providers  
  
  An initiaive is underway to fetch this information automatically, once it
  is made available from a trusted public source.  Expect this functionality
  in a future version of the module.

-In addition to the extra checks this module is adding, be aware that the 
  OpenID Provider (e.g. Google) is going to see the PAPE requests and perform
  extra checks of its own.  Until everything is exactly right, you may see 
  Invalid Page errors and the like on the OP and the login will fail.  The 
  most common cause of this is that you are not using a VALID (not 
  self-signed) HTTPS cert.  Your website will not pass discovery if 
  it is not using SSL!


For more information: 
---------------------

see: "GSA/ICAM OpenID" at: http://idmanagement.gov/pages.cfm/page/IDManagement-support-ICAM-adopted-schemes

The ICAM OpenID 2.0 Profile: http://www.idmanagement.gov/documents/ICAM_OpenID20Profile.pdf

OpenID PAPE 1.0: http://openid.net/specs/openid-provider-authentication-policy-extension-1_0.html


Initial development graciously sponsored by:  
--------------------------------------------
Mobio Technologies Inc.  http://www.mobio.net


Support:
--------
See the project page at: [TBD] http://drupal.org/project/openid_icam


Installation:
-------------
- Install and enable the latest stable release of XRDS Simple
   (http://drupal.org/project/xrds_simple)
- Place this module folder in your modules directory
- Enable the module in the modules list
- Visit the configuration page at admin/settings/openid_icam (see below)

Configuration:
--------------
- The module comes pre-configured to work "out of the box".  You should 
   probably not touch any of the settings, unless you know why you are 
   doing so.
- If this is your first install, turn on the debugging options at the bottom!
  For production sites, make sure they are turned off!
- If you want to force the user to authenticate each time at the OP, you can 
   leave the PAPE max_auth_age parameter to 0.
- If you want to allow cached logins at the OP (so if the user is already 
   signed into Google, they don't have to type their pasword again), you can 
      set the max_auth_age to a higher value.  For example, a value of 300
      says any login that is less than 5 minutes old is acceptible.

Getting it working:
-------------------
- Start by trying an OpenID login to Google.  Use this OpenID URL:  
   https://www.google.com/accounts/o8/id 

- Google will perform its own RP Discovery on your site.  You will see an 
   Invalid Page error on the google side if your site does not pass discovery.
   The most common reason for this is your site running over regular HTTP instead
   of HTTPS, or your SSL certificate being self signed.  Google may fail your
   site on either of these resulting in the invalid page.

- If you've checked everything and Google still won't let you login, make sure
   you have Clean URLs configured and turned on.

- If you've checked everything and you have Clean URLs and it's still not 
   working, make sure you have not set $base_url in settings.php, or if you 
   have, that it's the correct, valid cert HTTPS URL!

- If you've checked everything and you have Clean URLs and $base_url is 
   commented out in settings.php, you will have to inspect the OpenID login 
   form on your site and see what the "openid.return_to" parameter is set to.  
   This MUST be the correct https:// URL to your site, so if it's being overriden
   somewhere else, you will have to track that down.

...When everything is perfect, you will no longer see the Invalid page error
on the google side - instead you will get a login prompt or maybe just a 
confirmation page if you are already logged in, then return to your site.  


