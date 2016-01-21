<?php

/**
 * @file
 * Functions for taxonomy_term entity.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Add validate and submit
 */
function smartling_form_taxonomy_form_vocabulary_alter(&$form, &$form_state) {
  $form['#validate'][] = 'smartling_vocabulary_support_type_validate';
  $form['#submit'][] = 'smartling_vocabulary_support_type_submit';
}

/**
 * Smartling multilingual support form for vocabulary - Form Validate.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_vocabulary_support_type_validate(array &$form, array &$form_state) {
  if (!isset($form_state['confirm_delete']) || ($form_state['confirm_delete'] != TRUE)) {
    // Set old i18n_mode.
    $vocabulary = taxonomy_vocabulary_machine_name_load($form_state['values']['machine_name']);
    if (is_object($vocabulary) && !empty($vocabulary->machine_name)) {
      $form_state['old_i18n_mode'] = i18n_taxonomy_vocabulary_mode($vocabulary);
    }

    if (isset($form_state['values']['i18n_mode']) && ($form_state['values']['i18n_mode'] == I18N_MODE_LOCALIZE || $form_state['values']['i18n_mode'] == I18N_MODE_TRANSLATE)) {
      if (!smartling_is_configured()) {
        drupal_set_message(t('Please go to the <a href=\"/admin/structure/taxonomy/@machine_name/edit\">edit vocabulary</a> to set the translation mode for Smartling.', array('@url' => url('admin/config/regional/smartling'))), 'warning');
        form_set_error('i18n_translation');
      }
    }
  }
}

/**
 * Smartling multilingual support form for vocabulary - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_vocabulary_support_type_submit(array &$form, array &$form_state) {
  if (!isset($form_state['confirm_delete']) || ($form_state['confirm_delete'] != TRUE)) {
    // Delete content type in smartling support types.
    $bundle = $form_state['values']['machine_name'];
    if (isset($form_state['values']['i18n_mode'])) {
      if ($form_state['values']['i18n_mode'] != I18N_MODE_LOCALIZE && $form_state['values']['i18n_mode'] != I18N_MODE_TRANSLATE) {
        smartling_settings_get_handler()->deleteSingleBundleFromSettings('taxonomy_term', $bundle);
      }
      elseif (isset($form_state['old_i18n_mode']) && ($form_state['values']['i18n_mode'] != $form_state['old_i18n_mode'])) {
        smartling_settings_get_handler()->deleteSingleBundleFromSettings('taxonomy_term', $bundle);
      }
    }
  }
}

/**
 * Implements hook_form_BASE_FORM_ID_alter().
 */
function smartling_form_taxonomy_form_term_alter(&$form, $form_state) {
  $term = $form_state['term'];

  try {
    if (!empty($term->tid)) {
      if (smartling_translate_fields_configured($form['#vocabulary']->machine_name, 'taxonomy_term')) {
        $form = array_merge($form, smartling_get_term_settings_form($form, $form_state));
      }
      elseif (user_access('administer smartling')) {
        drupal_set_message(t('Please follow the page <a href="/admin/structure/taxonomy/@machine_name/edit">"edit vocabulary"</a> to set Translation mode for Smartling.', array('@machine_name' => $form['#vocabulary']->machine_name)));
      }
    }
    elseif (empty($term->tid) && !isset($_GET['translation']) && !isset($_GET['target'])) {
      $langcode = language_default()->language;
      $form['language']['#default_value'] = $langcode;
      unset($form['language']['#options'][LANGUAGE_NONE]);
    }
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
  }
}

/**
 * Taxonomy term smartling form.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 *
 * @return array
 *   Return FAPI array for taxonomy term smartling form.
 */
function smartling_get_term_settings_form(array $form, array &$form_state) {
  try {
    $form = drupal_container()
      ->get('smartling.forms.taxonomy_term_settings')
      ->buildForm($form, $form_state);
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
    return array();
  }
  return $form;
}

/**
 * Taxonomy term smartling form - Form Submit.
 *
 * @param array $form
 *   FAPI array.
 * @param array $form_state
 *   FAPI array.
 */
function smartling_get_term_settings_form_submit(array $form, array &$form_state) {
  //Sometimes instead of an object we have an array of taxonomy_term. In this case we need to convert it back to an object.
  if (is_array($form['#term'])) {
    $form['#term'] = (object) $form['#term'];
  }

  try {
    drupal_container()
      ->get('smartling.forms.taxonomy_term_settings')
      ->submitForm($form, $form_state);
  } catch (\Exception $e) {
    watchdog('smartling', $e->getMessage() . '   ' . print_r($e, TRUE), array(), WATCHDOG_CRITICAL);
  }

  unset($_GET['destination']);
}
