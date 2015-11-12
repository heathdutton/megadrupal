<?php
/**
 * @file
 * Internationalization (i18n) hooks.
 */

/**
 * Implements hook_i18n_string_info()
 */
function eloqua_subscription_center_i18n_string_info() {
  $groups['eloqua'] = array(
    'title' => t('Eloqua'),
    'description' => t('Eloqua related strings.'),
    'format' => FALSE, // This group doesn't have strings with format
    'list' => TRUE, // This group can list all strings
  );
  return $groups;
}

/**
 * Implements hook_i18n_string_list().
 */
function eloqua_subscription_center_i18n_string_list($group) {
  if ($group == 'eloqua' || $group == 'all') {
    $strings = array();

    $groups = variable_get('eloqua_subscription_center_groups', array());
    $enabled = eloqua_subscription_center_get_enabled_groups();

    foreach ($groups as $eid => $element) {
      if (in_array($eid, $enabled)) {
        $id = $element['#id'];
        $title = $element['#title'];
        $description = $element['#description'];

        $strings['eloqua']['form']['subscription'][$id] = array(
          'title' => $title,
          'description' => $description,
        );
      }
    }
    return $strings;
  }
}

/**
 * Implements hook_i18n_string_refresh().
 *
 * Refresh translations for all generated strings, either retrieved from Eloqua
 * or user overridden via admin config.
 */
function eloqua_subscription_center_i18n_string_refresh($group) {
  if ($group == 'eloqua') {
    eloqua_subscription_center_i18n_update_strings();
  }
  return TRUE;
}
