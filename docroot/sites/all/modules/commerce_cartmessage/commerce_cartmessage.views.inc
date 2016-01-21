<?php

/**
 * Provide shopping cart related Views integration.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_cartmessage_views_data_alter(&$data) {
  $data['commerce_order']['cartmessage'] = array(
    'title' => t('Cart message'),
    'help' => t('Displays a Rules defined message on the shopping cart pages.'),
    'area' => array(
      'handler' => 'commerce_cartmessage_handler_messageblock',
    ),
  );
}
