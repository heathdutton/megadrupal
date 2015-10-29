<?php

/**
 * @file
 *   Views support for statistcs pro module
 *
 */

/**
 * Implementation of hook_views_data().
 *
 * @return array
 */
function statspro_views_data() {
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'statspro') . '/statspro.inc';

  //Begin: statspro table
  $data['statspro']['table']['group'] = t('Statistics Pro');
  // Advertise this table as a possible base table
  $data['statspro']['table']['base'] = array(
    'field' => 'day',
    'title' => t('Daily statistics'),
    'help' => t('Stores site statistics aggregated per day.'),
    'weight' => 10,
  );
  // DATE
  $data['statspro']['day'] = array(
    'title' => t('Date'),
    'help' => t('Date of statistics.'),
    'field' => array('click sortable' => TRUE),
    'sort' => array('handler' => 'views_handler_sort'),
  );
  // all data fields
  $statspro = new StatsPro();
  $data_fields = $statspro->get_fields();
  foreach ($data_fields as $field => $field_desc) {
    $data['statspro'][$field] = array(
      'title' => strip_tags($field_desc),
      'help' => $field_desc,
      'field' => array(
        'handler' => 'statspro_views_handler_field_graph_trend',
        'click sortable' => TRUE,
      ),
      'sort' => array('handler' => 'views_handler_sort'),
      'filter' => array('handler' => 'views_handler_filter_numeric'),
    );
  }
  //End: statspro table

  //Begin: statspro_term table
  $data['statspro_term']['table']['group'] = t('Statistics Pro');
  // For other base tables, explain how we join
  $data['statspro_term']['table']['join'] = array(
    'taxonomy_term_data' => array(
      'field' => 'tid',
      'left_field' => 'tid',
    ),
  );
  // term amounts
  $data['statspro_term']['ncount'] = array(
    'title' => t('Amount nodes'),
    'help' => t('Amount of nodes for term'),
    'field' => array(
      'handler' => 'statspro_views_handler_field_graph',
      'click sortable' => TRUE,
    ),
    'sort' => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
  );
  //End: statspro_term table

  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 *
 * @return array
 */
function statspro_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'statspro') . '/views',
    ),
    'handlers' => array(
      'statspro_views_handler_field_graph' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'statspro_views_handler_field_graph_trend' => array(
        'parent' => 'views_handler_field_numeric',
      ),
    ),
  );
}
