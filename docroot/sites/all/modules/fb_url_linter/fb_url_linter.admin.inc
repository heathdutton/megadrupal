<?php
/**
 * @file
 * Drupal page callback functions with forms and other stuff to use.
 */

/**
 * Implements hook_form().
 *
 * Module configuration form.
 */
function fb_url_linter_configuration_form($form, &$form_state) {

  $node_types = node_type_get_types();
  $node_types_options = array();
  foreach ($node_types as $type) {
    $node_types_options[$type->type] = $type->name;
  }

  $form['fb_url_linter_cron_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content-types submitted by cron'),
    '#default_value' => variable_get('fb_url_linter_cron_types', array()),
    '#options' => $node_types_options,
    '#description' => t('Select which content-types you want to send to Linter service API on scheduled cron job.'),
  );

  $form['fb_url_linter_cron_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron items limit'),
    '#default_value' => variable_get('fb_url_linter_cron_limit', FB_URL_LINTER_CRON_LIMIT),
    '#size' => 10,
    '#maxlength' => 4,
    '#description' => t('Number of nodes that will proceed on cron execution.'),
    '#element_validate' => array('element_validate_number'),
  );

  $form['fb_url_linter_update_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content-types submitted on content update'),
    '#default_value' => variable_get('fb_url_linter_update_types', array()),
    '#options' => $node_types_options,
    '#description' => t('Content of these types will be submitted to Facebook Linter API immediately on node form submission.'),
  );

  // Filter checkbox values array before save.
  $form['array_filter'] = array(
    '#type' => 'value',
    '#value' => TRUE,
  );

  return system_settings_form($form);
}
