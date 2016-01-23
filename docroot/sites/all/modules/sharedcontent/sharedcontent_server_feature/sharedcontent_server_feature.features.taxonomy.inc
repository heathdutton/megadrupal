<?php
/**
 * @file
 * sharedcontent_server_feature.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function sharedcontent_server_feature_taxonomy_default_vocabularies() {
  return array(
    'sharedcontent_reason' => array(
      'name' => 'Reason',
      'machine_name' => 'sharedcontent_reason',
      'description' => '',
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
