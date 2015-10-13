<?php
/**
 * @file
 * hotfolder.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function hotfolder_taxonomy_default_vocabularies() {
  return array(
    'hotfolder_streams' => array(
      'name' => 'Hotfolder Streams',
      'machine_name' => 'hotfolder_streams',
      'description' => 'Streams for use with the Hotfolder Module',
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
