<?php
/**
 * @file
 * Set enabled Languages per Domain realm.
 */

/**
 * Implements hook_variable_info().
 */
function domain_variable_locale_variable_info($options) {
  // 'language_list' is a Drupal Core function. hook_init() is used to leverage
  // the variable table to override static_cached result of language_list().
  // This variable is exposed to the Domain Realm with the 'multidomain'
  // property.
  $variables['language_list'] = array(
    'title' => t('List of active languages'),
    'group' => 'regional_settings',
    'default callback' => 'domain_variable_locale_language_list_defaults',
    'type' => 'options',
    'options callback' => 'domain_variable_locale_language_list',
    'multidomain' => TRUE,
  );

  return $variables;
}

/**
 * Callback for default language_list settings.
 *
 * @return array $language_list
 *   - Saved values if form was saved before.
 *   - Defaults to all enabled languages.
 */
function domain_variable_locale_language_list_defaults($variable, $options) {
  // Return variable if form has been saved.
  $language_list = variable_get('language_list');
  if ($language_list) {
    return $language_list;
  }

  // If variable is empty, return all enabled languages. Effectively all options
  // are checked.
  $enabled_languages = array();
  drupal_static_reset('language_list');
  foreach (language_list() as $language) {
    if ($language->enabled) {
      // Array format: language_machine_name.
      $enabled_languages[] = $language->language;
    }
  }

  return $enabled_languages;
}

/**
 * Callback for language_list options list.
 *
 * Returns an option list with enabled languages.
 */
function domain_variable_locale_language_list($variable, $options) {
  $options_list = array();

  // Be sure we use all available languages.
  drupal_static_reset('language_list');

  foreach (language_list() as $language) {
    if ($language->enabled) {
      // Array format: language_machine_name->Name.
      $options_list[$language->language] = $language->name;
    }
  }
  return $options_list;
}
