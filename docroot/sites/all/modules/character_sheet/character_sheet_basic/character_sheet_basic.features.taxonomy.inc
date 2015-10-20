<?php
/**
 * @file
 * character_sheet_basic.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function character_sheet_basic_taxonomy_default_vocabularies() {
  return array(
    'skills' => array(
      'name' => 'Skills',
      'machine_name' => 'skills',
      'description' => 'Skills for the character sheet.',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
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
