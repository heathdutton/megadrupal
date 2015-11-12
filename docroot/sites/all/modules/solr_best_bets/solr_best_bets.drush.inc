<?php

/**
 * @file
 * Drush commands for Solr Best Bets.
 */

/**
 * Implements hook_drush_command.
 */
function solr_best_bets_drush_command() {
  $items = array();

  $items['elevate-xml-generate'] = array(
    'description' => 'Generates a valid elevate.xml file.',
    'arguments' => array(
      'environment-name' => 'The machine name of the search environment, usually in `module`:`environment_id` format.',
    ),
    'options' => array(
      //'outfile' => 'An optional outfile the XML document is written to.',
      'list-environments' => 'Print all available environments, one per line, and exit.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  return $items;
}

/**
 * Outputs stats for a single server.
 *
 * @param string $environment_name
 *   The machine name of the search environment, usually in
 *   `module`:`environment_id` format.
 */
function drush_solr_best_bets_elevate_xml_generate($environment_name = NULL) {
  // Print valid environments and exit if option is set.
  if (drush_get_option('list-environments')) {
    foreach (solr_best_bets_get_environments() as $environment) {
      drush_print($environment['name']);
    }
    return;
  }

  // Render the XML document for the passed environment.
  if (!$environment_name) {
    drush_set_error(DRUSH_APPLICATION_ERROR, dt('Environment required.'));
  }
  elseif (!$environment = solr_best_bets_environment_load($environment_name)) {
    drush_set_error(DRUSH_APPLICATION_ERROR, dt('Environment not valid.'));
  }
  else {
    module_load_include('inc', 'solr_best_bets', 'solr_best_bets.crud');
    drush_print(solr_best_bets_get_xml($environment), 0, NULL, FALSE);
  }
}
