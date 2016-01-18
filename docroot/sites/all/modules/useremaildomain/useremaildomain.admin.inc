<?php
/**
 * @file
 * Configure the settings form for the useremaildomain module.
 */

 /**
 * Form function, called by drupal_get_form()
 * in useremaildomain_menu().
 */
function useremaildomain_form($form, &$form_state) {
  $form['useremaildomain_domains'] = array(
    '#type' => 'textarea',
    '#title' => t('Approved domains'),
    '#default_value' => variable_get('useremaildomain_domains'),
    '#description' => t('Enter your approved email domains, separated by commas.'),
    '#rows' => 3,
    '#required' => TRUE,
  );

  $form['useremaildomain_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Email field description'),
    '#default_value' => variable_get('useremaildomain_description'),
    '#description' => t('The default description of what you want entered into the email field of the domains that will be approved.'),
    '#rows' => 3,
    '#required' => FALSE,
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_alter().
 *
 */
function useremaildomain_form_alterTEST(&$form, &$form_state, $form_id) {
  switch ($form_id) {
    case 'user_register_form':
      if (isset($form['account']) && is_array($form['account'])) {
        $form['account']['mail']['#title'] = t('E-mail');
      }
      else {
        $form['mail']['#title'] = t('E-mail');
      }
      break;
  }
}
