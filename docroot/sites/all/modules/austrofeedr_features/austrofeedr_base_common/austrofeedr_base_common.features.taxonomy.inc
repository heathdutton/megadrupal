<?php
/**
 * @file
 * austrofeedr_base_common.features.taxonomy.inc
 */

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function austrofeedr_base_common_taxonomy_default_vocabularies() {
  return array(
    'update_tags' => array(
      'name' => 'Austrofeedr Update Tags',
      'machine_name' => 'update_tags',
      'description' => 'Tags to classify austrofeedr updates',
      'hierarchy' => '1',
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
