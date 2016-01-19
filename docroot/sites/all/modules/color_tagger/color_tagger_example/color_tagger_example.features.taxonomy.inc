<?php
/**
 * @file
 * color_tagger_example.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function color_tagger_example_taxonomy_default_vocabularies() {
  return array(
    'colors' => array(
      'name' => 'Colors',
      'machine_name' => 'colors',
      'description' => 'A set of color organized by name with associated hexadecimal values.',
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
