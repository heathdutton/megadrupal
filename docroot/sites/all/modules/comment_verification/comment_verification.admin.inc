<?php
/**
 * @file
 * Global admin form for comment_verification module
 */

/**
 * Implements configuration form call back function.
 */
function comment_verification_admin_form($form, &$form_state) {
  $form = array();

  $form['comment_verification_email_subject'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('comment_verification_email_subject', t('Please verify your email for [site:name]')),
    '#title' => t('Anonymous Comment Email Verification Subject'),
  );

  $form['comment_verification_email_body'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('comment_verification_email_body', t('!verify_link')),
    '#title' => t('Anonymous Comment Email Verification Body'),
  );

  $period = drupal_map_assoc(
    array(
      0,
      3600,
      10800,
      21600,
      32400,
      43200,
      86400,
      172800,
      259200,
      604800,
      1209600,
      2419200,
      4838400,
      9676800,
    ),
    'format_interval'
  );
  $form['comment_verification_timer'] = array(
    '#type' => 'select',
    '#title' => t('Discard unverified anonymous comments older than'),
    '#default_value'   => variable_get('comment_verification_timer', 259200),
    '#options' => $period,
    '#description' => t('Requires a correctly configured <a href="@cron">cron maintenance task</a>.', array('@cron' => url('admin/reports/status')))
  );

  $form['comment_anon_register_prompt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prompt anonymous commenters to register after verification?'),
    '#default_value' => variable_get('comment_anon_register_prompt', FALSE),
    '#description' => t('If checked, a message will be presented to anonymous users who verify their comment asking them to register, with a link to the registration page.'),
  );

  $form['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
  );

  return system_settings_form($form);
}
