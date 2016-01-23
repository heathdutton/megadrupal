<?php
/**
 * @file
 * Administrative functionality for the Alt Login Page module.
 */

/**
 * FormAPI callback to build the settings form.
 */
function alt_login_page_settings_form() {
  $form['alt_login_page_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to the alternative login page'),
    '#default_value' => variable_get('alt_login_page_path'),
    '#required' => TRUE,
    '#description' => t('This should be a path to where the new login page will reside, e.g. "admin-login", "site-login", "secret-login-page-mu-ha-ha-ha".'),
  );

  $form['alt_login_page_logout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Bypass normal logout?'),
    '#description' => t('Checking this box causes users with the "Use alternative login page" permission to logout without running the functionality other modules have implemented. Note: this will destroy the session without triggering other implementations of hook_user_logout.'),
    '#default_value' => variable_get('alt_login_page_logout', FALSE),
  );

  $form['alt_login_page_logout_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path for the logout destination'),
    '#description' => t('Set this to "&lt;front&gt;" to redirect to the homepage.'),
    '#default_value' => variable_get('alt_login_page_logout_path', '<front>'),
    '#states' => array(
      'visible' => array(
        ':input[name="alt_login_page_logout"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['#submit'][] = 'alt_login_page_settings_form_submit';

  return system_settings_form($form);
}

/**
 * FormAPI submission callback for alt_login_page_settings_form().
 */
function alt_login_page_settings_form_submit($form, &$form_state) {
  variable_set('menu_rebuild_needed', TRUE);
}
