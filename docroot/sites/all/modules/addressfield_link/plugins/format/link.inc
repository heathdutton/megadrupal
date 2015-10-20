<?php

/**
 * @file
 * The default format for adresses.
 */

$plugin = array(
  'title' => t('Link'),
  'format callback' => 'addressfield_format_link_generate',
  'type' => 'link',
  'weight' => 100,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_format_link_generate(&$format, $address, $context = array()) {
  if ($context['mode'] == 'form') {
    $format['link_block'] = array(
      '#type' => 'addressfield_container',
      '#attributes' => array('class' => array('addressfield-container-inline')),
      '#weight' => 200,
      // The addressfield is considered empty without a country, hide all fields
      // until one is selected.
      '#access' => !empty($address['country']),
    );
    $format['link_block']['link_title'] = array(
      '#title' => t('Link title'),
      '#size' => 15,
      '#attributes' => array('class' => array('link-title')),
      '#type' => 'textfield',
      '#tag' => 'span',
      '#default_value' => isset($address['link_title']) ? $address['link_title'] : '',
    );
    $format['link_block']['link_url'] = array(
      '#title' => t('Link URL'),
      '#size' => 30,
      '#attributes' => array('class' => array('link-url')),
      '#type' => 'urlfield',
      '#tag' => 'span',
      '#default_value' => isset($address['link_url']) ? $address['link_url'] : '',
    );
  }
  else {
    // Add our own render callback for the format view
    $format['#pre_render'][] = '_addressfield_link_render_address';
  }
}
