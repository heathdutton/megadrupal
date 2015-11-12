<?php

/**
* Register our directory as a place to find provision classes.
*/
function provision_variables_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
* Implements hook_drush_init().
*/
function provision_variables_drush_init() {
  provision_variables_provision_register_autoload();
}

/**
* Implements hook_provision_services().
*/
function provision_variables_provision_services() {
  provision_variables_provision_register_autoload();
  return array('variables' => NULL);
}

/**
 * Implements hook_variables_provision_drupal_config().
 *
 * TODO: hook documentation is wrong?
 */
function provision_variables_provision_drupal_config($uri, $data) {
  if ($variables = d()->hosting_variables_variables) {
    drush_log('[hosting_variables] Setting custom variables from Variables tab.');

    $lines = array();
    $lines[] = "  # Custom variables from the Variables tab.";
    foreach ($variables as $name => $value) {
      // Attempt at basic sanitation. Needs more testing.
      $value = str_replace("\\", "", $value);
      $value = str_replace("'", "\'", $value);

      $lines[] = "  \$conf['" . $name . "'] = '" . $value . "';";
    }
    $lines[] = "";

    return implode("\n", $lines);
  }
}

