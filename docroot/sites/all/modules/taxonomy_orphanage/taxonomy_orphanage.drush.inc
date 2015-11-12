<?php

/**
 * @file
 * Drush callbacks for taxonomy_orphanage.
 */

/**
 * Implements hook_drush_command().
 */
function taxonomy_orphanage_drush_command() {
  $items = array();

  $items['taxonomy-orphanage-roundup'] = array(
    'description' => 'Finds and removes orphaned taxonomy references.',
    'aliases' => array('tor'),
    'options' => array(
      'limit' => 'The number of entities to process per field.',
    ),
  );

  return $items;
}

/**
 * Drush callback that passes off the roundup to our internal function.
 */
function drush_taxonomy_orphanage_roundup() {
  taxonomy_orphanage_roundup(FALSE, drush_get_option('limit', -1));
}

