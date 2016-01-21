<?php

/**
 * @file
 * Caascade admin file for validating and specifying form Caascade paths
 *

/**
 * Form API callback to validate the upload settings form.
 */
function caascade_admin_settings_validate($form, &$form_state) {

  $id = $form_state['values']['caascade_id'];

  if ($id != '' && !is_numeric($id))
  {
    form_set_error('caascade_id', t('Caascade ID must be numeric.'));
  }
}

/**
 * Menu callback for the upload settings form.
 */
function caascade_admin_settings($form, &$form_state) {

  $form['caascade_settings_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Caascade Account'),
    '#collapsible' => FALSE,
  );
  $form['caascade_settings_general']['caascade_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Caascade Numeric ID (optional)'),
    '#default_value' => variable_get('caascade_id', ''),
    '#size' => 12,
    '#maxlength' => 12,
    '#description' => t('Visible from your user settings page at math.tetragy.com.'),
  );
  $form['caascade_settings_general']['caascade_router'] = array(
    '#type' => 'textfield',
    '#title' => t('Caascade Router'),
    '#default_value' => variable_get('caascade_router', 'https://route.tetragy.com'),
    '#size' => 35,
    '#maxlength' => 35,
  );

  $form['caascade_settings_customize'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customize HTML'),
    '#collapsible' => FALSE,
    '#description' => t('Add HTML here to customize blocks.'),
  );
  $form['caascade_settings_customize']['caascade_pre_html'] = array(
    '#type' => 'textarea',
    '#title' => t('Caascade Pre HTML'),
    '#default_value' => variable_get('caascade_pre_html', ''),
    '#rows' => 6,
    '#description' => t('HTML added here will be included in every block before the Caascade control panel.'),
  );
  $form['caascade_settings_customize']['caascade_post_html'] = array(
    '#type' => 'textarea',
    '#title' => t('Caascade Post HTML'),
    '#default_value' => variable_get('caascade_post_html', '<div id="caascade-waiting"></div>'),
    '#rows' => 6,
    '#description' => t('HTML added here will be included in every block after the Caascade dialog box.'),
  );

  $form['caascade_recaptcha_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recaptcha'),
    '#collapsible' => FALSE,
    '#description' => '<p>' . t('The Caascade module integrates with Recaptcha to help prevent spam submissions. To enable this feature, enter your Recaptcha public and private keys below which may be obtained from Google.') . '</p>',
  );
  $form['caascade_recaptcha_fieldset']['caascade_recaptcha_publickey'] = array(
    '#type' => 'textfield',
    '#title' => t('Public Key (optional)'),
    '#default_value' => variable_get('caascade_recaptcha_publickey', ''),
  );
  $form['caascade_recaptcha_fieldset']['caascade_recaptcha_privatekey'] = array(
    '#type' => 'textfield',
    '#title' => t('Private Key (optional)'),
    '#default_value' => variable_get('caascade_recaptcha_privatekey', ''),
  );
  $form['caascade_recaptcha_fieldset']['caascade_recaptcha_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#default_value' => variable_get('caascade_recaptcha_theme', 'red'),
    '#options' => array('red' => t('Red'), 'white' => t('White'), 'blackglass' => t('Blackglass'), 'clean' => t('Clean')),
  );

  $form['caascade_hide_blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hide Blocks'),
    '#collapsible' => FALSE,
    '#description' => t('Checkmark boxes to hide groups of operations to reduce clutter.'),
  );
  $form['caascade_hide_blocks']['caascade_arithmetic_operators'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide arithmetic operator blocks'),
    '#default_value' => variable_get('caascade_arithmetic_operators', false),
  );
  $form['caascade_hide_blocks']['caascade_relational_operators'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide relational operator blocks'),
    '#default_value' => variable_get('caascade_relational_operators', false),
  );
	$form['caascade_hide_blocks']['caascade_trig_functions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide trigonometric functions blocks'),
    '#default_value' => variable_get('caascade_trig_functions', false),
  );
  $form['caascade_hide_blocks']['caascade_all_operators'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide *all* operator blocks'),
    '#default_value' => variable_get('caascade_all_operators', false),
  );

  $form['#validate'][] = 'caascade_admin_settings_validate';

  return system_settings_form($form);
}

