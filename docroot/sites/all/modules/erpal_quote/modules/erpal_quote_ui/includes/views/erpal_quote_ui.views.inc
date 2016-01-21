<?php

/**
 * @file
 * Views integration for Erpal quote PDF.
 */

/**
 * Implements hook_views_data_alter().
 */
function erpal_quote_ui_views_data_alter(&$data) {
  $data['views_entity_commerce_order']['erpal_quote_ui_order_link'] = array(
    'field' => array(
      'title' => t('Quote PDF'),
      'handler' => 'erpal_quote_ui_field_handler',
      'help' => t('Link to quote PDF file.'),
    ),
  );
}
