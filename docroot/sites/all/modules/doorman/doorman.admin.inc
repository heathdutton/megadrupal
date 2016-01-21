<?php
/**
 * @file
 * Admin interfaces for the Doorman module
 */

/**
 * Form to configure which pages are accessible to anonymous users.
 * Presented at: /admin/config/system/doorman.
 */
function doorman_admin_form() {
  $form = array();

  // The page-matching algorithm in Doorman is the same as that used by core:
  // drupal_match_path().
  // Reproduce the core description so that the explanation is consistent.
  $core_description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  $form['doorman_accessible_urls'] = array(
    '#type' => 'textarea',
    '#title' => t('Accessible URLs'),
    '#description' => t('Specify pages which are accessible to anonymous users (the URL %login_url is always accessible).', array('%login_url' => url('user/login'))) . '<br />' . $core_description,
    '#default_value' => variable_get('doorman_accessible_urls', ''),
  );
  
  $form['doorman_login_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Login page'),
    '#description' => t('Enter the URL to redirect anonymous users to, so that they can login.<br />Defaults to %login_url.', array('%login_url' => url('user/login'))),
    '#default_value' => variable_get('doorman_login_page', DOORMAN_DEFAULT_LOGIN_PAGE),
  );

  return system_settings_form($form);
}
