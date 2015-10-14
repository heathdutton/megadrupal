# Details
Integrate Drupal with the [Email Address Online Verification API](http://www.email-validator.net). As it is said in the service's website, the validation
process includes the following steps:
* Syntax verification
* DNS validation including MX record lookup
* Disposable email address (DEA) detection
* SMTP connection and availability checking
* Temporary unavailability detection
* Mailbox existence checking
* Catch-All testing
* Greylisting detection

In case the service is unavailable at the moment, this module may use Drupal's
built-in valid_email_address() function, assume a valid email address or assume
an invalid email address. The administrator of the website may change the
default action to use, using the configuration form of this module.

# Installation
Nothing unusual here. Just copy this module's folder in your site's modules
folder (ex. sites/all/modules) and enable the module from the modules' page.

# Configuration
Go to [Email Address Online Verification API](http://www.email-validator.net/email-address-online-verification-api.html) and get your free API key. Then go
to admin/config/people/emailvalidator and paste your API key. Make sure you also
decide what the module should do in case the service is unavailable.

# Integration
This module integrates with the user's registration form and the contact module.
It also validates the fields created by the [Email field widget](https://drupal.org/project/email).
Validation of the *Email field widget* may be skipped per field instance from
the UI using the field settings form.