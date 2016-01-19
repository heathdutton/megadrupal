<?php
/**
 * @file
 * Admin file for Getclicky settigns.
 */

/**
 * function returning a $form for configuring the module
 */
function adroll_admin_settings() {

  $form['adroll_adv_id'] = array(
    '#type' => 'textfield',
    '#title' => t('ADV ID'),
    '#default_value' => variable_get('adroll_adv_id', ''),
    '#size' => 30,
    '#maxlength' => 30,
    '#required' => TRUE,
  );

  $form['adroll_pix_id'] = array(
    '#type' => 'textfield',
    '#title' => t('PIX ID'),
    '#default_value' => variable_get('adroll_pix_id', ''),
    '#size' => 30,
    '#maxlength' => 30,
    '#required' => TRUE,
  );

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('User role tracking'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('Define what user roles should be tracked by AdRoll.'),
  );

  $form['roles']['adroll_target_roles'] = array(
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#default_value' => variable_get('adroll_target_roles', array()),
  );

  return system_settings_form($form);
}
