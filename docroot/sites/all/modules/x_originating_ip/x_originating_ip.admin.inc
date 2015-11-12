<?php

/**
 * @file
 * x_originating_ip.admin.inc
 */


/**
 * Build a form body for the configuration settings.
 */
function x_originating_ip_settings() {
  $form = array();

  $headers = _x_originating_ip_headers();

  $form['x_originating_ip_header'] = array(
    '#type' => 'radios',
    '#title' => t('Email origin header'),
    '#default_value' => variable_get('x_originating_ip_header', 'X-Originating-IP'),
    '#options' => $headers,
    '#element_validate' => array('_x_originating_ip_valid_header'),
    '#description' => t('Though Microsoft made the X-Originating-IP header popular with Hotmail, various development how-to documents have proposed alternative headers listed here.'),
  );

  return system_settings_form($form);
}

/**
 * Verify the header element is a valid option.
 */
function _x_originating_ip_valid_header($element, &$form_state) {
  if (!in_array($element['#value'], _x_originating_ip_headers())) {
    form_error($element, t('The @field field should be a valid email header.', array('@field' => $element['#title'])));
  }
}
