<?php
/**
 * @file
 * Variable definitions for Social Counters.
 */

/**
 * Implements hook_variable_group_info().
 */
function social_counters_variable_group_info() {
  $groups['social_counters'] = array(
    'title'       => t('Social Counters'),
    'description' => t('Social Counters settings.'),
    'access'      => 'administer site configuration',
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function social_counters_variable_info($options) {
  $variables['social_counters_settings_expire'] = array(
    'title'       => t('Expire time.'),
    'group'       => 'social_counters',
    'description' => t("The number of seconds social counters should be cached."),
  );

  $variables['social_counters_status'] = array(
    'title'       => t('Enabled Social Counters.'),
    'group'       => 'social_counters',
    'description' => t("Social Counters that are enabled."),
  );

  // Get counters.
  $counters = social_counters_get_counters();
  foreach ($counters as $social_network => $data) {
    $base = 'social_counters_' . $social_network;
    foreach ($data['variables'] as $var => $data) {
      $name = $base . '_' . $var;
      $variables[$name] = array(
        'title'       => $data['title'],
        'group'       => 'social_counters',
        'localize'    => TRUE,
        'description' => !empty($data['description']) ? $data['description'] : NULL,
      );
    }
  }

  return $variables;
}
