<?php
/**
 * @file
 * Admin page callbacks for the PeanutlabsMedia module.
 *
 * @ingroup peanutlabsmedia
 */

/**
 * Form builder; Configure the PeanutlabsMedia system.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function peanutlabsmedia_settings($form_state) {
  $form['general'] = array(
    '#type' => 'fieldset',
    '#weight' => 0,
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['general']['peanutlabs_userid'] = array(
    '#type' => 'textfield',
    '#title' => t('UserId'),
    '#default_value' => variable_get('peanutlabsmedia_userid', FALSE),
    '#description' => t('Enter the peanutlabsmedia application UserId (MD5 generated).'),
    '#required' => TRUE,
  );
  $form['general']['peanutlabs_appkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Key'),
    '#default_value' => variable_get('peanutlabsmedia_appkey', FALSE),
    '#description' => t('Enter the peanutlabsmedia application key.'),
    '#required' => TRUE,
  );
  $form['general']['peanutlabs_iframe_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Iframe Height'),
    '#default_value' => variable_get('peanutlabsmedia_frame_height', FALSE),
    '#description' => t('Enter the peanutlabsmedia iframe height.'),
    '#required' => TRUE,
  );
  $form['general']['peanutlabs_iframe_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Iframe Width'),
    '#default_value' => variable_get('peanutlabsmedia_frame_width', FALSE),
    '#description' => t('Enter the peanutlabsmedia iframe width.'),
    '#required' => TRUE,
  );
  $form['general']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'save settings',
  );
  return $form;
}

/**
 * Validate peanutlabsmedia_admin_settings form submissions.
 */

function peanutlabsmedia_settings_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['peanutlabs_iframe_height']) || !is_numeric($form_state['values']['peanutlabs_iframe_width'])) {
    form_set_error("num", 'Please enter numbers for height and width');
  }
}

function peanutlabsmedia_settings_submit($form, &$form_state) {
  variable_set('peanutlabsmedia_userid', $form_state['values']['peanutlabs_userid']);
  variable_set('peanutlabsmedia_appkey', $form_state['values']['peanutlabs_appkey']);
  variable_set('peanutlabsmedia_frame_height', $form_state['values']['peanutlabs_iframe_height']);
  variable_set('peanutlabsmedia_frame_width', $form_state['values']['peanutlabs_iframe_width']);
  drupal_set_message(t("Configuration Settings Saved."));
}

