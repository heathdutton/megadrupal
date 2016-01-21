<?php

/**
 * @file
 * Code for admin pages of Uptolike module.
 */

/**
 * Form builder for admin statistic page.
 */
function uptolike_admin_statistic_form($form, &$form_state) {
  $email = variable_get('uptolike_email', NULL);
  $key = variable_get('uptolike_key', NULL);
  $log_in = !empty($email) && !empty($key) ? TRUE : FALSE;

  if ($log_in) {
    $form['uptolike_statistic'] = array(
      '#markup' => uptolike_statistic(),
      '#attached' => array(
        'css' => array(drupal_get_path('module', 'uptolike') . '/css/uptolike-admin.css'),
        'js' => array((drupal_get_path('module', 'uptolike') . '/scripts/scripts.js')),
      ),
    );
  }

  $form['uptolike_account_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account information'),
    '#collapsible' => TRUE,
    '#collapsed' => $log_in,
  );

  if (!$log_in) {
    $form['uptolike_account_info']['uptolike_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#description' => t('Enter your email to receive a secret key.'),
      '#size' => 70,
      '#default_value' => variable_get('uptolike_email', NULL),
      '#required' => TRUE,
    );
    if ($email) {
      $form['uptolike_account_info']['uptolike_key'] = array(
        '#type' => 'textfield',
        '#title' => t('Secret key'),
        '#description' => t('Secret key to access statistics.'),
        '#size' => 70,
        '#default_value' => variable_get('uptolike_key', NULL),
        '#required' => TRUE,
      );
      $form['uptolike_statistic_hidden'] = array(
        '#markup' => uptolike_statistic('uptolike-hidden'),
        '#attached' => array(
          'css' => array(drupal_get_path('module', 'uptolike') . '/css/uptolike-admin.css'),
          'js' => array((drupal_get_path('module', 'uptolike') . '/scripts/scripts.js')),
        ),
      );
      $form['uptolike_status'] = array(
        '#type' => 'hidden',
        '#default_value' => '',
        '#attached' => array(
          'js' => array(
            array(
              'data' => array(
                'Uptolike' => array(
                  'email' => variable_get('uptolike_email', NULL),
                  'partner' => UPTOLIKE_PARTNER,
                  'projectId' => uptolike_project_id(),
                ),
              ),
              'type' => 'setting',
            ),
            array(
              // This is bad.
              'data' => 'http://crypto-js.googlecode.com/svn/tags/3.1.2/build/rollups/md5.js',
              'type' => 'external',
            ),
          ),
        ),
      );
    }
  }
  else {
    $form['uptolike_account_info']['#description'] = t('Your logged in as %email.', array('%email' => $email));
    $form['uptolike_account_info']['revoke'] = array(
      '#type' => 'submit',
      '#value' => t('Revoke access'),
      '#submit' => array('uptolike_admin_revoke'),
    );
  }

  $form['uptolike_feedback'] = array(
    '#type' => 'item',
    '#title' => t('Feedback !email.', array('!email' => l(UPTOLIKE_SUPPORT_EMAIL, 'mailto:' .  UPTOLIKE_SUPPORT_EMAIL))),
  );

  // Set a submit handler manually because the default submit handler
  // gets overridden by the system_settings_form() submit handler.
  $form['#submit'][] = 'uptolike_admin_statistic_form_submit';

  return system_settings_form($form);
}

/**
 * Validation handler for admin setting form.
 */
function uptolike_admin_statistic_form_validate($form, &$form_state) {
  if (isset($form_state['values']['uptolike_email'])) {
    if (!valid_email_address($form_state['values']['uptolike_email'])) {
      form_error($form['uptolike_account_info']['uptolike_email'], t('Email address is invalid.'));
    }
  }
  if (isset($form_state['values']['uptolike_status']) && ($form_state['values']['uptolike_status'] == 'badCredentials')) {
    form_error($form['uptolike_status'], t('Wrong key! Make sure you copied the key without extra characters (spaces, etc.).'));
    drupal_set_message(t('If the key has not been received, write a letter to support !email.',
      array('!email' => l(UPTOLIKE_SUPPORT_EMAIL, 'mailto:' .  UPTOLIKE_SUPPORT_EMAIL))), 'warning');
  }
  if (isset($form_state['values']['uptolike_status']) && ($form_state['values']['uptolike_status'] == 'foreignAccess')) {
    form_error($form['uptolike_status'], t('Wrong key! Make sure you copied the key without extra characters (spaces, etc.).'));
    drupal_set_message(t('This project belongs to another user. Contact Support !email specifying the site address in the letter.',
          array('!email' => l(UPTOLIKE_SUPPORT_EMAIL, 'mailto:' . UPTOLIKE_SUPPORT_EMAIL))), 'warning');
  }
}

/**
 * Submit handler for admin setting form.
 */
function uptolike_admin_statistic_form_submit($form, &$form_state) {
  $email = $form_state['values']['uptolike_email'];
  if (isset($email) && empty($form_state['values']['uptolike_key'])) {
    uptolike_user_registration($email);
    drupal_set_message(t('The secret code has been sent to %email. Check your email and enter secret code below.', array('%email' => $email)));
    drupal_set_message(t('If the letter with secret key is not coming, check your Spam folder.'));
  }
  // Secret key may have spaces at the end.
  if (isset($form_state['values']['uptolike_key'])) {
    $form_state['values']['uptolike_key'] = trim($form_state['values']['uptolike_key']);
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for uptolike_admin_statistic_form().
 */
function uptolike_form_uptolike_admin_statistic_form_alter(&$form, &$form_state) {
  $email = variable_get('uptolike_email', NULL);
  $key = $key = variable_get('uptolike_key', NULL);
  if (empty($form_state['values']['uptolike_email']) && empty($email)) {
    $form['actions']['submit']['#value'] = t('Get secret key');
  }
  if (!empty($email)) {
    $form['actions']['submit']['#value'] = t('Authorization');
  }
  // If statistic page is working.
  if (!empty($email) && !empty($key)) {
    unset($form['actions']['submit']);
  }
}

/**
 * Revoke statistic access.
 */
function uptolike_admin_revoke($form, &$form_state) {
  variable_set('uptolike_email', NULL);
  variable_set('uptolike_key', NULL);
}

/**
 * Form builder for admin settings page.
 */
function uptolike_admin_settings_form($form, &$form_state) {
  $form['uptolike_language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#description' => t('Select Uptolike share buttons language.'),
    '#options' => array(
      'en' => t('English'),
      'ru' => t('Russian'),
      'ua' => t('Ukrainian'),
      'de' => t('German'),
      'es' => t('Spanish'),
      'it' => t('Italian'),
      'lt' => t('Lithuanian'),
      'pl' => t('Polish'),
    ),
    '#default_value' => variable_get('uptolike_language', uptolike_default_language()),
  );

  $form['uptolike_number_of_blocks'] = array(
    '#type' => 'select',
    '#title' => t('Number of blocks'),
    '#description' => t('Number of Uptolike share buttons blocks.'),
    // Number of blocks, from 0 to 10.
    '#options' => range(0, 10),
    '#default_value' => variable_get('uptolike_number_of_blocks', 1),
  );

  // Prepare array of options.
  $options = array();
  foreach (entity_get_info() as $entity_type => $entity) {
    $options[$entity_type] = $entity['label'];
  }
  $form['uptolike_entities'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Entities'),
    '#description' => t('Show Uptolike share buttons on selected entities.'),
    '#options' => $options,
    '#default_value' => variable_get('uptolike_entities', array('node')),
  );

  $form['uptolike_every_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add main javascript code on every page'),
    '#description' => t('This is useful if you are using content filter.'),
    '#default_value' => variable_get('uptolike_every_page', 0),
  );

  $form['uptolike_feedback'] = array(
    '#type' => 'item',
    '#title' => t('Feedback !email.', array('!email' => l(UPTOLIKE_SUPPORT_EMAIL, 'mailto:' .  UPTOLIKE_SUPPORT_EMAIL))),
  );

  return system_settings_form($form);
}
