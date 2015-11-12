<?php

/**
 * @file
 * Mirrors field plugin declaration.
 */

/**
 * Field Declaration: taxonomy_term.
 */
$plugin = array(
  'title' => t('Taxonomy Term'),
  'category' => 'Field',
  'views' => array(
    'type' => 'taxonomy_term_reference_plain',
  ),
  'feeds' => array(
    'term_search' => '0',
    'autocreate' => '1',
  ),
);
