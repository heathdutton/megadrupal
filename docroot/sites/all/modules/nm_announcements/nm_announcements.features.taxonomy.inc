<?php
/**
 * @file
 * nm_announcements.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function nm_announcements_taxonomy_default_vocabularies() {
  return array(
    'nm_announcement_category' => array(
      'name' => 'Announcement Category',
      'machine_name' => 'nm_announcement_category',
      'description' => 'Categories for Announcement Posts',
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
