<?php

/**
 * Implements hook_form().
 */
function golemde_settings_form() {
  $form = array();

  $form['golemde_developer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Developer Key'),
    '#description' => t('The Golem.de API requires authentication via developer key. Visit api.golem.de to register and retrieve a key.'),
    '#size' => 40,
    '#maxlength' => 80,
    '#required' => TRUE,
    '#default_value' => variable_get('golemde_developer_key', ''),
  );

  return system_settings_form($form);
}
