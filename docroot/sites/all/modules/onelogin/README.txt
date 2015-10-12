# OneLogin SAML Drupal Integration

This module integrates OneLogin's PHP SAML library with Drupal, allowing Drupal
user authentication and user auto-provisioning via OneLogin.

---------------------------------------

### Requirements

* Libraries API (version 2.0 or higher): https://drupal.org/project/libraries
* OneLogin PHP SAML library (v1.0.0): https://github.com/onelogin/php-saml/releases
* A valid OneLogin account

---------------------------------------

### Installation instructions

1. Download and enable this module and all of its dependencies,
2. If one does not exist, create a libraries folder (e.g /sites/all/libraries),
3. Download and extract OneLogin's PHP SAML library to a folder called onelogin,
4. Place this folder within the libraries folder (/sites/all/libraries/onelogin)
5. Check "OneLogin SAML library" installation status at /admin/reports/status
6. Ensure that the "Administer OneLogin" permission is given to anyone who will
   be configuring this OneLogin Drupal module.

---------------------------------------

### Minimum configuration instructions

1. In your OneLogin account, add a new Drupal App.
2. For this app, in the "Consumer URL" field, enter the following, where
   "example.com" is your domain: http://example.com/onelogin_saml/consumer
3. Login to your Drupal website as you normally would to configure this module.
4. Navigate to /admin/config/people/onelogin
5. In the certificate field, paste in your OneLogin X.509 certificate.
6. In the SAML Login URL field, enter the login URL from the app you created
   and configured in OneLogin (e.g. https://app.onelogin.com/saml/signon/1234)

---------------------------------------

### Additional configurations

__Automatic Drupal user provisioning__
In addition to the above, this module provides a basic mechanism for automatic
Drupal user provisioning. This works by automatically creating Drupal users when
a valid SAML response is provided by OneLogin, but a corresponding user does not
yet exist.

This functionality can be enabled on the aforementioned configuration page. New
users will have a name corresponding to the e-mail address provided by OneLogin,
except with the "@" sign replaced with a period.

In some situations, you may wish to limit user auto-provisioning to a certain
subset of e-mail addresses; this can be done via regular expression matching.

__Requiring authentication via OneLogin__
In cases where your Drupal application serves as an Intranet or other internal
portal where it makes sense to lock down access to authenticated users, you can
check the "Require authentication via OneLogin globally" box. This will ensure
that all un-authenticated requests to your user app are sent to OneLogin for
authentication.

If you wish to conditionally restrict when authentication via OneLogin is
required, you can install the Context module, and use the "Require
authentication via OneLogin" context reaction for whatever conditions you deem
appropriate.

In both cases, if the user is already authenticated to OneLogin, they will never
see a form; they'll simply be logged in automatically.

---------------------------------------

### Reporting issues and contributing

Any issues should be reported to the drupal.org issue queue for this module:
https://drupal.org/project/issues/onelogin
