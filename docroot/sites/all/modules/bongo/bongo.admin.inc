<?php
/**
 * @file
 * Bongo administrative page.
 */

/**
 * Administrative settings form.
 */
function bongo_settings_form() {
  $form['bongo_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Bongo API key'),
    '#description' => t('Your Bongo API key. If you do not have one, you may !request.', array('!request' => l(t('Request an API key'), BONGO_API_KEY_REQUEST))),
    '#default_value' => variable_get('bongo_api_key', ''),
    '#element_validate' => array('_bongo_api_key_validate'),
  );
  return system_settings_form($form);
}

/**
 * Bongo API key validation.
 *
 * The Bongo key must be alphanumeric.
 */
function _bongo_api_key_validate($element, &$form_state) {
  if (!ctype_alnum($element['#value'])) {
    form_error($element, t('This field must only consist of alphanumeric
      characters.'));
  }
}
