<?php
/**
 * @file
 * minds_eye_vampire.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function minds_eye_vampire_taxonomy_default_vocabularies() {
  return array(
    'me_v_knowledges' => array(
      'name' => 'Mind\'s Eye Vampire Knowledges',
      'machine_name' => 'me_v_knowledges',
      'description' => 'Knowledges for Mind\'s Eye Vampire character sheets',
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
    'me_v_skills' => array(
      'name' => 'Mind\'s Eye Vampire Skills',
      'machine_name' => 'me_v_skills',
      'description' => 'Skills for Mind\'s Eye Vampire character sheets',
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
    'me_v_talents' => array(
      'name' => 'Mind\'s Eye Vampire Talents',
      'machine_name' => 'me_v_talents',
      'description' => 'Talents for Mind\'s Eye Vampire character sheets',
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
