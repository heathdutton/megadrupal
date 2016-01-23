<?php
/**
 * @file hannotaatio.admin.inc
 * Provides the Hannotaatio administrative interface.
 */

/**
 *  Hannotaatio Administrative settings form.
 */
function hannotaatio_admin() {
  $form = array();

  $form['hannotaatio_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 64,
    '#default_value' => variable_get('hannotaatio_apikey', ''),
    '#description' => t('Insert your API key here.'),
  );

  $form['hannotaatio_capture_stylesheets'] = array(
    '#type' => 'checkbox',
    '#title' => t('Capture stylesheets'),
    '#description' => t('Captures stylesheets when making feedback.'),
    '#default_value' => variable_get('hannotaatio_capture_stylesheets', TRUE),
  );

  $form['hannotaatio_capture_images'] = array(
    '#type' => 'checkbox',
    '#title' => t('Capture images'),
    '#description' => t("Captures images when making feedback."),
    '#default_value' => variable_get('hannotaatio_capture_images', TRUE),
  );

  $form['hannotaatio_notification_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification email'),
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 64,
    '#default_value' => variable_get('hannotaatio_notification_email', ''),
    '#description' => t('Insert notification email here.'),
  );
  
  $form['hannotaatio_script_location'] = array(
    '#type' => 'textfield',
    '#title' => t('Script location'),
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 64,
    '#default_value' => variable_get('hannotaatio_script_location', ''),
    '#description' => t('If you are hosting your own Hannotaatio server enter the hannotaatio.js script address here. Remeber to enter also either http:// or https:// in front.'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'hannotaatio_admin_submit';

  return $form;
}

/**
 * Rebuild the JS file only on Save submit.
 *
 * @param <type> $form
 * @param <type> $form_state
 */
function hannotaatio_admin_submit($form, &$form_state) {

  // Check if submit button was pressed and rebuild the functionality.
  // We must use "clicked_button" becaus values['op'] was unset by
  // system_settings_form_submit.
  $op = isset($form_state['clicked_button']['#value']) ? $form_state['clicked_button']['#value'] : '';
  if ($op == t('Save configuration')) {
    _hannotaatio_build_hannotaatio_js();
  }
}
