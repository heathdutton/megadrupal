<?php
// $Id$


/**
 * @file
 * This file enables views integration of the userpoints nodeaccess data.
 */

function userpoints_nodeaccess_views_views_data() {
  $data = array();
  $data['userpoints_nodeaccess']['table']['group']  = t('Userpoints Nodeaccess');

  $data['userpoints_nodeaccess']['table']['base'] = array(
    'field' => 'pnaid',
    'title' => t('Userpoint Nodeaccess'),
    'help' => t('Userpoint nodeaccess are node access purchases.'),
  );

  $data['userpoints_nodeaccess']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['userpoints_nodeaccess']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('The date when access to the node has been bought.'),
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

  // Add relationship to user table.
  $data['userpoints_nodeaccess']['uid'] = array(
    'title' => t('User'),
    'help' => t('Relate the userpoints nodeaccess table to the user table.'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Users'),
      'handler' => 'views_handler_relationship',
    ),
  );

  // Add relationship to user table.
  $data['userpoints_nodeaccess']['nid'] = array(
    'title' => t('Node'),
    'help' => t('Relate the userpoints nodeaccess table to the node table.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'label' => t('Node'),
      'handler' => 'views_handler_relationship',
    ),
  );
  
  
  
  $data['userpoints_nodeaccess_points']['table']['group'] = t('Userpoints Nodeaccess');
    
  $data['userpoints_nodeaccess_points']['table']['join'] = array(
    'node' => array(
      'handler' => 'views_join',
      'left_table' => 'node',
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['userpoints_nodeaccess_points']['nid'] = array(
    'title' => t('Access Status'),
    'help' => t('Shows whether the current user can access each node.'),
    'field' => array(
      'handler' => 'userpoints_nodeaccess_handler_field_buyaccess',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'userpoints_nodeaccess_handler_filter_access',
    )
  );

  $data['userpoints_nodeaccess_points']['points'] = array(
    'title' => t('Points'),
    'help' => t('The points the node costs to get access to.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  
  return $data;
}



/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function userpoints_nodeaccess_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'userpoints_nodeaccess'),
    ),
    'handlers' => array(
      'userpoints_nodeaccess_handler_field_buyaccess' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
