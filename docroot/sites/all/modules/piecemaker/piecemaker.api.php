<?php

/**
 * @file
 * Hooks provided by the Piecemaker module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows modules to define different handlers for the Piecemaker API
 * 
 * @return
 *   An Associtive array with ID keys that contain a callback and an access callback
 */
function hook_piecemaker_handler() {
  return array(
    'example_piecemaker' => array(
      'callback' => 'example_piecemaker_settings',
      'access' => 'example_piecemaker_access',
    ),
  );
}

/**
 * An Example of what a Piecemaker xml array should look like
 * Returns the xml array for the piecemaker example
 * 
 * @param $key
 *   The key from which we would retrieve the settings if this were real
 * 
 * @return
 *   A structured array from which the piecemaker API will build an XML settings file
 */
function example_piecemaker_settings($key) {
  $xml['Settings'] = _piecemaker_default_settings();
  $xml['Transitions'] = variable_get('piecemaker_default_transitions', array());
  $items[] = array(
    '#type' => 'Image',
    '#attributes' => array(
      'Source' => 'https://mail.google.com/mail/u/0/images/2/5/planets/base/gmail_solid_white_beta.png',
      'Title' => 'An Image Node',
    ),
    'Hyperlink' => array(
      '#attributes' => array(
        'URL' => 'http://google.com',
        'Target' => '_parent',
      ),
    ),
    'Text' => 'Test text. Just to show whats up.' 
  );
  $items[] = array(
    '#type' => 'Video',
    '#attributes' => array(
      'Source' => 'https://mail.google.com/mail/u/0/images/2/5/planets/base/gmail_solid_white_beta.png',
      'Title' => 'A Video Node',
      'Width' => '960',
      'Height' => '360',
      'Autoplay' => 'true',
    ),
    'Image' => array(
      '#attributes' => array(
        'Source' => 'http://google.com',
      ),
    ), 
  );
  $items[] = array(
    '#type' => 'Flash',
    '#attributes' => array(
      'Source' => 'https://mail.google.com/mail/u/0/images/2/5/planets/base/gmail_solid_white_beta.png',
      'Title' => 'A Flash Node',
    ),
    'Image' => array(
      '#attributes' => array(
        'Source' => 'http://google.com',
      ),
    ), 
  );
  $xml['Contents'] = $items;
  
  return $xml;
}