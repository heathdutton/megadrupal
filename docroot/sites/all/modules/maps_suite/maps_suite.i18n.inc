<?php

/**
 * @file
 * Provides i18n integration for MaPS Suite module.
 */

/**
 * Implements hook_i18n_string_info().
 *
 * We do not implement hook_locale() since i18n String module will do
 * it for us. Otherwise it would interfere with i18n String module work.
 */
function maps_suite_i18n_string_info() {
  $groups = array();

  $groups[MAPS_SUITE_LOCALE_TEXTGROUP] = array(
    'title' => t('MaPS Suite'),
    'description' => t('Specific terms for MaPS Suite related items.'),
    // This group doesn't have strings with format.
    'format' => FALSE,
    // This group cannot list all strings.
    'list' => FALSE,
  );

  return $groups;
}
