<?php
/**
 * @file
 * Changes some settings of Domain Variable module.
 */

/**
 * Implements hook_variable_info_alter().
 *
 * Add variables to 'localize' and 'multidomain' realms.
 * If a variable is added to both realms it will be unionized.
 */
function domain_integration_variable_variable_info_alter(&$variables, $options) {

  // Language dependent variables.
  $realm = 'localize';
  $list = array(
    'date_format_[date_type]',
  );
  foreach ($list as $name) {
    if (isset($variables[$name])) {
      $variables[$name][$realm] = TRUE;
    }
  }

  // Domain dependent variables.
  $realm = 'multidomain';
  $list = array(
    'date_format_[date_type]',
    'site_default_country',
  );
  foreach ($list as $name) {
    if (isset($variables[$name])) {
      $variables[$name][$realm] = TRUE;
    }
  }
}
