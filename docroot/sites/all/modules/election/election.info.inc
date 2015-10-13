<?php
/**
 * @file
 * Entity property information for the Election module.
 */

/**
 * Implements hook_entity_property_info().
 */
function election_entity_property_info() {
  $info = array();
  // Add metadata about election properties.
  $properties = &$info['election']['properties'];
  $properties = array(
    'election_id' => array(
      'label' => t('Election ID'),
      'description' => t('Primary key: the unique identifier for an election.'),
      'type' => 'integer',
      'schema field' => 'election_id',
    ),
    'type' => array(
      'label' => t('Type'),
      'type' => 'token',
      'description' => t('The election type.'),
      'required' => TRUE,
      'schema field' => 'type',
      'options list' => '_election_get_type_options',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'uid' => array(
      'label' => t('Owner'),
      'description' => t('The user who created or who owns the election.'),
      'type' => 'user',
      'schema field' => 'uid',
    ),
    'published' => array(
      'label' => t('Published status'),
      'description' => t('Whether or not the election is published.'),
      'type' => 'boolean',
      'options list' => '_election_get_published_options',
      'schema field' => 'published',
      'required' => TRUE,
    ),
    'title' => array(
      'label' => t('Title'),
      'description' => t('The title of the election.'),
      'type' => 'text',
      'schema field' => 'title',
      'required' => TRUE,
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'vstatus' => array(
      'label' => t('Voting status'),
      'description' => t('The voting status of the election.'),
      'type' => 'integer',
      'schema field' => 'vstatus',
      'required' => TRUE,
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'vopen_time' => array(
      'label' => t('Voting open time'),
      'description' => t('The date and time when voting opens.'),
      'type' => 'date',
      'schema field' => 'vopen_time',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'vclose_time' => array(
      'label' => t('Voting closing time'),
      'description' => t('The date and time when voting closes.'),
      'type' => 'date',
      'schema field' => 'vclose_time',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'nstatus' => array(
      'label' => t('Nominations status'),
      'description' => t('The nominations status of the election.'),
      'type' => 'integer',
      'schema field' => 'nstatus',
      'required' => TRUE,
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'nopen_time' => array(
      'label' => t('Nominations open time'),
      'description' => t('The date and time when nominations open.'),
      'type' => 'date',
      'schema field' => 'nopen_time',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'nclose_time' => array(
      'label' => t('Nominations closing time'),
      'description' => t('The date and time when nominations close.'),
      'type' => 'date',
      'schema field' => 'nclose_time',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'created' => array(
      'label' => t('Created date'),
      'description' => t('The date when the election was created.'),
      'type' => 'date',
      'schema field' => 'created',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'changed' => array(
      'label' => t('Last modified date'),
      'description' => t('The date when the election settings were last updated.'),
      'type' => 'date',
      'schema field' => 'changed',
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );
  return $info;
}
