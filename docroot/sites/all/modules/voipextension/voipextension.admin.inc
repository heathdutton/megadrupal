<?php

/**
 * @file
 * VoIP extension field administration.
 */

/**
 * Get form menu callback.
 */
function voipextension_admin_settings() {
  drupal_add_js(drupal_get_path('module', 'voipextension') . '/voipextension.admin.js');
  $form = array();
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'voipextension') . '/voipextension.admin.css' => array(
      'group' => CSS_THEME,
    ),
  );
  $form['voipextension_valid_script_names'] = array(
    '#title' => t('Valid scripts'),
    '#type' => 'checkboxes',
    '#options' => VoipScript::getScriptNames(),
    '#default_value' => variable_get('voipextension_valid_script_names', array()),
    '#description' => t('Scripts that will be available for VoIP extensions. You can select multiple scripts. If none are selected all will be available.'),

  );
  $form['voipextension_default_script_name'] = array(
    '#title' => t('Site default script'),
    '#type' => 'select',
    '#options' => VoipScript::getScriptNames(),
    '#default_value' => variable_get('voipextension_default_script_name', 'voipextension_default_extension_script'),
    '#description' => t('The script, of those valid, that will be used by default.'),
  );
  $fields = array();
  foreach(field_info_field_map() as $field_name => $field_info) {
    if ($field_info['type'] == 'voipextension') {
      $fields[$field_name] = $field_name; 
    }
  }
  $form['voipextension_default_field'] = array(
    '#title' => t('VoIP extension field'),
    '#type' => 'select',
    '#options' => $fields,
    '#default_value' => variable_get('voipextension_default_field', ''),
    '#description' => t('The field that is used by default to look up an extension.'),
  );
  if (count($fields) == 0) {
    form_set_error('voipextension_default_field', t('You will also need to attach an extension field to the enitities that you want to VoIP extension.'));
  }
  elseif (count($fields) > 1) {
    $form['voipextension_default_field']['#description'] .= ' ' . t('Only one extension field is used by default.') . '<br />' . t('Either use the same extension field on each entity, or if you want more extension types see the documentation about writing different scripts.');
  }
  $form['array_filter'] = array('#type' => 'hidden');
  $form = system_settings_form($form);
  $form['#submit'][] = 'voipextension_admin_settings_submit';
  return $form;
}

/**
 * voipextension_admin_settings() form additional submit handler.
 *
 * $form['array_filter'] = array('#type' => 'hidden'); uses numeric keys
 * the id as key and value makes life a lot easier.
 */
function voipextension_admin_settings_submit(&$form, &$form_state) {
  $valid = array();
  foreach($form_state['values']['voipextension_valid_script_names'] as $id => $value) {
    if ($value && isset($form['voipextension_valid_script_names'][$id])) { $valid[$id] = $id; }
  }
  variable_set('voipextension_valid_script_names', $valid);
}
