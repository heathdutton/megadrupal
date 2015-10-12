<?php

/**
 * Implements hook_views_data_alter().
 */
function relify_views_data_alter(&$data) {
  // Add a simple recommended product area handler.
  $data['views_entity_commerce_line_item']['recommended_product'] = array(
    'title' => t('Recommended product'),
    'help' => t('Show a simple product recommendation if there is a single line item argument in the View.'),
    'area' => array(
      'handler' => 'relify_handler_area_recommended_product',
    ),
  );
}
