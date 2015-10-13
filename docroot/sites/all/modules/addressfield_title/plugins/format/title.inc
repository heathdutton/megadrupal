<?php

/**
 * @file
 * The default format for adresses.
 */

$plugin = array(
  'title' => t('Title'),
  'format callback' => 'addressfield_format_title_generate',
  'type' => 'title',
  'weight' => -99,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_format_title_generate(&$format, $address, $context = array()) {
  if ($context['mode'] == 'form') {
    // Create the list of prefixes.
    $prefixes = array();
    $prefixe_values = explode(' ', variable_get("addressfield_title_prefix", "Miss Mrs Ms Mr Dr Prof Rev Lady Lord Sir"));
    foreach ($prefixe_values as $prefixe_value) {
      $prefixes[$prefixe_value] = t($prefixe_value);
    }

    $format['person_title'] = array(
      '#type' => variable_get('addressfield_title_form_element', 'select'),
      '#empty_option' => t('Please choose'),
      '#options' => $prefixes,
      '#title' => t('Title'),
      '#required' => TRUE,
      '#tag' => 'span',
      '#attributes' => array('class' => array('person-title')),
      '#default_value' => isset($address['person_title']) ? $address['person_title'] : '',
      '#weight' => -200,
    );
  }
  else {
    // Add our own render callback for the format view.
    $format['#pre_render'][] = '_addressfield_title_render_address';
  }
}



