<?php
/**
 * @file
 * multi_currency_pricelist.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function multi_currency_pricelist_taxonomy_default_vocabularies() {
  return array(
    'pricelist_section' => array(
      'name' => 'Pricelist section',
      'machine_name' => 'pricelist_section',
      'description' => 'A group of pricelist items (for example starters, main dishes, deserts).',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
