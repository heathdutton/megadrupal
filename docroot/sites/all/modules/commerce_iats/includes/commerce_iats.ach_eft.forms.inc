<?php
/**
 * @file
 * Forms for use in ACH/EFT payment processing.
 */

/**
 * Returns a set of bank account form elements for USD currency transactions.
 *
 * @see commerce_iats_ach_eft_payment_form
 */
function commerce_iats_ach_eft_payment_form_usd($fields = array(), $default = array()) {
  $fields['routing_number'] = '';
  $fields['type'] = '';

  return commerce_iats_ach_eft_payment_form($fields, $default);
}

/**
 * Returns a set of bank account form elements for CAD currency transactions.
 *
 * @see commerce_iats_ach_eft_payment_form
 */
function commerce_iats_ach_eft_payment_form_cad($fields = array(), $default = array()) {
  $fields['bank_number'] = '';
  $fields['transit_number'] = '';
  $fields['type'] = '';

  return commerce_iats_ach_eft_payment_form($fields, $default);
}

/**
 * Returns a set of bank account form elements for GBP currency transactions.
 *
 * @see commerce_iats_ach_eft_payment_form
 */
function commerce_iats_ach_eft_payment_form_gbp($fields = array(), $default = array()) {
  $fields['sort_code'] = '';

  return commerce_iats_ach_eft_payment_form($fields, $default);
}

/**
 * Returns a set of bank account form elements.
 *
 * @param array $fields
 *   An array specifying the bank account fields that should be included
 *   on the form.
 * @param array $default
 *   An array of default values for the available bank account fields.
 *
 * @return array
 *   An array of form elements.
 */
function commerce_iats_ach_eft_payment_form($fields = array(), $default = array()) {
  $form = array(
    'bank_account' => array(
      '#tree' => TRUE,
    ),
  );

  if (isset($fields['owner'])) {
    $form['bank_account']['owner'] = array(
      '#type' => 'textfield',
      '#title' => t('Account owner'),
      '#default_value' => isset($default['owner']) ? $default['owner'] : NULL,
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 64,
      '#size' => 32,
    );
  }

  /**
   * Routing number used for USD currency only.
   * 9 digits.
   */
  if (isset($fields['routing_number'])) {
    $form['bank_account']['routing_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Routing number'),
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 9,
      '#size' => 16,
    );
  }

  /**
   * Bank number used for CAD currency only.
   * 3 digits.
   */
  if (isset($fields['bank_number'])) {
    $form['bank_account']['bank_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Bank number'),
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 3,
      '#size' => 8,
    );
  }

  /**
   * Transit number used for CAD currency only.
   * 5 digits.
   */
  if (isset($fields['transit_number'])) {
    $form['bank_account']['transit_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Transit number'),
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 5,
      '#size' => 16,
    );
  }

  /**
   * Sort code used for GBP currency only.
   * 6 digits in the format XX-XX-XX.
   */
  if (isset($fields['sort_code'])) {
    $form['bank_account']['sort_code'] = array(
      '#type' => 'textfield',
      '#title' => t('Sort code'),
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 8,
      '#size' => 16,
    );
  }

  if (isset($fields['number'])) {
    $form['bank_account']['number'] = array(
      '#type' => 'textfield',
      '#title' => t('Account number'),
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 64,
      '#size' => 32,
    );
  }

  if (isset($fields['type'])) {
    $form['bank_account']['type'] = array(
      '#type' => 'select',
      '#title' => t('Account type'),
      '#options' => commerce_iats_ach_eft_account_types(),
      '#default_value' => isset($default['type']) ? $default['type'] : NULL,
      '#required' => TRUE,
    );
  }

  return $form;
}

/**
 * Validates a set of bank account details entered via the ACH/EFT form.
 *
 * @param array $details
 *   An array of bank account details as retrieved from the account array in
 *   the form values of a form containing the ACH/EFT form.
 * @param array $settings
 *   Settings used for calling validation functions and setting form errors:
 *   - form_parents: an array of parent elements identifying where the ACH/EFT
 *     card form was situated in the form array
 *
 * @return bool
 *   TRUE or FALSE indicating the validity of all the data.
 *
 * @see commerce_iats_ach_eft_payment_form()
 */
function commerce_iats_ach_eft_payment_form_validate($details, $settings) {
  $prefix = implode('][', $settings['form_parents']) . '][';
  $valid = TRUE;

  if (isset($details['routing_number'])) {
    if (strlen($details['routing_number']) != 9) {
      form_set_error($prefix . 'routing_number', t('Please enter a nine digit routing number.'));
      $valid = FALSE;
    }
  }

  if (isset($details['bank_number'])) {
    if (strlen($details['bank_number']) != 3) {
      form_set_error($prefix . 'bank_number', t('Please enter a three digit bank number.'));
      $valid = FALSE;
    }
  }

  if (isset($details['transit_number'])) {
    if (strlen($details['transit_number']) != 5) {
      form_set_error($prefix . 'transit_number', t('Please enter a five digit transit number.'));
      $valid = FALSE;
    }
  }

  if (isset($details['sort_code'])) {
    $sort_code = commerce_iats_format_sort_code($details['sort_code']);
    if (strlen($sort_code) != 6) {
      form_set_error($prefix . 'sort_code', t('Please enter a six digit sort code.'));
      $valid = FALSE;
    }
  }

  $account_types = commerce_iats_ach_eft_account_types();
  if (isset($details['type'])) {
    if (!isset($account_types[$details['type']])) {
      form_set_error($prefix . 'type', t('Please choose a bank account type from the list.'));
      $valid = FALSE;
    }
  }

  return $valid;
}

/**
 * Returns an associative array of valid account types for ACH/EFT payments.
 *
 * @return array
 *   Array of valid bank account types.
 */
function commerce_iats_ach_eft_account_types() {
  $types = array(
    'CHECKING' => 'Checking',
    'SAVING' => 'Saving',
  );
  return $types;
}
