<?php
/**
 * @file
 * bassets_client_image_metadata.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function bassets_client_image_metadata_taxonomy_default_vocabularies() {
  return array(
    'image_motive_category' => array(
      'name' => 'Motivkategorie',
      'machine_name' => 'image_motive_category',
      'description' => 'Kategorie der Abbildungen',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -9,
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
    'image_motive_type' => array(
      'name' => 'Motivtyp',
      'machine_name' => 'image_motive_type',
      'description' => 'Typ der Abbildungen',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -10,
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
    'image_tags' => array(
      'name' => 'Bild-Schlagwörter',
      'machine_name' => 'image_tags',
      'description' => 'freie Verschlagwortung der Bildinhalte',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -8,
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
