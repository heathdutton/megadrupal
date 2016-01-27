<?php
/**
 * @file
 * Project Browser module admin pages.
 */

// ======================================
// Administration Page:
// ======================================

/**
 * Admin Settings Form
 */
function project_browser_admin() {
  $form['main'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['main']['project_browser_servers'] = array(
    '#type' => 'textarea',
    '#title' => t('Repositories'),
    '#default_value' => variable_get('project_browser_servers', ''),
    '#description' => t("Add new repositories to use for the Project Browser, one per line, in 
      the 'url|method|Site Name' format. Drupal.org is added by default, and doesn't need to be 
      set here."),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}
