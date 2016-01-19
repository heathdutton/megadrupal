<?php

/**
 * @file
 * Functions for node entity.
 */

/**
 * Implements hook_form_BASE_FORM_ID_alter().
 */
function smartling_form_node_form_alter(&$form, $form_state) {
  try {
    if (!smartling_translate_fields_configured($form['#node']->type, 'node')) {
      if (user_access('administer smartling')) {
        drupal_set_message(t('Please go to <a href="@link">Smartling settings</a> to finish configuration.', array('@link' => url('admin/config/regional/smartling'))), 'warning');
      }
      return;
    }
    if (smartling_nodes_method($form['#node']->type) && !isset($form['#node']->tnid)) {
      return;
    }

    if (smartling_fields_method($form['#node']->type) && !isset($form['#node']->tnid)) {
      return;
    }

    $form = array_merge($form, smartling_get_node_settings_form($form, $form_state));
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Enable Smartling translation on a node type
 */
function smartling_form_node_type_form_alter(&$form, &$form_state) {
  $details = t('It will be possible to use Smartling service for automatic content translation.');

  $form['workflow']['language_content_type']['#description'] .= ' ' . $details;
  $form['#submit'][] = 'smartling_multilanguage_support_node_type_submit';
  //$form['#validate'][] = 'smartling_multilanguage_support_node_type_validate';
}

/**
 * Smartling multilingual support form for node types - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_multilanguage_support_node_type_submit(array &$form, array &$form_state) {
  // Delete content type in smartling support types.
  $bundle = $form_state['values']['type'];

  try {
    if ($form_state['values']['language_content_type'] != SMARTLING_FIELDS_METHOD_KEY || $form_state['values']['language_content_type'] != SMARTLING_NODES_METHOD_KEY) {
      smartling_settings_get_handler()->deleteSingleBundleFromSettings('node', $bundle);
    }
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
  }
}

/**
 * Node smartling form.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for node smartling form.
 */
function smartling_get_node_settings_form(array $form, array &$form_state) {
  try {
    $form = drupal_container()
      ->get('smartling.forms.node_settings')
      ->buildForm($form, $form_state);
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
    return array();
  }

  return $form;
}

/**
 * Node smartling form - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_get_node_settings_form_submit(array $form, array &$form_state) {
  try {
    drupal_container()
      ->get('smartling.forms.node_settings')
      ->submitForm($form, $form_state);
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
  }
  unset($_GET['destination']);
}
