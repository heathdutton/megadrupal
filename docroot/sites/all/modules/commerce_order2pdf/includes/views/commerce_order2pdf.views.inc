<?php 

/**
 * Implements hook_views_data()
 */
function commerce_order2pdf_views_data() {
  $data = array();
  $data['commerce_order']['order2pdf_link'] = array(
    'field' => array(
      'title' => t('Download pdf order'),
      'help' => t('Provide a simple link to download the order.'),
      'handler' => 'commerce_order2pdf_handler_field_order_pdf_link',
    ),
  );
   return $data;
 }