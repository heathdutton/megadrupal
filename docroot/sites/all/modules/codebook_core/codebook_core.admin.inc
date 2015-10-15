<?php

/**
 * @file
 * Administrative page callbacks for Codebook Core.
 */

/**
 * Codebook configuration form.
 */
function codebook_core_admin_config() {
  $form = array();
  
  $form['codebook_core_num_sep'] = array(
    '#type' => 'textfield',
    '#title' => t('Document number seperator'),
    '#description' => t(''),
    '#default_value' => variable_get('codebook_core_num_sep', CODEBOOK_CORE_NUM_SEP),
    '#required' => TRUE,
    '#size' => 5,
    '#maxlength' => 1,
  );

  foreach (array('codebook_title', 'codebook_chapter', 'codebook_section') as $type) {
    $key = "codebook_core_num_len_$type";
    $label = node_type_get_name($type);
    $form[$key] = array(
      '#type' => 'textfield',
      '#title' => t('!type number length', array('!type' => $label)),
      '#description' => t(''),
      '#default_value' => variable_get($key, 3),
      '#required' => TRUE,
      '#size' => 5,
      '#maxlength' => 1,
    );
  }
  
  return system_settings_form($form);
}
