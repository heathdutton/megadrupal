<?php

/**
 * Form callback: Webform backend settings form.
 */
function campaignion_recent_supporters_webform_settings_form($form, &$form_state) {
  $cron = campaignion_recent_supporters_webform_cron_settings();
  $form['#tree'] = TRUE;
  $form['campaignion_recent_supporters_webform_cron'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron cleanup tasks'),
    '#description' => t('This setting control how many recent supporters are kept and for how long.'),
    'max_age' => array(
      '#type' => 'select',
      '#title' => t('Maximum age'),
      '#description' => t('The cron job will delete all recent supporter entries older than this.'),
      '#options' => array(-1 => t('- none -'), 24*36000 => t('1 day'), 7*24*3600 => t('7 days'), 30*24*3600 => t('30 days'), 90*24*3600 => t('90 days')),
      '#default_value' => $cron['max_age'],
    ),
    'max_number' => array(
      '#type' => 'textfield',
      '#title' => t('Maximum number'),
      '#description' => t('Only keep this number of entries per action / node. 0 will deactivate this feature.'),
      '#default_value' => $cron['max_number'],
    ),
  );

  $mapping = campaignion_recent_supporters_webform_mapping();
  $element = array(
    '#type' => 'fieldset',
    '#title' => t('Webform form_key mapping'),
    '#description' => t('The webform backend will search for the following form keys when it tries to find supporter data in a webform submission'),
  );
  $element['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
  );
  $element['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
  );
  $element['country'] = array(
    '#type' => 'textfield',
    '#title' => t('Country'),
  );
  foreach ($mapping as $field => $form_keys) {
    $element[$field]['#default_value'] = implode(', ', $form_keys);
  }
  $form['campaignion_recent_supporters_webform_mapping'] = $element;
  $form = system_settings_form($form);
  array_unshift($form['#submit'], 'campaignion_recent_supporters_webform_settings_form_submit');
  return $form;
}

/**
 * Form submit callback: Webform backend settings form.
 */
function campaignion_recent_supporters_webform_settings_form_submit($form, &$form_state) {
  foreach ($form_state['values']['campaignion_recent_supporters_webform_cron'] as &$value) {
    $value = (int) $value;
  }
  foreach ($form_state['values']['campaignion_recent_supporters_webform_mapping'] as &$form_keys) {
    $form_keys = array_map('trim', explode(',', $form_keys));
  }
}
