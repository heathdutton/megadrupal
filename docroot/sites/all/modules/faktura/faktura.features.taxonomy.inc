<?php
/**
 * @file
 * faktura.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function faktura_taxonomy_default_vocabularies() {
  return array(
    'invoice_item_units' => array(
      'name' => 'Invoice item units',
      'machine_name' => 'invoice_item_units',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
