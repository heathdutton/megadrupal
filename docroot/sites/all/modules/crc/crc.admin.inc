<?php

/**
 * @file
 * Configures the Create related content page.
 */

function crc_content_type_alter(&$form, $form_state) {
  $type = $form['#node_type']->type;
  $settings = crc_settings($type);
  $form['crc'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Create related content settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['crc']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable create related content form on this node type.'),
    '#default_value' => $settings['status'],
  );
  foreach (crc_module_implements('crc_type_settings') as $module) {
    if (empty($settings[$module])) {
      $settings[$module] = array();
    }
    $func = $module . '_crc_type_settings';
    $form['crc'][$module] = $func($type, $settings[$module], $form, $form_state);
  }
  $form['#submit'][] = 'crc_content_type_submit';
}

/**
 * We have to do this automatically as the native autosave assumes that we
 * only want the array keys.
 */
function crc_content_type_submit($form, &$form_state) {
  $type = empty($form['#node_type']->type) ? $form['identity']['type']['#value'] : $form['#node_type']->type;
  foreach (crc_module_implements('crc_type_settings_submit') as $module) {
    $function = $module . '_crc_type_settings_submit';
    $function($type, $form_state['values']['crc'][$module], $form, $form_state);
  }
  variable_set('crc_settings_'. $type, $form_state['values']['crc']);
  unset($form_state['values']['crc']);
}
