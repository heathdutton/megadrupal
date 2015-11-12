<?php

/**
 * @file
 *   Drush integration for appserver.
 */

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function sparqlfqc_drush_command() {
  $items = array();

  $items['sparqlfqc-get-data'] = array(
    'description' => dt('Get/Refresh RDF data for SPARQL Federated Query Cache.'),
    'aliases' => array('sgd'),
    'drupal dependencies' => array('sparql', 'sparql_endpoint'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function sparqlfqc_drush_help($section) {
  switch ($section) {
    case 'drush:federated-query-cache':
      return dt("Cache federated query data from SPQARQL endpoints.");
  }
}

/**
 * Drush command callback.
 *
 * This is where the action takes place.
 *
 * In this function, all of Drupals API is (usually) available, including
 * any functions you have added in your own modules/themes.
 *
 * To print something to the terminal window, use drush_print().
 */
function drush_sparqlfqc_get_data() {
  sparqlfqc_get_data();
}
