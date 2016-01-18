<?php

/**
 * @file
 * Admin settings page for module.
 */

/**
 * Settings page for module.
 */
function smart_breadcrumb_admin_settings() {
  $page_cache = variable_get('cache', 0);

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Settings'),
    '#collapsible' => FALSE
  );

  $form['settings']['smart_breadcrumb_home'] = array(
    '#type' => 'textfield',
    '#title' => t('Home breadcrumb text'),
    '#default_value' => variable_get('smart_breadcrumb_home', NULL),
    '#description' => t('Text to use for the home anchor in the breadcrumb trail.')
  );

  $form['settings']['smart_breadcrumb_home_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Home breadcrumb class'),
    '#default_value' => variable_get('smart_breadcrumb_home_class', NULL),
    '#description' => t('Class value to be used for the home anchor in the breadcrumb trail.')
  );

  $form['settings']['smart_breadcrumb_current'] = array(
    '#type' => 'textfield',
    '#title' => t('Current class'),
    '#default_value' => variable_get('smart_breadcrumb_current', NULL),
    '#description' => t('Class value to be used for the last item in the breadcrumb trail (i.e. the current page).')
  );

  $form['settings']['smart_breadcrumb_tag'] = array(
    '#type' => 'textfield',
    '#title' => t('Wrapper Tag'),
    '#default_value' => variable_get('smart_breadcrumb_tag', 'span'),
    '#description' => t('HTML tag to use for each item in the breadcrumb.')
  );

  $form['settings']['smart_breadcrumb_link_wrap'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wrap linked breadcrumbs with Wrapper Tag'),
    '#default_value' => variable_get('smart_breadcrumb_link_wrap', 0),
    '#description' => t('Wraps linked breadcrumbs, including the Home breadcrumb, with wrapper tag specified above.')
  );

  $form['settings']['smart_breadcrumb_devel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Developer Mode'),
    '#default_value' => $page_cache ? 0 : variable_get('smart_breadcrumb_devel', 0),
    '#disabled' => $page_cache ? TRUE : FALSE,
    '#description' => $page_cache ? t('Disabled automatically due to page cache being enabled. To enable, page caching must be set to disabled in the !url.', array('!url' => l('Performance settings', 'admin/settings/performance'))) : t('Does not used cached values for breadcrumb trail. Due to performance degregation, this should be disabled in a production environment.')
  );

  $form['settings']['smart_breadcrumb_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Bypass Smart Breadcrumb on the following pages (ensure to include AJAX driven pages)'),
    '#default_value' => variable_get('smart_breadcrumb_exclude', NULL),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'))
  );

  // force to developer mode if page cache is enabled, otherwise use Smart Breadcrumb setting
  $devel = variable_get('cache', 0) ? 0 : variable_get('smart_breadcrumb_devel', 0);

  return system_settings_form($form);
}
