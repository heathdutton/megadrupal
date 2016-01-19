<?php
/**
 * @file
 * The magic forms api file.
 */

/**
 * Register form field validators.
 *
 * @return array
 *   An array of validators indexed by the id.
 */
function hook_magic_forms_validators() {
  $items = array();

  $items['email'] = array(
    // The title of the validator.
    'title' => t('Email'),

    // The description of the validator.
    'description' => t('A simple email validator.'),

    // The filename to the validator file (optional).
    'file' => 'validators/email.inc',

    // The callback function to call.
    'callback' => 'magic_forms_validators_email_validate',

    // The field types that this validator can be used against (optional).
    'types' => array('textfield', 'emailfield'),
  );

  return $items;
}

/**
 * Alter the registered validators.
 *
 * @param array $validators
 *   An array of validators.
 */
function hook_magic_forms_validators_alter(&$validators) {
  // Alter a validator.
}

/**
 * Alter the magic forms config.
 *
 * @param array $config
 *   The magic forms config.
 */
function hook_magic_forms_config_alter(&$config) {
  $config = array_merge(array(
    'custom-setting' => variable_get('custom-settings', TRUE),
  ), $config);
}

/**
 * Alter the element after processed my magic forms.
 *
 * @param array $element
 *   The element to alter.
 * @param array $config
 *   The magic forms config.
 */
function hook_magic_forms_element_alter(&$element, $config) {
  // Alter the form element.
}

/**
 * Alter the prerendered element after magic forms.
 *
 * @param array $element
 *   The element to alter.
 * @param array $config
 *   The magic forms config.
 */
function hook_magic_forms_element_prerender_alter(&$element, $config) {
  // Do something to alter the prerender state of the element.
}
