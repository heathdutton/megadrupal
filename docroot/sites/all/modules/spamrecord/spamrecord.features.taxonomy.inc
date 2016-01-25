<?php
/**
 * @file
 * spamrecord.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function spamrecord_taxonomy_default_vocabularies() {
  return array(
    'spamrecord_msg_type' => array(
      'name' => 'Message type',
      'machine_name' => 'spamrecord_msg_type',
      'description' => 'Type of message: phone/sms/fax…',
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
