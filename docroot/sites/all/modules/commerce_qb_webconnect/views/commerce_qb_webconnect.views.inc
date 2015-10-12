<?php

/**
 * Implements hook_views_data_alter().
 */
function commerce_qb_webconnect_views_data_alter(&$data) {
  
  // Expose a label/link to the referenced data.
  $data['commerce_qb_exports']['reference'] = array(
    'title' => t('Data reference'),
    'help' => t('Displays a reference to the data stored in an export record.'),
    'field' => array(
      'handler' => 'commerce_qb_webconnect_handler_field_export_label',
    ),
  );
}
