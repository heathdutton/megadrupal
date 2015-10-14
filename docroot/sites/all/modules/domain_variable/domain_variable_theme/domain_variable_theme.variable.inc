<?php
/**
 * @file
 * Set active Theme per Domain realm.
 */

/**
 * Implements hook_variable_info().
 */
function domain_variable_theme_variable_info($options) {
  // 'theme_default' is a Drupal Core variable. By exposing it to Variable
  // module it can be changed per Domain Realm.
  // This variable is exposed to the Domain Realm with the 'multidomain'
  // property.
  $variables['theme_default'] = array(
    'title' => t('Active theme'),
    'group' => 'theme_settings',
    'default callback' => 'domain_variable_theme_theme_default_defaults',
    'type' => 'select',
    'options callback' => 'domain_variable_theme_theme_list',
    'multidomain' => TRUE,
  );

  return $variables;
}

/**
 * Callback for default theme settings.
 */
function domain_variable_theme_theme_default_defaults($variable, $options) {
  return variable_get('theme_default', 'bartik');
}

/**
 * Callback for theme options list.
 *
 * Returns an option list with enabled themes.
 */
function domain_variable_theme_theme_list($variable, $options) {
  $options_list = array();

  foreach (list_themes() as $theme) {
    if ($theme->status) {
      // Array format: theme_machine_name->Name (Description).
      $options_list[$theme->name] = $theme->info['name'] . " (" . $theme->info['description'] . ")";
    }
  }
  return $options_list;
}
