<?php

/**
 * @file
 * Implement Views integration for ec_buynow module.
 */

/**
 * Implements hook_views_data().
 */
function ec_buynow_views_data() {
  $data['ec_product']['hide_buynow_link'] = array(
    'title' => t('Hide buy now link'),
    'help' => t('Lists products where the buy now link is not used.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Hide Buynow'),
    ),
  );

  return $data;
}

