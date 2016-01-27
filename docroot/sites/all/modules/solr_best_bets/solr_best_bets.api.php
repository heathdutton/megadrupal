<?php

/**
 * @file
 * Hooks provided by the Acquia Search Best Bets module.
 */

/**
 * Defines environments in order to correctly set the unique identifier.
 *
 * Module such as Apache Solr Search Integration and Search API create unique
 * identifiers differently. This hook allows wach module to build their unique'
 * identifiers as they see fit for the different environments they support.
 *
 * @return array
 *   An associative array keyed by the machine name of the environment, usually
 *   in `module`:`environment` format, to an array containing:
 *   - label: The human readable label of the environment
 *   - id callbacks: The ID callbacks that are used to convert the content ID
 *     into the unique identifier stored in the index.
 *   - id options: (optional) An array of options passed as the second argument
 *     to the id callabcks.
 *   - unique field: (optional) The name of the field in the Solr index that
 *     stores the unique identifier. Defaults to "id".
 */
function hook_solr_best_bets_environment_info() {
  return array(
    'apachesolr:solr' => array(
      'label' => t('Localhost'),
      'id callbacks' => array('apachesolr_entity_document_id'),
      'id options' => array('some_option' => 'some_value'),
      'unique field' => 'id',
    ),
  );
}
