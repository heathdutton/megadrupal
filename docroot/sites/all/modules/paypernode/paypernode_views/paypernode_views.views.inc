<?php

/**
 * @file
 * This files contains hooks implementation for Views integration.
 *
 */

function paypernode_views_views_data() {

  $data = array();

  $data['paypernode_tracker']['table']['group'] = t('Content');

  $data['paypernode_tracker']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );
  $data['paypernode_tracker']['tid'] = array(
    'title' => t('Paypernode Tracker ID'),
    'help' => t('The Paypernode Tracker ID of this node, if it has been created with Paypernode System.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  return $data;
}

