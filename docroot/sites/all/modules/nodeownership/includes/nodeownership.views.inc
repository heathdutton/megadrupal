<?php

/**
 * @file
 * Provide views data and handlers for nodeownership.module.
 */

/**
 * Implements hook_views_data()
 */
function nodeownership_views_data() {

  // ----------------------------------------------------------------
  // nodeownership table -- basic table information.

  $data['nodeownership']['table']['group'] = t('Node ownership');
  $data['nodeownership']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Node ownership claims'),
    'access query tag' => 'node_access',
  );
  $data['nodeownership']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'node' => array(
      'right_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // ----------------------------------------------------------------
  // nodeownership -- fields

  // Uid.
  $data['nodeownership']['uid'] = array(
    'title' => t('User uid'),
    'help' => t('The user ID if the claimant'),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name',
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'label' => 'User',
      'help' => t('Bring in data about the user.'),
    ),
  );

  // Nid.
  $data['nodeownership']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The Nid of the node being claimed'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'relationship table' => 'nodeownership',
      'relationship field' => 'nid',
      'label' => 'node',
      'help' => t('Bring in data about the node being claimed.'),
      'skip base' => 'nodeownership',
    ),
  );

  // Contact info.
  $data['nodeownership']['contact'] = array(
    'title' => t('Contact'),
    'help' => t('The contact information left by the claimant.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['nodeownership']['notes'] = array(
    'title' => t('Notes'),
    'help' => t('Additional notes left by claimant.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  // Timestamp.
  $data['nodeownership']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('The time and date that this claim was made.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Status.
  $data['nodeownership']['status'] = array(
    'title' => t('Status'),
    'help' => t('The status of a given claim. E.g., pending, approved, or declined.'),
    'field' => array(
      'handler' => 'views_handler_field_nodeownership_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_nodeownership_status',
      'label' => t('Status'),
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Accept link.
  $data['nodeownership']['approve_link'] = array(
    'real field' => 'id',
    'field' => array(
      'title' => t('Approve link'),
      'help' => t('Approve claim'),
      'handler' => 'views_handler_field_nodeownership_approve_link',
    ),
  );

  // Decline link.
  $data['nodeownership']['decline_link'] = array(
    'real field' => 'id',
    'field' => array(
      'title' => t('Decline link'),
      'help' => t('Decline claim.'),
      'handler' => 'views_handler_field_nodeownership_decline_link',
    ),
  );

  return $data;
}

