<?php

/**
 * @file
 * Coupon views extra data.
 */

/**
 * Alter the views data to enable some additional features for commerce_stock_sources in views.
 */
function commerce_stock_sources_views_data_alter(&$data) {
  if (isset($data['commerce_stock_sources'])) {
    $data['commerce_stock_sources']['table']['base']['access query tag'] = 'commerce_stock_sources_access';
  }

  $data['commerce_stock_sources']['edit']['moved to'] = array('views_entity_commerce_stock_sources', 'edit');
  $data['views_entity_commerce_stock_sources']['edit'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the content.'),
      'handler' => 'views_handler_field_stock_source_link',
    ),
  );
}
