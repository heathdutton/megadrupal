<?php

/**
 * @file
 * Administrative UI for the Mailin API settings.
 */

use Mailin\API;
use Mailin\Log;

/**
 * Form builder; Mailin API settings form.
 *
 * @see mailin_admin_settings_form_validate()
 * @ingroup forms
 */
function mailin_admin_settings_form($form, &$form_state) {
  if (!defined('MAILIN_LIBRARY_INSTALLED') || FALSE === MAILIN_LIBRARY_INSTALLED) {
    drupal_set_message(t('The Mailin library is not correctly installed.'), 'warning');
    $form['#library_error'] = FALSE;
  }

  $form['mailin_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailin API Key'),
    //'#description' => t(''), // @todo what is the admin path on Mailin application to get the API key?
    '#default_value' => variable_get('mailin_api_key', ''),
  );

  $languages = language_list('enabled');

  $form['mailin_language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#description' => t('Mailin is not language aware, so you have to specify the language that is used to export the user data to Mailin attributes.'),
    '#options' => array('' => t('Default language')) + array_map(function($v) { return t($v->name); }, $languages[1]),
    '#default_value' => variable_get('mailin_language', ''),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler; validate the API key if any.
 *
 * @see mailin_admin_settings_form()
 */
function mailin_admin_settings_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['mailin_api_key'])) {
    if (!empty($form['#library_error'])) {
      drupal_set_message(t('The API key cannot be tested since the Mailin library is not corerctly installed.'));
    }
    else {
      $mailin = new API($form_state['values']['mailin_api_key'], new MailinRequestDrupal());
      $mailin->getFolders();

      if (!Log::getLastOperationStatus()) {
        // Key not found in database.
        if (Log::getLastOperationError()) {
          form_set_error('mailin_api_key', t('The API key is not valid.'));
        }
        else {
          form_set_error('mailin_api_key', t('The Mailin server is not available.'));
        }
      }
    }
  }
}