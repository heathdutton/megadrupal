<?php

/**
 * @file
 * Links variables for translation.
 */
/**
 * Implements hook_variable_group_info()
 */
function luxe_variable_group_info() {
  // Group for variable that have no group
  $groups['luxe'] = array(
    'title' => t('Luxe'),
    'description' => t("Fields use in Luxe."),
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
/**
* Implements hook_variable_info().
*/
function luxe_variable_info($options) {

  // This is the very minimum we need, some descriptive name.

  $variable['luxe_login_title'] = array(
    'title' => t('Login Form Title'),
    'group' => 'luxe',
    'localize' => TRUE,
  );
  $variable['luxe_recover_text'] = array(
    'title' => t("Text for the login form's 'recover password' link."),
    'group' => 'luxe',
    'localize' => TRUE,
  );
  $variable['luxe_login_register_title'] = array(
    'title' => t('Title for login form registration block'),
    'group' => 'luxe',
    'localize' => TRUE,
  );
  $variable['luxe_login_register_title'] = array(
    'title' => t('Registration block text for login form'),
    'group' => 'luxe',
    'localize' => TRUE,
  );
  $variable['luxe_register_title'] = array(
    'title' => t('Registration Form Title'),
    'group' => 'luxe',
    'localize' => TRUE,
  );
  $variable['luxe_register_username_description'] = array(
    'title' => t('Registration Form Username Field Description'),
    'group' => 'luxe',
    'localize' => TRUE,
  );
  $variable['luxe_register_mail_description'] = array(
    'title' => t('Registration Form Email Field Description'),
    'group' => 'luxe',
    'localize' => TRUE,
  );
  $variable['luxe_recover_title'] = array(
    'title' => t('Password Recovery Form Title'),
    'group' => 'luxe',
    'localize' => TRUE,
  );
  return $variable;
}