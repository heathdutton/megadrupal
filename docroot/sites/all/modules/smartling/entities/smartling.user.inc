<?php

/**
 * @file
 * Functions for taxonomy_term entity.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Disabled method on taxonomy vocabulary form
 */
function smartling_form_user_profile_form_alter(&$form, &$form_state) {
  try {
    if (!smartling_translate_fields_configured($form['#bundle'], 'user')) {
      if (user_access('administer smartling')) {
        drupal_set_message(t('Please go to <a href="@link">Smartling settings</a> to finish configuration.', array('@link' => url('admin/config/regional/smartling'))), 'warning');
      }
    }
    $user = $form['#user'];

    if (!empty($user->uid)) {
      $form = array_merge($form, smartling_get_user_settings_form($form, $form_state));
    }
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
  }
}

/**
 * User smartling form.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for user smartling form.
 */
function smartling_get_user_settings_form(array $form, array &$form_state) {
  try {
    $form = drupal_container()
      ->get('smartling.forms.user_settings')
      ->buildForm($form, $form_state);
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
    return array();
  }

  return $form;
}

/**
 * User smartling form - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_get_user_settings_form_submit(array $form, array &$form_state) {
  try {
    drupal_container()
      ->get('smartling.forms.user_settings')
      ->submitForm($form, $form_state);
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
  }

  unset($_GET['destination']);
}
