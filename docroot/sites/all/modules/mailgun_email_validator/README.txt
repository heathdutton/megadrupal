CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Configuration
 * Maintainers

INTRODUCTION
------------

This modules allow you to validated your Email widget field module
and Webform email field through Mailgun Email Validator,which is much more
than regex validation. Mailgun validator is based on real-world data +
RFC spec to ensure more accurate validation.

CONFIGURATION
-------------

After enabling this module, please check admin/config/services/mailgun.
Here you can insert your Mailgun api credential, which will include
api username (usually it would be "api") and public-key.

Check for Open SSL extension enabled.

Configure Email field widget :

 * Install Email field Module.
 * Create cck field for email using email widget.
 * While creating the field, Mailgun email validator enable
   checkbox will be provided within Field setting form.
 * Enabling Mailgun email validator will validate email widget
   field,if invalid email is entered Mailgun will provide you with
   suggested email address right below your email field.

Configure Webform email field :

 * Install Webform Module.
 * Create webform and add email field.
 * While creating the field, Mailgun email validator enable
   checkbox will be provided within webform component setting form.
 * Enabling Mailgun email validator will validate email widget
   field,if invalid email is entered Mailgun will provide you with
   suggested email address right below your email field.

MAINTAINERS
-----------
Current maintainers:
 * Sandeep Kumbhatil - https://www.drupal.org/u/sandeep.kumbhatil
 * Avinash Shukla - https://www.drupal.org/u/nashkrammer
