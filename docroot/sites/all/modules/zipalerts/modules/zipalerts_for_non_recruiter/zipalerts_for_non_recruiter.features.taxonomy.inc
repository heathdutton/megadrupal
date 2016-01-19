<?php

/**
 * @file
 * File zipalerts_for_non_recruiter.features.taxonomy.inc.
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function zipalerts_for_non_recruiter_taxonomy_default_vocabularies() {
  return array(
    'geography' => array(
      'name' => 'Geography',
      'machine_name' => 'geography',
      'description' => '',
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
