<?php

/**
 * @file
 * Defines the validation functions for the ideal connector UI.
 */

/**
 * Validation callback: validate the merchant ID field.
 *
 * @see ideal_advanced_admin_form()
 */
function ideal_advanced_validate_merchant_id($element, &$form_state) {
  if (!$form_state['ideal_config']->numericValidate($element['#value'])) {
    form_error($element, t('Incorrect merchant id supplied'));
  }
}

/**
 * Validation callback: validate the SUB ID field.
 *
 * @see ideal_advanced_admin_form()
 */
function ideal_advanced_validate_sub_id($element, &$form_state) {
  if (!empty($element['#value']) && !$form_state['ideal_config']->numericValidate($element['#value'])) {
    form_error($element, t('Incorrect sub id supplied'));
  }
}

/**
 * Validation callback: validate the private key path.
 *
 * @see ideal_advanced_admin_form()
 */
function ideal_advanced_validate_private_key_path($element, &$form_state) {
  if (!$form_state['ideal_config']->pathValidate($element['#value'])) {
    form_error($element, t('Could not find the private key'));
  }
  else {
    $key_validate = $form_state['ideal_config']->keyValidate($element['#value'], $form_state['values']['private_key_password']);
    if (isset($key_validate['error'])) {
      form_error($element, t('There is an issue with the private key: @error', array('@error' => implode(' ', $key_validate['error']))));
    }
  }
}

/**
 * Validation callback: validate the private certificate path.
 *
 * @see ideal_advanced_admin_form()
 */
function ideal_advanced_validate_private_certificate_path($element, &$form_state) {
  if (!$form_state['ideal_config']->pathValidate($element['#value'])) {
    form_error($element, t('Could not find the private certificate'));
  }
}

/**
 * Validation callback: validate the ideal server url.
 *
 * @see ideal_advanced_admin_form()
 */
function ideal_advanced_validate_ideal_server_url($element, &$form_state) {
  if (!$form_state['ideal_config']->urlValidate($element['#value'])) {
    form_error($element, t('Invalid ideal server url supplied'));
  }
}

/**
 * Validation callback: validate the public certificate path.
 *
 * @see ideal_advanced_admin_form()
 */
function ideal_advanced_validate_public_certificate_path($element, &$form_state) {
  if (!$form_state['ideal_config']->pathValidate($element['#value'])) {
    form_error($element, t('Could not find the public certificate'));
  }
}

/**
 * Validation callback: validate the icon path.
 *
 * @see ideal_advanced_admin_form()
 */
function ideal_advanced_validate_icon_path($element, &$form_state) {
  if (!empty($element['#value']) && !is_readable($element['#value'])) {
    form_error($element, t('Could not find the iDEAL icon'));
  }
}
