<?php

/**
 * @file
 * Defines the rules defaults.
 */

/**
 * Implements hook_default_rules_configuration_alter().
 */
function ideal_advanced_commerce_default_rules_configuration_alter(&$configs) {
  foreach ($configs as $key => $config) {
    if (preg_match("/ideal_advanced/i", $key)) {
      $configs[$key]->condition('ideal_advanced_commerce_config_validate');
    }
  }
}
