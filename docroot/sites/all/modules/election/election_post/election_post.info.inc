<?php
/**
 * @file
 * Entity property information for the Election Post module.
 */

/**
 * Implements hook_entity_property_info().
 */
function election_post_entity_property_info() {
  $info = array();
  // Add metadata about election post properties.
  $properties = &$info['election_post']['properties'];
  $properties = array(
    'post_id' => array(
      'label' => t('Post ID'),
      'description' => t('Primary key: the unique identifier for a post.'),
      'type' => 'integer',
      'schema field' => 'post_id',
    ),
    'election_id' => array(
      'label' => t('Election ID'),
      'description' => t('The election for this post.'),
      'type' => 'election',
      'schema field' => 'election_id',
      'required' => TRUE,
    ),
    'type' => array(
      'label' => t('Type'),
      'type' => 'token',
      'schema field' => 'type',
      'required' => TRUE,
    ),
    'published' => array(
      'label' => t('Published status'),
      'description' => t('Whether or not the post is published.'),
      'type' => 'boolean',
      'options list' => '_election_get_published_options',
      'schema field' => 'published',
      'required' => TRUE,
    ),
    'title' => array(
      'label' => t('Title'),
      'description' => t('The title of the election post.'),
      'type' => 'text',
      'schema field' => 'title',
      'required' => TRUE,
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'rids' => array(
      'label' => t('Eligible roles'),
      'type' => 'list<role>',
      'description' => t('Roles that may vote for this post.'),
      'schema field' => 'rids',
    ),
    'allow_abstention' => array(
      'label' => t('Allow abstention'),
      'description' => t('Whether or not to allow voters to abstain.'),
      'type' => 'boolean',
      'schema field' => 'allow_abstention',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'vstatus_inheritance' => array(
      'label' => t('Voting status inheritance'),
      'description' => t('The voting status of the post.'),
      'type' => 'integer',
      'schema field' => 'vstatus_inheritance',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'options list' => '_election_post_get_inheritance_options',
    ),
    'nstatus_inheritance' => array(
      'label' => t('Nominations status inheritance'),
      'description' => t('The nominations status of the post.'),
      'type' => 'integer',
      'schema field' => 'nstatus_inheritance',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'options list' => '_election_post_get_inheritance_options',
    ),
    'vacancy_count' => array(
      'label' => t('Number of vacancies'),
      'description' => t('The number of vacancies in the post.'),
      'type' => 'integer',
      'schema field' => 'vacancy_count',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
    ),
    'created' => array(
      'label' => t('Created time'),
      'description' => t('The date when the post was created.'),
      'type' => 'date',
      'schema field' => 'created',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'changed' => array(
      'label' => t('Last modified time'),
      'description' => t('The date when the post was last updated.'),
      'type' => 'date',
      'schema field' => 'changed',
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );
  return $info;
}
