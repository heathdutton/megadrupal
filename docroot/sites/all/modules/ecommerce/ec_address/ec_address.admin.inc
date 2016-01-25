<?php

/**
 * @file
 * Address administration screens.
 */

/**
 * Insert the address fields to the form.
 *
 * @param $edit
 *   array(optional), the current form array
 *
 * @ingroup form
 */
function ec_address_form($form, $form_state, $uid = NULL, $aid = NULL) {
  global $user;

  $address = array(
    'firstname' => '',
    'lastname' => '',
    'street1' => '',
    'street2' => '',
    'zip' => '',
    'city' => '',
    'state' => '',
    'country' => variable_get('ec_country', 0),
  );

  if (isset($aid)) {
    $address = db_query('SELECT * FROM {ec_address} WHERE aid = :aid', array(':aid' => $aid))->fetchAssoc();
  }

  module_load_include('inc', 'ec_store', 'ec_store');
  drupal_add_js(drupal_get_path('module', 'ec_address') . '/js/address.js');

  $form = ec_store_address_form($address);

  if (isset($uid)) {
    $form['uid'] = array(
      '#type' => 'value',
      '#value' => $uid,
    );
  }

  if (!empty($address['aid'])) {
    $form['aid'] = array(
      '#type' => 'value',
      '#value' => $address['aid'],
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update address'),
      '#weight' => 9,
    );
    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#weight' => 10,
      '#submit' => array('ec_address_form_cancel'),
    );
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create new address'),
      '#weight' => 9,
    );
  }

  $form['#prefix'] = '<div class="address-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Validate an address being saved
 */
function ec_address_form_validate(&$form, &$form_state) {
  if ( variable_get('store_ignore_state', FALSE)) {
    return;
  }

  if (($info = ec_store_get_region_info($form_state['values']['country'])) && empty($info['states'][drupal_strtoupper($form_state['values']['state'])])) {
    form_set_error('state', t('Please select a state.'));
  }
}

/**
 * Submit an address being saved
 */
function ec_address_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['aid'])) {
    drupal_write_record('ec_address', $form_state['values'], 'aid');
  }
  else {
    drupal_write_record('ec_address', $form_state['values']);
  }

  drupal_set_message(t('The address has been saved.'));
  $form_state['redirect'] = "user/{$form_state['values']['uid']}/address";
}

/**
 * Don't update the address and send the user back to the previous page.
 */
function ec_address_form_cancel(&$form, &$form_state) {
  $form_state['redirect'] = "user/{$form_state['values']['uid']}/address";
}

/**
 * Delete address from the system
 */
function ec_address_delete_confirm($form, $form_state, $uid, $aid) {
  if ($address = db_query('SELECT * FROM {ec_address} WHERE uid = :uid AND aid = :aid', array(':uid' => $uid, ':aid' => $aid))->fetchAssoc()) {
    $form = array();
    $form['uid'] = array(
      '#type' => 'value',
      '#value' => $uid,
    );
    $form['aid'] = array(
      '#type' => 'value',
      '#value' => $aid,
    );

    return confirm_form($form, t('Do you really want to delete this address'), 'user/' . $uid . '/ec_address', theme('formatted_address', array('address' => $address)), t('Delete Address'));
  }
  else {
    drupal_not_found();
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_address_delete_confirm_submit(&$form, &$form_state) {
  db_delete('ec_address')
    ->condition('aid', $form_state['values']['aid'])
    ->execute();
  drupal_set_message(t('Address deleted.'));
  $form_state['redirect'] = 'user/' . $form_state['values']['uid'] . '/ec_address';
}
