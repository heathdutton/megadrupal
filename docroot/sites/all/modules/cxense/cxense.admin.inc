<?php
/**
 * @file
 * Administrative page callbacks for the cXense module.
 */

/**
 * General configuration form for controlling the cXense behaviour.
 */
function cxense_admin() {
  $form = array();

  $form['cxense_site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('The site ID of your cXense account.'),
    '#default_value' => variable_get('cxense_site_id', ''),
    '#size' => 30,
    '#maxlength' => 30,
    '#required' => TRUE,
    '#description' => t("Specify your cXense Site-ID here."),
  );

  return system_settings_form($form);
}
