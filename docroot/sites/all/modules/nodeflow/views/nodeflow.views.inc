<?php
/**
 * @file
 * Make nodeflow table available to views.
 */

/**
 * Implements hook_views_data().
 */
function nodeflow_views_data() {
  $data['nodeflow']['table']['group'] = t('Nodeflow');

  $data['nodeflow']['position'] = array(
    'title' => t('Position'),
    'help' => t('Position of the node'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
   $data['nodeflow']['nfid'] = array(
    'title' => t('Nodeflow queue ID'),
    'help' => t('ID number of the nodeflow queue.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
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


/**
 * Implements hook_views_data_alter().
 *
 * This adds the relationship between node and nodeflow.
 */
function nodeflow_views_data_alter(&$data) {
  $data['node']['nodeflow_rel'] = array(
    'group' => t('Nodeflow'),
    'title' => t('Nodeflow'),
    'help' => t('Create a relationship to nodeflow positions.'),
    'real field' => 'nid',
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'nodeflow',
      'base field' => 'nid',
      'field' => 'nid',
      'label' => t('positions'),
      'title' => t('positions'),
    ),
  );
}
