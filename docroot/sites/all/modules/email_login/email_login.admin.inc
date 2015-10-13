<?php

/**
 * @file
 * Administrative functions for Email Login.
 */

/**
 * Form constructor for the email_login Settings form.
 */
function email_login_settings_form($form, &$form_state) {
  if (count(email_login_get_user_roles()) == 0) {
    $form['no_user_roles'] = array(
      '#title' => t('No user roles'),
      '#type' => 'fieldset',
    );
    $form['no_user_roles']['message'] = array(
      '#markup' => t('Currently there are no user roles in the system.'),
    );
    return $form;
  }
  else {
    $form['email_login_roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select Roles'),
      '#description' => t('Users having only selected role can login with just email address'),
      '#default_value' => variable_get('email_login_roles', array('0')),
      '#options' => email_login_get_user_roles(),
    );
    return system_settings_form($form);
  }
}

/**
 * Get User roles data.
 */
function email_login_get_user_roles() {
  $query = db_select('role', 'r');
  $query->fields('r', array('rid', 'name'));
  $query->condition('r.rid',
                    array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID, 3),
                    'NOT IN');
  $roles = $query->execute()->fetchAllKeyed();

  return $roles;
}
