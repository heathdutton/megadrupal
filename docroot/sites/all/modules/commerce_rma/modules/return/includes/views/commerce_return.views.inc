<?php

/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Commerce Return entity Views definitions.
 */
class CommerceReturnViewsController extends EntityDefaultViewsController {
  /**
   * Override EntityDefaultViewsController::views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    // Show the operations using CTools dropbutton menu.
    $data['commerce_return']['operations_dropbutton'] = array(
      'title' => t('Operations'),
      'help' => t('Show the operation links using dropbutton menu.'),
      'field' => array(
        'handler' => 'commerce_return_handler_field_operations_dropbutton',
      ),
      // This is a dummy field, so attach it to the ID column.
      'real field' => 'return_id',
    );

    return $data;
  }
}

function commerce_return_views_data_alter(&$data) {
  $data['commerce_line_item']['commerce_return_status'] = array(
    'title' => t('Return status'),
    'help' => t('Show the status of the returned product.'),
    'field' => array(
      'handler' => 'commerce_line_item_handler_field_commerce_return_status'
    ),
    // This is a dummy field, so attach it to the ID column.
    'real field' => 'line_item_id',
  );

  $data['commerce_line_item']['commerce_return_reason'] = array(
    'title' => t('Return reason'),
    'help' => t('Show the customer reason for the returned product.'),
    'field' => array(
      'handler' => 'commerce_line_item_handler_field_commerce_return_reason',
    ),
    // This is a dummy field, so attach it to the ID column.
    'real field' => 'line_item_id',
  );
}