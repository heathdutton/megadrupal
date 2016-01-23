<?php
/**
 * @file
 * austrofeedr_base_subscription.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function austrofeedr_base_subscription_taxonomy_default_vocabularies() {
  return array(
    'subscription_tags' => array(
      'name' => 'Austrofeedr Subscription Tags',
      'machine_name' => 'subscription_tags',
      'description' => 'Incoming tags from austrofeedr subscription sources',
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
