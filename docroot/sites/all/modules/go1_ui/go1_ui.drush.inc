<?php
/**
 * @file go1_ui.drush.inc
 */

require_once dirname(__FILE__) . '/drush/go1_services.inc';
require_once dirname(__FILE__) . '/drush/go1_twig.inc';

/**
 * Implements hook_drush_command()
 */
function go1_ui_drush_command() {
  $cmds = array();

  $cmds['go1_service'] = array(
    'callback' => 'drush_go1_services',
    'description' => 'List all services',
    'examples' => array(
      'drush go1_service' => 'List all services',
      'drush go1_service server_name' => 'View definition of service_name.',
    ),
    'aliases' => array('go1-service', 'go1s'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $cmds['go1_doc_twig'] = array(
    'callback' => 'drush_go1_twig_doc',
    'description' => 'Lists twig functions, filters, globals and tests present in the current project',
    'examples' => array(
      'drush twig:doc' => 'Display all Twig functions/filters',
      'drush twig:doc drupalView' => 'View details of drupalView filter.'
    ),
    'aliases' => array('twig:doc'),
  );

  return $cmds;
}
