<?php
/**
 * @file
 * nm_blog.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function nm_blog_taxonomy_default_vocabularies() {
  return array(
    'nm_blog_category' => array(
      'name' => 'Blog Category',
      'machine_name' => 'nm_blog_category',
      'description' => 'Categories for Blog Posts',
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
