<?php
/**
 * @file
 * Provide views data and handlers for reviews.module
 */

/**
 * Implements hook_views_data()
 */
function reviews_views_data() {

  // Add the reviews table to the group.
  $data['reviews']['table']['group'] = t('Reviews');

  // Specify the base table.
  $data['reviews']['table']['base'] = array(
    'field' => 'rid',
    'title' => t('Reviews'),
    'help' => t('Reviews for node.'),
  );

  // Join the node and users table.
  $data['reviews']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // The username of the reviewer.
  $data['reviews']['uid'] = array(
    'title' => t('Username'),
    'help' => t('The username of the reviewer.'),
    'field' => array(
      'handler' => 'reviews_handler_field_username',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // The review content.
  $data['reviews']['review'] = array(
    'title' => t('Review'),
    'help' => t('The review content added to the node.'),
    'field' => array(
      'handler' => 'reviews_handler_field_review',
      'click sortable' => TRUE, // This is use by the table display plugin.
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Review date & time.
  $data['reviews']['created'] = array(
    'title' => t('Created'),
    'help' => t('The date and time the review was made.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
    	'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // The review status.
  $data['reviews']['status'] = array(
    'title' => t('Published'),
    'help' => t('Whether or not the review is published.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE, // This is use by the table display plugin.
      'output formats' => array(
        'published-notpublished' => array(t('Published'), t('Not published')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Published'),
      'type' => 'yes-no',
      'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statment
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Add the reviews_count table to the group.
  $data['reviews_count']['table']['group'] = t('Reviews');

  // Join the reviews count table.
  $data['reviews_count']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // Node reviews count.
  $data['reviews_count']['count'] = array(
    'title' => t('Node review count'),
    'help' => t('Filter results based on whether the node has any reviews.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Relationship to the 'Node' table
  $data['reviews']['nid'] = array(
    'title' => t('Node'),
    'help' => t('The particular node the review is attached to'),
    'relationship' => array(
      'label' => t('Node'),
      'base' => 'node',
      'base field' => 'nid',
      // This allows us to not show this relationship if the base is already
      // node so users won't create circular relationships.
      'skip base' => array('node'),
    ),
  );

  // Relationship to the 'Node' table
  $data['reviews']['uid'] = array(
    'title' => t('User'),
    'help' => t('The particular user the review was submitted by'),
    'relationship' => array(
      'label' => t('User'),
      'base' => 'users',
      'base field' => 'uid',
      // This allows us to not show this relationship if the base is already
      // node so users won't create circular relationships.
      'skip base' => array('users'),
    ),
  );

  return $data;

}

/**
* Implementation of hook_views_handlers().
*/
function reviews_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'reviews') . '/views',
    ),
    'handlers' => array(
      'reviews_handler_field_review' => array(
        'parent' => 'views_handler_field',
      ),
      'reviews_handler_field_username' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
