<?php

/**
 * @file
 * Node Translation Access forms.
 */

/**
 * Deny access form for node.
 */
function node_translation_access_languages_form(&$form, &$form_state, $node) {
  $options = node_translation_access_get_language_list();
  $selected_languages = array();
  if (isset($node->nid)) {
    $selected_languages = db_query('SELECT language FROM node_translation_access WHERE nid=:nid', array(':nid' => $node->nid))->fetchAllKeyed(0, 0);
  }
  
  $form['translation_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select languages to deny access'),
    '#weight' => 100,
    '#group' => 'additional_settings',
    '#tree' => TRUE,
  );
  $form['translation_access']['languages'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $selected_languages,
  );
  return $form;
}

/**
 * Deny access form for content types.
 */
function node_translation_access_content_type_languages_form(&$form, &$form_state) {
  $options = node_translation_access_get_language_list();
  $denied = variable_get('node_translation_access_content_types', array());
  $content_type = $form['#node_type']->type;
  $selected_languages = array();
  if (isset($denied[$content_type])) {
    $selected_languages = $denied[$content_type];
  }
  $form['translation_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select languages to deny access'),
    '#weight' => 100,
    '#group' => 'additional_settings',
    '#tree' => TRUE,
  );
  $form['translation_access']['languages'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $selected_languages,
  );
  $form['#submit'][] = 'node_translation_access_content_type_languages_form_submit';
  return $form;
}

/**
 * Submit callback.
 */
function node_translation_access_content_type_languages_form_submit($form, $form_state) {
  $content_type = $form_state['values']['type'];
  $denied = variable_get('node_translation_access_content_types', array());
  foreach ($form_state['values']['translation_access']['languages'] as $langcode => $state) {
    $denied[$content_type][$langcode] = $state;
  }
  variable_set('node_translation_access_content_types', $denied);  
}
