<?php

/**
 * @file
 * Integration of Private Taxonomy with Views.
 */

/**
 * Implements hook_views_data().
 */
function private_taxonomy_views_data() {
  $data['user_term'] = array(
    'table' => array(
      'group' => t('Private Taxonomy term'),
      'join' => array(
        'taxonomy_term_data' => array(
          'left_field' => 'tid',
          'field' => 'tid',
        ),
      ),
    ),
  );
  $data['user_term']['uid'] = array(
    'title' => t('Current user'),
    'help' => t('Filter the terms for the current user.'),
    'filter' => array(
      'handler' => 'views_handler_filter_user_current',
    ),
    'argument' => array(
      'title' => t('User'),
      'help' => t('Filter the terms by user.'),
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'uid',
    ),
  );
  $data['user_term']['tid'] = array(
    'title' => t('Content owner'),
    'help' => t('Relate a term to its owner'),
    'relationship' => array(
      'base' => 'taxonomy_term_data',
      'base field' => 'tid',
      'handler' => 'views_handler_relationship',
      'label' => t('Owner'),
    ),
  );

  return $data;
}
