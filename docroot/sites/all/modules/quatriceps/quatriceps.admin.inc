<?php

/**
 * @file
 * Quatriceps admin file for validating and specifying form Quatriceps paths
 *

/**
 * Form API callback to validate the upload settings form.
 */
function quatriceps_admin_settings_validate($form, &$form_state) {

  $id = $form_state['values']['quatriceps_id'];

  if ($id != '' && !is_numeric($id))
  {
    form_set_error('quatriceps_id', t('Quatriceps ID must be numeric.'));
  }
}

/**
 * Menu callback for the upload settings form.
 */
function quatriceps_admin_settings($form, &$form_state) {

  $form['quatriceps_settings_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Quatriceps Account'),
    '#collapsible' => FALSE,
  );
  $form['quatriceps_settings_general']['quatriceps_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Quatriceps Numeric ID (optional)'),
    '#default_value' => variable_get('quatriceps_id', ''),
    '#size' => 12,
    '#maxlength' => 12,
    '#description' => t('An ID may be obtained from math.tetragy.com.'),
  );
  $form['quatriceps_settings_general']['quatriceps_router'] = array(
    '#type' => 'textfield',
    '#title' => t('Quatriceps Router'),
    '#default_value' => variable_get('quatriceps_router', 'https://route.tetragy.com'),
    '#size' => 35,
    '#maxlength' => 35,
  );

  $form['quatriceps_settings_customize'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customize HTML'),
    '#collapsible' => FALSE,
    '#description' => t('Add HTML here to customize blocks.'),
  );
  $form['quatriceps_settings_customize']['quatriceps_pre_html'] = array(
    '#type' => 'textarea',
    '#title' => t('Quatriceps Pre HTML'),
    '#default_value' => variable_get('quatriceps_pre_html', ''),
    '#rows' => 6,
    '#description' => t('HTML added here will be included in every block before the Quatriceps div.'),
  );
  $form['quatriceps_settings_customize']['quatriceps_post_html'] = array(
    '#type' => 'textarea',
    '#title' => t('Quatriceps Post HTML'),
    '#default_value' => variable_get('quatriceps_post_html', '<div id="quatriceps-waiting"></div>'),
    '#rows' => 6,
    '#description' => t('HTML added here will be included in every block after the Quatriceps div.'),
  );

  $form['quatriceps_recaptcha_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recaptcha'),
    '#collapsible' => FALSE,
    '#description' => '<p>' . t('The Quatriceps module integrates with Recaptcha to help prevent spam submission. To enable this feature, enter your Recaptcha public and private keys below which may be obtained from Google. To disable, ensure the public and private keys are empty.') . '</p>',
  );
  $form['quatriceps_recaptcha_fieldset']['quatriceps_recaptcha_publickey'] = array(
    '#type' => 'textfield',
    '#title' => t('Public Key (optional)'),
    '#default_value' => variable_get('quatriceps_recaptcha_publickey', ''),
  );
  $form['quatriceps_recaptcha_fieldset']['quatriceps_recaptcha_privatekey'] = array(
    '#type' => 'textfield',
    '#title' => t('Private Key (optional)'),
    '#default_value' => variable_get('quatriceps_recaptcha_privatekey', ''),
  );
  $form['quatriceps_recaptcha_fieldset']['quatriceps_recaptcha_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#default_value' => variable_get('quatriceps_recaptcha_theme', 'red'),
    '#options' => array('red' => t('Red'), 'white' => t('White'), 'blackglass' => t('Blackglass'), 'clean' => t('Clean')),
  );

  $form['quatriceps_hide_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hide Blocks'),
    '#collapsible' => FALSE,
    '#description' => t('Checkmark boxes to hide groups of operations to reduce clutter.'),
  );
  $form['quatriceps_hide_blocks']['quatriceps_arithmetic_operators'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide arithmetic operator blocks'),
    '#default_value' => variable_get('quatriceps_arithmetic_operators', false),
  );
  $form['quatriceps_hide_blocks']['quatriceps_all_operators'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide *all* operator blocks'),
    '#default_value' => variable_get('quatriceps_all_operators', false),
  );

  $form['#validate'][] = 'quatriceps_admin_settings_validate';

  return system_settings_form($form);
}

