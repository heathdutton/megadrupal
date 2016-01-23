<?php

function requirejs_admin_settings($form, &$form_state) {
  $form['optimizer'] = array(
    '#title' => t('Optimizer'),
    '#type' => 'fieldset',
  );
  $form['optimizer']['requirejs_optimizer_enabled'] = array(
    '#title' => t('Enable optimizer'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('requirejs_optimizer_enabled', FALSE),
  );

  $form['optimizer']['requirejs_optimizer_path'] = array(
    '#title' => t('Optimizer path'),
    '#type' => 'textfield',
    '#description' => t('The full path to execute the r.js optimizer which depends on node.  The easiest way to install it is via npm and the command <code>npm install -g requirejs</code> run as root.'),
    '#default_value' => requirejs_get_optimizer_path(),
  );

  $form['optimizer']['requirejs_optimizer_clear_cache'] = array(
    '#type' => 'submit',
    '#value' => t('Clear optimizer cache'),
    '#submit' => array('requirejs_admin_clear_optimizer_cache'),
  );

  return system_settings_form($form);
}

function requirejs_admin_clear_optimizer_cache($form, &$form_state) {
  requirejs_clear_optimizer_cache();
  drupal_set_message(t('Optimizer cache cleared.'));
}

