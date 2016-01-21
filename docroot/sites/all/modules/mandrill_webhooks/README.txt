## SUMMARY


## REQUIREMENTS

* Mandrill module http://drupal.org/projects/mandrill
* Mandrill PHP library (https://bitbucket.org/mailchimp/mandrill-api-php/get/1.0.52.zip)
* Libraries module http://drupal.org/projects/libraries

## INSTALLATION
* You need to have a Mandrill API Key.
* The Mandrill library must be downloaded into your libraries folder. It's
  available at https://bitbucket.org/mailchimp/mandrill-api-php/get/1.0.52.zip
  or by using the included example drush make file.
  Proper libraries structure:
    - libraries/
      - mandrill/
        - docs/
        - src/
          - Mandrill.php
          - Mandrill/
        - LICENSE
        - composer.json

## INSTALLATION NOTES


## CONFIGURATION

Set Mandrill API Key per instructions in Mandrill Module

The mandrill_webhooks module provides an interface to create/edit/delete
Webhooks via the API. It also provides an endpoint where Mandrill can post
data back to the site. This module does not store the posted data, but makes
it available to other modules implementing hook_mandrill_webhook.

Example:

/**
 * Implements hook_mandrill_webhook().
 */
function example_mandrill_webhook($data) {
  // Do something with the $data array here.
}
