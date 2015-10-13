<?php
/**
 * @file
 * Entity property information for the Election Candidate module.
 */

/**
 * Implements hook_entity_property_info().
 */
function election_candidate_entity_property_info() {
  $info = array();
  // Add metadata about candidate properties.
  $properties = &$info['election_candidate']['properties'];
  $properties = array(
    'candidate_id' => array(
      'label' => t('Candidate ID'),
      'description' => t('Primary key: the unique identifier for a candidate.'),
      'schema field' => 'candidate_id',
    ),
    'election_id' => array(
      'label' => t('Election ID'),
      'description' => t('The election for this candidate.'),
      'type' => 'election',
      'schema field' => 'election_id',
      'required' => TRUE,
    ),
    'post_id' => array(
      'label' => t('Post ID'),
      'description' => t('The post for which the candidate is standing.'),
      'type' => 'election_post',
      'schema field' => 'post_id',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
    ),
    'type' => array(
      'label' => t('Type'),
      'description' => t('The candidate type.'),
      'type' => 'token',
      'required' => TRUE,
      'schema field' => 'type',
      'options list' => '_election_candidate_get_type_options',
    ),
    'published' => array(
      'label' => t('Published status'),
      'description' => t('Whether or not the candidate is published.'),
      'type' => 'boolean',
      'options list' => '_election_get_published_options',
      'schema field' => 'published',
      'required' => TRUE,
    ),
    'cstatus' => array(
      'label' => t('Candidate status'),
      'description' => t('The electoral status of the candidate.'),
      'type' => 'integer',
      'options list' => '_election_candidate_get_cstatus_options',
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'cstatus',
      'required' => TRUE,
    ),
    'uid' => array(
      'label' => t('User'),
      'description' => t('The user associated with the candidate.'),
      'type' => 'user',
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'uid',
    ),
    'created' => array(
      'label' => t('Created time'),
      'description' => t('The date when the candidate was created (or nomination date).'),
      'type' => 'date',
      'schema field' => 'created',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'changed' => array(
      'label' => t('Last modified time'),
      'description' => t('The date when the candidate was last updated.'),
      'type' => 'date',
      'schema field' => 'changed',
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );
  return $info;
}
