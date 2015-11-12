<?php
/**
 * @file
 */

/**
 * Implements hook_drush_command().
 */
function microservices_drush_command() {
  return array(
    'microservices-schema' => array(
      'description' => 'Export microservices schema',
      'callback' => 'drush_microservices_schema',
    ),
  );
}

function drush_microservices_schema() {
  print '$conf[\'microservices_schema\'] = ' . var_export(microservices_info(TRUE), TRUE) . ";\n";
}
