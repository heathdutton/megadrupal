<?php
/**
 * @file
 * storiedmaps_story.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function storiedmaps_story_taxonomy_default_vocabularies() {
  return array(
    'story_type' => array(
      'name' => 'Story type',
      'machine_name' => 'story_type',
      'description' => 'For categorizing stories.',
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
