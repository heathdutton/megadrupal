<?php

/**
 * @file
 * The admin form handling of the inspectlet module.
 */

/**
 * Builds the settings form.
 */
function inspectlet_settings_form($form, &$form_state) {
  $form['inspectlet_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Inspectlet code'),
    '#description' => t('Insert your Inspectlet snippet here.'),
    '#default_value' => variable_get('inspectlet_code'),
    '#rows' => 20,
  );
  $form['#submit'][] = 'inspectlet_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Strips out the HTML comments and the <script> tags.
 */
function inspectlet_settings_form_submit($form, &$form_state) {
  $code = $form_state['values']['inspectlet_code'];

  $code = preg_replace('/<!-- Begin Inspectlet Embed Code -->/', '', $code);
  $code = preg_replace('/<!-- End Inspectlet Embed Code -->/', '', $code);
  $code = preg_replace('/<script.*?>/', '', $code);
  $code = preg_replace('/<\/script>/', '', $code);
  $code = trim($code);

  variable_set('inspectlet_js', $code);
}
