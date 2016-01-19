<?php

/**
 * @file
 * Views integration file for Ad module.
 */

/**
 * Implements hook_views_data().
 */
function ad_views_data() {
  $tables['ad_node']['table']['group'] = t('Ad Node');

  // Define how the ad_node table is linked to the node table.
  $tables['ad_node']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  // Describe the two fields in the ad_node database table.
  $tables['ad_node']['total_impressions'] = array(
    'title' => t('Total Impressions'),
    'help' => t('The total number of impressions of an ad.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'label' => t('Total Impressions'),
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $tables['ad_node']['total_clicks'] = array(
    'title' => t('Total Clicks'),
    'help' => t('The total number of clicks of an ad.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'label' => t('Total Clicks'),
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $tables;
}
