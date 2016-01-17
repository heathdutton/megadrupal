<?php
/**
 * @file
 * recruiter_job_application.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function recruiter_job_application_taxonomy_default_vocabularies() {
  return array(
    'job_application_rejection_reason' => array(
      'name' => 'Job application rejection reason',
      'machine_name' => 'job_application_rejection_reason',
      'description' => 'Possible reasons for rejection',
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
