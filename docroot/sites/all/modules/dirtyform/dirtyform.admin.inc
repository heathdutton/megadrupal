<?php

/**
 * @file
 * Adminstrative pages.
 */

/**
 * Settings form.
 */
function dirtyform_config_form($form, &$form_state) {

  $default_value = implode("\n", dirtyform_form_id_whitelist());
  $form['form_id_whitelist'] = array(
    '#type' => 'textarea',
    '#title' => t('Enabled Form IDs'),
    '#description' => t('One per line.'),
    '#default_value' => $default_value,
  );

  $default_value = implode("\n", dirtyform_form_id_blacklist());
  $form['form_id_blacklist'] = array(
    '#type' => 'textarea',
    '#title' => t('Disabled Form IDs'),
    '#description' => t('One per line.'),
    '#default_value' => $default_value,
  );

  $default_value = implode("\n", dirtyform_form_id_regex());
  $form['form_id_regex'] = array(
    '#type' => 'textarea',
    '#title' => t('Form ID Regular Expressions'),
    '#description' => t('One per line, including delimeters, e.g. <code>/.*_node_form/</code>.'),
    '#default_value' => $default_value,
  );

  $form = system_settings_form($form);

  $form['#validate'] = array('dirtyform_config_form_validate');
  $form['#submit'] = array('dirtyform_config_form_submit');

  return $form;
}

/**
 * Validation handler for dirtyform_config_form().
 */
function dirtyform_config_form_validate($form, &$form_state) {
  $lines = array_filter(
    preg_split('/[\n\r]+/', $form_state['values']['form_id_regex']));
  array_map(function ($line) {
    if (preg_match($line, '') === FALSE) {
      form_set_error('form_id_regex',
        t('The line %line could not be parsed.', array('%line' => $line)));
    }
  }, $lines);
}

/**
 * Submit handler for dirtyform_config_form().
 */
function dirtyform_config_form_submit($form, &$form_state) {
  dirtyform_form_id_whitelist(array_filter(
    preg_split('/[\n\r]+/', $form_state['values']['form_id_whitelist'])));
  dirtyform_form_id_blacklist(array_filter(
    preg_split('/[\n\r]+/', $form_state['values']['form_id_blacklist'])));
  dirtyform_form_id_regex(array_filter(
    preg_split('/[\n\r]+/', $form_state['values']['form_id_regex'])));
}
