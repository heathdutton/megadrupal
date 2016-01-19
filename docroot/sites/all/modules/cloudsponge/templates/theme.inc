<?php

/**
 * Preprocessor for theme('cloudsponge_widget').
 */
function cloudsponge_preprocess_cloudsponge_widget(&$vars) {
  cloudsponge_add_js();

  $providers = array_filter(variable_get('cloudsponge_providers', array()));

  $links = array(
    'facebook' => '//www.cloudsponge.com/images/address_books/facebook.png',
    'linkedin' => '//www.cloudsponge.com/images/address_books/linkedin.png',
    'yahoo' => '//www.cloudsponge.com/images/address_books/yahoo.png',
    'windowslive' => '//www.cloudsponge.com/images/address_books/msn.png',
    'gmail' => '//www.cloudsponge.com/images/address_books/google.png',
    'aol' => '//www.cloudsponge.com/images/address_books/aol.png',
    'plaxo' => '//www.cloudsponge.com/images/address_books/plaxo_button.png',
    'addressbook' => '//www.cloudsponge.com/images/address_books/apple.png',
    'outlook' => '//www.cloudsponge.com/images/address_books/microsoft.png',
  );

  $vars['providers'] = array();
  foreach ($providers as $provider) {
    $vars['providers'][$provider] = $links[$provider];
  }

  $settings = array(
    'redirect' => (bool)variable_get('cloudsponge_redirect', FALSE),
    'redirect_url' => url(token_replace(variable_get('cloudsponge_redirect_url', ''))),
    'skip_choosing' => (bool)variable_get('cloudsponge_skip_choosing', FALSE),
  );

  drupal_add_js(array('cloudsponge' => $settings), 'setting');
}
