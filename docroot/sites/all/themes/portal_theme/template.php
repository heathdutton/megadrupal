<?php
/**
 * @file
 * Functions and overrides for the Portal theme.
 */

/**
 * Implements template_preprocess_html.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function portal_theme_preprocess_html(&$variables) {
  $settings = variable_get('theme_portal_theme_settings', '') ? variable_get('theme_portal_theme_settings', '') : array();

  $variables['classes_array'] = array();
  if ($variables['is_front']) {
    $variables['classes_array'][] = 'front';
  }
  else {
    $variables['classes_array'][] = 'not-front';
  }

  if (theme_get_setting('portal_theme_show_front_page_title') && $variables['is_front']) {
    $variables['classes_array'][] = 'with-front-page-title';
  }
}

/**
 * Implements template_preprocess_page.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function portal_theme_preprocess_page(&$variables) {
  $variables['classes_array'][] = 'wrapper-body';

  // Handle logo setting.
  if (theme_get_setting('portal_theme_enable_logo')) {
    $variables['logo'] = theme_get_setting('logo');
  }
  else {
    $variables['logo'] = '';
  }

  // Handle attribution setting.
  if (theme_get_setting('portal_theme_attribution')) {
    $variables['portal_themeattribution'] = theme_get_setting('portal_theme_attribution');
  }
  else {
  $variables['portal_themeattribution'] = '';
  }

  // Handle CiviCRM setting.
  if (theme_get_setting('portal_theme_is_civicrm')) {
    $variables['portal_themecivicrm'] = theme_get_setting('portal_theme_is_civicrm');
  }
  else {
    $variables['portal_themecivicrm'] = '';
  }

  // Handle Client Name/URL settings.
  if (theme_get_setting('portal_theme_client_name')) {
    $variables['portal_themeclientname'] = theme_get_setting('portal_theme_client_name');
  }
  else {
    $variables['portal_themeclientname'] = 'Your Client';
  }

  if (theme_get_setting('portal_theme_client_url')) {
    $variables['portal_themeclienturl'] = theme_get_setting('portal_theme_client_url');
  }
  else {
    $variables['portal_themeclienturl'] = 'http://example.com';
  }

  // Handle Designer Name/URL settings.
  if (theme_get_setting('portal_theme_designer_name')) {
    $variables['portal_themedesignername'] = theme_get_setting('portal_theme_designer_name');
  }
  else {
    $variables['portal_themedesignername'] = 'MJCO';
  }

  if (theme_get_setting('portal_theme_designer_url')) {
    $variables['portal_themedesignerurl'] = theme_get_setting('portal_theme_designer_url');
  }
  else {
    $variables['portal_themedesignerurl'] = 'http://mjco.me.uk';
  }

  // Override Page/Block titles.
  if (arg(0) == 'user') {
    switch (arg(1)) {
      case 'register':
        drupal_set_title(t('Registration:'));
        break;

      case 'password':
        drupal_set_title(t('Password Reset:'));
        break;

      case 'login':
        drupal_set_title(t('Login Required:'));
        break;

    }
  }
}
