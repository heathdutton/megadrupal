<?php
/**
 * @file
 * nm_events.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function nm_events_taxonomy_default_vocabularies() {
  return array(
    'nm_event_category' => array(
      'name' => 'Event Category',
      'machine_name' => 'nm_event_category',
      'description' => 'Categories for Event Posts',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
      'metatags' => array(),
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
