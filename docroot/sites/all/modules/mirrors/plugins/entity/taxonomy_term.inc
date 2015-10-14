<?php

/**
 * @file
 * Mirrors Node entity plugin declaration.
 */

/**
 * Entity Declaration: taxonomy term.
 *
 * @todo Property "tid" not supported by feeds. Using "name" as unique identifier.
 */
$plugin = array(
  'title' => t('Taxonomy term'),
  'category' => 'Entity',
  'views' => array(
    'base_table' => 'taxonomy_term_data',
    'filters' => array(
      'machine_name' => 'taxonomy_vocabulary',
    ),
    'relationships' => array(
      'parent' => 'taxonomy_term_hierarchy',
    ),
    'sorts' => array(
      'tid' => 'taxonomy_term_data',
    ),
  ),
  'feeds' => array(
    'processor' => 'FeedsTermProcessor',
  ),
  'properties' => array(
    // TermID not supported by Feeds.
    'name' => array(
      'type' => 'text',
      'feeds' => array(
        'unique' => TRUE,
      ),
    ),
    'parent' => array(
      'type' => 'integer',
      'views' => array(
        'field' => 'name',
        'relationship' => 'parent',
      ),
    ),
    'weight' => array(
      'type' => 'integer',
    ),
    'description' => array(
      'type' => 'text',
    ),
  ),
);
