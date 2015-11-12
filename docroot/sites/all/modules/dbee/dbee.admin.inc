<?php

/**
 * @file
 * Administrations functions for the dbee module, on the admin/settings/dbee
 * page.
 */

/**
 * Form builder; administer dbee module options.
 *
 * Add on option to enable to loggin using either username or email address.
 *
 * @see dbee_admin_form_validate()
 * @see dbee_admin_form_submit()
 * @ingroup forms
 */
function dbee_admin_form($form, $form_state) {
  $form['dbee'] = array(
    '#type' => 'fieldset',
    '#title' => t('DataBase Email Encryption module'),
    '#description' => t('All email addresses are encrypted now'),
  );
  module_load_include('module', 'dbee');
  $email_registration = (dbee_mail_login_fix());
  if ($email_registration) {
    variable_set('dbee_loggin_email', (!$email_registration) ? 0 : 1);
  }

  $form['dbee']['loggin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow to loggin using email address'),
    '#default_value' => variable_get('dbee_loggin_email', 0),
    '#disabled' => $email_registration,
  );
  if ($email_registration) {
    $form['dbee']['loggin']['#description'] = t('This feature is handled by the %module module.', array('%module' => (dbee_logintoboggan_fix()) ? t('LoginToboggan') : t('Email Registration')));
  }

  $description = t('Enable this feature only if you meet performance troubles on the user search.');
  $description .= '<br />' . t('Thoses performance troubles may occures on one specific situation :');
  $conditions = array();
  $conditions[] = t('<a href="@url">Searching a user</a>,', array('@url' => url('search/user')));
  $conditions[] = t("by a connected user granted with the 'administer users' permission,", array('@url' => url('search/user')));
  $conditions[] = t('on a site with a lot of register users.');
  $description .= theme('item_list', array('items' => $conditions));
  $description .= t("As a limitation, only completed emails will be finded. As an example, on site storing a user with the 'john@example.com' mail value, if this checkbox is checked, searching this user using the key word 'hn@examp' will fail but the whole email address search key 'john@example.com' will succeed. In both cases, the result will be returned very fast.");
  $form['dbee']['custom_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable searching users via uncompleted email address.'),
    '#default_value' => variable_get('dbee_custom_search', 0),
    '#description' => $description,
  );

  // Load button.
  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate dbee module options.
 */
function dbee_admin_form_validate($form, &$form_state) {
  // Validating function
  if (isset($form_state['values']['loggin']) && !in_array($form_state['values']['loggin'], array('', 0, 1))) {
    form_set_error('loggin', t("The '%setting' setting is not valid", array('%setting' => t('Allow to loggin using email address'))));
  }
  if (isset($form_state['values']['custom_search']) && !in_array($form_state['values']['custom_search'], array('', 0, 1))) {
    form_set_error('custom_search', t("The '%setting' setting is not valid", array('%setting' => t('Disable searching users via uncompleted email address.'))));
  }
}

/**
 * Submit dbee module options.
 */
function dbee_admin_form_submit($form, &$form_state) {
  if ($form_state['values']['loggin'] != variable_get('dbee_loggin_email', 0) || $form_state['values']['custom_search'] != variable_get('dbee_custom_search', 0)) {
    variable_set('dbee_loggin_email', trim(check_plain($form_state['values']['loggin'])));
    variable_set('dbee_custom_search', trim(check_plain($form_state['values']['custom_search'])));
    drupal_set_message(t('New setting has been saved'));
  }
}
