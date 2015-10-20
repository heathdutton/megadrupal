<?php

/**
 * @file
 * Generates the iframe code for the remote registration on Simplenews.
 *
 * Date: 21-nov-2013
 * File: simplenews_iregister.admin.inc
 * Author: Callegari Stefano
 */

/**
 * Settings iframe key.
 *
 * SC Thu Nov 21 23:51:18 CET 2013 stefano
 *
 * @param array $form
 *        form data
 * @param array $form_state
 *        form data
 *
 * @return array the form structure
 */
function simplenews_iregister_admin_setting($form, &$form_state) {
  $form['simplenews_iregister_general_settings']['key'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#title' => t('The key to validate the iframe'),
    '#description' => t('Insert an alphanumeric case sensitive string [a-z 0-9]. <strong>Avoid to change the string or the old iframe will be rejected</strong>.'),
    '#required' => TRUE,
    '#default_value' => check_plain(variable_get('simplenews_iregister_key', "")));
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
    '#weight' => 99);

  return $form;
}

/**
 * Form validate.
 *
 * SC Thu Nov 21 23:56:47 CET 2013 stefano
 */
function simplenews_iregister_admin_setting_validate($form, &$form_state) {
  if (preg_match('|.*[^a-zA-z0-9]+.*|', $form_state['values']['key'])) {
    form_set_error('key', t('Only alphanumeric characters.'));
  }
}

/**
 * Form submit.
 *
 * SC Thu Nov 21 23:58:10 CET 2013 stefano.
 */
function simplenews_iregister_admin_setting_submit($form, &$form_state) {
  // Checks if changed.
  //
  // SC 31/gen/2014 14:49:35 stefano.
  $old_key = variable_get('simplenews_iregister_key', '');

  variable_set('simplenews_iregister_key', $form_state['values']['key']);

  if ($old_key != variable_get('simplenews_iregister_key', '')) {
    $message = t('You have just changed the general key. <strong>You must regenerate the iframe for every newsletter or inform the newsletter owners</strong>.');
    drupal_set_message(filter_xss_admin($message), 'warning');
    watchdog('simplenews_iregister', $message, array(), WATCHDOG_WARNING);
  }
}
