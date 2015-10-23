<?php

/**
 * @file
 * Helper functions for Menu translation access module.
 */

/**
 * Deny access form for menu item.
 */
function menu_translation_access_menu_languages_form(&$form, &$form_state) {
  $options = node_translation_access_get_language_list();
  $denied = variable_get('menu_translation_access_menu_items', array());
  $mlid = $form['mlid']['#value'];
  $selected_languages = array();
  if (isset($denied[$mlid])) {
    $selected_languages = $denied[$mlid];
  }

  $form['translation_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select languages to deny access'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 100,
    '#tree' => TRUE,
  );
  $form['translation_access']['languages'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $selected_languages,
  );
  $form['#submit'][] = 'menu_translation_access_menu_languages_form_submit';

  return $form;
}

/**
 * Submit callback.
 */
function menu_translation_access_menu_languages_form_submit($form, $form_state) {
  $mlid = $form['mlid']['#value'];
  if ($mlid) {
    $denied = variable_get('menu_translation_access_menu_items', array());
    $denied[$mlid] = array();
    foreach ($form_state['values']['translation_access']['languages'] as $langcode => $state) {
      if ($state) {
        $denied[$mlid][$langcode] = $state;
      }
    }
    variable_set('menu_translation_access_menu_items', $denied);
  }
}

/**
 * Check menu item translation access.
 */
function menu_translation_access_menu_denied($mlid) {
  global $language;
  $denied = variable_get('menu_translation_access_menu_items', array());
  if (isset($denied[$mlid][$language->language])) {
    return TRUE;
  }
  return FALSE;
}
