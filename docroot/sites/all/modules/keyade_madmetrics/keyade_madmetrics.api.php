<?php

/**
 * @file
 * API and hooks documentation for the Keyade Madmetrics module.
 */

/**
 * Alter GTM Data Layer variables before they are added to the page.
 *
 * @param array $variables
 *   An array of GTM Data Layer variables to be added to the page.
 * @param string $event
 *   A triggered event for which the variables are being added.
 *   Could be user_register, newsletter_signup or checkout_complete.
 */
function hook_keyade_madmetrics_generate_datalayer_alter(&$variables, $event) {
  if ($event == 'user_register') {
    $account = user_load($variables['userId']);
    $variables['userLanguage'] = $account->language;
  }
}
